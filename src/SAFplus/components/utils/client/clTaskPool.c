/*
 * Copyright (C) 2002-2012 OpenClovis Solutions Inc.  All Rights Reserved.
 *
 * This file is available  under  a  commercial  license  from  the
 * copyright  holder or the GNU General Public License Version 2.0.
 * 
 * The source code for  this program is not published  or otherwise 
 * divested of  its trade secrets, irrespective  of  what  has been 
 * deposited with the U.S. Copyright office.
 * 
 * This program is distributed in the  hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied  warranty  of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 * 
 * For more  information, see  the file  COPYING provided with this
 * material.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#ifdef __linux__
#include <sys/types.h>
#include <sys/syscall.h>
#endif
#include <clTaskPool.h>
#include <clCommonErrors.h>
#include <clDebugApi.h>
#include <clCpmIpi.h>

#define CL_TASK_POOL_UNUSED_TASKS (0x20)
#define CL_TASK_POOL_UNUSED_IDLE_TIME {.tsSec=0,.tsMilliSec=500}
#define CL_TASK_POOL_JOB_IDLE_TIME CL_TASK_POOL_UNUSED_IDLE_TIME
#define CL_TASK_POOL_DELAY (100)
#define DEFAULT_TASKS_NORMAL (0x10)
#define DEFAULT_TASKS_EXCLUSIVE (1)

#define CL_TASK_POOL_RUNNING (0x1)
#define CL_TASK_POOL_ACTIVE (0x2)

#ifndef VXWORKS_BUILD
#define CL_TASK_POOL_IDLE_TIMEOUT (45)
#else
#define CL_TASK_POOL_IDLE_TIMEOUT (0)
#endif
#define CL_TASK_POOL_IDLE_TIMEOUT_USECS (1000000L * CL_TASK_POOL_IDLE_TIMEOUT)

#ifdef __linux__
#define GET_TASK_ID(pStats) ( (pStats)->taskId )
#define SET_TASK_ID(pStats) do { (pStats)->taskId = (ClOsalTaskIdT)syscall(SYS_gettid); } while(0)
#define CLEAR_TASK_ID(pStats) do { (pStats)->taskId = 0; } while(0)
#else
#define SET_TASK_ID(pStats) do {;}while(0)
#define GET_TASK_ID(pStats) ( (pStats)->tId )
#define CLEAR_TASK_ID(pStats) do {;}while(0)
#endif

typedef struct ClTaskPoolRef
{
    ClTaskPoolT *tp;
} ClTaskPoolRefT;

typedef struct ClTaskPoolArg
{
    ClTaskPoolT *tp;
    ClTaskPoolStatsT *pStats;
}ClTaskPoolArgT;

#if 0
static ClEoQueueT gClTaskPoolUnused = {                           \
    .queue = CL_JOB_LIST_INITIALIZER(gClTaskPoolUnused.queue),    \
};
#endif

static ClUint32T gClTaskPoolKey;
static ClBoolT gClEoHeartbeatDisabled = CL_FALSE;
static ClTaskPoolStatsT gClNullTaskPoolStats;
static ClTaskPoolT gClNullTaskPool = { .pStats = &gClNullTaskPoolStats };
static ClTaskPoolRefT gClNullTaskPoolRef = { .tp = &gClNullTaskPool };
static ClBoolT gClTaskPoolInitialized;

/*
 * Create a pthread key for storing task pool info. for each thread.
 */
static void taskPoolKeyDestructor(void *v)
{
    free(v);
}

ClRcT clTaskPoolInitialize(void)
{
    ClRcT rc = CL_OK;
    clOsalMutexInit(&gClNullTaskPool.mutex);
    clOsalCondInit(&gClNullTaskPool.cond);
    if(clParseEnvBoolean("CL_EO_TASK_MONITOR"))
    {
        static const ClCharT *const aspEOs[] = { 
            "AMF", "LOG", "GMS", "EVT", "CKP", "MSG", "NAM", "FLT", "ALM", "COR", "TXN", NULL,
        };
        register ClInt32T i;
        for(i = 0; aspEOs[i]; ++i)
            if(!strncmp(CL_EO_NAME, aspEOs[i], strlen(aspEOs[i])))
                break;

        if(!aspEOs[i])
        {
            gClEoHeartbeatDisabled = CL_TRUE;
            clLogNotice("TASK", "MONITOR", "Disabling task pool heartbeat for EO [%s]", CL_EO_NAME);
        }
    }
    rc = clOsalTaskKeyCreate(&gClTaskPoolKey, taskPoolKeyDestructor);
    if(rc != CL_OK) 
        return rc;
    gClTaskPoolInitialized = CL_TRUE;
    return rc;
}

