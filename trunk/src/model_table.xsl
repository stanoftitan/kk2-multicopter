<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0' xmlns='http://www.w3.org/1999/xhtml'>
	<xsl:output method='text' encoding='utf-8'/>

<xsl:template match='/'>
/*
 * model_table.c
 *
 * Created: 30.07.2012 12:39:29
 *  Author: OliverS
 *
 * $Id: model_table.c 48 2013-02-27 16:30:26Z olischulz24@googlemail.com $
 */

#include "global.h"
#include "mixer.h"
#include &lt;/pgmspace.h&gt;

#define P_STR static const char PROGMEM
<xsl:for-each select='//model'>
P_STR strModel<xsl:number format='01'/>[] = "<xsl:value-of select='@name'/>";
</xsl:for-each>
	
<xsl:for-each select='//model'>
// <xsl:value-of select='@name'/>
static const model_t model<xsl:number format='01'/> PROGMEM =
{
    strModel<xsl:number format='01'/>, <xsl:value-of select='count(channel)'/>,
    {
    //    thr roll pitch yaw offs  flags
<xsl:for-each select ='channel'>        {{ <xsl:value-of select='@throttle'/>, <xsl:value-of select='@aileron'
/>, <xsl:value-of select='@elevator'/>, <xsl:value-of select='@rudder'/>, <xsl:value-of select='@offset'
/>, <xsl:choose>
	<xsl:when test='@type="motor"'>FLAG_ESC</xsl:when>
	<xsl:otherwise>FLAG_SERVO</xsl:otherwise>
</xsl:choose> }},
</xsl:for-each>
    }
};

</xsl:for-each>
</xsl:template>

</xsl:stylesheet>
