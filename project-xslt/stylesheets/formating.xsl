<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:format="http://pmade.org/xmlns/project-formating"
>
<!--=================================================================================================-->
<xsl:template match="format:monospace"><TT><xsl:apply-templates/></TT></xsl:template>
<!--=================================================================================================-->
<xsl:template match="format:italic"><I><xsl:apply-templates/></I></xsl:template>
<!--=================================================================================================-->
<xsl:template match="format:bold">
    <B>
	<xsl:choose>
	    <xsl:when test="@color"><FONT color="{@color}"><xsl:apply-templates/></FONT></xsl:when>
	    <xsl:otherwise><xsl:apply-templates/></xsl:otherwise>
	</xsl:choose>
    </B>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>