ClRcT clTaskPoolFinalize(void)
{
    if(!gClTaskPoolInitialized) return CL_OK;
    gClTaskPoolInitialized = CL_FALSE;
    return clOsalTaskKeyDelete(gClTaskPoolKey);
}

static ClRcT clTaskPoolDataSet(ClPtrT data)
{
    ClRcT rc = CL_OK;
    if( (rc = clOsalTaskDataSet(gClTaskPoolKey, (ClOsalTaskDataT)data) ) != CL_OK)
    {
        clLogWarning("DATA", "SET", "Task pool data set returned [%#x]", rc);
    }
    return rc;
}

static ClRcT clTaskPoolDataGet(ClPtrT *pData)
{
    ClOsalTaskDataT *pThreadData = (ClOsalTaskDataT*)pData;
    ClRcT rc = CL_OK;
    if(!gClTaskPoolInitialized) 
        return CL_TASKPOOL_RC(CL_ERR_NOT_INITIALIZED);
    if(!pData)
        return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);
    if( (rc = clOsalTaskDataGet(gClTaskPoolKey, pThreadData) ) != CL_OK)
    {
        *pData = (ClPtrT)&gClNullTaskPoolRef;
        rc = CL_OK;
    }
    return rc;
}

void clTaskPoolEntry(ClTaskPoolArgT *pArg)
{
    ClTaskPoolT *tp = pArg->tp;
    ClTaskPoolStatsT *pStats = pArg->pStats;
    ClTimeT lastWakeup = 0;
    ClTimeT currentWakeup = 0;
    ClTimeT delta = 0;
    ClTaskPoolRefT *tRef = calloc(1, sizeof(*tRef));
    CL_ASSERT(tRef != NULL);
    tRef->tp = tp;
    clHeapFree(pArg);

    SET_TASK_ID(pStats);
    clTaskPoolDataSet((ClPtrT)tRef);
    clOsalMutexLock(&tp->mutex);
    /*
     * clLogDebug("TASK", "STAT", "Started task [%d]", (int)GET_TASK_ID(pStats));
    */
    while( (tp->flags & CL_TASK_POOL_RUNNING) ) /* Gas TODO, trigger tasks to quit*/
    {      
        ClRcT clrc = CL_OK;
        ClTimerTimeOutT timer = { .tsSec = CL_TASK_POOL_IDLE_TIMEOUT, 
                                  .tsMilliSec = 0 
        };  /* should be configurable? */
        if (tp->preIdleFn) 
        {
            clOsalMutexUnlock(&tp->mutex);
            clrc = tp->preIdleFn(tp->preIdleCookie);
            clOsalMutexLock(&tp->mutex);
        }

        if (!tp->onDeckFn)
        {
            clMetricAdjust(&tp->numIdleTasks,1);
            clrc = clOsalCondWait (&tp->cond, &tp->mutex, timer);
            clMetricAdjust(&tp->numIdleTasks,-1);
            if (CL_GET_ERROR_CODE(clrc) == CL_ERR_TIMEOUT)
            {
                /*
                 * Keep atleast 1 task in the taskpool ready to process clashing incoming requests.
                 */
                if(tp->numTasks.value > 1)
                {
                    /*
                     * Quit this task if its idle for quite sometime.
                     */
                    break;
                }
            }
        }
        lastWakeup = currentWakeup;
        currentWakeup = clOsalStopWatchTimeGet();

        if( (tp->flags & CL_TASK_POOL_RUNNING)
            &&
            !tp->onDeckFn)
        {
            /*
             * Wakeup with no on-deck. So an idle wakeup.
             * Measure idle wakeup times across to see if it adds up
             */
            if(CL_TASK_POOL_IDLE_TIMEOUT_USECS > 0 
               &&
               lastWakeup)
            {
                delta += currentWakeup - lastWakeup;
                if(delta >= CL_TASK_POOL_IDLE_TIMEOUT_USECS 
                   &&
                   tp->numTasks.value > 1)
                {
                    break;
                }
            }
        }
        else
        {
            /*
             * reset idle measure.
             */
            delta = 0;
        }

        while ( (tp->flags & CL_TASK_POOL_RUNNING) 
                && 
                tp->onDeckFn)
        {
            ClCallbackT       fn = tp->onDeckFn;
            ClPtrT     cookie = tp->onDeckCookie;

            /* Clear this so that another call can be loaded in */
            tp->onDeckFn = 0;
            tp->onDeckCookie = 0;
            clOsalCondBroadcast(&tp->cond);  /* Wake up any caller that is waiting to load a task */
            /* Note that its a bit inefficient to use the same Cond to both wake the task producers
               and consumers... */

            /* 
             * Check if the task pool was quiesced.
             */
            rescan:
            if( !(tp->flags & CL_TASK_POOL_ACTIVE) )
            {
                /*
                 * We block here for a resume to wake us back.
                 */
                ClTimerTimeOutT delay = {.tsSec = 0, .tsMilliSec = 0 };
                clMetricAdjust(&tp->numIdleTasks,1);
                clrc = clOsalCondWait (&tp->cond, &tp->mutex, delay);
                clMetricAdjust(&tp->numIdleTasks,-1);
                if( !(tp->flags & CL_TASK_POOL_RUNNING)) break;
                goto rescan;
            }

            /* Call the user's function with our locks unlocked */
            ++tp->pendingJobs;
            if(tp->monitorTimer)
            {
                pStats->startTime = clOsalStopWatchTimeGet();
            }
            clOsalMutexUnlock(&tp->mutex);
            CL_DEBUG_PRINT(CL_DEBUG_INFO, ("Task pool is running a user's fn")); 
            fn(cookie);
            clOsalMutexLock(&tp->mutex);
            pStats->startTime = 0;
            --tp->pendingJobs;
        }

    }
    pStats->tId = 0;
    /*    
     * clLogDebug("TASK", "DELETE", "Task [%d] is exiting. Remaining [%d]", 
     (int)GET_TASK_ID(pStats), tp->numTasks.value-1);
    */
    CLEAR_TASK_ID(pStats);
    clMetricAdjust(&tp->numTasks, -1);  
    clOsalMutexUnlock(&tp->mutex);
}


