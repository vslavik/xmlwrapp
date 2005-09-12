<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
    xmlns:d="http://pmade.org/xmlns/project-download"
    xmlns:dx="http://pmade.org/pjones/dtd/dir2xml"
>

<xsl:import href="master.xsl"/>
<xsl:import href="navigation.xsl"/>
<xsl:output method="html" indent="yes" encoding="us-ascii"/>

<!--=================================================================================================-->

<xsl:param name="PEAK-XML-BASE-DIR" select="'/'"/>
<xsl:variable name="project-root"   select="document(concat($PEAK-XML-BASE-DIR, 'project.xml'))"/>
<xsl:variable name="filelist-root"  select="document(concat($PEAK-XML-BASE-DIR, 'dlfiles.xml'))"/>

<!--=================================================================================================-->
<xsl:template match="/">
    <xsl:call-template name="master-header">
	<xsl:with-param name="project-root" select="$project-root"/>
	<xsl:with-param name="title-extra"  select="' - Project Downloads'"/>
	<xsl:with-param name="this-section" select="'download'"/>
    </xsl:call-template>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="d:download">
    <xsl:apply-templates/>
    <xsl:apply-templates select="$filelist-root/dx:directory"/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="d:requirements">
    <TABLE border="0" cellpadding="2" cellspacing="0" width="{$page-max-width}">
	<TR>
	    <TD align="left" valign="top">
		<xsl:apply-templates/>
	    </TD>

	    <TD align="left" valign="top">
		<xsl:call-template name="show-latest"/>
	    </TD>
	</TR>
    </TABLE>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="d:list">
    <TABLE border="0" cellpadding="2" cellspacing="0" width="100%">
	<TR>
	    <TD class="mbox" align="left" valign="middle">
		<SPAN class="msection"><xsl:value-of select="@title"/></SPAN>
	    </TD>
	</TR>

	<TR>
	    <TD align="left" valign="top">
		<UL><xsl:apply-templates/></UL>
	    </TD>
	</TR>
    </TABLE>
    <BR/><BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="d:item">
    <LI><xsl:apply-templates/></LI>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="d:ostable">
    <TABLE border="0" cellpadding="2" cellspacing="0" width="{$page-max-width}">
	<TR>
	    <TD class="mbox" align="left" valign="middle">
		<SPAN class="msection">Supported Operating Systems and Hardware</SPAN>
	    </TD>
	</TR>

	<TR>
	    <TD align="left" valign="top">
		<TABLE border="0" cellpadding="2" cellspacing="1" width="100%">
		    <TR class="row0">
			<TD align="right" ><B>Operating System</B></TD>
			<TD align="right" ><B>Hardware</B></TD>
			<TD align="center"><B>OS &gt;= Version</B></TD>
			<TD align="left"  ><B>Compiler</B></TD>
			<TD align="left"  ><B>Status</B></TD>
			<TD align="left"  ><B>Notes</B></TD>
		    </TR>

		    <xsl:for-each select="d:os/d:compiler">
			<TR class="row{position() mod 2}">
			    <TD align="right" ><xsl:value-of select="../@name"/></TD>
			    <TD align="right" ><xsl:value-of select="../@hardware"/></TD>
			    <TD align="center"><xsl:value-of select="../@version"/></TD>
			    <TD align="left"  ><xsl:value-of  select="@name"/></TD>

			    <TD align="left">
				<FONT>
				    <xsl:choose>
					<xsl:when test="@status = 'Supported'">
					    <xsl:attribute name="color">green</xsl:attribute>
					</xsl:when>
					
					<xsl:otherwise>
					    <xsl:attribute name="color">red</xsl:attribute>
					</xsl:otherwise>
				    </xsl:choose>

				    <xsl:value-of  select="@status"/>
				</FONT>
			    </TD>

			    <TD align="left">
				<xsl:choose>
				    <xsl:when test="@notes">
					<xsl:value-of select="@notes"/>
					<xsl:if test="d:notes">&#160;<A href="#{generate-id(d:notes)}">more ...</A></xsl:if>
				    </xsl:when>

				    <xsl:otherwise>&#160;</xsl:otherwise>
				</xsl:choose>
			    </TD>
			</TR>
		    </xsl:for-each>
		</TABLE>
	    </TD>
	</TR>

	<TR>
	    <TD align="left" valign="middle">
		Any item marked with a status of "Not Supported" is being worked on and may be supported in a future
		release. See inline notes for more information.<BR/><BR/>
	    </TD>
	</TR>

	<xsl:for-each select="d:os/d:compiler">
	    <xsl:if test="d:notes">
		<TR>
		    <TD align="left" valign="middle">
			<A name="{generate-id(d:notes)}"><B><FONT color="red">Note:</FONT></B></A>&#160;
			<TT><xsl:value-of select="concat('[', ../@name, '/', ../@hardware, '/', @name, ']')"/></TT>
			<xsl:apply-templates select="d:notes"/><BR/><BR/>
		    </TD>
		</TR>
	    </xsl:if>
	</xsl:for-each>
    </TABLE>
    <BR/><BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="show-latest">
    <xsl:variable name="latest-version" select="$project-root/p:project/p:variables/p:variable[@name = 'download-source']/@value"/>

    <TABLE border="0" cellpadding="2" cellspacing="0" width="100%">
	<TR>
	    <TD class="mbox" colspan="2" align="left" valign="middle">
		<SPAN class="msection">Latest Version</SPAN>
	    </TD>
	</TR>

	<TR>
	    <TD align="right" valign="middle">Filename:&#160;</TD>
	    <TD align="left"  valign="middle"><TT><A href="download/{$filelist-root/dx:directory/dx:file[@name = $latest-version]/@link}"><xsl:value-of select="$latest-version"/></A></TT></TD>
	</TR>

	<TR>
	    <TD align="right" valign="middle">File Size:&#160;</TD>
	    <TD align="left"  valign="middle"><TT><xsl:value-of select="$filelist-root/dx:directory/dx:file[@name = $latest-version]/dx:hsize"/></TT></TD>
	</TR>

	<TR>
	    <TD align="right" valign="middle">Date/Time:&#160;</TD>
	    <TD align="left"  valign="middle"><xsl:value-of select="$filelist-root/dx:directory/dx:file[@name = $latest-version]/dx:time"/></TD>
	</TR>
    </TABLE>
    <BR/><BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="dx:directory">
    <TABLE border="0" cellpadding="2" cellspacing="0" width="{$page-max-width}">
	<TR>
	    <TD class="mbox" align="left" valign="middle">
		<SPAN class="msection">Downloads</SPAN>
	    </TD>
	</TR>

	<TR>
	    <TD align="left" valign="top">
		<TABLE border="0" cellpadding="2" cellspacing="1" width="100%">
		    <TR class="row0">
			<TD align="right"><B>Date/Time</B></TD>
			<TD align="right"><B>File Size</B></TD>
			<TD align="left" ><B>Filename</B></TD>
		    </TR>
		    
		    <xsl:for-each select="dx:file[position() &lt; 11]">
			<TR class="row{position() mod 2}">
			    <TD align="right"><xsl:value-of select="dx:time"/></TD>
			    <TD align="right"><TT><xsl:value-of select="dx:hsize"/></TT></TD>
			    <TD align="left" ><TT>
				<A href="download/{@link}"><xsl:value-of select="@name"/></A>
			    </TT></TD>
			</TR>
		    </xsl:for-each>
		</TABLE>
	    </TD>
	</TR>
    </TABLE>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>

