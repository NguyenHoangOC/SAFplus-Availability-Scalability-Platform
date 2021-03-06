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
/*******************************************************************************
 * ModuleName  : amf                                                           
 * File        : clAmsNotifications.c
 *******************************************************************************/

/*******************************************************************************
 * Description :                                                                
 *
 * This is the server side code for the AMS notification service.
 *
 *
 ***************************** Editor Commands ********************************
 * For vi/vim
 * :set shiftwidth=4
 * :set softtabstop=4
 * :set expandtab
 *****************************************************************************/
 
/******************************************************************************
 * Include files needed to compile this file
 *****************************************************************************/

#include <clAmsServerUtils.h>
#include <clAmsNotifications.h>
#include "xdrClAmsNotificationDescriptorT.h"

#include <clFaultDefinitions.h>
#include <clEventApi.h>
#include <clCommon.h>

#define CL_AMS_STRING_NTF(S)                                            \
    (   ((S) == CL_AMS_NOTIFICATION_SI_PARTIALLY_ASSIGNED)  ? "partially assigned" : \
        ((S) == CL_AMS_NOTIFICATION_SI_FULLY_ASSIGNED)      ? "fully assigned" : \
        ((S) == CL_AMS_NOTIFICATION_SI_UNASSIGNED)          ? "unassigned" : \
        "Unknown" )

ClVersionT eventVersion = {'B',01,01};
static ClBoolT gClAmsNotificationDebug = CL_FALSE;

/*****************************************************************************
 * Notification related functions 
 ****************************************************************************/

ClRcT 
clAmsNotificationEventInitialize (void)
{

    ClRcT  rc = CL_OK;
    ClAmsT  *ams = &gAms;
    ClEventChannelOpenFlagsT flags = CL_EVENT_CHANNEL_PUBLISHER 
                                     | CL_EVENT_GLOBAL_CHANNEL;

    ClEventPatternT clAmsEventPattern[] = 
    {
        {0, sizeof (CL_AMS_EVENT_PATTERN), (ClUint8T*)&CL_AMS_EVENT_PATTERN}
    };

    ClEventPatternArrayT clAmsEventPatternArray = 
    {
        0,sizeof(clAmsEventPattern)/sizeof(ClEventPatternT),clAmsEventPattern 
    };


    /*
     * Initialize the event client library for AMS notifications
     */

    AMS_CALL ( clEventInitialize( &ams->eventInitHandle, NULL, &eventVersion) );

    ClNameT clAmsEventChannelName = {0};

    strcpy(clAmsEventChannelName.value, CL_AMS_EVENT_CHANNEL_NAME);
    clAmsEventChannelName.length = strlen(CL_AMS_EVENT_CHANNEL_NAME) ;


    /*
     * Open the event channel for AMS notifications
     */


    if ( ams->serviceState == CL_AMS_SERVICE_STATE_RUNNING )
    {
        flags |= CL_EVENT_CHANNEL_CREATE;
    }

    if ( (rc = clEventChannelOpen(
                    ams->eventInitHandle, 
                    &clAmsEventChannelName,
                    flags,
                    (ClTimeT) -1,
                    &ams->eventChannelOpenHandle) )
            != CL_OK )
        goto error;


    /*
     * Create an event for AMS notifications
     */

    if ( (rc = clEventAllocate(
                    ams->eventChannelOpenHandle,
                    &ams->eventHandle) )
            != CL_OK )
        goto error;

    ClNameT  clAmsEventPublisherName = {0};

    strcpy (clAmsEventPublisherName.value, CL_AMS_EVENT_PUBLISHER_NAME);
    clAmsEventPublisherName.length = strlen (CL_AMS_EVENT_PUBLISHER_NAME);

    if ( ( rc = clEventAttributesSet(
                    ams->eventHandle, 
                    &clAmsEventPatternArray,
                    CL_EVENT_HIGHEST_PRIORITY, 
                    0, 
                    &clAmsEventPublisherName) )
            != CL_OK )
        goto error;

    gClAmsNotificationDebug = clParseEnvBoolean("AMS_NOTIFICATION_DEBUG");

    return CL_OK;

error:

    return CL_AMS_RC (rc);

}


