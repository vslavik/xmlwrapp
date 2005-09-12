<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
>

<xsl:import href="master.xsl"/>
<xsl:import href="navigation.xsl"/>
<xsl:output method="html" indent="yes" encoding="us-ascii"/>

<!--=================================================================================================-->

<xsl:param name="PEAK-XML-BASE-DIR" select="'/'"/>
<xsl:variable name="project-root" select="document(concat($PEAK-XML-BASE-DIR, 'project.xml'))"/>

<!--=================================================================================================-->
<xsl:template match="/">
    <xsl:call-template name="master-header">
	<xsl:with-param name="project-root" select="$project-root"/>
	<xsl:with-param name="title-extra"  select="' - API Documentation'"/>
	<xsl:with-param name="this-section" select="'doxygen'"/>
    </xsl:call-template>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:project">
    <HR width="{$page-max-width}" size="2" noshade="yes"/>
    <BR/><BR/>

    <DIV align="center">
	<TABLE border="0" cellpadding="0" cellspacing="0" width="{$page-max-width}">
	    <TR>
		<TD align="left" valign="top">
		    <xsl:text>

		    </xsl:text>

		    <xsl:comment>SPLIT-ME</xsl:comment>

		    <xsl:text>

		    </xsl:text>
		</TD>
	    </TR>
	</TABLE>
    </DIV>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>