/* Must be called with the mutex locked */
static void clTaskPoolNewTask(ClTaskPoolT *tp)
{
    if (tp->numTasks.value<tp->maxTasks.value)
    {
        ClInt32T i;
        for (i = 0; i<tp->maxTasks.value;i++)
        {
            if (tp->pStats[i].tId == 0)
            {
                ClRcT rc = CL_OK;
                ClTaskPoolArgT *pArg = clHeapCalloc(1, sizeof(*pArg));
                CL_ASSERT(pArg != NULL);
                clLog(CL_LOG_SEV_TRACE,"TSK","POL", "Creating new task");
                pArg->tp = tp;
                pArg->pStats = tp->pStats + i;
                tp->pStats[i].heartbeatDisabled = gClEoHeartbeatDisabled;
                tp->pStats[i].startTime = 0;
                rc = clOsalTaskCreateAttached("task pool", CL_OSAL_SCHED_OTHER, tp->priority, 0,
                                              (void* (*) (void*)) clTaskPoolEntry, pArg, &tp->pStats[i].tId);
                CL_ASSERT(rc == CL_OK);
                /*
                 * Detach to reclaim resources when it exits on exceeding idle time
                 */
                rc = clOsalTaskDetach(tp->pStats[i].tId);
                CL_ASSERT(rc == CL_OK);
                clMetricAdjust(&tp->numTasks, 1);
                return;
            }
        } 
        clLog(CL_LOG_SEV_WARNING,"TSK","POL", "Task pool is out of free task slots. NumTasks [%d], MaxTasks [%d]",tp->numTasks.value, tp->maxTasks.value);
      
    }
}

