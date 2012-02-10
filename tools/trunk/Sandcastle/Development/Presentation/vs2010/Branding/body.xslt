<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
								xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
								exclude-result-prefixes="msxsl branding"
								xmlns:mtps="http://msdn2.microsoft.com/mtps"
								xmlns:xhtml="http://www.w3.org/1999/xhtml"
								xmlns:branding="urn:FH-Branding"
								xmlns:xs="http://www.w3.org/2001/XMLSchema"
>

	<xsl:import href="Dev10body.xslt"/>

	<!-- pass all unbranded content through transform-->
	<xsl:template match="*|/"
								mode="self-branding">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:if test="@style">
				<xsl:attribute name="style">
					<xsl:value-of select="@style"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates mode="self-branding"/>
			<!--Bug Fix-->
			<!--<xsl:if test="not(*) and not(text())">
				<xsl:value-of select="' '"/>
			</xsl:if>-->
		</xsl:copy>
	</xsl:template>

	<xsl:template match="xhtml:body"
								mode="self-branding"
								name="body-self-branding">
		<xsl:call-template name="body"/>
	</xsl:template>

	<xsl:template match="xhtml:img"
								mode="self-branding">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:attribute name="src">
				<xsl:value-of select="branding:BuildContentPath($contentFolder,@src)"/>
			</xsl:attribute>
			<xsl:if test="@style">
				<xsl:attribute name="style">
					<xsl:value-of select="@style"/>
				</xsl:attribute>
			</xsl:if>
		</xsl:copy>
	</xsl:template>

</xsl:stylesheet>
