<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
								xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								xmlns:mtps="http://msdn2.microsoft.com/mtps"
								xmlns:branding="urn:FH-Branding"
								xmlns:xhtml="http://www.w3.org/1999/xhtml"
>
	<xsl:import href="LanguageSpecificText.xslt"/>

	<!-- Convert old-style LST to new style -->
	<xsl:template match="xhtml:span[@class='languageSpecificText']"
								name="old-lst">
		<xsl:choose>
			<xsl:when test="count(xhtml:span[@class]) = count(*)">
				<xsl:variable name="v_id"
											select="generate-id(.)"/>

				<xsl:element name="span"
										 namespace="{$xhtml}"
										 xml:space="preserve"><xsl:attribute name="id"><xsl:value-of select="$v_id"/></xsl:attribute><xsl:text> </xsl:text></xsl:element>
				<xsl:element name="script"
										 namespace="{$xhtml}">
					<xsl:attribute name="type">
						<xsl:value-of select="'text/javascript'"/>
					</xsl:attribute>
					addToLanSpecTextIdSet("<xsl:value-of select="$v_id"/>?<xsl:for-each select="xhtml:span">
						<xsl:value-of select ="@class"/>
						<xsl:value-of select ="'='"/>
						<xsl:value-of select ="."/>
						<xsl:if test="following-sibling::*">
							<xsl:value-of select ="'|'"/>
						</xsl:if>
					</xsl:for-each>");
				</xsl:element>
			</xsl:when>
			<xsl:otherwise>
				<xsl:copy-of select="."/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>