static void clAmsNotificationLog(ClAmsNotificationDescriptorT *notification)
{
    if(!notification) return ;

    switch(notification->type)
    {
    case CL_AMS_NOTIFICATION_SI_PARTIALLY_ASSIGNED:
        {
            clLogInfo("AMF", "NTF", "Generating SI [%s] event", CL_AMS_STRING_NTF(notification->type));
            clLogInfo("AMF", "NTF", "SI name : [%.*s]",
                      notification->siName.length,
                      notification->siName.value);
            clLogInfo("AMF", "NTF", "SU name : [%.*s]",
                      notification->suName.length,
                      notification->suName.value);
            clLogInfo("AMF", "NTF", "HA State : [%s]",
                      CL_AMS_STRING_H_STATE(notification->newHAState));

        }
        break;

    case CL_AMS_NOTIFICATION_SI_FULLY_ASSIGNED:
        {
            clLogInfo("AMF", "NTF", "Generating SI [%s] event", CL_AMS_STRING_NTF(notification->type));
            clLogInfo("AMF", "NTF", "SI name : [%.*s]",
                      notification->siName.length,
                      notification->siName.value);
            clLogInfo("AMF", "NTF", "SU name : [%.*s]",
                      notification->suName.length,
                      notification->suName.value);
            clLogInfo("AMF", "NTF", "HA State : [%s]",
                      CL_AMS_STRING_H_STATE(notification->newHAState));

        }
        break;

    case CL_AMS_NOTIFICATION_SI_UNASSIGNED:
        {
            clLogInfo("AMF", "NTF", "Generating SI [%s] event", CL_AMS_STRING_NTF(notification->type));
            clLogInfo("AMF", "NTF", "SI name : [%.*s]",
                      notification->siName.length,
                      notification->siName.value);
            clLogInfo("AMF", "NTF", "SU name : [%.*s]",
                      notification->suName.length,
                      notification->suName.value);
            clLogInfo("AMF", "NTF", "HA State : [%s]",
                      CL_AMS_STRING_H_STATE(notification->newHAState));
        }
        break;

    case CL_AMS_NOTIFICATION_SU_HA_STATE_CHANGE:
        {
            clLogInfo("AMF", "NTF", "Generating SU HA state change event");
            clLogInfo("AMF", "NTF", "SU name : [%.*s]",
                      notification->suName.length,
                      notification->suName.value);
            clLogInfo("AMF", "NTF", "SI name : [%.*s]",
                      notification->siName.length,
                      notification->siName.value);
            clLogInfo("AMF", "NTF", "Last HA State : [%s]",
                      CL_AMS_STRING_H_STATE(notification->lastHAState));
            clLogInfo("AMF", "NTF", "New HA State : [%s]",
                      CL_AMS_STRING_H_STATE(notification->newHAState));
        }
        break;
            
    default:
        {
            break;
        }
    }
}

ClRcT clAmsNotificationEventPublish(
        ClAmsNotificationDescriptorT  *notification )
{

    ClEventIdT eventID = 0;
    ClAmsT  *ams = &gAms;
    ClPtrT  data = NULL;
    ClUint32T  msgLength = 0;
    ClBufferHandleT  out = {0};

    AMS_CHECKPTR (!notification );

    if ( ams->eventServerReady == CL_FALSE )
    {
        AMS_LOG (CL_DEBUG_TRACE,("Event server not ready\n"));
        return CL_OK;
    }

    if(gClAmsNotificationDebug)
        clAmsNotificationLog(notification);

    AMS_CALL ( clBufferCreate(&out) );

    AMS_CALL ( clAmsNotificationMarshalNtfDescr(notification,&data,&msgLength,
                out) );

    AMS_CALL ( clEventPublish ( ams->eventHandle, (const void *) data, 
                msgLength, &eventID));

    AMS_CALL ( clBufferDelete(&out) );
    
    clHeapFree(data);

    return CL_OK;

}

ClRcT clAmsMgmtCCBNotificationEventPayloadSet(ClAmsNotificationTypeT type,
                                              const ClAmsEntityT *entity,
                                              ClAmsNotificationDescriptorT *notification)
{
    if(!entity) 
        return CL_AMS_RC(CL_ERR_INVALID_PARAMETER);
    notification->type = type;
    notification->entityType = entity->type;
    memcpy(&notification->entityName, &entity->name, sizeof(notification->entityName));
    notification->entityName.length = strlen(entity->name.value);
    return CL_OK;
}

static ClRcT clAmsSINotificationEventPayloadSet(const ClAmsEntityT *entity,
                                                const ClAmsEntityRefT *entityRef,
                                                const ClAmsHAStateT lastHAState,
                                                const ClAmsNotificationTypeT ntfType,
                                                ClAmsNotificationDescriptorT *notification)
{
    ClAmsSUT *su = (ClAmsSUT *)entity;
    ClAmsSUSIRefT *siRef = (ClAmsSUSIRefT *)entityRef;
            
    notification->type = ntfType;
    notification->entityType = CL_AMS_ENTITY_TYPE_SI;
    memcpy(&notification->siName, &siRef->entityRef.entity.name, sizeof(ClNameT));
    memcpy(&notification->suName, &su->config.entity.name, sizeof(ClNameT));
    notification->siName.length = strlen(siRef->entityRef.entity.name.value);
    notification->suName.length = strlen(su->config.entity.name.value);
    notification->lastHAState = lastHAState;
    notification->newHAState = siRef->haState;

    return CL_OK;
}

