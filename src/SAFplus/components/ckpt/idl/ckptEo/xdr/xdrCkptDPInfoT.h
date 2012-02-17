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
*               and unmarshall funtions of CkptDPInfoT 
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/

#ifndef _XDR_CKPT_DP_INFO_T_H_
#define _XDR_CKPT_DP_INFO_T_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "clXdrApi.h"

#include "clCkptApi.h"
#include "clCkptCommon.h"
#include "clCkptExtApi.h"
#include "xdrCkptSectionInfoT.h"

struct _CkptDPInfoT_4_0_0;

typedef struct _CkptDPInfoT_4_0_0 {
    ClUint64T    maxCkptSize;
    ClUint64T    maxScnSize;
    ClUint64T    maxScnIdSize;
    ClUint32T    maxScns;
    ClUint32T    numScns;
    ClUint32T    genIndex;
    CkptSectionInfoT_4_0_0*    pSection;

}CkptDPInfoT_4_0_0;


ClRcT  clXdrMarshallCkptDPInfoT_4_0_0(void *,ClBufferHandleT , ClUint32T);

ClRcT  clXdrUnmarshallCkptDPInfoT_4_0_0(ClBufferHandleT, void *);

#define clXdrMarshallArrayCkptDPInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallArray((pointer), sizeof(CkptDPInfoT_4_0_0), (multiplicity), clXdrMarshallCkptDPInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallArrayCkptDPInfoT_4_0_0(msg,pointer, multiplicity) \
clXdrUnmarshallArray((msg),(pointer), sizeof(CkptDPInfoT_4_0_0), (multiplicity), clXdrUnmarshallCkptDPInfoT_4_0_0)

#define clXdrMarshallPointerCkptDPInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPointer((pointer), sizeof(CkptDPInfoT_4_0_0), (multiplicity), clXdrMarshallCkptDPInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPointerCkptDPInfoT_4_0_0(msg,pointer) \
clXdrUnmarshallPointer((msg),(pointer), sizeof(CkptDPInfoT_4_0_0), clXdrUnmarshallCkptDPInfoT_4_0_0)

#define clXdrMarshallPtrCkptDPInfoT_4_0_0(pointer, multiplicity, msg, isDelete) \
clXdrMarshallPtr((pointer), sizeof(CkptDPInfoT_4_0_0), (multiplicity), clXdrMarshallCkptDPInfoT_4_0_0, (msg), (isDelete))

#define clXdrUnmarshallPtrCkptDPInfoT_4_0_0(msg,pointer,multiplicity) \
clXdrUnmarshallPtr((msg),(pointer), sizeof(CkptDPInfoT_4_0_0),multiplicity, clXdrUnmarshallCkptDPInfoT_4_0_0)


typedef CkptDPInfoT_4_0_0 CkptDPInfoT;



#ifdef __cplusplus
}
#endif

#endif /*_XDR_CKPT_DP_INFO_T_H_*/