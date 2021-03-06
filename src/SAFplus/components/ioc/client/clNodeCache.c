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

/**
 * This file implements node version cache for ASP nodes.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <clNodeCache.h>
#include <clCpmExtApi.h>

#define CL_NODE_CACHE_SEGMENT "/CL_NODE_CACHE"

#define CL_CACHE_ALIGN(v, a)  ( ((v)+(a)-1) & ~((a)-1) )
#define CL_NODE_CACHE_ALIGN(v)  CL_CACHE_ALIGN(v, 8)

#define CL_NODE_CACHE_SEGMENT_SIZE  CL_NODE_CACHE_ALIGN((ClUint32T)(sizeof(ClNodeCacheHeaderT)+CL_IOC_MAX_NODES*sizeof(ClNodeCacheEntryT)))

#define CL_NODE_MAP_SIZE  (CL_CACHE_ALIGN(CL_IOC_MAX_NODES, 8) >> 3 )
#define CL_NODE_MAP_WORDS (CL_CACHE_ALIGN(CL_NODE_MAP_SIZE, 4) >> 2 )

/*
 *Faster to operate on words than bytes.
 */
#define CL_NODE_MAP_SET(map, bit)  do {         \
    (map)[ (bit) >> 5] |= ( 1 << ((bit) & 31) ) ;	\
}while(0)

#define CL_NODE_MAP_CLEAR(map, bit) do {        \
    (map)[ (bit) >> 5 ] &= ~(1 << ((bit) & 31));	\
}while(0)

#define CL_NODE_MAP_TEST(map, bit) \
  ( (map)[(bit) >> 5] & ( 1 << ((bit) & 31) ) ) ? 1 : 0

#define CL_NODE_CACHE_HEADER_BASE(base) ( (ClNodeCacheHeaderT*)(base) )
#define CL_NODE_CACHE_ENTRY_BASE(base)  ( (ClNodeCacheEntryT*) ( (ClNodeCacheHeaderT*)base + 1 ) )

static ClUint8T *gpClNodeCache;
static ClCharT gClNodeCacheSegment[CL_MAX_NAME_LENGTH+1];
static ClFdT gClNodeCacheFd;
static ClOsalSemIdT gClNodeCacheSem;
static ClBoolT gClNodeCacheOwner;

typedef struct ClNodeCacheHeader
{
    ClInt32T maxNodes;
    ClInt32T currentNodes;
    ClIocNodeAddressT minVersionNode;
    ClUint32T minVersion;
    ClIocNodeAddressT currentLeader;
    ClUint32T nodeMap[CL_NODE_MAP_WORDS];
}ClNodeCacheHeaderT;

typedef struct ClNodeCacheEntry
{
    ClUint32T version; /*entry address is the index into the segment.*/
    ClUint32T capability; /* node capability mask */
    ClCharT nodeName[CL_NODE_CACHE_NODENAME_MAX];
}ClNodeCacheEntryT;

static ClBoolT gClAspNativeLeaderElection;

static ClRcT clNodeCacheCreate(void)
{
    ClRcT rc = CL_OK;
    ClFdT fd;

    clOsalShmUnlink(gClNodeCacheSegment);
    rc = clOsalShmOpen(gClNodeCacheSegment, O_RDWR | O_CREAT | O_EXCL, 0666, &fd);
    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Node cache shm open of segment [%s] returned [%#x]",
                   gClNodeCacheSegment, rc);
        goto out;
    }

    rc = clOsalFtruncate(fd, CL_NODE_CACHE_SEGMENT_SIZE);
    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Node cache segment truncate of size [%d] returned [%#x]",
                   (ClUint32T) CL_NODE_CACHE_SEGMENT_SIZE, rc);
        goto out_unlink;
    }

    rc = clOsalMmap(0, CL_NODE_CACHE_SEGMENT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0, 
                    (ClPtrT*)&gpClNodeCache);
    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Node cache segment mmap returned [%#x]", rc);
        goto out_unlink;
    }

    gClNodeCacheFd = fd;

    rc = clOsalSemCreate((ClUint8T*)gClNodeCacheSegment, 1, &gClNodeCacheSem);

    if(rc != CL_OK)
    {
        ClOsalSemIdT semId = 0;

        if(clOsalSemIdGet((ClUint8T*)gClNodeCacheSegment, &semId) != CL_OK)
        {
            clLogError("NODE", "CACHE", "Node cache segment sem creation error while fetching sem id");
            goto out_unlink;
        }

        if(clOsalSemDelete(semId) != CL_OK)
        {
            clLogError("NODE", "CACHE", "Node cache segment sem creation error while deleting old sem id");
            goto out_unlink;
        }

        rc = clOsalSemCreate((ClUint8T*)gClNodeCacheSegment, 1, &gClNodeCacheSem);
    }

    return rc;

    out_unlink:
    clOsalShmUnlink(gClNodeCacheSegment);
    close((ClInt32T)fd);

    out:
    return rc;
}
    
