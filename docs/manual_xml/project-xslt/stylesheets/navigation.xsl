<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
>
<!--=================================================================================================-->
<xsl:template name="navigation-handle-head">
    <STYLE type="text/css">
	<xsl:comment>
	    .navinactive {
		color: <xsl:value-of select="$page-foreground-color"/>;
		background-color: <xsl:value-of select="$page-background-color"/>;
		border-width: 2px;
		border-style: solid;
		border-color: <xsl:value-of select="$master-rule-color"/>;
		font-weight: bold;
		padding: 4px;
	    }

	    .navactive {
		color: <xsl:value-of select="$master-hlink-color"/>;
		background-color: <xsl:value-of select="$page-background-color"/>;
		border-width: 2px;
		border-style: solid;
		border-color: <xsl:value-of select="$master-box-border-color"/>;
		font-weight: bold;
		padding: 4px;
	    }
    	</xsl:comment>
    </STYLE>
</xsl:template>	
<!--=================================================================================================-->
<xsl:template name="navigation-top-bar">
    <xsl:param name="name" select="'missing project name'"/>

    <TABLE border="0" width="100%" cellpadding="0" cellspacing="0">
	<TR>
	    <TD align="left">&#160;<A href="http://pmade.org">pmade.org</A></TD>
	    <TD align="right"><A href="http://pmade.org/software/">Software</A>&#160;<B>&gt;</B>&#160;<xsl:value-of select="$name"/>&#160;</TD>
	</TR>
    </TABLE>
    
    <BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template mode="navigation-generate" match="p:project">
    <xsl:param name="this-section" select="'main'"/>

    <TABLE border="0" width="{$page-max-width}" cellpadding="0" cellspacing="0">
	<TR>
	    <TD align="center" valign="middle">
		<TABLE border="0" cellpadding="2" cellspacing="0" width="100%">
		    <TR>
			<xsl:call-template name="navigation-main">
			    <xsl:with-param name="this-section" select="$this-section"/>
			</xsl:call-template>

			<xsl:apply-templates mode="navigation-generate" select="p:section">
			    <xsl:with-param name="this-section" select="$this-section"/>
			</xsl:apply-templates>
		    </TR>
		</TABLE>
	    </TD>
	</TR>
    </TABLE>

    <BR/>
</xsl:template>
<!--=================================================================================================-->
<xsl:template name="navigation-main">
    <xsl:param name="this-section" select="'main'"/>

    <TD align="center" valign="middle">
	<xsl:choose>
	    <xsl:when test="$this-section = 'main'">
		<xsl:attribute name="class">navactive</xsl:attribute>
		<xsl:text>main</xsl:text>
	    </xsl:when>

	    <xsl:otherwise>
		<xsl:attribute name="class">navinactive</xsl:attribute>
		<A href="{$html-dir}index.html">main</A>
	    </xsl:otherwise>
	</xsl:choose>
    </TD>

    <xsl:if test="p:section">
	<TD align="center" valign="middle">
	    <HR size="2" width="100%" noshade="yes"/>
	</TD>
    </xsl:if>
</xsl:template>
<!--=================================================================================================-->
<xsl:template mode="navigation-generate" match="p:section">
    <xsl:param name="this-section" select="'main'"/>

    <xsl:variable name="link">
	<xsl:choose>
	    <xsl:when test="@title = 'bugs'">
		<xsl:value-of select="concat($bugs-base, @product)"/>
	    </xsl:when>

	    <xsl:when test="@title = 'cvs'">
		<xsl:value-of select="concat($cvs-base, @module)"/>
	    </xsl:when>

	    <xsl:when test="@link">
		<xsl:value-of select="@link"/>
	    </xsl:when>

	    <xsl:when test="@file and contains(@file, '.xml')">
		<xsl:value-of select="concat($html-dir, substring(@file, 0, string-length(@file) - 3), '.html')"/>
	    </xsl:when>
	</xsl:choose>
    </xsl:variable>

    <TD align="center" valign="middle">
	<xsl:choose>
	    <xsl:when test="$this-section = @title">
		<xsl:attribute name="class">navactive</xsl:attribute>
		<xsl:value-of select="@title"/>
	    </xsl:when>

	    <xsl:otherwise>
		<xsl:attribute name="class">navinactive</xsl:attribute>
		<A href="{$link}"><xsl:value-of select="@title"/></A>
	    </xsl:otherwise>
	</xsl:choose>
    </TD>

    <xsl:if test="position() != last()">
	<TD align="center" valign="middle">
	    <HR size="2" width="100%" noshade="yes"/>
	</TD>
    </xsl:if>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>
