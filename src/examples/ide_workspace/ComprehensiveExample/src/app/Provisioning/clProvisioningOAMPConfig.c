
/******************************************************************************
 *
 * clProvisioningOAMPConfig.c
 *
 ******************************************************************************
 * This code is auto-generated by OpenClovis IDE Version 3.1
 *
 ***************************** Description ************************************
 *
 * This is a source file for oamp configuration. 
 *
 *****************************************************************************/
 
#include <clOmClassId.h>
#include <clProvisioningOAMPConfig.h>
#define CL_MAX_OBJ 256

#define PROVISIONING_CLOCKSTATUS_PROV_CLASS_NAME    "Provisioning_CLOCKSTATUS_PROV_CLASS_NAME"
#define PROVISIONING_CLOCKSTATUS_PROV_CLASS_VERSION    0x1
#define PROVISIONING_CLOCKSTATUS_PROV_MAX_SLOTS    1   

#define PROVISIONING_CLOCKTABLE_PROV_CLASS_NAME    "Provisioning_CLOCKTABLE_PROV_CLASS_NAME"
#define PROVISIONING_CLOCKTABLE_PROV_CLASS_VERSION    0x1
#define PROVISIONING_CLOCKTABLE_PROV_MAX_SLOTS    1   

#define PROVISIONING_TIMESETTABLE_PROV_CLASS_NAME    "Provisioning_TIMESETTABLE_PROV_CLASS_NAME"
#define PROVISIONING_TIMESETTABLE_PROV_CLASS_VERSION    0x1
#define PROVISIONING_TIMESETTABLE_PROV_MAX_SLOTS    1   

ClOmClassControlBlockT pAppOmClassTbl[] =
{
    {  
    PROVISIONING_CLOCKSTATUS_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_CLOCKSTATUS_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clProvisioningCLOCKSTATUSProvConstructor, 
    clProvisioningCLOCKSTATUSProvDestructor,
    NULL,
    PROVISIONING_CLOCKSTATUS_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    PROVISIONING_CLOCKSTATUS_PROV_MAX_SLOTS,
    CL_OM_PROV_CLOCKSTATUS_CLASS_TYPE
},

{  
    PROVISIONING_CLOCKTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_CLOCKTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clProvisioningCLOCKTABLEProvConstructor, 
    clProvisioningCLOCKTABLEProvDestructor,
    NULL,
    PROVISIONING_CLOCKTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    PROVISIONING_CLOCKTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_CLOCKTABLE_CLASS_TYPE
},

{  
    PROVISIONING_TIMESETTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_TIMESETTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clProvisioningTIMESETTABLEProvConstructor, 
    clProvisioningTIMESETTABLEProvDestructor,
    NULL,
    PROVISIONING_TIMESETTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    PROVISIONING_TIMESETTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_TIMESETTABLE_CLASS_TYPE
},

};

ClUint32T appOmClassCnt = sizeof(pAppOmClassTbl)/sizeof(ClOmClassControlBlockT);
