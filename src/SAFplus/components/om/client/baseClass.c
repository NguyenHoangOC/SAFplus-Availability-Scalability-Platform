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
 * ModuleName  : om                                                            
 * File        : baseClass.c
 *******************************************************************************/

/*******************************************************************************
 * Description :                                                                
 * This module contains the definitions for the Base Object.
 *******************************************************************************/

#include <clCommon.h>
#include <clOmBaseClass.h>
#include <clDebugApi.h>
#include <clCommonErrors.h>
#include "clOmApi.h"

#define MAX_OBJ 256


ClRcT baseClass_constructor(void* pThis, void *pUsrData, 
	ClUint32T usrDataLen);
ClRcT baseClass_destructor(void* pThis, void *pUsrData, 
	ClUint32T usrDataLen);
ClRcT baseObject_fGetClassType(CL_OM_BASE_CLASS* pThis, ClOmClassTypeT* eType);

CL_OM_INIT_METHOD_TABLE(CL_OM_BASE_CLASS)
    baseObject_fGetClassType
CL_OM_END


ClOmClassControlBlockT gBaseClassTable  =
{
          CL_OM_BASE_CLASS_NAME,          /* baseClass name */
          sizeof(CL_OM_BASE_CLASS),       /* size */
          0,             /* extends from */
          baseClass_constructor,   /* constructor */
          baseClass_destructor,    /* destructor */
          CL_OM_BASE_CLASS_METHOD_TABLE,  /* pointer to methods struct */
          CL_OM_BASE_CLASS_VERSION,           /* version */
          0,                                            /* Instance table ptr */
          MAX_OBJ,      /* Maximum number of classes */
          0,                                            /* cur instance count */
          CL_OM_BASE_CLASS_MAX_SLOTS,     /* max slots */
          CL_OM_BASE_CLASS_TYPE  /* my class type */
};

/**
 * constructor
 *
 * @return ClRcT
 */
ClRcT baseClass_constructor(void* pThis, void *pUsrData, 
	ClUint32T usrDataLen) 
{
    /* *** INSERT user implementation *** */
    CL_ASSERT(pThis);
    return CL_OK;
    /* *** END user implementation *** */
}


/**
 * destructor
 *
 * @return ClRcT
 */
ClRcT baseClass_destructor(void* pThis, void *pUsrData, 
	ClUint32T usrDataLen) 
{
    /* *** INSERT user implementation *** */
    CL_ASSERT(pThis);

    return CL_OK;
    /* *** END user implementation *** */
}

/**
 * fGetClasstType
 *
 * @param eType
 * @return ClRcT
 */
ClRcT baseObject_fGetClassType(CL_OM_BASE_CLASS* pThis, ClOmClassTypeT* eType) {
    /* *** INSERT user implementation *** */
    CL_FUNC_ENTER();
    CL_ASSERT(pThis);
    CL_ASSERT(eType);

	*eType = (pThis->__objType >> 16);

    CL_FUNC_EXIT();
    return CL_OK;
    /* *** END user implementation *** */
}