static ClRcT clNodeCacheOpen(void)
{
    ClRcT rc = CL_OK;
    ClFdT fd;
    
    rc = clOsalShmOpen(gClNodeCacheSegment, O_RDWR, 0666, &fd);
    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Node cache [%s] segment open returned [%#x]",
                   gClNodeCacheSegment, rc);
        goto out;
    }

    rc = clOsalMmap(0, CL_NODE_CACHE_SEGMENT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0, 
                    (ClPtrT*)&gpClNodeCache);

    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Node cache segment mmap returned [%#x]", rc);
        close((ClInt32T)fd);
        goto out;
    }

    gClNodeCacheFd = fd;

    rc = clOsalSemIdGet((ClUint8T*)gClNodeCacheSegment, &gClNodeCacheSem);

    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Node cache semid get returned [%#x]", rc);
        close((ClInt32T)fd);
    }

    out:
    return rc;
}

ClRcT clNodeCacheInitialize(ClBoolT createFlag)
{
    ClRcT rc = CL_OK;
    ClUint32T capability = 0;
    ClNameT nodeName = {0};

    if(gpClNodeCache)
        goto out;

    gClAspNativeLeaderElection = !clParseEnvBoolean("CL_ASP_OPENAIS_LEADER_ELECTION");

    snprintf(gClNodeCacheSegment, sizeof(gClNodeCacheSegment)-1,
             "%s_%d", CL_NODE_CACHE_SEGMENT, clIocLocalAddressGet());

    if(createFlag == CL_TRUE)
    {
        rc = clNodeCacheCreate();
    }
    else
    {
        rc = clNodeCacheOpen();
    }
    
    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Segment initialize returned [%#x]", rc);
        goto out;
    }

    CL_ASSERT(gpClNodeCache != NULL);

    if(!createFlag)
    {
        goto out;
    }

    CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->maxNodes = CL_IOC_MAX_NODES;
    CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentNodes = 0;
    memset(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap, 0, 
           sizeof(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap));

    if(clCpmIsSC())
    {
        CL_NODE_CACHE_SC_CAPABILITY_SET(capability);
        CL_NODE_CACHE_SC_PROMOTE_CAPABILITY_SET(capability);
    }
    else if(clCpmIsSCCapable())
    {
        CL_NODE_CACHE_SC_PROMOTE_CAPABILITY_SET(capability);
        CL_NODE_CACHE_PL_CAPABILITY_SET(capability);
    }
    else
    {
        CL_NODE_CACHE_PL_CAPABILITY_SET(capability);
    }
    
    clCpmLocalNodeNameGet(&nodeName);

    clNodeCacheUpdate(clIocLocalAddressGet(), 
                      CL_VERSION_CODE(5, 0, 0), capability,
                      &nodeName);

    rc = clOsalMsync(gpClNodeCache, CL_NODE_CACHE_SEGMENT_SIZE, MS_ASYNC);
    if(rc != CL_OK)
    {
        clLogError("NODE", "CACHE", "Node cache segment msync returned [%#x]", rc);
        goto out;
    }

    gClNodeCacheOwner = CL_TRUE;

    out:
    return rc;
}

ClRcT clNodeCacheFinalize(void)
{
    ClRcT rc = CL_ERR_NOT_INITIALIZED;
    ClBoolT cacheOwner = CL_FALSE;

    if(!gpClNodeCache)
        goto out;

    clOsalSemLock(gClNodeCacheSem);
    if( (cacheOwner = gClNodeCacheOwner) )
    {
        clOsalMsync(gpClNodeCache, CL_NODE_CACHE_SEGMENT_SIZE, MS_SYNC);
    }

    clOsalMunmap(gpClNodeCache, CL_NODE_CACHE_SEGMENT_SIZE);

    gpClNodeCache = NULL;

    clOsalSemUnlock(gClNodeCacheSem);

    if(cacheOwner)
    {
        clOsalShmUnlink(gClNodeCacheSegment);
        clOsalSemDelete(gClNodeCacheSem);
    }

    out:    
    return rc;
}