ClRcT clTaskPoolWake(ClTaskPoolHandleT handle)
{
    ClTaskPoolT *tp = (ClTaskPoolT *) handle;

    if(!tp) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);

    clOsalMutexLock(&tp->mutex);

    /* If there are no idle tasks, start one up so that the pool is active */
    if (tp->numIdleTasks.value==0) clTaskPoolNewTask(tp);

    clOsalCondBroadcast(&tp->cond);  /* Wake up tasks! */

    clOsalMutexUnlock(&tp->mutex); 

    return CL_OK;
}

ClRcT clTaskPoolRun(ClTaskPoolHandleT handle, ClCallbackT func, ClPtrT cookie)
{
    ClTaskPoolT *tp = (ClTaskPoolT *) handle;

    if(!tp) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);

    clOsalMutexLock(&tp->mutex);

    /* If there are no idle tasks, start one up for this new job */
    if (tp->numIdleTasks.value==0) clTaskPoolNewTask(tp);

    while(tp->onDeckFn) /* While someone's job is already waiting for a task... */
    {
        ClRcT clrc;
        ClTimerTimeOutT timer = { 3, 0 }; /* should be configurable? */
        clTaskPoolNewTask(tp); /* ... create a new task for it. */
        
        clrc = clOsalCondWait (&tp->cond, &tp->mutex, timer);
        if (CL_GET_ERROR_CODE(clrc) == CL_ERR_TIMEOUT)
        {
            /* GAS todo -- too long a wait... are the tasks stuck*/
        }

    }

    CL_ASSERT(tp->onDeckFn == NULL);
    CL_ASSERT(tp->onDeckCookie == 0);

    tp->onDeckFn = func;
    tp->onDeckCookie = cookie;
    clOsalCondSignal(&tp->cond);  /* Wake up tasks! */

    clOsalMutexUnlock(&tp->mutex);
    return CL_OK;  
}


ClRcT clTaskPoolCreate(ClTaskPoolHandleT *pHandle, ClInt32T maxTasks, ClCallbackT preIdleFunc, ClPtrT preIdleCookie)
{
    ClTaskPoolT *pTaskPool=NULL;
    ClRcT rc = CL_TASKPOOL_RC(CL_ERR_NO_MEMORY);

    if(!pHandle) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);

    pTaskPool = (ClTaskPoolT*) clHeapCalloc(1, sizeof(*pTaskPool));
    if(pTaskPool == NULL)
    {
        CL_DEBUG_PRINT(CL_DEBUG_ERROR,("Error allocating memory"));
        goto out;
    }

    pTaskPool->pStats = clHeapCalloc(maxTasks, sizeof(*pTaskPool->pStats));
    if(!pTaskPool->pStats)
    {
        CL_DEBUG_PRINT(CL_DEBUG_ERROR,("Error allocating memory"));
        goto out_free;
    }

    rc = clOsalMutexInit(&pTaskPool->mutex);
    CL_ASSERT(rc == CL_OK);
    rc = clOsalCondInit(&pTaskPool->cond);
    CL_ASSERT(rc == CL_OK);
    pTaskPool->flags |= (CL_TASK_POOL_RUNNING | CL_TASK_POOL_ACTIVE);
    memset(&pTaskPool->monitorThreshold, 0, sizeof(pTaskPool->monitorThreshold));
    clMetricInit(&pTaskPool->maxTasks, "maxTasks", maxTasks, NULL, 0, "Maximum number of threads that can be in this pool.");
    clMetricInit(&pTaskPool->numTasks, "numTasks", 0, NULL, 0, "Current number of threads in this pool.");
    clMetricInit(&pTaskPool->numIdleTasks, "numIdleTasks", 0, NULL, 0, "Current number of threads in this pool that are waiting for something to do.");
    pTaskPool->monitorActive = CL_FALSE;
    pTaskPool->priority = CL_OSAL_THREAD_PRI_NOT_APPLICABLE;
    pTaskPool->preIdleFn = preIdleFunc;
    pTaskPool->preIdleCookie = preIdleCookie;
    pTaskPool->pendingJobs = 0;
    *pHandle = (ClTaskPoolHandleT) pTaskPool;
    rc = CL_OK;
    goto out;

    out_free:
    if(pTaskPool->pStats)
    {
        clHeapFree(pTaskPool->pStats);
    }
    if(pTaskPool) clHeapFree(pTaskPool);

    out:
    return rc;
}

