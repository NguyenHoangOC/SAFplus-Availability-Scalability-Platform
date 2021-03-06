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
 * ModuleName  : gms
 * File        : clGmsRmdServer.h
 *******************************************************************************/

/*******************************************************************************
 * Description :
 *******************************************************************************/


#ifndef _CL_GMS_RMD_SERVER_H_
#define _CL_GMS_RMD_SERVER_H_

# ifdef __cplusplus
extern "C" {
# endif

#include <clCommon.h>
#include <clBufferApi.h>
#include <clEoApi.h>
#include <clGmsCommon.h>
#include <clGmsMsg.h>
#include <clGmsCli.h>
#include <clGmsDb.h>

/*
 * Pass one of the enums above and this will return the function id
 * that has to be passed to the RmdWithMsg call
 */

#define GMS_FUNC_ID(x) CL_EO_GET_FULL_FN_NUM(CL_GMS_CLIENT_TABLE_ID, x)


#define GMS_TOTAL_RMD_FUNC_LIST     26
extern ClEoPayloadWithReplyCallbackT 
			gmsServerClientFuncList[GMS_TOTAL_RMD_FUNC_LIST];

/* prototypes */

ClRcT   _clGmsRmdTrackCurrent(
                CL_IN       ClGmsDbT                    *thisCluster,
                CL_IN       ClGmsClusterTrackRequestT   req,
                CL_INOUT    ClBufferHandleT      *outMsg);


ClRcT   _clGmsRmdTrackAsync(
                CL_IN   ClGmsDbT                  *thisCluster,
                CL_IN   ClGmsClusterTrackRequestT req);


/* Callback function prototypes */
extern ClRcT cl_gms_cluster_track_callback(
        CL_IN   ClIocAddressT addr,
        CL_IN   ClGmsClusterTrackCallbackDataT *data);

extern ClRcT cl_gms_cluster_member_get_callback(
        CL_IN   ClIocAddressT addr,
        CL_IN   ClGmsClusterMemberGetCallbackDataT *data);

extern ClRcT cl_gms_cluster_member_eject_callback(
        CL_IN   ClIocAddressT addr,
        CL_IN   ClGmsClusterMemberEjectCallbackDataT *data);

extern ClRcT cl_gms_group_track_callback(
        CL_IN   ClIocAddressT addr,
        CL_IN   ClGmsGroupTrackCallbackDataT *data);

extern ClRcT cl_gms_group_member_get_callback(
        CL_IN   ClIocAddressT addr,
        CL_IN   ClGmsGroupMemberGetCallbackDataT *data);

extern ClRcT cl_gms_group_member_eject_callback(
        CL_IN   ClIocAddressT addr,
        CL_IN   ClGmsGroupMemberEjectCallbackDataT *data);
extern ClRcT cl_gms_group_mcast_callback(
        CL_IN   const ClIocAddressT                 addr,
        CL_IN   ClGmsGroupMcastCallbackDataT* const data);



#ifdef  __cplusplus
}
#endif

#endif  /* _CL_GMS_RMD_SERVER_H_ */


