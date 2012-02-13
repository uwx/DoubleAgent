<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:msxsl="urn:schemas-microsoft-com:xslt"
    exclude-result-prefixes="msxsl"
    xmlns:mtps="http://msdn2.microsoft.com/mtps"
    xmlns:xhtml="http://www.w3.org/1999/xhtml"
    xmlns:branding="urn:FH-Branding"
>
  <xsl:variable name="errorTitle">Requires localization!</xsl:variable>
  <xsl:variable name="errorMessage">Requires localization!</xsl:variable>

  <xsl:template match="xhtml:errorLink" name="errorLink">
    <xsl:element name="div" namespace="{$xhtml}">
      <xsl:attribute name="id">mainSection</xsl:attribute>
      <xsl:attribute name="style">margin-top:-30px</xsl:attribute>
      <xsl:element name="div" namespace="{$xhtml}">
        <xsl:attribute name="id">mainBody</xsl:attribute>
        <xsl:value-of select="$errorMessage"/>
        <xsl:element name="br" namespace="{$xhtml}"/>
        <xsl:element name="br" namespace="{$xhtml}"/>

        <xsl:element name="table" namespace="{$xhtml}">
          <xsl:attribute name="class">OH_borderlessTable</xsl:attribute>
          <xsl:element name="tr" namespace="{$xhtml}">
            <xsl:element name="td" namespace="{$xhtml}">
              <xsl:element name="img" namespace="{$xhtml}">
                <xsl:attribute name="src">
                  <xsl:call-template name="ms-xhelp">
                    <xsl:with-param name="ref" select="'1_404_bullet.gif'"/>
                  </xsl:call-template>
                </xsl:attribute>
                <xsl:attribute name="alt">1</xsl:attribute>
                <xsl:attribute name="title">1</xsl:attribute>
              </xsl:element>
            </xsl:element>
            <xsl:element name="td" namespace="{$xhtml}">
              <xsl:value-of select="$errorMsgLine1"/>
              <xsl:element name="br" namespace="{$xhtml}"/>
              <xsl:value-of select="branding:GetSubString($errorMsgLine2,0)"/>
              <xsl:text> </xsl:text>
              <xsl:element name="a" namespace="{$xhtml}">
                <xsl:attribute name="href">
                  <xsl:call-template name="ms-xhelp">
                    <xsl:with-param name="ref" select="$localehelp"/>
                  </xsl:call-template>
                  <xsl:value-of select="'#firstLink'"/>
                </xsl:attribute>
                <xsl:attribute name="target">_blank</xsl:attribute>
                <xsl:value-of select="branding:GetSubString($errorMsgLine2,1)"/>
              </xsl:element>
              <xsl:text> </xsl:text>
              <xsl:value-of select="branding:GetSubString($errorMsgLine2,2)"/>
            </xsl:element>
          </xsl:element>
          <xsl:element name="tr" namespace="{$xhtml}">
            <xsl:element name="td" namespace="{$xhtml}">
              <xsl:attribute name="style">height:10px</xsl:attribute>
              <xsl:text> </xsl:text>
            </xsl:element>
          </xsl:element>
          <xsl:element name="tr" namespace="{$xhtml}">
            <xsl:element name="td" namespace="{$xhtml}">
              <xsl:element name="img" namespace="{$xhtml}">
                <xsl:attribute name="src">
                  <xsl:call-template name="ms-xhelp">
                    <xsl:with-param name="ref" select="'2_404_bullet.gif'"/>
                  </xsl:call-template>
                </xsl:attribute>
                <xsl:attribute name="alt">2</xsl:attribute>
                <xsl:attribute name="title">2</xsl:attribute>
              </xsl:element>
            </xsl:element>
            <xsl:element name="td" namespace="{$xhtml}">
              <xsl:value-of select="$errorMsgLine3"/>
              <xsl:element name="br" namespace="{$xhtml}"/>
              <xsl:element name="a" namespace="{$xhtml}">
                <xsl:attribute name="href">
                  <xsl:value-of select="branding:GetOnlineID(.)"/>
                </xsl:attribute>
                <xsl:attribute name="target">_blank</xsl:attribute>
                <xsl:value-of select="branding:GetOnlineID(.)"/>
              </xsl:element>
            </xsl:element>
          </xsl:element>
        </xsl:element>

        <!-- Other options-->

        <xsl:choose>
          <xsl:when test="$f1-error-page and $error-page">
            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:attribute name="class">OH_CollapsibleAreaRegion</xsl:attribute>
              <xsl:element name="div" namespace="{$xhtml}">
                <xsl:attribute name="class">OH_404_regiontitle</xsl:attribute>
              </xsl:element>
              <xsl:element name="div" namespace="{$xhtml}">
                <xsl:attribute name="class">OH_RegionToggle</xsl:attribute>
                <xsl:text> </xsl:text>
              </xsl:element>
            </xsl:element>
            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:attribute name="class">OH_clear</xsl:attribute>
              <xsl:text> </xsl:text>
            </xsl:element>
            <xsl:element name="br" namespace="{$xhtml}"/>

            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:apply-templates select="following-sibling::*[1]" mode="f1-error"/>
            </xsl:element>
          </xsl:when>
          <xsl:otherwise>
            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:attribute name="class">OH_CollapsibleAreaRegion</xsl:attribute>
              <xsl:element name="div" namespace="{$xhtml}">
                <xsl:attribute name="class">OH_404_regiontitle</xsl:attribute>
                <xsl:value-of select="$errorMsgLine4"/>
              </xsl:element>
              <xsl:element name="div" namespace="{$xhtml}">
                <xsl:attribute name="class">OH_RegionToggle</xsl:attribute>
                <xsl:text> </xsl:text>
              </xsl:element>
            </xsl:element>
            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:attribute name="class">OH_clear</xsl:attribute>
              <xsl:text> </xsl:text>
            </xsl:element>
            <xsl:element name="br" namespace="{$xhtml}"/>

            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:value-of select="$errorMsgLine5"/>
              <xsl:element name="br" namespace="{$xhtml}"/>
              <xsl:value-of select="branding:GetSubString($errorMsgLine6,0)"/>
              <xsl:text> </xsl:text>
              <xsl:element name="a" namespace="{$xhtml}">
                <xsl:attribute name="href">
                  <xsl:call-template name="ms-xhelp">
                    <xsl:with-param name="ref" select="$localehelp"/>
                  </xsl:call-template>
                  <xsl:value-of select="'#secondLink'"/>
                </xsl:attribute>
                <xsl:attribute name="target">_blank</xsl:attribute>
                <xsl:value-of select="branding:GetSubString($errorMsgLine6,1)"/>
              </xsl:element>
              <xsl:text> </xsl:text>
              <xsl:value-of select="branding:GetSubString($errorMsgLine6,2)"/>
            </xsl:element>
            <xsl:element name="br" namespace="{$xhtml}"/>

            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:value-of select="branding:GetSubString($errorMsgLine7,0)"/>
              <xsl:text> </xsl:text>
              <xsl:element name="a" namespace="{$xhtml}">
                <xsl:attribute name="href">
                  <xsl:call-template name="ms-xhelp">
                    <xsl:with-param name="ref" select="$localehelp"/>
                  </xsl:call-template>
                  <xsl:value-of select="'#thirdLink'"/>
                </xsl:attribute>
                <xsl:attribute name="target">_blank</xsl:attribute>
                <xsl:value-of select="branding:GetSubString($errorMsgLine7,1)"/>
              </xsl:element>
              <xsl:value-of select="branding:GetSubString($errorMsgLine7,2)"/>
            </xsl:element>
            <xsl:element name="br" namespace="{$xhtml}"/>

            <!-- Search-->
            <xsl:element name="span" namespace="{$xhtml}">
              <xsl:value-of select="$errorMsgLine8"/>
            </xsl:element>
            <xsl:element name="div" namespace="{$xhtml}">
              <xsl:attribute name="id">searchDiv</xsl:attribute>
              <xsl:call-template name="displaySearchForm">
                <xsl:with-param name="searchTextboxValue" select="''"/>
                <xsl:with-param name="searchButtonValue" select="$searchButtonText"/>
                <xsl:with-param name="product" select="$product"/>
                <xsl:with-param name="version" select="$version"/>
                <xsl:with-param name="locale" select="$locale"/>
              </xsl:call-template>
            </xsl:element>
          </xsl:otherwise>
        </xsl:choose>
        
      </xsl:element>
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>
