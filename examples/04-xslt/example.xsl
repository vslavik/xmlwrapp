<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" indent="yes" encoding="us-ascii"/>
<!--========================================================================-->
<xsl:template match="/">
    <xsl:apply-templates/>
</xsl:template>
<!--========================================================================-->
<xsl:template match="root">
    root - <xsl:value-of select="@name"/>
    <xsl:apply-templates/>
</xsl:template>
<!--========================================================================-->
<xsl:template match="child">
    child - <xsl:value-of select="@name"/>
</xsl:template>
<!--========================================================================-->
</xsl:stylesheet>
