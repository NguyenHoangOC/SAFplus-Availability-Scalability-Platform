
/******************************************************************************
 *
 * clamfMgmtOAMPConfig.c
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
#include <clamfMgmtOAMPConfig.h>
#define CL_MAX_OBJ 256

#define AMFMGMT_SAAMFAPPLICATIONTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFAPPLICATIONTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFAPPLICATIONTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFAPPLICATIONTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFNODETABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFNODETABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFNODETABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFNODETABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSGTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSGTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSGTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSGTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSUTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSUTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSUTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSUTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSITABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSITABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSITABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSITABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSUSPERSIRANKTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSUSPERSIRANKTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSUSPERSIRANKTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSUSPERSIRANKTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSGSIRANKTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSGSIRANKTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSGSIRANKTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSGSIRANKTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSGSURANKTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSGSURANKTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSGSURANKTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSGSURANKTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSISIDEPTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSISIDEPTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSISIDEPTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSISIDEPTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFCOMPTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFCOMPTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFCOMPTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFCOMPTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFCOMPCSTYPESUPPORTEDTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFCOMPCSTYPESUPPORTEDTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFCOMPCSTYPESUPPORTEDTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFCOMPCSTYPESUPPORTEDTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFCSITABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFCSITABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFCSITABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFCSITABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFCSICSIDEPTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFCSICSIDEPTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFCSICSIDEPTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFCSICSIDEPTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFCSINAMEVALUETABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFCSINAMEVALUETABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFCSINAMEVALUETABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFCSINAMEVALUETABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFCSTYPEATTRNAMETABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFCSTYPEATTRNAMETABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFCSTYPEATTRNAMETABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFCSTYPEATTRNAMETABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSUSITABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSUSITABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSUSITABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSUSITABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFHEALTHCHECKTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFHEALTHCHECKTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFHEALTHCHECKTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFHEALTHCHECKTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSCOMPCSITABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFSCOMPCSITABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSCOMPCSITABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSCOMPCSITABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFPROXYPROXIEDTABLE_PROV_CLASS_NAME    "amfMgmt_SAAMFPROXYPROXIEDTABLE_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFPROXYPROXIEDTABLE_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFPROXYPROXIEDTABLE_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFSCALARS_PROV_CLASS_NAME    "amfMgmt_SAAMFSCALARS_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFSCALARS_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFSCALARS_PROV_MAX_SLOTS    1   

#define AMFMGMT_SAAMFTRAPOBJECT_ALARM_CLASS_NAME    "amfMgmt_SAAMFTRAPOBJECT_ALARM_CLASS_NAME"
#define AMFMGMT_SAAMFTRAPOBJECT_ALARM_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFTRAPOBJECT_ALARM_MAX_SLOTS    1   

#define AMFMGMT_SAAMFTRAPOBJECT_PROV_CLASS_NAME    "amfMgmt_SAAMFTRAPOBJECT_PROV_CLASS_NAME"
#define AMFMGMT_SAAMFTRAPOBJECT_PROV_CLASS_VERSION    0x1
#define AMFMGMT_SAAMFTRAPOBJECT_PROV_MAX_SLOTS    1   

ClOmClassControlBlockT pAppOmClassTbl[] =
{
    {  
    AMFMGMT_SAAMFAPPLICATIONTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFAPPLICATIONTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFAPPLICATIONTABLEProvConstructor, 
    clamfMgmtSAAMFAPPLICATIONTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFAPPLICATIONTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFAPPLICATIONTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFAPPLICATIONTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFNODETABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFNODETABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFNODETABLEProvConstructor, 
    clamfMgmtSAAMFNODETABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFNODETABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFNODETABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFNODETABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSGTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSGTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSGTABLEProvConstructor, 
    clamfMgmtSAAMFSGTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSGTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSGTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSGTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSUTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSUTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSUTABLEProvConstructor, 
    clamfMgmtSAAMFSUTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSUTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSUTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSUTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSITABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSITABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSITABLEProvConstructor, 
    clamfMgmtSAAMFSITABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSITABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSITABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSITABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSUSPERSIRANKTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSUSPERSIRANKTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSUSPERSIRANKTABLEProvConstructor, 
    clamfMgmtSAAMFSUSPERSIRANKTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSUSPERSIRANKTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSUSPERSIRANKTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSUSPERSIRANKTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSGSIRANKTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSGSIRANKTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSGSIRANKTABLEProvConstructor, 
    clamfMgmtSAAMFSGSIRANKTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSGSIRANKTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSGSIRANKTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSGSIRANKTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSGSURANKTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSGSURANKTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSGSURANKTABLEProvConstructor, 
    clamfMgmtSAAMFSGSURANKTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSGSURANKTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSGSURANKTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSGSURANKTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSISIDEPTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSISIDEPTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSISIDEPTABLEProvConstructor, 
    clamfMgmtSAAMFSISIDEPTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSISIDEPTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSISIDEPTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSISIDEPTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFCOMPTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFCOMPTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFCOMPTABLEProvConstructor, 
    clamfMgmtSAAMFCOMPTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFCOMPTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFCOMPTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFCOMPTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFCOMPCSTYPESUPPORTEDTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFCOMPCSTYPESUPPORTEDTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFCOMPCSTYPESUPPORTEDTABLEProvConstructor, 
    clamfMgmtSAAMFCOMPCSTYPESUPPORTEDTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFCOMPCSTYPESUPPORTEDTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFCOMPCSTYPESUPPORTEDTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFCOMPCSTYPESUPPORTEDTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFCSITABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFCSITABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFCSITABLEProvConstructor, 
    clamfMgmtSAAMFCSITABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFCSITABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFCSITABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFCSITABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFCSICSIDEPTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFCSICSIDEPTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFCSICSIDEPTABLEProvConstructor, 
    clamfMgmtSAAMFCSICSIDEPTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFCSICSIDEPTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFCSICSIDEPTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFCSICSIDEPTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFCSINAMEVALUETABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFCSINAMEVALUETABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFCSINAMEVALUETABLEProvConstructor, 
    clamfMgmtSAAMFCSINAMEVALUETABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFCSINAMEVALUETABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFCSINAMEVALUETABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFCSINAMEVALUETABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFCSTYPEATTRNAMETABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFCSTYPEATTRNAMETABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFCSTYPEATTRNAMETABLEProvConstructor, 
    clamfMgmtSAAMFCSTYPEATTRNAMETABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFCSTYPEATTRNAMETABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFCSTYPEATTRNAMETABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFCSTYPEATTRNAMETABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSUSITABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSUSITABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSUSITABLEProvConstructor, 
    clamfMgmtSAAMFSUSITABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSUSITABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSUSITABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSUSITABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFHEALTHCHECKTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFHEALTHCHECKTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFHEALTHCHECKTABLEProvConstructor, 
    clamfMgmtSAAMFHEALTHCHECKTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFHEALTHCHECKTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFHEALTHCHECKTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFHEALTHCHECKTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSCOMPCSITABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSCOMPCSITABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSCOMPCSITABLEProvConstructor, 
    clamfMgmtSAAMFSCOMPCSITABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFSCOMPCSITABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSCOMPCSITABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSCOMPCSITABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFPROXYPROXIEDTABLE_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFPROXYPROXIEDTABLE_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFPROXYPROXIEDTABLEProvConstructor, 
    clamfMgmtSAAMFPROXYPROXIEDTABLEProvDestructor,
    NULL,
    AMFMGMT_SAAMFPROXYPROXIEDTABLE_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFPROXYPROXIEDTABLE_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFPROXYPROXIEDTABLE_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFSCALARS_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFSCALARS_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFSCALARSProvConstructor, 
    clamfMgmtSAAMFSCALARSProvDestructor,
    NULL,
    AMFMGMT_SAAMFSCALARS_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFSCALARS_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFSCALARS_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFTRAPOBJECT_ALARM_CLASS_NAME,
    sizeof(CL_OM_ALARM_SAAMFTRAPOBJECT_CLASS),
    CL_OM_ALARM_CLASS_TYPE,
    clamfMgmtSAAMFTRAPOBJECTAlarmConstructor, 
    clamfMgmtSAAMFTRAPOBJECTAlarmDestructor,
    NULL,
    AMFMGMT_SAAMFTRAPOBJECT_ALARM_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFTRAPOBJECT_ALARM_MAX_SLOTS,
    CL_OM_ALARM_SAAMFTRAPOBJECT_CLASS_TYPE
},

{  
    AMFMGMT_SAAMFTRAPOBJECT_PROV_CLASS_NAME,
    sizeof(CL_OM_PROV_SAAMFTRAPOBJECT_CLASS),
    CL_OM_PROV_CLASS_TYPE,
    clamfMgmtSAAMFTRAPOBJECTProvConstructor, 
    clamfMgmtSAAMFTRAPOBJECTProvDestructor,
    NULL,
    AMFMGMT_SAAMFTRAPOBJECT_PROV_CLASS_VERSION,
    0,
    CL_MAX_OBJ,
    0,
    AMFMGMT_SAAMFTRAPOBJECT_PROV_MAX_SLOTS,
    CL_OM_PROV_SAAMFTRAPOBJECT_CLASS_TYPE
},


};

ClUint32T appOmClassCnt = sizeof(pAppOmClassTbl)/sizeof(ClOmClassControlBlockT);
