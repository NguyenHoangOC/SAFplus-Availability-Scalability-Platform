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
*               and unmarshall funtions of CkptInfoT 
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/

#ifndef _XDR_CKPT_INFO_T_H_
#define _XDR_CKPT_INFO_T_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "clXdrApi.h"

#include "clCkptExtApi.h"
#include "clCkptCommon.h"
#include "clCkptApi.h"
#include "xdrCkptCPInfoT.h"
#include "xdrCkptDPInfoT.h"

struct _CkptInfoT_4_0_0;

typedef struct _CkptInfoT_4_0_0 {
    ClHandleT    ckptHdl;
    ClNameT*    pName;
    CkptCPInfoT_4_0_0*    pCpInfo;
    CkptDPInfoT_4_0_0*    pDpInfo;

}CkptInfoT_4_0_0;


ClRcT  clXdrMarshallCkptInfoT_4_0_0(void *,ClBufferHandleT , ClUint32T);

ClRcT  clXdrUnmarshallCkptInfoT_4_0_0(ClBufferHandleT, void *);

#define clXdrMarshallArrayCkptInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallArray((pointer), sizeof(CkptInfoT_4_0_0), (multiplicity), clXdrMarshallCkptInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallArrayCkptInfoT_4_0_0(msg,pointer, multiplicity) \
clXdrUnmarshallArray((msg),(pointer), sizeof(CkptInfoT_4_0_0), (multiplicity), clXdrUnmarshallCkptInfoT_4_0_0)

#define clXdrMarshallPointerCkptInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPointer((pointer), sizeof(CkptInfoT_4_0_0), (multiplicity), clXdrMarshallCkptInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPointerCkptInfoT_4_0_0(msg,pointer) \
clXdrUnmarshallPointer((msg),(pointer), sizeof(CkptInfoT_4_0_0), clXdrUnmarshallCkptInfoT_4_0_0)

#define clXdrMarshallPtrCkptInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPtr((pointer), sizeof(CkptInfoT_4_0_0), (multiplicity), clXdrMarshallCkptInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPtrCkptInfoT_4_0_0(msg,pointer,multiplicity) \
clXdrUnmarshallPtr((msg),(pointer), sizeof(CkptInfoT_4_0_0),multiplicity, clXdrUnmarshallCkptInfoT_4_0_0)


typedef CkptInfoT_4_0_0 CkptInfoT;


struct _CkptInfoT_5_0_0;

typedef struct _CkptInfoT_5_0_0 {
    ClHandleT    ckptHdl;
    ClNameT*    pName;
    CkptCPInfoT_5_0_0*    pCpInfo;
    CkptDPInfoT_4_0_0*    pDpInfo;

}CkptInfoT_5_0_0;


ClRcT  clXdrMarshallCkptInfoT_5_0_0(void *,ClBufferHandleT , ClUint32T);

ClRcT  clXdrUnmarshallCkptInfoT_5_0_0(ClBufferHandleT, void *);

#define clXdrMarshallArrayCkptInfoT_5_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallArray((pointer), sizeof(CkptInfoT_5_0_0), (multiplicity), clXdrMarshallCkptInfoT_5_0_0, (msg), (isDelete))

#define clXdrUnmarshallArrayCkptInfoT_5_0_0(msg,pointer, multiplicity) \
clXdrUnmarshallArray((msg),(pointer), sizeof(CkptInfoT_5_0_0), (multiplicity), clXdrUnmarshallCkptInfoT_5_0_0)

#define clXdrMarshallPointerCkptInfoT_5_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPointer((pointer), sizeof(CkptInfoT_5_0_0), (multiplicity), clXdrMarshallCkptInfoT_5_0_0, (msg), (isDelete))

#define clXdrUnmarshallPointerCkptInfoT_5_0_0(msg,pointer) \
clXdrUnmarshallPointer((msg),(pointer), sizeof(CkptInfoT_5_0_0), clXdrUnmarshallCkptInfoT_5_0_0)

#define clXdrMarshallPtrCkptInfoT_5_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPtr((pointer), sizeof(CkptInfoT_5_0_0), (multiplicity), clXdrMarshallCkptInfoT_5_0_0, (msg), (isDelete))

#define clXdrUnmarshallPtrCkptInfoT_5_0_0(msg,pointer,multiplicity) \
clXdrUnmarshallPtr((msg),(pointer), sizeof(CkptInfoT_5_0_0),multiplicity, clXdrUnmarshallCkptInfoT_5_0_0)



#ifdef __cplusplus
}
#endif

#endif /*_XDR_CKPT_INFO_T_H_*/
