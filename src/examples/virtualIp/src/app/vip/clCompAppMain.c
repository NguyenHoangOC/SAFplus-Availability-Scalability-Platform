/******************************************************************************
 *
 * clCompAppMain.c
 *
 ***************************** Legal Notice ***********************************
 *
 * This file is autogenerated by OpenClovis IDE, Copyright (C) 2002-2009 by 
 * OpenClovis. All rights reserved.
 *
 ***************************** Description ************************************
 *
 * This file provides a skeleton for writing a SAF aware component. Application
 * specific code should be added between the ---BEGIN_APPLICATION_CODE--- and
 * ---END_APPLICATION_CODE--- separators.
 *
 * Template Version: 1.0
 *
 *****************************************************************************/

/******************************************************************************
 * Include files needed to compile this file
 *****************************************************************************/

/*
 * POSIX Includes.
 */
#include <assert.h>

/*
 * Basic ASP Includes.
 */
#include <clCommon.h>

/*
 * ASP Client Includes.
 */
#include <clLogApi.h>

#include <clCpmApi.h>
#include <saAmf.h>

/*
 * ---BEGIN_APPLICATION_CODE---
 */
 
#include "clCompAppMain.h"
#include "../common/netwrapper.h"
ClRcT GetVaiFromCsi(const SaNameT       *compName,const unsigned char* name, VirtualIpAddress* ret);
ClRcT OperateOnAllCsis(const SaNameT       *compName,char* op,SaAmfCSIDescriptorT* csiDescriptor);
void HandleCsiChange(char* op, const SaNameT* compName, SaAmfCSIDescriptorT* csiDescriptor);

/*
 * ---END_APPLICATION_CODE---
 */

/******************************************************************************
 * Optional Features
 *****************************************************************************/

/*
 * This is necessary if the component wishes to provide a service that
 * will be used by other components.
 */

#if HAS_EO_SERVICES

extern ClRcT vip_EOClientInstall(void);

#endif

/*
 * This template has a few default clprintfs. These can be disabled by
 * changing clprintf to a null function
 */

#define clprintf(severity, ...)   clAppLog(CL_LOG_HANDLE_APP, severity, 10, \
                                  CL_LOG_AREA_UNSPECIFIED, CL_LOG_CONTEXT_UNSPECIFIED,\
                                  __VA_ARGS__)

/*
 * ---BEGIN_APPLICATION_CODE---
 */

/*
 * ---END_APPLICATION_CODE---
 */

/******************************************************************************
 * Global Variables.
 *****************************************************************************/

pid_t mypid;
SaAmfHandleT amfHandle;

ClBoolT unblockNow = CL_FALSE;

/*
 * ---BEGIN_APPLICATION_CODE---
 */

/*
 * Declare other global variables here.
 */

/*
 * ---END_APPLICATION_CODE---
 */

/*
 * Description of this EO
 */

ClEoConfigT clEoConfig =
{
    COMP_EO_NAME,               /* EO Name                                  */
    COMP_EO_THREAD_PRIORITY,    /* EO Thread Priority                       */
    COMP_EO_NUM_THREAD,         /* No of EO thread needed                   */
    COMP_IOC_PORT,              /* Required Ioc Port                        */
    COMP_EO_USER_CLIENT_ID, 
    COMP_EO_USE_THREAD_MODEL,   /* Thread Model                             */
    NULL,                       /* Application Initialize Callback          */
    NULL,                       /* Application Terminate Callback           */
    clCompAppStateChange,       /* Application State Change Callback        */
    clCompAppHealthCheck,       /* Application Health Check Callback        */
};

/*
 * Basic libraries used by this EO. The first 6 libraries are
 * mandatory, the others can be enabled or disabled by setting to
 * CL_TRUE or CL_FALSE.
 */

