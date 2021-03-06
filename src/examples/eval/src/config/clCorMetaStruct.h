/*******************************************************************************
*
* This file is auto-generated by OpenClovis IDE version 2.3.0
*
* clCorMetaStruct.h
* 
*
********************************************************************************/
#ifndef _CL_COR_META_STRUCT_H_
#define _CL_COR_META_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

enum ENUM_classes
{
	CLASS_RESERVED=65536,
		CLASS_CHASSIS_MO ,
	CLASS_CSA104RES_MO ,
	CLASS_CSA104RES_PROV_MSO ,
	CLASS_CSA105RES_MO ,
	CLASS_CSA105RES_PROV_MSO ,
	CLASS_CSA105RES_ALARM_MSO ,
	CLASS_SCNODERES_MO ,
	CLASS_PAYLOADNODERES_MO ,

	COR_CLASS_MAX
};
enum ENUM_attributes
{
	ATTRIBUTE_RESERVED = 1,
	CSA104RES_COUNTER,
	CSA104RES_DELTA_T,
	CSA105RES_COUNTER,
	CSA105RES_COUNTER_THRESH,
	CSA105RES_DELTA_T,

	COR_ATTRIBUTES_MAX
};

#ifdef __cplusplus
}
#endif

#endif
