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
* Description : Marshall routine for ClAmsNotificationDescriptorT
*     
* THIS FILE IS AUTO-GENERATED BY OPENCLOVIS IDE. EDIT THIS FILE AT
* YOUR OWN RISK. ANY CHANGE TO THIS FILE WILL BE OVERWRITTEN ON
* RE-GENERATION.
*     
*********************************************************************/

#include "xdrClAmsNotificationDescriptorT.h"

ClRcT clXdrMarshallClAmsNotificationDescriptorT_4_0_0(void* pGenVar, ClBufferHandleT msg, ClUint32T isDelete)
{
    ClAmsNotificationDescriptorT_4_0_0* pVar = (ClAmsNotificationDescriptorT_4_0_0*)pGenVar;
    ClRcT rc         = CL_OK;
    ClUint32T length = 0;    

    if ((void*)0 == pVar)
    {
        clXdrMarshallClUint32T(&length, msg, 0);
    }
    else
    {
        length = 1;
        clXdrMarshallClUint32T(&length, msg, 0);

    rc = clXdrMarshallClAmsNotificationTypeT_4_0_0(&(pVar->type),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsEntityTypeT_4_0_0(&(pVar->entityType),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClNameT(&(pVar->entityName),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClNameT(&(pVar->faultyCompName),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClNameT(&(pVar->siName),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClNameT(&(pVar->suName),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsHAStateT_4_0_0(&(pVar->lastHAState),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsHAStateT_4_0_0(&(pVar->newHAState),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsRecoveryT_4_0_0(&(pVar->recoveryActionTaken),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClUint16T(&(pVar->repairNecessary),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsOperStateT_4_0_0(&(pVar->lastOperState),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsOperStateT_4_0_0(&(pVar->newOperState),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsAdminStateT_4_0_0(&(pVar->lastAdminState),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    rc = clXdrMarshallClAmsAdminStateT_4_0_0(&(pVar->newAdminState),msg,isDelete);
    if (CL_OK != rc)
    {
        return rc;
    }

    }

    return rc;
}


