<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml"/>

    <xsl:template name="foo">
        <output>
            <xsl:value-of select="$x"/>
        </output>
        <!-- this is intentionally invalid: the parameters must be declared
             at the top of the template -->
        <xsl:param name="x"/>
    </xsl:template>

    <xsl:template match="/">
        <xsl:call-template name="foo">
            <xsl:with-param name="x">42</xsl:with-param>
        </xsl:call-template>
    </xsl:template>

</xsl:stylesheet>