/*
 * Do a bitmap scan to update the cache header used for min. version gets on
 * resets of min nodeaddress.
 */

static void nodeCacheMinVersionSet(void)
{
    ClInt32T i;
    ClInt32T j;
    ClIocNodeAddressT minVersionNode = CL_IOC_BROADCAST_ADDRESS;
    ClUint32T minVersion = (ClUint32T)-1;

    for(i = 0; i < CL_NODE_MAP_WORDS; ++i)
    {
        ClUint32T mask = CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap[i];
        if(!mask) continue;
        for(j = 0; j < 32; ++j)
        {
            ClIocNodeAddressT node = (i << 5) + j;
            if( (mask & ( 1 << j)) )
            {
                ClUint32T version = CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[node].version;
                if(version && version < minVersion)
                {
                    minVersion = version;
                    minVersionNode = node;
                }
            }
        }
    }

    if(minVersion != (ClUint32T)-1)
    {
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersionNode = minVersionNode;
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion = minVersion;
    }
    else
    {
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersionNode = 0;
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion = 0;
    }

}

static ClRcT nodeCacheViewGetWithFilterFast(ClNodeCacheMemberT *pMembers, 
                                            ClUint32T *pMaxMembers, ClUint32T capFilter,
                                            ClBoolT compat)
{
    ClInt32T i;
    ClInt32T j;
    ClUint32T maxMembers = 0;
    ClUint32T currentMembers = 0;

    if(!pMembers || !pMaxMembers || !(maxMembers = *pMaxMembers))
        return CL_ERR_INVALID_PARAMETER;

    if(!gpClNodeCache)
        return CL_ERR_NOT_INITIALIZED;

    if(compat)
    {
        ClUint32T minVersion = CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion;
        if(minVersion && minVersion < CL_VERSION_CODE(5, 0, 0))
        {
            *pMaxMembers = 0;
            return CL_OK;
        }
        if(!gClAspNativeLeaderElection)
        {
            *pMaxMembers = 0;
            return CL_OK;
        }
    }

    if(!capFilter) capFilter = ~capFilter;

    for(i = 0; i < CL_NODE_MAP_WORDS; ++i)
    {
        ClUint32T mask = CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap[i];
        if(!mask) continue;
        for(j = 0; j < 32; ++j)
        {
            ClIocNodeAddressT node = (i << 5) + j;
            if( (mask & ( 1 << j)) )
            {
                if((CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[node].capability & capFilter))
                {
                    pMembers[currentMembers].address = node;
                    pMembers[currentMembers].version = CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[node].version;
                    pMembers[currentMembers].capability = CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[node].capability;
                    pMembers[currentMembers].name[0] = 0;
                    strncat(pMembers[currentMembers].name,
                            CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[node].nodeName,
                            sizeof(pMembers[currentMembers].name)-1);

                    if(++currentMembers >= maxMembers)
                        goto out;
                }
            }
        }
    }

    out:
    *pMaxMembers = currentMembers;
    return CL_OK;
}

ClRcT clNodeCacheViewGetWithFilterFast(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers, ClUint32T capFilter)
{
    return nodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, capFilter, CL_FALSE);
}

ClRcT clNodeCacheViewGetWithFilterFastSafe(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers, ClUint32T capFilter)
{
    return nodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, capFilter, CL_TRUE);
}

ClRcT clNodeCacheViewGetWithFilter(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers,
                                   ClUint32T capMask)
{
    ClRcT rc = CL_OK;
    clOsalSemLock(gClNodeCacheSem);
    rc = nodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, capMask, CL_FALSE);
    clOsalSemUnlock(gClNodeCacheSem);
    return rc;
}

ClRcT clNodeCacheViewGetWithFilterSafe(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers,
                                       ClUint32T capMask)
{
    ClRcT rc = CL_OK;
    clOsalSemLock(gClNodeCacheSem);
    rc = nodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, capMask, CL_TRUE);
    clOsalSemUnlock(gClNodeCacheSem);
    return rc;
}

