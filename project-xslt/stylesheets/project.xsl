<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
    xmlns:n="http://pmade.org/xmlns/project-news"
    xmlns:r="http://pmade.org/xmlns/project-resources"
>
<xsl:import href="master.xsl"/>
<xsl:output method="html" indent="yes" encoding="us-ascii"/>
<!--=================================================================================================-->
<xsl:template match="/">
    <xsl:call-template name="master-header">
	<xsl:with-param name="project-root" select="."/>
	<xsl:with-param name="title-extra"  select="''"/>
	<xsl:with-param name="this-section" select="'main'"/>
    </xsl:call-template>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:project">
    <TABLE border="0" width="{$page-max-width}" cellpadding="0" cellspacing="0">
	<TR>
	    <TD align="left" valign="top" width="70%">
		<xsl:choose>
		    <xsl:when test="p:features">
			<xsl:call-template name="project-features"/>
		    </xsl:when>
		
		    <xsl:otherwise>&#160;</xsl:otherwise>
		</xsl:choose>
	    </TD>
	    <TD rowspan="3" align="left" valign="top" width="30%" height="100%">
		<xsl:choose>
		    <xsl:when test="p:section[@title = 'resources']">
			<xsl:call-template name="project-resources"/>
		    </xsl:when>

		    <xsl:otherwise>&#160;</xsl:otherwise>
		</xsl:choose>
	    </TD>
	</TR>

	<TR>
	    <TD align="left" valign="top" width="70%">
		<xsl:choose>
		    <xsl:when test="p:section[@title = 'news']">
			<xsl:apply-templates select="document(p:section[@title = 'news']/@file)/n:news/n:entry[1]"/>
		    </xsl:when>

		    <xsl:otherwise>&#160;</xsl:otherwise>
		</xsl:choose>
	    </TD>
	</TR>

	<TR>
	    <TD align="left" valign="top" width="70%">
		<xsl:choose>
		    <xsl:when test="p:variables/p:variable[@name = 'download-source']">
			<xsl:call-template name="project-download"/>
		    </xsl:when>
		
		    <xsl:otherwise>&#160;</xsl:otherwise>
		</xsl:choose>
	    </TD>
	</TR>
    </TABLE>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="project-features">
    <TABLE border="0" cellpadding="2" cellspacing="0" width="98%">
	<TR>
	    <TD class="mbox" align="left" valign="middle" width="100%">
		<SPAN class="msection">Features:</SPAN>
	    </TD>

	</TR>

	<TR>
	    <TD align="left" valign="top">
		<UL>
		    <xsl:for-each select="p:features/p:feature">
			<LI><xsl:apply-templates/></LI>
		    </xsl:for-each>
		</UL>
	    </TD>
	</TR>
    </TABLE>
    <BR/><BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="project-download">
    <xsl:variable name="download-source" select="p:variables/p:variable[@name = 'download-source']/@value"/>

    <TABLE border="0" cellpadding="2" cellspacing="0" width="98%">
	<TR>
	    <TD class="mbox" align="left" valign="middle" width="100%">
		<SPAN class="msection">Latest Version:&#160;</SPAN>
		<A href="download/{$download-source}"><TT><xsl:value-of select="$download-source"/></TT></A>
	    </TD>
	</TR>

	<xsl:if test="p:section[@title = 'download']">
	    <xsl:variable name="download-file" select="p:section[@title = 'download']/@file"/>

	    <TR>
		<TD align="left" valign="top">
		    For build and use requirements, please see the
		    <A href="{substring($download-file, 0, string-length($download-file) - 2)}html">download page</A>.
		</TD>
	    </TR>
	</xsl:if>
    </TABLE>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="project-resources">
    <xsl:variable name="rfile" select="p:section[@title = 'resources']/@file"/>

    <TABLE cellpadding="2" cellspacing="0" border="0" width="98%">
	<TR>
	    <TD class="mbox" align="left" valign="middle" width="100%">
		<A class="msection" href="{substring($rfile, 0, string-length($rfile) - 2)}html">Resources</A>
	    </TD>
	</TR>
	<TR>
	    <TD align="center" valign="top">
		<TABLE border="0" cellpadding="2" cellspacing="0">
		    <xsl:for-each select="document($rfile)/r:resources/r:section">
			<xsl:variable name="anchor">
			    <xsl:call-template name="master-title-to-anchor">
				<xsl:with-param name="title" select="@title"/>
			    </xsl:call-template>
			</xsl:variable>

			<TR>
			    <TD>
				<A href="{substring($rfile, 0, string-length($rfile) - 2)}html#{$anchor}"><xsl:value-of select="@title"/></A>
			    </TD>
			</TR>
		    </xsl:for-each>

		    <xsl:if test="p:section[@title = 'news']/@file">
			<TR>
			    <TD>
				<xsl:variable name="nfile" select="p:section[@title = 'news']/@file"/>
				<A href="{substring($nfile, 0, string-length($nfile) - 2)}html">Project News</A>
			    </TD>
			</TR>
		    </xsl:if>

		    <xsl:if test="p:section[@title = 'cvs']/@link">
			<TR>
			    <TD>
				<A href="{p:section[@title = 'cvs']/@link}">CVS Web Access</A>
			    </TD>
			</TR>
		    </xsl:if>
		</TABLE>
	    </TD>
	</TR>
    </TABLE>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:description">
    <xsl:apply-templates/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="p:para">
    <P><xsl:apply-templates/></P>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="n:entry">
    <TABLE border="0" cellpadding="2" cellspacing="0" width="98%">
	<TR>
	    <TD class="mbox" align="left" valign="middle" width="100%">
		<SPAN class="msection">Latest News:&#160;</SPAN>
		<xsl:call-template name="master-output-date">
		    <xsl:with-param name="year"	    select="@year"/>
		    <xsl:with-param name="month"    select="@month"/>
		    <xsl:with-param name="day"	    select="@day"/>
		</xsl:call-template>
	    </TD>
	</TR>
	
	<TR>
	    <TD align="left" valign="top">
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