ClRcT clTaskPoolDelete(ClTaskPoolHandleT handle)
{
    ClRcT rc = CL_OK;
    ClTaskPoolT *tp = (ClTaskPoolT*)handle;

    if(!tp) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);

    rc = clTaskPoolStop(handle);
    clTaskPoolMonitorDelete(handle);

    clOsalCondDestroy(&tp->cond);
    clOsalMutexDestroy(&tp->mutex);

    clHeapFree(tp->pStats);
    clHeapFree(tp);
    return rc;  
}

ClRcT clTaskPoolStart(ClTaskPoolHandleT handle)
{
    ClRcT rc;
    ClTaskPoolT *tp = (ClTaskPoolT*)handle;
    if(!tp) 
    {
        rc = CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);
        clDbgCodeError(rc,("NULL task pool handle"));
        return rc;
    }

    clOsalMutexLock(&tp->mutex);
    /* set the flag and then wake them all up */
    tp->flags |= CL_TASK_POOL_RUNNING;
    clOsalCondBroadcast(&tp->cond);

    if(tp->monitorTimer)
    {
        ClBoolT state = CL_FALSE;
        rc = clTimerIsStopped(tp->monitorTimer, &state);
        if(rc == CL_OK && state)
            clTimerStart(tp->monitorTimer);
    }
    clOsalMutexUnlock(&tp->mutex);
    
    return CL_OK;
}

ClRcT clTaskPoolStopAsync(ClTaskPoolHandleT handle)
{
    ClRcT rc;
    ClTaskPoolT *tp = (ClTaskPoolT*)handle;
    if(!tp) 
    {
        rc = CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);
        clDbgCodeError(rc,("NULL task pool handle"));
        return rc;
    }

    clOsalMutexLock(&tp->mutex);
    /* set the flag and then wake them all up */
    tp->flags &= ~CL_TASK_POOL_RUNNING;
    /*
     * Clear on deck function.
     */
    tp->onDeckFn = NULL;
    tp->onDeckCookie = NULL;
    clOsalCondBroadcast(&tp->cond);

    clOsalMutexUnlock(&tp->mutex);
    
    return CL_OK;
}


ClRcT clTaskPoolStop(ClTaskPoolHandleT handle)
{
    ClRcT rc;
    ClTaskPoolT *tp = (ClTaskPoolT*)handle;
    ClOsalTaskIdT taskId = 0;
    ClInt32T i = 0;
    ClTimerTimeOutT delay = {.tsSec = 0, .tsMilliSec = 500};

    if(!tp) 
    {
        rc = CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);
        clDbgCodeError(rc,("NULL task pool handle"));
        return rc;
    }

    clOsalSelfTaskIdGet(&taskId);
    clTaskPoolStopAsync(handle);

    clOsalMutexLock(&tp->mutex);
    clOsalCondBroadcast(&tp->cond);
    for (i = 0; i < tp->maxTasks.value; i++)
    {
        if (tp->pStats[i].tId && tp->pStats[i].tId != taskId)
        {
            ClInt32T tries = 0;
            while(tp->pStats[i].tId 
                  &&
                  tries++ < 10)
            {
                clOsalMutexUnlock(&tp->mutex);
                clOsalTaskDelay(delay);
                clOsalMutexLock(&tp->mutex);
            }
            if(tp->pStats[i].tId) 
                clOsalTaskKill(tp->pStats[i].tId, SIGKILL);
        }
        tp->pStats[i].tId = 0;
    }
    if(tp->monitorTimer)
        clTimerStop(tp->monitorTimer);
    clOsalMutexUnlock(&tp->mutex);

    return CL_OK;
}