ClUint8T clEoBasicLibs[] =
{
    COMP_EO_BASICLIB_OSAL,      /* Lib: Operating System Adaptation Layer   */
    COMP_EO_BASICLIB_TIMER,     /* Lib: Timer                               */
    COMP_EO_BASICLIB_BUFFER,    /* Lib: Buffer Management                   */
    COMP_EO_BASICLIB_IOC,       /* Lib: Intelligent Object Communication    */
    COMP_EO_BASICLIB_RMD,       /* Lib: Remote Method Dispatch              */
    COMP_EO_BASICLIB_EO,        /* Lib: Execution Object                    */
    COMP_EO_BASICLIB_OM,        /* Lib: Object Management                   */
    COMP_EO_BASICLIB_HAL,       /* Lib: Hardware Adaptation Layer           */
    COMP_EO_BASICLIB_DBAL,      /* Lib: Database Adaptation Layer           */
};

/*
 * Client libraries used by this EO. All are optional and can be
 * enabled or disabled by setting to CL_TRUE or CL_FALSE.
 */

ClUint8T clEoClientLibs[] =
{
    COMP_EO_CLIENTLIB_COR,      /* Lib: Common Object Repository            */
    COMP_EO_CLIENTLIB_CM,       /* Lib: Chassis Management                  */
    COMP_EO_CLIENTLIB_NAME,     /* Lib: Name Service                        */
    COMP_EO_CLIENTLIB_LOG,      /* Lib: Log Service                         */
    COMP_EO_CLIENTLIB_TRACE,    /* Lib: Trace Service                       */
    COMP_EO_CLIENTLIB_DIAG,     /* Lib: Diagnostics                         */
    COMP_EO_CLIENTLIB_TXN,      /* Lib: Transaction Management              */
    CL_FALSE,                   /* NA */
    COMP_EO_CLIENTLIB_PROV,     /* Lib: Provisioning Management             */
    COMP_EO_CLIENTLIB_ALARM,    /* Lib: Alarm Management                    */
    COMP_EO_CLIENTLIB_DEBUG,    /* Lib: Debug Service                       */
    COMP_EO_CLIENTLIB_GMS,       /* Lib: Cluster/Group Membership Service    */
    COMP_EO_CLIENTLIB_PM        /* Lib: Performance Management              */
};

/******************************************************************************
 * Application Life Cycle Management Functions
 *****************************************************************************/

/*
 * main
 * -------------------
 * This function is invoked when the application is to be initialized.
 */

