<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" indent="no" encoding="us-ascii"/>
<xsl:param name="foo" select="'default'"/>
<xsl:template match="/"><HTML><BODY>foo == <xsl:value-of select="$foo"/><xsl:apply-templates/></BODY></HTML></xsl:template>
<xsl:template match="root"><H1>root</H1><xsl:apply-templates/></xsl:template>
<xsl:template match="child"><H3>child</H3><xsl:apply-templates/></xsl:template>
</xsl:stylesheet>