ClRcT clTaskPoolResume(ClTaskPoolHandleT handle)
{
  ClRcT rc;
  ClTaskPoolT *tp = (ClTaskPoolT*)handle;

  if(!tp) 
    {
      rc = CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);
      clDbgCodeError(rc,("NULL task pool handle"));
      return rc;
    }

  clOsalMutexLock(&tp->mutex);
  tp->flags |= CL_TASK_POOL_ACTIVE;
  clOsalCondBroadcast(&tp->cond);
  clOsalMutexUnlock(&tp->mutex);

  if(tp->monitorCallback)
      clTaskPoolMonitorStart(handle, tp->monitorThreshold, tp->monitorCallback);

  return CL_OK;
}

ClRcT clTaskPoolQuiesce(ClTaskPoolHandleT handle)
{
    ClTaskPoolT *tp = handle;
    ClOsalTaskIdT taskId = 0;
    ClInt32T pendingJobs = 0;
    ClTimerTimeOutT delay = {.tsSec = 0, .tsMilliSec = 50 };
    ClUint32T i = 0;

    if(!tp)
        return CL_TASKPOOL_RC(CL_ERR_INVALID_HANDLE);

    clOsalSelfTaskIdGet(&taskId);

    clOsalMutexLock(&tp->mutex);
    tp->flags &= ~CL_TASK_POOL_ACTIVE;
    /*
     * Ignore waiting on self and heartbeat disabled tasks.
     */
    for(i = 0; i < tp->maxTasks.value; ++i)
    {
        if(tp->pStats[i].tId)
        {
            if(tp->pStats[i].tId == taskId)
                ++pendingJobs;
            else if(tp->pStats[i].startTime 
                    && 
                    tp->pStats[i].heartbeatDisabled)
                ++pendingJobs;
        }
    }
    while(tp->pendingJobs > pendingJobs)
    {
        clOsalMutexUnlock(&tp->mutex);
        clOsalTaskDelay(delay);
        clOsalMutexLock(&tp->mutex);
    }
    if(tp->monitorTimer)
        clTimerStop(tp->monitorTimer);
    clOsalMutexUnlock(&tp->mutex);
    return CL_OK;
}

static ClRcT clTaskPoolMonitor(void *pArg)
{
    ClTaskPoolT *tp = pArg;
    ClInt32T i;
    ClTimeT threshold = 0;
    clOsalMutexLock(&tp->mutex);
    if(!tp->monitorActive)
    {
        goto out_unlock;
    }
    threshold = (ClTimeT)tp->monitorThreshold.tsSec*1000000;
    threshold += (ClTimeT)tp->monitorThreshold.tsMilliSec * 1000;
    for(i = 0; i < tp->maxTasks.value; ++i)
    {
        ClTimeT startTime = tp->pStats[i].startTime;
        ClTimeT currentTime = clOsalStopWatchTimeGet();
        if(startTime && (currentTime - startTime >= threshold))
        {
            ClTaskPoolMonitorCallbackT monitorCallback = tp->monitorCallback;
            ClRcT rc = CL_OK;

            clOsalMutexUnlock(&tp->mutex);
            if(monitorCallback && tp->pStats[i].tId && !tp->pStats[i].heartbeatDisabled)
            {
                clLogNotice("TASK", "MONITOR", 
                            "Task pool tid [%lld], task [%lld] seems to be deadlocked",
                            tp->pStats[i].tId, GET_TASK_ID(&tp->pStats[i]));
                if(tp->pStats[i].iocSendTime 
                   && 
                   (currentTime - tp->pStats[i].iocSendTime) >= (threshold>>1))
                {
                    clLogNotice("TASK", "MONITOR", "Task pool task [%lld] seems to be deadlocked in IOC send because of a "
                                "potential TIPC broadcast-link permanent congestion bug.",
                                GET_TASK_ID(&tp->pStats[i]));
                    clCpmWatchdogRestart(CL_TRUE);
                    /*
                     * unreached mostly depending on arch.
                     */
                }
                rc = monitorCallback(GET_TASK_ID(&tp->pStats[i]), currentTime - startTime, threshold);
            }
            clOsalMutexLock(&tp->mutex);
            /*  
             * Failure return code means that the task was killed.
             */
            if(rc != CL_OK)
            {
                tp->pStats[i].tId = 0;
                clMetricAdjust(&tp->numTasks, -1);
            }
        }
    }
    out_unlock:
    clOsalMutexUnlock(&tp->mutex);
    return CL_OK;
}

