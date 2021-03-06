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


/*********************************************************************
* ModuleName  : idl
*********************************************************************/
/*********************************************************************
* Description : This file contains the declarations for marshall 
*               and unmarshall funtions of ClAlarmHandleInfoIDLT 
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/

#ifndef _XDR_CL_ALARM_HANDLE_INFO_IDLT_H_
#define _XDR_CL_ALARM_HANDLE_INFO_IDLT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "clXdrApi.h"

#include "clCorMetaData.h"
#include "clAlarmDefinitions.h"
#include "xdrClAlarmInfoIDLT.h"

struct _ClAlarmHandleInfoIDLT_4_0_0;

typedef struct _ClAlarmHandleInfoIDLT_4_0_0 {
    ClUint32T    alarmHandle;
    ClAlarmInfoIDLT_4_0_0    alarmInfo;

}ClAlarmHandleInfoIDLT_4_0_0;


ClRcT  clXdrMarshallClAlarmHandleInfoIDLT_4_0_0(void *,ClBufferHandleT , ClUint32T);

ClRcT  clXdrUnmarshallClAlarmHandleInfoIDLT_4_0_0(ClBufferHandleT, void *);

#define clXdrMarshallArrayClAlarmHandleInfoIDLT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallArray((pointer), sizeof(ClAlarmHandleInfoIDLT_4_0_0), (multiplicity), clXdrMarshallClAlarmHandleInfoIDLT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallArrayClAlarmHandleInfoIDLT_4_0_0(msg,pointer, multiplicity) \
clXdrUnmarshallArray((msg),(pointer), sizeof(ClAlarmHandleInfoIDLT_4_0_0), (multiplicity), clXdrUnmarshallClAlarmHandleInfoIDLT_4_0_0)

#define clXdrMarshallPointerClAlarmHandleInfoIDLT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPointer((pointer), sizeof(ClAlarmHandleInfoIDLT_4_0_0), (multiplicity), clXdrMarshallClAlarmHandleInfoIDLT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPointerClAlarmHandleInfoIDLT_4_0_0(msg,pointer) \
clXdrUnmarshallPointer((msg),(pointer), sizeof(ClAlarmHandleInfoIDLT_4_0_0), clXdrUnmarshallClAlarmHandleInfoIDLT_4_0_0)

#define clXdrMarshallPtrClAlarmHandleInfoIDLT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPtr((pointer), sizeof(ClAlarmHandleInfoIDLT_4_0_0), (multiplicity), clXdrMarshallClAlarmHandleInfoIDLT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPtrClAlarmHandleInfoIDLT_4_0_0(msg,pointer,multiplicity) \
clXdrUnmarshallPtr((msg),(pointer), sizeof(ClAlarmHandleInfoIDLT_4_0_0),multiplicity, clXdrUnmarshallClAlarmHandleInfoIDLT_4_0_0)


typedef ClAlarmHandleInfoIDLT_4_0_0 ClAlarmHandleInfoIDLT;



#ifdef __cplusplus
}
#endif

#endif /*_XDR_CL_ALARM_HANDLE_INFO_IDLT_H_*/
