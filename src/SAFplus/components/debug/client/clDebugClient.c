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
 * ModuleName  : debug
 * File        : clDebugClient.c
 *******************************************************************************/

/*******************************************************************************
 * Description :
 * This file contains DebugCli related APIs 
 *****************************************************************************/

#define __CLIENT__
#include <clCommon.h>
#include <clEoApi.h>
#include <clDebugRmd.h>
#include "clDebugCliFuncTable.h"

ClRcT clDebugClientTableRegister(ClEoExecutionObjT *pThis)
{
    return clEoClientTableRegister(CL_EO_CLIENT_SYM_MOD(gAspFuncTable, DEBUGCli),
                                   pThis->eoPort);
}