ClRcT clTaskPoolMonitorStart(ClTaskPoolHandleT handle, ClTimerTimeOutT monitorThreshold, ClTaskPoolMonitorCallbackT monitorCallback)
{
    ClTaskPoolT *tp = handle;
    ClTimerTimeOutT monitorInterval = {.tsSec = CL_TASKPOOL_DEFAULT_MONITOR_INTERVAL,
                                       .tsMilliSec = 0
    };
    ClTimeT monitorUnits = 0;
    ClTimeT monitorDefaultUnits = (ClTimeT)monitorInterval.tsSec * 1000 + monitorInterval.tsMilliSec;;
    ClRcT rc = CL_OK;

    if(!tp || !monitorCallback) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);

    if(!monitorThreshold.tsSec && !monitorThreshold.tsMilliSec)
    {
        monitorThreshold.tsSec = CL_TASKPOOL_DEFAULT_MONITOR_INTERVAL;
        monitorThreshold.tsMilliSec = 0;
    }

    monitorUnits = (ClTimeT)monitorThreshold.tsSec * 1000  + monitorThreshold.tsMilliSec;
    if(monitorUnits < monitorDefaultUnits)
        monitorInterval = monitorThreshold;

    memcpy(&tp->monitorThreshold, &monitorThreshold, sizeof(tp->monitorThreshold));

    clOsalMutexLock(&tp->mutex);

    if(!(tp->flags & CL_TASK_POOL_RUNNING))
    {
        rc = CL_TASKPOOL_RC(CL_ERR_NOT_EXIST);
        goto out_unlock;
    }

    tp->monitorCallback = monitorCallback;
    tp->monitorActive = CL_TRUE;
    if(!tp->monitorTimer)
    {
        rc = clTimerCreateAndStart(monitorInterval, CL_TIMER_REPETITIVE, CL_TIMER_SEPARATE_CONTEXT, 
                                   clTaskPoolMonitor, (void*)tp, &tp->monitorTimer);
    }
    else
    {
        rc = clTimerUpdate(tp->monitorTimer, monitorInterval);
    }

    if(rc != CL_OK)
    {
        tp->monitorCallback = NULL;
        goto out_unlock;
    }

    out_unlock:
    clOsalMutexUnlock(&tp->mutex);

    return rc;
}

ClRcT clTaskPoolMonitorStop(ClTaskPoolHandleT handle)
{
    ClRcT rc = CL_OK;
    ClTaskPoolT *tp = handle;

    if(!tp) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);
    
    clOsalMutexLock(&tp->mutex);
    if(tp->monitorTimer)
    {
        clTimerStop(tp->monitorTimer);
        tp->monitorActive = CL_FALSE;
    }
    clOsalMutexUnlock(&tp->mutex);

    return rc;
}

ClRcT clTaskPoolMonitorDelete(ClTaskPoolHandleT handle)
{
    ClRcT rc = CL_OK;
    ClTaskPoolT *tp = handle;
    ClTimerHandleT monitorTimer = 0;

    if(!tp) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);

    clOsalMutexLock(&tp->mutex);
    if( (monitorTimer = tp->monitorTimer) )
    {
        tp->monitorActive = CL_FALSE;
        tp->monitorTimer = 0;
    }
    clOsalMutexUnlock(&tp->mutex);
    if(monitorTimer)
    {
        /*
         * Synchronous delete. Won't return if the timer callback is running
         */
        clTimerDelete(&monitorTimer);
    }
    return rc;
}