ClRcT clNodeCacheViewGet(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers)
{
    ClRcT rc = CL_OK;
    clOsalSemLock(gClNodeCacheSem);
    rc = nodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, 0, CL_FALSE);
    clOsalSemUnlock(gClNodeCacheSem);
    return rc;
}

ClRcT clNodeCacheViewGetSafe(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers)
{
    ClRcT rc = CL_OK;
    clOsalSemLock(gClNodeCacheSem);
    rc = nodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, 0, CL_TRUE);
    clOsalSemUnlock(gClNodeCacheSem);
    return rc;
}

ClRcT clNodeCacheViewGetFast(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers)
{
    return clNodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, 0);
}

/*  
 * Check for the min. vesion of the code to be the supported one before returning the view.
 * for the sake of backward compat.
 */
ClRcT clNodeCacheViewGetFastSafe(ClNodeCacheMemberT *pMembers, ClUint32T *pMaxMembers)
{
    return nodeCacheViewGetWithFilterFast(pMembers, pMaxMembers, 0, CL_TRUE);
}

static ClRcT nodeCacheMemberGetFast(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember,
                                    ClBoolT compat)
{
    ClRcT rc = CL_OK;

    if(!node || node >= CL_IOC_MAX_NODES || !pMember)
        return CL_ERR_INVALID_PARAMETER;

    if(!gpClNodeCache)
        return CL_ERR_NOT_INITIALIZED;

    if(compat)
    {
        ClUint32T minVersion = CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion;
        if(minVersion && minVersion < CL_VERSION_CODE(5, 0, 0))
        {
            return CL_ERR_NOT_SUPPORTED;
        }
        if(!gClAspNativeLeaderElection)
        {
            return CL_ERR_NOT_SUPPORTED;
        }
    }

    if(CL_NODE_MAP_TEST(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap, node))
    {
        ClNodeCacheEntryT *entry = &CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[node];
        pMember->address = node;
        pMember->name[0] = 0;
        strncat(pMember->name, entry->nodeName, sizeof(entry->nodeName)-1);
        pMember->version = entry->version;
        pMember->capability = entry->capability;
    }
    else
    {
        rc = CL_ERR_NOT_EXIST;
    }
    return rc;
}

ClRcT clNodeCacheMemberGetFast(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember)
{
    return nodeCacheMemberGetFast(node, pMember, CL_FALSE);
}

ClRcT clNodeCacheMemberGetFastSafe(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember)
{
    return nodeCacheMemberGetFast(node, pMember, CL_TRUE);
}

ClRcT clNodeCacheMemberGet(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember)
{
    ClRcT rc;
    clOsalSemLock(gClNodeCacheSem);
    rc = nodeCacheMemberGetFast(node, pMember, CL_FALSE);
    clOsalSemUnlock(gClNodeCacheSem);
    return rc;
}

ClRcT clNodeCacheMemberGetSafe(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember)
{
    ClRcT rc;
    clOsalSemLock(gClNodeCacheSem);
    rc = nodeCacheMemberGetFast(node, pMember, CL_TRUE);
    clOsalSemUnlock(gClNodeCacheSem);
    return rc;
}

static ClRcT nodeCacheMemberGetExtended(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember,
                                        ClUint32T retries, ClUint32T msecDelay,
                                        ClBoolT compat)
{
    ClRcT rc = CL_OK;
    ClUint32T i = 0;
    ClTimerTimeOutT delay = {.tsSec = 0, .tsMilliSec = msecDelay};
    clOsalSemLock(gClNodeCacheSem);
    while(i++ <= retries)
    {
        rc = nodeCacheMemberGetFast(node, pMember, compat);
        if(CL_GET_ERROR_CODE(rc) == CL_ERR_NOT_EXIST)
        {
            clOsalSemUnlock(gClNodeCacheSem);
            if(i > retries)
                goto out;
            clOsalTaskDelay(delay);
            clOsalSemLock(gClNodeCacheSem);
        }
        else break;
    }
    clOsalSemUnlock(gClNodeCacheSem);

    out:
    return rc;
}

ClRcT clNodeCacheMemberGetExtended(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember,
                                   ClUint32T retries, ClUint32T msecDelay)
{
    return nodeCacheMemberGetExtended(node, pMember, retries, msecDelay, CL_FALSE);
}

