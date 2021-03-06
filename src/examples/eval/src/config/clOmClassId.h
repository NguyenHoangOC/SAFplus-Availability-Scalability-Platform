/******************************************************************************
 *
 * clOmClassId.h
 *
 ******************************************************************************
 * This code is auto-generated by OpenClovis IDE Version 2.3.0
 *
 ***************************** Description ************************************
 *
 * This is a header for OM class IDs
 *
 *****************************************************************************/

#ifndef _CL_OM_CLASS_ID_H_
#define _CL_OM_CLASS_ID_H_
                                                                                                                                                                                    
#ifdef __cplusplus
extern "C"
{
#endif

#include <clOmCommonClassTypes.h>

#define OM_APP_MIN_CLASS_TYPE (CL_OM_CMN_CLASS_TYPE_END + 1)
#define OM_APP_MAX_CLASS_TYPE (CL_OM_PROV_PAYLOADNODERES_CLASS_TYPE)

enum ClOmClassIdsT
{
    CL_OM_CLASS_TYPE_START = CL_OM_CMN_CLASS_TYPE_END + 1,
    CL_OM_PROV_CSA104RES_CLASS_TYPE,
    CL_OM_PROV_CSA105RES_CLASS_TYPE,
    CL_OM_ALARM_CSA105RES_CLASS_TYPE,	
    CL_OM_PROV_SCNODERES_CLASS_TYPE,
    CL_OM_PROV_PAYLOADNODERES_CLASS_TYPE    
};

#ifdef __cplusplus
}
#endif
#endif /* _CL_OM_CLASS_ID_H_ */

