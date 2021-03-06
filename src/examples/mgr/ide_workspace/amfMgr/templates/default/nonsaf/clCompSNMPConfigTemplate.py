from string import Template
   
headerTemplate = Template("""\
/******************************************************************************
 *
 * clSNMPConfig.c
 *
 ******************************************************************************
 * This code is auto-generated by OpenClovis IDE Version 3.1
 *
 *****************************************************************************/
 
#include <clSnmpDataDefinition.h>
#include <clCorMetaStruct.h>
#include <clAlarmDefinitions.h>




ClSnmpOidMapTableItemT  clOidMapTable[] = {
""")


trapOidMapTableTemplate = Template("""\
ClSnmpTrapOidMapTableItemT  clTrapOidMapTable[] = {
""")


moTemplate = Template("""\
{CLASS_${classid}_MO, CL_COR_INSTANCE_WILD_CARD}
""")

oidTemplate = Template("""
{{(ClUint8T *) \"${oid}\", 1},
{{""")

serviceIdTemplate = Template("""\
}, ${serviceId}, ${depth}}, {""")

AttributeIdTemplate = Template("""\
${attrId}, """)

probableCauseTemplate = Template("""\
(ClUint32T) ${probCause}, ${specProb}, NULL},

""")

AttributeEndString = """\
CL_MED_ATTR_VALUE_END}},
"""

StructEndString = """\
{{NULL, 0}, {{{0, 0}}, 0, 0}, {0}}
};
"""
TrapEndString = """\
{{NULL, 0}, {{{0, 0}}, 0, 0}, {0}, 0, 0, NULL}
};
"""
