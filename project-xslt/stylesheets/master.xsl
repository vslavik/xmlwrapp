<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
    xmlns:date="http://exslt.org/dates-and-times"
    extension-element-prefixes="date"
>
<!--=================================================================================================-->
<xsl:import href="navigation.xsl"/>
<xsl:import href="linking.xsl"/>
<xsl:import href="formating.xsl"/>
<!--=================================================================================================-->
<!-- General Variables ==============================================================================-->
<xsl:param name="page-background-color"		select="'#ffffff'"/>
<xsl:param name="page-foreground-color"		select="'#000000'"/>
<xsl:param name="page-max-width"		select="'750'"/>
<xsl:param name="image-dir"			select="'../images/'"/>
<xsl:param name="html-dir"			select="''"/>
<xsl:param name="bugs-base"			select="'http://bugs.pmade.org/bugzilla/enter_bug.cgi?product='"/>
<xsl:param name="cvs-base"			select="'http://cvs.pmade.org/view/'"/>
<!--=================================================================================================-->
<xsl:variable name="portfolio-link"		>http://pmade.org/portfolio/</xsl:variable>
<xsl:variable name="resume-link"		>http://pmade.org/portfolio/resume.html</xsl:variable>
<xsl:variable name="email"			>software@pmade.org</xsl:variable>
<!--=================================================================================================-->
<!-- Master Style  Variables ========================================================================-->
<xsl:param name="master-box-color"		select="'#e2e2e2'"/>
<xsl:param name="master-box-border-color"	select="'#9fc7ea'"/>
<xsl:param name="master-rule-color"		select="'#e2e2e2'"/>
<xsl:param name="master-link-color"		select="'#4c8dbf'"/>
<xsl:param name="master-vlink-color"		select="'#4c8dbf'"/>
<xsl:param name="master-hlink-color"		select="'#cccccc'"/>
<xsl:param name="master-copyright-color"	select="'#444444'"/>
<xsl:param name="master-row-even-color"		select="'#dddddd'"/>
<xsl:param name="master-row-odd-color"		select="'#eeeeee'"/>
<!--=================================================================================================-->
<xsl:attribute-set name="body-attr-set">
    <xsl:attribute name="bgcolor"><xsl:value-of select="$page-background-color"/></xsl:attribute>
    <xsl:attribute name="text"><xsl:value-of select="$page-foreground-color"/></xsl:attribute>
    <xsl:attribute name="marginwidth">0</xsl:attribute>
    <xsl:attribute name="marginheight">0</xsl:attribute>
    <xsl:attribute name="leftmargin">0</xsl:attribute>
    <xsl:attribute name="topmargin">0</xsl:attribute>
