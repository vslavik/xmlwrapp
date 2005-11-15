<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:p="http://pmade.org/xmlns/project-main"
    xmlns:r="http://pmade.org/xmlns/project-resources">

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
	<xsl:with-param name="title-extra"  select="' - Project Resources'"/>
	<xsl:with-param name="this-section" select="'resources'"/>
    </xsl:call-template>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="r:resources">
    <TABLE border="0" cellpadding="2" cellspacing="0" width="{$page-max-width}">
	<xsl:for-each select="r:section">
	    <TR>
		<TD align="center" valign="top">
		    <xsl:apply-templates select="."/>
		    <BR/><BR/>
		</TD>
	    </TR>
	</xsl:for-each>
    </TABLE>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="r:section">
    <xsl:variable name="anchor">
	<xsl:call-template name="master-title-to-anchor">
	    <xsl:with-param name="title" select="@title"/>
	</xsl:call-template>
    </xsl:variable>

    <TABLE cellpadding="2" cellspacing="0" width="100%">
	<TR>
	    <TD class="mbox" align="left" valign="middle" colspan="2">
		<A name="{$anchor}"/>
		<SPAN class="msection"><xsl:value-of select="@title"/></SPAN>
	    </TD>
	</TR>

	<xsl:apply-templates/>
    </TABLE>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="r:item">
    <xsl:variable name="link">
	<xsl:choose>
	    <xsl:when test="starts-with(@link, 'CVS/')">
		<xsl:value-of select="concat($cvs-base, substring(@link, 5))"/>
		<xsl:if test="not(contains(@link, '?'))">?rev=HEAD&amp;content-type=text/vnd.viewcvs-markup</xsl:if>
	    </xsl:when>

	    <xsl:otherwise>
		<xsl:value-of select="@link"/>
	    </xsl:otherwise>
	</xsl:choose>
    </xsl:variable>

    <TR>
	<TD align="right" valign="top">
	    <A href="{$link}"><xsl:value-of select="@title"/></A>
	</TD>

	<TD align="left" valign="top">
	    <xsl:apply-templates/>
	</TD>
    </TR>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="r:anoncvs">
    <xsl:variable name="prjname" select="$project-root/p:project/@name"/>

    <xsl:variable name="cvs-module">
	<xsl:choose>
	    <xsl:when test="@module"><xsl:value-of select="@module"/></xsl:when>
	    <xsl:otherwise><xsl:value-of select="$prjname"/></xsl:otherwise>
	</xsl:choose>
    </xsl:variable>

    <TR>
	<TD align="center" valign="top">
	    <TABLE border="0" width="90%" cellpadding="4" cellspacing="0">
		<TR>
		    <TD align="left">
			It is possible to checkout the source code for <xsl:value-of select="$prjname"/>
			using an anonymous cvs account. First, login using the anonymous username.
		    </TD>
		</TR>

		<TR>
		    <TD align="center">
			<TT>cvs -d :pserver:anonymous@cvs.pmade.org:/webcvs login</TT>
		    </TD>
		</TR>

		<TR>
		    <TD align="left">
			Although you will be prompted for a password, this anonymous
			account does not have one. You should just press enter. Then you
			can checkout <xsl:value-of select="$cvs-module"/>.
		    </TD>
		</TR>

		<TR>
		    <TD align="center">
			<TT>cvs -d :pserver:anonymous@cvs.pmade.org:/webcvs co <xsl:value-of select="$cvs-module"/></TT>
		    </TD>
		</TR>

		<TR>
		    <TD align="left">
			Since all development is done on a branch, you might want to use
			the <A href="{$cvs-base}{$cvs-module}/">cvs web interface</A>
			to find the branch you want to checkout. For example, to
			checkout branch <TT>BRN-0_3_0</TT> you would use the following command.
		    </TD>
		</TR>

		<TR>
		    <TD align="center">
			<TT>cvs -d :pserver:anonymous@cvs.pmade.org:/webcvs co -r BRN-0_3_0 <xsl:value-of select="$cvs-module"/></TT>
		    </TD>
		</TR>
	    </TABLE>
	</TD>
    </TR>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="r:mlists">
    <xsl:variable name="subscribe">http://pmade.org/cgi-bin/subscribe.pl</xsl:variable>
    <xsl:variable name="base">http://pmade.org/</xsl:variable>

    <TR>
	<TD align="center" valign="top" colspan="2">
	    <TABLE border="0" width="90%" cellpadding="4" cellspacing="0">
		<TR>
		    <TD align="center" valign="top">
			<FORM method="POST" action="{$subscribe}">
			    <INPUT type="hidden" name="digest" value="0"/>
			    <INPUT type="hidden" name="lc" value="{count(r:list)}"/>

			    <TABLE border="0" cellpadding="4" cellspacing="0" width="100%">
				<TR>
				    <TD class="mbox" colspan="2" align="center" valign="middle">
					<DIV class="msection">Subscribe</DIV>
				    </TD>
				</TR>

				<TR>
				    <TD align="right" valign="middle">Your Name:</TD>
				    <TD align="left"  valign="middle"><INPUT type="Text" name="fullname" size="30"/></TD>
				</TR>

				<TR>
				    <TD align="right" valign="middle">Your email address:</TD>
				    <TD align="left"  valign="middle"><INPUT type="Text" name="email" size="30"/></TD>
				</TR>

				<TR>
				    <TD align="right" valign="middle">Pick a password:</TD>
				    <TD align="left"  valign="middle"><INPUT type="Password" name="pw" size="30"/></TD>
				</TR>

				<TR>
				    <TD align="right" valign="middle">Reenter password:</TD>
				    <TD align="left"  valign="middle"><INPUT type="Password" name="pw-conf" size="30"/></TD>
				</TR>

				<xsl:for-each select="r:list">
				    <TR>
					<TD align="left" valign="middle" nowrap="true">
					    <INPUT type="checkbox" checked="true" name="ck{position()}" value="1"/>
					    <xsl:value-of select="@title"/>
					    <INPUT type="hidden" name="ckl{position()}" value="{@name}"/>
					</TD>

					<xsl:if test="position() = 1">
					    <TD rowspan="{count(../r:list)}" align="right" valign="middle">
						<INPUT type="Submit" name="email-button" value="Subscribe"/>
					    </TD>
					</xsl:if>
				    </TR>
				</xsl:for-each>

			    </TABLE>
			</FORM>
		    </TD>

		    <TD align="center" valign="top">
			<TABLE border="0" cellpadding="4" cellspacing="0" width="100%">
			    <TR>
				<TD class="mbox" align="center" valign="middle">
				    <DIV class="msection">Archives and Options</DIV>
				</TD>
			    </TR>

			    <xsl:for-each select="r:list">
				<TR>
				    <TD align="left" valign="middle">
					<DIV class="msection"><xsl:value-of select="@title"/></DIV>
					[<A href="{$base}mailman/options/{@name}">edit options</A>] &#160;
					[<A href="{$base}pipermail/{@name}">view archives</A>]
				    </TD>
				</TR>

				<TR>
				    <TD align="left" valign="top">
					<xsl:apply-templates/><BR/><BR/>
				    </TD>
				</TR>
			    </xsl:for-each>
			</TABLE>
		    </TD>
		</TR>
	    </TABLE>
	</TD>
    </TR>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="r:text">
    <TR>
	<TD align="center" valign="top" colspan="2">
	    <TABLE border="0" width="90%" cellpadding="4" cellspacing="0">
		<xsl:apply-templates/>
	    </TABLE>
	</TD>
    </TR>
</xsl:template>
<!--=================================================================================================-->
<xsl:template match="r:para">
    <TR>
	<TD align="left"><xsl:apply-templates/></TD>
    </TR>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>