ClRcT clNodeCacheMemberGetExtendedSafe(ClIocNodeAddressT node, ClNodeCacheMemberT *pMember,
                                       ClUint32T retries, ClUint32T msecDelay)
{
    return nodeCacheMemberGetExtended(node, pMember, retries, msecDelay, CL_TRUE);
}

ClRcT clNodeCacheUpdate(ClIocNodeAddressT nodeAddress, ClUint32T version, 
                        ClUint32T capability, ClNameT *nodeName)
{
    ClRcT   rc = CL_OK;
    ClNodeCacheEntryT *entry;

    if(nodeAddress >= CL_IOC_MAX_NODES) return CL_ERR_INVALID_PARAMETER;

    rc = clOsalSemLock(gClNodeCacheSem);
    if (rc != CL_OK)
        return rc;

    if(!gpClNodeCache)
    {
        clOsalSemUnlock(gClNodeCacheSem);
        return CL_ERR_NOT_INITIALIZED;
    }

    if(!CL_NODE_MAP_TEST(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap, nodeAddress))
    {
        CL_NODE_MAP_SET(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap, nodeAddress);
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentNodes += 1;
    }

    entry = &CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress];
    if(version)
        entry->version = version;
    if(capability)
        entry->capability = capability;
    if(nodeName)
    {
        entry->nodeName[0] = 0;
        strncat(entry->nodeName, (const ClCharT*)nodeName->value, 
                CL_MIN(nodeName->length, sizeof(entry->nodeName)-1));
    }
    clLogNotice("CACHE", "SET", "Updating node cache entry for node [%d: %s] with version [%#x], capability [%#x]",
                nodeAddress, entry->nodeName, entry->version, entry->capability);

    /*
     * Update node min version/address.
     */
    if(version && 
       (!CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion || 
        version < CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion))
    {
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion = version;
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersionNode = nodeAddress;
    }

    clOsalSemUnlock(gClNodeCacheSem);

    return CL_OK;
}

static ClRcT __nodeCacheReset(ClIocNodeAddressT nodeAddress, ClBoolT soft)
{
    ClRcT   rc = CL_OK;

    if(nodeAddress >= CL_IOC_MAX_NODES) return CL_ERR_INVALID_PARAMETER;

    rc = clOsalSemLock(gClNodeCacheSem);
    if (rc != CL_OK)
        return rc;

    if(!gpClNodeCache)
    {
        clOsalSemUnlock(gClNodeCacheSem);
        return CL_ERR_NOT_INITIALIZED;
    }

    if(CL_NODE_MAP_TEST(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap, nodeAddress))
    {
        CL_NODE_MAP_CLEAR(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap, nodeAddress);
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentNodes -= 1;
        if(!soft)
        {
            CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].version = 0;
        }
        if(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersionNode == nodeAddress)
        {
            CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersionNode = 0;
            CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion = 0;

            /*
             *Update nodes min version/address.
             */
            nodeCacheMinVersionSet();
        }
        if(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentLeader == nodeAddress)
        {
            CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentLeader = 0;
        }
    }
    clOsalSemUnlock(gClNodeCacheSem);

    return CL_OK;
}

ClRcT clNodeCacheReset(ClIocNodeAddressT nodeAddress)
{
    return __nodeCacheReset(nodeAddress, CL_FALSE);
}

ClRcT clNodeCacheSoftReset(ClIocNodeAddressT nodeAddress)
{
    return __nodeCacheReset(nodeAddress, CL_TRUE);
}

ClRcT clNodeCacheCapabilitySet(ClIocNodeAddressT nodeAddress, ClUint32T capability, ClUint32T flag)
{
    if(nodeAddress >= CL_IOC_MAX_NODES || !gpClNodeCache)
        return CL_ERR_INVALID_PARAMETER;

    clOsalSemLock(gClNodeCacheSem);
    switch(flag)
    {
    case CL_NODE_CACHE_CAP_ASSIGN:
        CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].capability = capability;
        break;

    case CL_NODE_CACHE_CAP_AND:
        CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].capability &= capability;
        break;

    case CL_NODE_CACHE_CAP_MERGE:
        CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].capability |= capability;
        break;

    case CL_NODE_CACHE_CAP_CLEAR:
        CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].capability &= ~capability;
        break;

    default:
        break;
    }
    clLogNotice("CAP", "SET", "Node cache capability set to [%#x] for node [%d]",
                CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].capability, nodeAddress);
    clOsalSemUnlock(gClNodeCacheSem);
    return CL_OK;
}