static ClRcT clAmsSUNotificationEventPayloadSet(const ClAmsEntityT *entity,
                                                const ClAmsEntityRefT *entityRef,
                                                const ClAmsHAStateT lastHAState,
                                                ClAmsNotificationDescriptorT *notification)
{
    ClAmsSIT *si = (ClAmsSIT *)entity;
    ClAmsSISURefT *suRef = (ClAmsSISURefT *)entityRef;

    notification->type = CL_AMS_NOTIFICATION_SU_HA_STATE_CHANGE;
    notification->entityType = CL_AMS_ENTITY_TYPE_SU;
    memcpy(&notification->suName, &suRef->entityRef.entity.name, sizeof(ClNameT));
    memcpy(&notification->siName, &si->config.entity.name, sizeof(ClNameT));
    notification->suName.length = strlen(suRef->entityRef.entity.name.value);
    notification->siName.length = strlen(si->config.entity.name.value);
    notification->lastHAState = lastHAState;
    notification->newHAState = suRef->haState;

    return CL_OK;
}

ClRcT clAmsNotificationEventPayloadSet(const ClAmsEntityT *entity,
                                       const ClAmsEntityRefT *entityRef,
                                       const ClAmsHAStateT lastHAState,
                                       const ClAmsNotificationTypeT ntfType,
                                       ClAmsNotificationDescriptorT *notification)
{
    AMS_CHECKPTR (!entity || !entityRef || !notification);

    switch(entity->type)
    {
        case CL_AMS_ENTITY_TYPE_SU:
        {
            clAmsSINotificationEventPayloadSet(entity,
                                               entityRef,
                                               lastHAState,
                                               ntfType,
                                               notification);
            break;
        }
        case CL_AMS_ENTITY_TYPE_SI:
        {
            clAmsSUNotificationEventPayloadSet(entity,
                                               entityRef,
                                               lastHAState,
                                               notification);
            break;
        }
        default:
        {
            break;
        }
    }

    return CL_OK;
}

ClRcT clAmsOperStateNotificationPublish(ClAmsEntityT *pEntity, 
                                        ClAmsOperStateT lastOperState,
                                        ClAmsOperStateT newOperState)
{
    ClAmsNotificationDescriptorT descriptor = {0};
    if(!pEntity)
        return CL_AMS_RC(CL_ERR_INVALID_PARAMETER);
    descriptor.type = CL_AMS_NOTIFICATION_OPER_STATE_CHANGE;
    descriptor.entityType = pEntity->type;
    clNameSet((ClNameT*)&descriptor.entityName, pEntity->name.value);
    descriptor.lastOperState = lastOperState;
    descriptor.newOperState = newOperState;
    return clAmsNotificationEventPublish(&descriptor);
}

ClRcT clAmsAdminStateNotificationPublish(ClAmsEntityT *pEntity, 
                                         ClAmsAdminStateT lastAdminState,
                                         ClAmsAdminStateT newAdminState)
{
    ClAmsNotificationDescriptorT descriptor = {0};
    if(!pEntity)
        return CL_AMS_RC(CL_ERR_INVALID_PARAMETER);
    descriptor.type = CL_AMS_NOTIFICATION_ADMIN_STATE_CHANGE;
    descriptor.entityType = pEntity->type;
    clNameSet((ClNameT*)&descriptor.entityName, pEntity->name.value);
    descriptor.lastAdminState = lastAdminState;
    descriptor.newAdminState =  newAdminState;
    return clAmsNotificationEventPublish(&descriptor);
}

ClRcT clAmsNotificationMarshalNtfDescr (
        CL_IN  ClAmsNotificationDescriptorT  *ntf,
        CL_OUT ClPtrT  *dataPtr,
        CL_INOUT  ClUint32T  *msgLength,
        CL_INOUT  ClBufferHandleT  out )

{

    AMS_CHECKPTR (!dataPtr || !ntf || !msgLength);

    AMS_CALL ( VDECL_VER(clXdrMarshallClAmsNotificationDescriptorT, 4, 0, 0)(ntf,out,0) );

    AMS_CALL ( clBufferLengthGet(out, msgLength) );

    AMS_CALL ( clBufferFlatten(out, (ClUint8T **)dataPtr));

    return CL_OK;
}

