<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
>
<!--=================================================================================================-->
<xsl:output method="html" indent="yes" encoding="us-ascii"/>
<!--=================================================================================================-->
<xsl:template match="/">
    <xsl:apply-templates/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:project">
    <HTML>
	<BODY>
	    <HR/>
	    <DIV align="center"><H1><xsl:value-of select="@name"/></H1></DIV>
	    <HR/>
	    <xsl:apply-templates/>

	    <xsl:if test="p:variables/p:variable[@name = 'website']">
		<BR/>
		For more information about <xsl:value-of select="@name"/> please go to:<BR/>
		<xsl:value-of select="p:variables/p:variable[@name = 'website']/@value"/>
	    </xsl:if>
	</BODY>
    </HTML>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:description">
    <xsl:apply-templates/>
    <BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:para">
    <P><xsl:apply-templates/></P>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:features">
    <H1>Features:</H1>
    <UL><xsl:apply-templates/></UL>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:feature">
    <LI><xsl:apply-templates/></LI>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="*"/>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>