static ClRcT taskPoolMonitorSet(ClBoolT disable)
{
    ClRcT rc = CL_OK;
    ClTaskPoolRefT *tRef = NULL;
    ClTaskPoolT *tp = NULL;
    ClOsalTaskIdT tid = 0;
    ClUint32T i;
    rc = clTaskPoolDataGet((ClPtrT*)&tRef);
    if(rc != CL_OK)
    {
        clLogError("MONITOR", "DISABLE", "Task pool monitor disable failed with [%#x]", rc);
        return rc;
    }
    if(!(tp = tRef->tp))
    {
        return CL_TASKPOOL_RC(CL_ERR_NOT_EXIST);
    }
    if((rc = clOsalSelfTaskIdGet(&tid)) != CL_OK)
        return rc;
    clOsalMutexLock(&tp->mutex);
    if(!(tp->flags & CL_TASK_POOL_RUNNING) || 
       !(tp->flags & CL_TASK_POOL_ACTIVE) || 
       !tp->monitorCallback || !tp->monitorTimer)
    {
        clOsalMutexUnlock(&tp->mutex);
        return CL_TASKPOOL_RC(CL_ERR_INVALID_STATE);
    }
    for(i = 0; i < tp->maxTasks.value; ++i)
    {
        if(tp->pStats[i].tId && tp->pStats[i].tId == tid)
        {
            if(disable)
            {
                if(!tp->pStats[i].heartbeatDisabled)
                    tp->pStats[i].heartbeatDisabled = CL_TRUE;
                else break;
            }
            else
            {
                if(tp->pStats[i].heartbeatDisabled)
                {
                    tp->pStats[i].heartbeatDisabled = CL_FALSE;
                    tp->pStats[i].startTime = clOsalStopWatchTimeGet();
                }
                else break;
            }
            clLogInfo("MONITOR", "SET", "%s task pool monitor for thread [%lld]",
                      disable ? "Disabling" : "Enabling", tid);
            goto out_unlock;
        }
    }
    rc = CL_TASKPOOL_RC(CL_ERR_NOT_EXIST);
    out_unlock:
    clOsalMutexUnlock(&tp->mutex);
    return rc;
}

/*
 * This disables the task pool monitoring for the current thread
 * Expected to be called from a rmd/idl processing thread context.
 * incase it decides to sleep indefinitely.
 */
ClRcT clTaskPoolMonitorDisable(void)
{
    return taskPoolMonitorSet(CL_TRUE);
}


/*
 * Enable the task pool monitoring for the disabled current thread in the task pool
 */
ClRcT clTaskPoolMonitorEnable(void)
{
    return taskPoolMonitorSet(CL_FALSE);
}

ClRcT clTaskPoolRecordIOCSend(ClBoolT start)
{
    ClRcT rc = CL_OK;
    ClTaskPoolRefT *tRef = NULL;
    ClTaskPoolT *tp = NULL;
    ClOsalTaskIdT tid = 0;
    ClUint32T i;

    rc = clTaskPoolDataGet((ClPtrT*)&tRef);

    if(rc != CL_OK || !(tp = tRef->tp)) return CL_OK; /*not in the task pool*/

    if((rc = clOsalSelfTaskIdGet(&tid)) != CL_OK)
        return rc;

    clOsalMutexLock(&tp->mutex);
    if(!(tp->flags & CL_TASK_POOL_RUNNING) || 
       !(tp->flags & CL_TASK_POOL_ACTIVE) || 
       !tp->monitorCallback || !tp->monitorTimer)
    {
        clOsalMutexUnlock(&tp->mutex);
        return CL_OK;
    }

    for(i = 0; i < tp->maxTasks.value; ++i)
    {
        if(tp->pStats[i].tId && tp->pStats[i].tId == tid)
        {
            if(start)
            {
                tp->pStats[i].iocSendTime = clOsalStopWatchTimeGet();
            }
            else
            {
                tp->pStats[i].iocSendTime = 0;
            }
            goto out_unlock;
        }
    }
    rc = CL_TASKPOOL_RC(CL_ERR_NOT_EXIST);
    out_unlock:
    clOsalMutexUnlock(&tp->mutex);
    return rc;
}

ClRcT clTaskPoolStatsGet(ClTaskPoolHandleT handle, ClTaskPoolUsageT *poolUsage)
{
    ClTaskPoolT *pool = (ClTaskPoolT*)handle;
    if(!pool || !poolUsage) return CL_TASKPOOL_RC(CL_ERR_INVALID_PARAMETER);
    clOsalMutexLock(&pool->mutex);
    poolUsage->numTasks = pool->numTasks.value;
    poolUsage->numIdleTasks = pool->numIdleTasks.value;
    poolUsage->maxTasks = pool->maxTasks.value;
    clOsalMutexUnlock(&pool->mutex);
    return CL_OK;
}
