<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt" exclude-result-prefixes="msxsl branding"
                xmlns:mtps="http://msdn2.microsoft.com/mtps"
                xmlns:xhtml="http://www.w3.org/1999/xhtml"
                xmlns:branding="urn:FH-Branding"
                xmlns:xs="http://www.w3.org/2001/XMLSchema"
                >

  <xsl:template name="footer">
    <xsl:element name="div" namespace="{$xhtml}">
      <xsl:attribute name="class">OH_footer</xsl:attribute>
      <xsl:element name="p" namespace="{$xhtml}">
        <xsl:value-of disable-output-escaping="yes" select="concat('&#xA9;',' ',$copyright)"/>
      </xsl:element>
      <xsl:call-template name="feedback-link-bottom">
        <xsl:with-param name="prolog">
          <xsl:value-of select="concat('&#160;',$feedbackfooterText)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:element>
  </xsl:template>

  <xsl:template name="feedback-link">
    <xsl:param name="prolog" select="''"/>
    <xsl:element name="div" namespace="{$xhtml}">
      <xsl:attribute name="class">OH_feedbacklink</xsl:attribute>
      <xsl:element name="a" namespace="{$xhtml}">
        <xsl:attribute name="href">
          <xsl:choose>
            <xsl:when test="starts-with($feedbacklink,'http:') or starts-with($feedbacklink,'https:')">
              <xsl:value-of select="$feedbacklink"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="'mailto:'"/>
              <xsl:value-of select="$feedback-alias"/>
              <xsl:value-of select="'?subject='" />
              <xsl:value-of select="branding:GetUrlEncode2($feedback-subject)" />
              <xsl:value-of select="'&amp;body='" />
              <xsl:value-of select="branding:replace(branding:GetUrlEncode2($feedbackprivactText),'+','%20')" />
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="target">
          <xsl:if test="starts-with($feedbacklink,'http:') or starts-with($feedbacklink,'https:')">
            <xsl:text>_blank</xsl:text> 
          </xsl:if>
        </xsl:attribute>
        <xsl:if test="starts-with($feedbacklink,'http:') or starts-with($feedbacklink,'https:')">
          <xsl:element name="img" namespace="{$xhtml}">
            <xsl:attribute name="src">
              <xsl:call-template name="ms-xhelp">
                <xsl:with-param name="ref" select="'online_icon.gif'"/>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:attribute name="class">
              <xsl:value-of select="'OH_offlineIcon'"/>
            </xsl:attribute>
            <xsl:attribute name="alt">
              <xsl:value-of select="$onlineTooltip"/>
            </xsl:attribute>
            <xsl:attribute name="title">
              <xsl:value-of select="$onlineTooltip"/>
            </xsl:attribute>
          </xsl:element>&#160;
        </xsl:if>
        <xsl:value-of select="$feedbackText" />
      </xsl:element>
      <xsl:value-of select="$prolog"/>
    </xsl:element>
  </xsl:template>

  <xsl:template name="feedback-link-bottom">
    <xsl:param name="prolog" select="''"/>
    <xsl:element name="div" namespace="{$xhtml}">
      <xsl:attribute name="class">OH_feedbacklink</xsl:attribute>
      <xsl:element name="a" namespace="{$xhtml}">
        <xsl:attribute name="href">
          <xsl:choose>
            <xsl:when test="starts-with($feedbacklink,'http:') or starts-with($feedbacklink,'https:')">
              <xsl:value-of select="$feedbacklink"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="'mailto:'"/>
              <xsl:value-of select="$feedback-alias"/>
              <xsl:value-of select="'?subject='" />
              <xsl:value-of select="branding:GetUrlEncode2($feedback-subject)" />
              <xsl:value-of select="'&amp;body='" />
              <xsl:value-of select="branding:replace(branding:GetUrlEncode2($feedbackprivactText),'+','%20')" />
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:attribute name="target">
          <xsl:if test="starts-with($feedbacklink,'http:') or starts-with($feedbacklink,'https:')">
            <xsl:text>_blank</xsl:text>
          </xsl:if>
        </xsl:attribute>
        <xsl:if test="starts-with($feedbacklink,'http:') or starts-with($feedbacklink,'https:')">
          <xsl:element name="img" namespace="{$xhtml}">
            <xsl:attribute name="src">
              <xsl:call-template name="ms-xhelp">
                <xsl:with-param name="ref" select="'online_icon.gif'"/>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:attribute name="class">
              <xsl:value-of select="'OH_offlineIcon'"/>
            </xsl:attribute>
            <xsl:attribute name="alt">
              <xsl:value-of select="$onlineTooltip"/>
            </xsl:attribute>
            <xsl:attribute name="title">
              <xsl:value-of select="$onlineTooltip"/>
            </xsl:attribute>
          </xsl:element>&#160;
        </xsl:if>
        <xsl:value-of select="$feedbackText" />
      </xsl:element>
      <xsl:choose>
        <xsl:when test="$locale= 'KO-KR' or $locale='ZH-CN' or $locale='ZH-TW'">
          <xsl:value-of select="' '"/>
          <xsl:value-of select="branding:GetAnchorTagValue($feedbackpolicy,'preText')"/>
          <xsl:element name="a" namespace="{$xhtml}">
            <xsl:attribute name="href">
              <xsl:value-of select="branding:GetAnchorTagValue($feedbackpolicy,'tag')"/>
            </xsl:attribute>
            <xsl:attribute name="target">_blank</xsl:attribute>
            <xsl:value-of select="branding:GetAnchorTagValue($feedbackpolicy,'text')"/>
          </xsl:element>
          <xsl:value-of select="')'"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$prolog" />
        </xsl:otherwise>
      </xsl:choose>

    </xsl:element>

  </xsl:template>

</xsl:stylesheet>
