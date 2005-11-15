<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
    xmlns:n="http://pmade.org/xmlns/project-news"
    xmlns:r="http://pmade.org/xmlns/project-resources"
>
<xsl:import href="master.xsl"/>
<xsl:output method="html" indent="yes" encoding="us-ascii"/>
<!--=================================================================================================-->

<xsl:param name="PEAK-XML-BASE-DIR" select="'/'"/>
<xsl:variable name="project-root" select="document(concat($PEAK-XML-BASE-DIR, 'project.xml'))"/>

<!--=================================================================================================-->
<xsl:template match="/">
    <xsl:call-template name="master-header">
	<xsl:with-param name="project-root" select="$project-root"/>
	<xsl:with-param name="title-extra"  select="' - Project News'"/>
	<xsl:with-param name="this-section" select="'news'"/>
    </xsl:call-template>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="n:news">
    <xsl:apply-templates select="n:entry[position() &lt; 11]"/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="n:entry">
    <TABLE boder="0" cellpadding="2" cellspacing="0" width="{$page-max-width}">
	<TR>
	    <TD class="mbox" align="left" valign="middle">
		<TABLE border="0" width="100%" cellpadding="2" cellspacing="2">
		    <TR>
			<TD align="left" valign="middle">
			    <DIV class="msection">
				<xsl:value-of select="@title"/>
			    </DIV>
			</TD>
			
			<TD align="right" valign="middle">
			    <xsl:call-template name="master-output-date">
				<xsl:with-param name="year" select="@year"/>
				<xsl:with-param name="month" select="@month"/>
				<xsl:with-param name="day" select="@day"/>
			    </xsl:call-template>
			</TD>
		    </TR>
		</TABLE>
	    </TD>

	</TR>

	<TR>
	    <TD align="left" valign="middle">
		<xsl:apply-templates/>
	    </TD>
	</TR>
    </TABLE>
    <BR/><BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="n:para">
    <P><xsl:apply-templates/></P>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>