ClRcT clNodeCacheLeaderUpdate(ClIocNodeAddressT lastLeader,
                              ClIocNodeAddressT currentLeader)
{
    if(!gpClNodeCache)
        return CL_ERR_INVALID_PARAMETER;

    clOsalSemLock(gClNodeCacheSem);
    if((ClInt32T)lastLeader > 0 && lastLeader < CL_IOC_MAX_NODES)
    {
        CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[lastLeader].capability &= ~__LEADER_CAPABILITY_MASK;
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentLeader = 0;
        clLogNotice("CAP", "SET", "Node cache capability for last leader [%d] is [%#x]",
                    lastLeader, CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[lastLeader].capability);
    }
    if((ClInt32T)currentLeader > 0 && currentLeader < CL_IOC_MAX_NODES)
    {
        CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[currentLeader].capability |= __LEADER_CAPABILITY_MASK;
        CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentLeader = currentLeader;
        clLogNotice("CAP", "SET", "Node cache capability for current leader [%d] is [%#x]",
                    currentLeader, CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[currentLeader].capability);
    }
    clOsalSemUnlock(gClNodeCacheSem);
    return CL_OK;
}

ClRcT clNodeCacheLeaderGet(ClIocNodeAddressT *pCurrentLeader)
{
    ClRcT rc = CL_OK;

    if(!pCurrentLeader)
        return CL_ERR_INVALID_PARAMETER;

    clOsalSemLock(gClNodeCacheSem);
    if(!gpClNodeCache)
    {
        clOsalSemUnlock(gClNodeCacheSem);
        return CL_ERR_NOT_INITIALIZED;
    }

    if( !(*pCurrentLeader = CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->currentLeader) )
    {
        rc = CL_ERR_NOT_EXIST;
    }
    clOsalSemUnlock(gClNodeCacheSem);

    return rc;
}

ClRcT clNodeCacheVersionAndCapabilityGet(ClIocNodeAddressT nodeAddress, 
                                         ClUint32T *pVersion,
                                         ClUint32T *pCapability)
{
    ClRcT rc = CL_OK;
    if(nodeAddress >= CL_IOC_MAX_NODES) return CL_ERR_INVALID_PARAMETER;
    
    if(!pVersion && !pCapability)
        return CL_ERR_INVALID_PARAMETER;

    if(pVersion)
        *pVersion = 0;

    rc = clOsalSemLock(gClNodeCacheSem);
    if (rc != CL_OK)
        return rc;

    if(!gpClNodeCache)
    {
        clOsalSemUnlock(gClNodeCacheSem);
        return CL_ERR_NOT_INITIALIZED;
    }

    if(CL_NODE_MAP_TEST(CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->nodeMap, nodeAddress))
    {
        if(pVersion)
            *pVersion = CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].version;
        if(pCapability)
            *pCapability = CL_NODE_CACHE_ENTRY_BASE(gpClNodeCache)[nodeAddress].capability;
    }
    else
    {
        rc = CL_ERR_NOT_EXIST;
    }
    clOsalSemUnlock(gClNodeCacheSem);
    
    return rc;
}

ClRcT clNodeCacheVersionGet(ClIocNodeAddressT nodeAddress, 
                            ClUint32T *pVersion)
{
    return clNodeCacheVersionAndCapabilityGet(nodeAddress, pVersion, NULL);
}

ClRcT clNodeCacheMinVersionGet(ClIocNodeAddressT *pNodeAddress, ClUint32T *pVersion)
{
    ClRcT   rc = CL_OK;

    if(!pVersion) return CL_ERR_INVALID_PARAMETER;

    rc = clOsalSemLock(gClNodeCacheSem);
    if (rc != CL_OK)
        return rc;

    if(!gpClNodeCache)
    {
        clOsalSemUnlock(gClNodeCacheSem);
        return CL_ERR_NOT_INITIALIZED;
    }

    *pVersion = CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersion;
    if(pNodeAddress)
        *pNodeAddress = CL_NODE_CACHE_HEADER_BASE(gpClNodeCache)->minVersionNode;
    clOsalSemUnlock(gClNodeCacheSem);

    return CL_OK;
}

ClBoolT clAspNativeLeaderElection(void)
{
    return gClAspNativeLeaderElection;
}
