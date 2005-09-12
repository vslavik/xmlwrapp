<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:link="http://pmade.org/xmlns/project-linking"
>
<!--=================================================================================================-->
<xsl:template match="link:ulink">
    <A href="{@href}"><xsl:apply-templates/></A>
</xsl:template>
<!--=================================================================================================-->
<!--
    vim:tw=120
-->
</xsl:stylesheet>