int main(int argc, char *argv[])
{
    SaNameT             appName = {0};
    SaAmfCallbacksT     callbacks;
    SaVersionT          version;
    ClIocPortT          iocPort;
    ClRcT               rc = SA_AIS_OK;

    SaSelectionObjectT dispatch_fd;
    fd_set read_fds;
    
    /*
     * ---BEGIN_APPLICATION_CODE---
     */

    /*
     * Declare other local variables here.
     */

    /*
     * ---END_APPLICATION_CODE---
     */

    /*
     * Get the pid for the process and store it in global variable.
     */

    mypid = getpid();

    /*
     * Initialize and register with CPM. 'version' specifies the
     * version of AMF with which this application would like to
     * interface. 'callbacks' is used to register the callbacks this
     * component expects to receive.
     */

    version.releaseCode  = 'B';
    version.majorVersion = 01;
    version.minorVersion = 01;
    
    callbacks.saAmfHealthcheckCallback          = NULL;
    callbacks.saAmfComponentTerminateCallback   = clCompAppTerminate;
    callbacks.saAmfCSISetCallback               = clCompAppAMFCSISet;
    callbacks.saAmfCSIRemoveCallback            = clCompAppAMFCSIRemove;
    callbacks.saAmfProtectionGroupTrackCallback = NULL;
        
    /*
     * Initialize AMF client library.
     */

    if ( (rc = saAmfInitialize(&amfHandle, &callbacks, &version)) != SA_AIS_OK) 
        goto errorexit;

    FD_ZERO(&read_fds);

    /*
     * Get the AMF dispatch FD for the callbacks
     */
    if ( (rc = saAmfSelectionObjectGet(amfHandle, &dispatch_fd)) != SA_AIS_OK)
        goto errorexit;
    
    FD_SET(dispatch_fd, &read_fds);

    
#if HAS_EO_SERVICES


    rc = vip_EOClientInstall();

#endif

    /*
     * Do the application specific initialization here.
     */

    /*
     * ---BEGIN_APPLICATION_CODE---
     */

    // ...

    /*
     * ---END_APPLICATION_CODE---
     */

    /*
     * Now register the component with AMF. At this point it is
     * ready to provide service, i.e. take work assignments.
     */

    if ( (rc = saAmfComponentNameGet(amfHandle, &appName)) != SA_AIS_OK) 
        goto errorexit;
    if ( (rc = saAmfComponentRegister(amfHandle, &appName, NULL)) != SA_AIS_OK) 
        goto errorexit;

    /*
     * Print out standard information for this component.
     */

    clEoMyEoIocPortGet(&iocPort);
    
    clprintf (CL_LOG_SEV_INFO, "Component [%s] : PID [%d]. Initializing\n", appName.value, mypid);
    clprintf (CL_LOG_SEV_INFO, "   IOC Address             : 0x%x\n", clIocLocalAddressGet());
    clprintf (CL_LOG_SEV_INFO, "   IOC Port                : 0x%x\n", iocPort);

    /*
     * ---BEGIN_APPLICATION_CODE---
     */

    // ...

    /*
     * ---END_APPLICATION_CODE---
     */

    /*
     * Block on AMF dispatch file descriptor for callbacks
     */
    do
    {
        if( select(dispatch_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
        {
		    clprintf (CL_LOG_SEV_ERROR, "Error in select()");
			perror("");
            break;
        }
        saAmfDispatch(amfHandle, SA_DISPATCH_ALL);
    }while(!unblockNow);      

    /*
     * Do the application specific finalization here.
     */

    /*
     * ---BEGIN_APPLICATION_CODE---
     */

    // ...

    /*
     * ---END_APPLICATION_CODE---
     */

    if((rc = saAmfFinalize(amfHandle)) != SA_AIS_OK)
	{
        clprintf (CL_LOG_SEV_ERROR, "AMF finalization error[0x%X]", rc);
	}

    clprintf (CL_LOG_SEV_INFO, "AMF Finalized");
    
    return 0;

errorexit:

    clprintf (CL_LOG_SEV_ERROR, "Component [%s] : PID [%d]. Initialization error [0x%x]\n",
              appName.value, mypid, rc);

    return -1;
}


/*
 * clCompAppTerminate
 * ------------------
 * This function is invoked when the application is to be terminated.
 */

void clCompAppTerminate(SaInvocationT invocation, const SaNameT *compName)
{
    SaAisErrorT rc = SA_AIS_OK;

    clprintf (CL_LOG_SEV_INFO, "Component [%s] : PID [%d]. Terminating\n",
              compName->value, mypid);

    /*
     * ---BEGIN_APPLICATION_CODE--- 
     */

    // ...

    /*
     * ---END_APPLICATION_CODE---
     */
    
    /*
     * Unregister with AMF and respond to AMF saying whether the
     * termination was successful or not.
     */

    if ( (rc = saAmfComponentUnregister(amfHandle, compName, NULL)) != SA_AIS_OK)
        goto errorexit;

    saAmfResponse(amfHandle, invocation, SA_AIS_OK);

    clprintf (CL_LOG_SEV_INFO, "Component [%s] : PID [%d]. Terminated\n",
              compName->value, mypid);

    unblockNow = CL_TRUE;
    
    return;

errorexit:

    clprintf (CL_LOG_SEV_ERROR, "Component [%s] : PID [%d]. Termination error [0x%x]\n",
              compName->value, mypid, rc);

    return;
}

/*
 * clCompAppStateChange
 * ---------------------
 * This function is invoked to change the state of an EO.
 *
 * WARNING: This function is deprecated, and may not be supported in the future.
 * Usage of this feature is discouraged.
 */

ClRcT clCompAppStateChange(ClEoStateT eoState)
{
    switch (eoState)
    {
        case CL_EO_STATE_SUSPEND:
        {
            /*
             * ---BEGIN_APPLICATION_CODE---
             */

            // ...

            /*
             * ---END_APPLICATION_CODE---
             */

            break;
        }

        case CL_EO_STATE_RESUME:
        {
            /*
             * ---BEGIN_APPLICATION_CODE---
             */

            // ...

            /*
             * ---END_APPLICATION_CODE---
             */

            break;
        }
        
        default:
        {
            break;
        }
    }
 
    return CL_OK;
}

/*
 * clCompAppHealthCheck
 * --------------------
 * This function is invoked to perform a healthcheck on the application. The
 * health check logic is application specific.
 */

ClRcT clCompAppHealthCheck(ClEoSchedFeedBackT* schFeedback)
{
    /*
     * Add code for application specific health check below. The
     * defaults indicate EO is healthy and polling interval is
     * unaltered.
     */

    /*
     * ---BEGIN_APPLICATION_CODE---
     */
    
    schFeedback->freq   = CL_EO_DEFAULT_POLL; 
    schFeedback->status = CL_CPM_EO_ALIVE;

    /*
     * ---END_APPLICATION_CODE---
     */

    return CL_OK;
}

/******************************************************************************
 * Application Work Assignment Functions
 *****************************************************************************/

/*
 * clCompAppAMFCSISet
 * ------------------
 * This function is invoked when a CSI assignment is made or the state
 * of a CSI is changed.
 */

void clCompAppAMFCSISet(SaInvocationT       invocation,
                        const SaNameT       *compName,
                        SaAmfHAStateT       haState,
                        SaAmfCSIDescriptorT csiDescriptor)
{
    /*
     * ---BEGIN_APPLICATION_CODE--- 
     */

    // ...

    /*
     * ---END_APPLICATION_CODE---
     */

    /*
     * Print information about the CSI Set
     */

    clprintf (CL_LOG_SEV_INFO, "Component [%s] : PID [%d]. CSI Set Received\n", 
              compName->value, mypid);

    clCompAppAMFPrintCSI(csiDescriptor, haState);

    /*
     * Take appropriate action based on state
     */

    switch ( haState )
    {
        case SA_AMF_HA_ACTIVE:
        {
            /*
             * AMF has requested application to take the active HA state 
             * for the CSI.
             */

            /*
             * ---BEGIN_APPLICATION_CODE---
             */
            HandleCsiChange("up",compName,&csiDescriptor);

            /*
             * ---END_APPLICATION_CODE---
             */

            saAmfResponse(amfHandle, invocation, SA_AIS_OK);
            break;
        }

        case SA_AMF_HA_STANDBY:
        {
            /*
             * AMF has requested application to take the standby HA state 
             * for this CSI.
             */

            /*
             * ---BEGIN_APPLICATION_CODE---
             */

            HandleCsiChange("down",compName,&csiDescriptor);

            /*
             * ---END_APPLICATION_CODE---
             */

            saAmfResponse(amfHandle, invocation, SA_AIS_OK);
            break;
        }

        case SA_AMF_HA_QUIESCED:
        {
            /*
             * AMF has requested application to quiesce the CSI currently
             * assigned the active or quiescing HA state. The application 
             * must stop work associated with the CSI immediately.
             */

            /*
             * ---BEGIN_APPLICATION_CODE---
             */

            HandleCsiChange("down",compName,&csiDescriptor);

            /*
             * ---END_APPLICATION_CODE---
             */

            saAmfResponse(amfHandle, invocation, SA_AIS_OK);
            break;
        }

        case SA_AMF_HA_QUIESCING:
        {
            /*
             * AMF has requested application to quiesce the CSI currently
             * assigned the active HA state. The application must stop work
             * associated with the CSI gracefully and not accept any new
             * workloads while the work is being terminated.
             */

            /*
             * ---BEGIN_APPLICATION_CODE---
             */
            HandleCsiChange("down",compName,&csiDescriptor);  

            /*
             * ---END_APPLICATION_CODE---
             */

            saAmfCSIQuiescingComplete(amfHandle, invocation, SA_AIS_OK);
            break;
        }

        default:
        {
            assert(0);
            break;
        }
    }

    return;
}

/*
 * clCompAppAMFCSIRemove
 * ---------------------
 * This function is invoked when a CSI assignment is to be removed.
 */

void clCompAppAMFCSIRemove(SaInvocationT  invocation,
                           const SaNameT  *compName,
                           const SaNameT  *csiName,
                           SaAmfCSIFlagsT csiFlags)
{
    clprintf (CL_LOG_SEV_INFO, "Component [%s] : PID [%d]. CSI Remove Received\n", 
              compName->value, mypid);

    clprintf (CL_LOG_SEV_INFO, "   CSI                     : %s\n", csiName->value);
    clprintf (CL_LOG_SEV_INFO, "   CSI Flags               : 0x%d\n", csiFlags);

    /*
     * Add application specific logic for removing the work for this CSI.
     */

    /*
     * ---BEGIN_APPLICATION_CODE---
     */
    if (1)
    {
    VirtualIpAddress temp;
    if (CL_OK == GetVaiFromCsi(compName,csiName->value, &temp)) AddRemVirtualAddress("down",&temp);
    }

    /*
     * ---END_APPLICATION_CODE---
     */

    saAmfResponse(amfHandle, invocation, SA_AIS_OK);

    return;
}

/******************************************************************************
 * Utility functions 
 *****************************************************************************/

/*
 * clCompAppAMFPrintCSI
 * --------------------
 * Print information received in a CSI set request.
 */

void clCompAppAMFPrintCSI(SaAmfCSIDescriptorT csiDescriptor,
                          SaAmfHAStateT haState)
{
    clprintf (CL_LOG_SEV_INFO,
              "CSI Flags : [%s]",
              STRING_CSI_FLAGS(csiDescriptor.csiFlags));

    if (SA_AMF_CSI_TARGET_ALL != csiDescriptor.csiFlags)
    {
        clprintf (CL_LOG_SEV_INFO, "CSI Name : [%s]", 
                  csiDescriptor.csiName.value);
    }

    if (SA_AMF_CSI_ADD_ONE == csiDescriptor.csiFlags)
    {
        ClUint32T i = 0;
        
        clprintf (CL_LOG_SEV_INFO, "Name value pairs :");
        for (i = 0; i < csiDescriptor.csiAttr.number; i++)
        {
            clprintf (CL_LOG_SEV_INFO, "Name : [%s]",
                      csiDescriptor.csiAttr.
                      attr[i].attrName);
            clprintf (CL_LOG_SEV_INFO, "Value : [%s]",
                      csiDescriptor.csiAttr.
                      attr[i].attrValue);
        }
    }
    
    clprintf (CL_LOG_SEV_INFO, "HA state : [%s]",
              STRING_HA_STATE(haState));

    if (SA_AMF_HA_ACTIVE == haState)
    {
        clprintf (CL_LOG_SEV_INFO, "Active Descriptor :");
        clprintf (CL_LOG_SEV_INFO,
                  "Transition Descriptor : [%d]",
                  csiDescriptor.csiStateDescriptor.
                  activeDescriptor.transitionDescriptor);
        clprintf (CL_LOG_SEV_INFO,
                  "Active Component : [%s]",
                  csiDescriptor.csiStateDescriptor.
                  activeDescriptor.activeCompName.value);
    }
    else if (SA_AMF_HA_STANDBY == haState)
    {
        clprintf (CL_LOG_SEV_INFO, "Standby Descriptor :");
        clprintf (CL_LOG_SEV_INFO,
                  "Standby Rank : [%d]",
                  csiDescriptor.csiStateDescriptor.
                  standbyDescriptor.standbyRank);
        clprintf (CL_LOG_SEV_INFO, "Active Component : [%s]",
                  csiDescriptor.csiStateDescriptor.
                  standbyDescriptor.activeCompName.value);
    }
}

/*
 * ---BEGIN_APPLICATION_CODE---
 */
/*
 * Insert any other utility functions here.
 */
void clNameFromSaName(ClNameT* dst, const SaNameT* src)
{
    dst->length = src->length;
    memcpy(dst->value,src->value,src->length);
    if (dst->length < CL_MAX_NAME_LENGTH)  /* NULL terminate if possible */
    {
        dst->value[dst->length] = 0;
    }
    
}


ClRcT GetVaiFromCsi(const SaNameT       *compName,const unsigned char* name, VirtualIpAddress* ret)
{
    ClRcT rc;
    ClCpmCompCSIRefT csiRef = { 0 };
    ClInt32T i;
    ClNameT clname;

    clNameFromSaName(&clname,compName);
           
    rc = clCpmCompCSIList(&clname, &csiRef);
    if(rc != CL_OK)
    {
        clLogError("APP", "CSISET", "Comp CSI get returned [%#x]", rc);
        return rc;
    }
    else
    {

        for(i = 0; i < csiRef.numCSIs; ++i)
        {
            ClCpmCompCSIT *pCSI = &csiRef.pCSIList[i];

            if (strncmp(pCSI->csiDescriptor.csiName.value,(const char*) name, pCSI->csiDescriptor.csiName.length) ==0)
            {                        

                clprintf(CL_LOG_SEV_INFO,"Comp [%.*s], CSI [%.*s], HA state [%s]",
                         compName->length,
                         compName->value, 
                         pCSI->csiDescriptor.csiName.length,
                         pCSI->csiDescriptor.csiName.value,
                         STRING_HA_STATE(pCSI->haState));

                GetVirtualAddressInfoAsp(&pCSI->csiDescriptor, ret);
                /*clprintf(CL_LOG_SEV_INFO,"Found VIP: %s %s %s", ret.ip,ret.netmask,ret.dev); */
                if(csiRef.pCSIList) clHeapFree(csiRef.pCSIList);
                return CL_OK;
            }                    

        }
        if(csiRef.pCSIList) clHeapFree(csiRef.pCSIList);
    }

    return CL_ERR_NOT_EXIST;
}


ClRcT OperateOnAllCsis(const SaNameT       *compName,char* op,SaAmfCSIDescriptorT* csiDescriptor)
{    
    ClRcT rc;
    ClCpmCompCSIRefT csiRef = { 0 };
    ClInt32T i;
    ClNameT clname;

    clNameFromSaName(&clname,compName);

    rc = clCpmCompCSIList(&clname, &csiRef);
    if(rc != CL_OK)
    {
        clLogError("APP", "CSISET", "Comp CSI get returned [%#x]", rc);
    }
    else
    {
        for(i = 0; i < csiRef.numCSIs; ++i)
        {
            VirtualIpAddress temp;
            ClCpmCompCSIT *pCSI = &csiRef.pCSIList[i];

            clprintf(CL_LOG_SEV_INFO,"Comp [%.*s], CSI [%.*s], HA state [%s]",
                     compName->length,
                     compName->value, 
                     pCSI->csiDescriptor.csiName.length,
                     pCSI->csiDescriptor.csiName.value,
                     STRING_HA_STATE(pCSI->haState));

            GetVirtualAddressInfoAsp(&pCSI->csiDescriptor, &temp);
            clprintf(CL_LOG_SEV_INFO,"VIP %s: %s %s %s", op, temp.ip,temp.netmask,temp.dev);
            AddRemVirtualAddress(op,&temp);

        }
        if(csiRef.pCSIList)
            clHeapFree(csiRef.pCSIList);
    }
    return CL_OK;
    
}

void HandleCsiChange(char* op,const SaNameT* compName, SaAmfCSIDescriptorT* csiDescriptor)
{
    
            if (csiDescriptor->csiFlags == CL_AMS_CSI_FLAG_TARGET_ALL)
            {
                OperateOnAllCsis(compName,op,csiDescriptor);
            }
            else if (csiDescriptor->csiFlags == CL_AMS_CSI_FLAG_TARGET_ONE)
            {
                VirtualIpAddress temp;
                if (CL_OK == GetVaiFromCsi(compName,csiDescriptor->csiName.value, &temp)) AddRemVirtualAddress(op,&temp);        
            }        
            else /* CL_AMS_CSI_FLAG_ADD_ONE */
            {
                VirtualIpAddress temp;
                GetVirtualAddressInfo(csiDescriptor, &temp);
                clprintf(CL_LOG_SEV_INFO,"Add one VIP: %s %s %s", temp.ip,temp.netmask,temp.dev);
                AddRemVirtualAddress(op,&temp);
            }
}

/*
 * ---END_APPLICATION_CODE---
 */