</xsl:attribute-set>
<!--=================================================================================================-->
<xsl:template name="master-handle-head">
    <STYLE type="text/css">
	<xsl:comment>
	    a:link {
		text-decoration: none;
		color: <xsl:value-of select="$master-link-color"/>;
	    }

	    a:active {
		text-decoration: none;
		color: <xsl:value-of select="$master-link-color"/>;
	    }

	    a:visited {
		text-decoration: none;
		color: <xsl:value-of select="$master-vlink-color"/>;
	    }

	    a:hover {
		text-decoration: underline;
		color: <xsl:value-of select="$master-link-color"/>;
		background-color: <xsl:value-of select="$master-hlink-color"/>;
	    }

	    .mbox {
		border-width: 2px;
		border-style: solid;
		border-color: <xsl:value-of select="$master-box-border-color"/>;
		background-color: <xsl:value-of select="$master-box-color"/>;
	    }

	    .mrule {
		border-width: 2px;
		border-style: solid;
		border-color: <xsl:value-of select="$master-rule-color"/>;
	    }
    
	    .msection {
		font-weight: bold;
		font-size: 110%;
		color: <xsl:value-of select="$page-foreground-color"/>;
	    }

	    .mblocktext {
		font-size: 120%;
		text-align: justify;
	    }

	    .mcopyright {
		font-size: 80%;
		font-style: italic;
		color: <xsl:value-of select="$master-copyright-color"/>;
	    }

	    .row0 {
		background-color: <xsl:value-of select="$master-row-even-color"/>;
	    }

	    .row1 {
		background-color: <xsl:value-of select="$master-row-odd-color"/>;
	    }
	</xsl:comment>
    </STYLE>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="master-header">
    <xsl:param name="project-root" select="'missing'"/>
    <xsl:param name="title-extra" select="''"/>
    <xsl:param name="this-section" select="'main'"/>
    <xsl:param name="logo-text" select="''"/>

    <HTML>
	<HEAD>
	    <TITLE><xsl:value-of select="concat($project-root/p:project/@name, $title-extra)"/></TITLE>
	    <xsl:call-template name="master-handle-head"/>
	    <xsl:call-template name="navigation-handle-head"/>

	    <xsl:if test="$this-section = 'doxygen'">
		<LINK href="doxygen.css" rel="stylesheet" type="text/css"/>
	    </xsl:if>
	</HEAD>

	<BODY xsl:use-attribute-sets="body-attr-set">

	    <xsl:call-template name="navigation-top-bar">
		<xsl:with-param name="name" select="$project-root/p:project/@name"/>
	    </xsl:call-template>

	    <DIV align="center">
		<TABLE border="0" width="{$page-max-width}" cellpadding="0" cellspacing="0">
		    <TR>
			<TD width="50%" align="center" valign="middle">
			    <IMG
				src="{$image-dir}{$project-root/p:project/p:variables/p:variable[@name = 'logo-image']/@value}"
				alt="{$project-root/p:project/@name} Logo"
				border="0" 
			    />
			</TD>
			<TD width="50%">
			    <xsl:choose>
				<xsl:when test="$this-section = 'main'">
				    <xsl:attribute name="align">left</xsl:attribute>
				    <xsl:attribute name="valign">top</xsl:attribute>
				    <xsl:attribute name="class">mblocktext</xsl:attribute>
				    <xsl:apply-templates select="$project-root/p:project/p:description"/>
				</xsl:when>

				<xsl:when test="$this-section = 'resources'">
				    <xsl:attribute name="align">center</xsl:attribute>
				    <xsl:attribute name="valign">middle</xsl:attribute>
				    <IMG src="{$image-dir}resources.jpg" border="0"/>
				</xsl:when>

				<xsl:when test="$this-section = 'news'">
				    <xsl:attribute name="align">center</xsl:attribute>
				    <xsl:attribute name="valign">middle</xsl:attribute>
				    <IMG src="{$image-dir}news.jpg" border="0"/>
				</xsl:when>

				<xsl:when test="$this-section = 'download'">
				    <xsl:attribute name="align">center</xsl:attribute>
				    <xsl:attribute name="valign">middle</xsl:attribute>
				    <IMG src="{$image-dir}download.jpg" border="0"/>
				</xsl:when>

				<xsl:when test="$this-section = 'doxygen'">
				    <xsl:attribute name="align">center</xsl:attribute>
				    <xsl:attribute name="valign">middle</xsl:attribute>
				    <IMG src="{$image-dir}doxygen.jpg" border="0"/>
				</xsl:when>

				<xsl:when test="string-length($logo-text)">
				    <xsl:attribute name="align">left</xsl:attribute>
				    <xsl:attribute name="valign">top</xsl:attribute>
				    <xsl:attribute name="class">mblocktext</xsl:attribute>
				    <xsl:value-of select="$logo-text"/>
				</xsl:when>

				<xsl:otherwise>
				    Missing Logo
				</xsl:otherwise>
			    </xsl:choose>
			</TD>
		    </TR>
		</TABLE>
		    
		<BR/>

		<xsl:apply-templates mode="navigation-generate" select="$project-root/p:project">
		    <xsl:with-param name="this-section" select="$this-section"/>
		</xsl:apply-templates>

		<xsl:apply-templates/>

		<BR/><BR/>

		<TABLE border="0" cellpadding="0" cellspacing="0" width="{$page-max-width}">
		    <xsl:if test="$project-root/p:project/p:variables/p:variable[@name = 'sourceforge']">
			<xsl:variable name="sfid" select="$project-root/p:project/p:variables/p:variable[@name = 'sfid']/@value"/>

			<TR>
			    <TD colspan="2" width="100%" align="right" valign="middle">
				<A href="{$project-root/p:project/p:variables/p:variable[@name = 'sourceforge']/@value}">
				    <IMG src="http://sourceforge.net/sflogo.php?group_id={$sfid}&amp;type=1" width="88" height="31" border="0" alt="SourceForge.net Logo"/>
				</A>
			    </TD>
			</TR>
		    </xsl:if>
		    <TR><TD colspan="2" width="100%"><HR width="100%" size="2" noshade="yes"/></TD></TR>
		    <TR>
			<TD align="left" valign="top">
			    <SPAN class="mcopyright">
				Copyright &#169; <xsl:value-of select="date:year()"/>
				by Peter Jones (<B><A href="{$portfolio-link}">Portfolio</A>, <A href="{$resume-link}">Resume</A></B>), 
				All Rights Reserved
			    </SPAN>
			</TD>

			<TD align="right" valign="top">
			    <A class="mcopyright" href="mailto:{$email}"><xsl:value-of select="$email"/></A>
			</TD>
		    </TR>
		</TABLE>

		<BR/><BR/>
	    </DIV>

	</BODY>
    </HTML>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="master-output-date">
    <xsl:param name="year"  select="'1974'"/>
    <xsl:param name="month" select="'01'"/>
    <xsl:param name="day"   select="'01'"/>

    <xsl:value-of select="date:day-name(concat($year, '-', $month, '-', $day))"/>,&#160;
    <xsl:value-of select="date:month-name(concat('--', $month, '--'))"/>&#160;

    <xsl:choose>
	<xsl:when test="$day = '01' or $day = '21' or $day = '31'">
	    <xsl:value-of select="concat($day, 'st,')"/>&#160;
	</xsl:when>

	<xsl:when test="$day = '02' or $day = '22'">
	    <xsl:value-of select="concat($day, 'nd,')"/>&#160;
	</xsl:when>
    
	<xsl:when test="$day = '03' or $day = '23'">
	    <xsl:value-of select="concat($day, 'rd,')"/>&#160;
	</xsl:when>

	<xsl:otherwise>
	    <xsl:value-of select="concat($day, 'th,')"/>&#160;
	</xsl:otherwise>
    </xsl:choose>

    <xsl:value-of select="$year"/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="master-section-rule">
    <DIV align="left"><HR class="mrule" width="90%"/></DIV>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="master-title-to-anchor">
    <xsl:param name="title" select="MISSING"/>
    <xsl:value-of select="translate($title, ' ', '_')"/>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>
