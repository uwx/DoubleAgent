﻿<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
								xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
								exclude-result-prefixes="msxsl branding"
								xmlns:mtps="http://msdn2.microsoft.com/mtps"
								xmlns:xhtml="http://www.w3.org/1999/xhtml"
								xmlns:branding="urn:FH-Branding"
								xmlns:xs="http://www.w3.org/2001/XMLSchema"
								xmlns:cs="urn:Get-Paths"

>
	<xsl:import href="Dev10head.xslt"/>

	<xsl:template match="xhtml:head"
								mode="self-branding">
		<xsl:call-template name="head"/>
	</xsl:template>

	<xsl:template match="xhtml:script"
								mode="self-branding">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:choose>
				<xsl:when test="@src">
					<xsl:attribute name="src">
						<xsl:value-of select="branding:BuildContentPath($contentFolder,@src)"/>
					</xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:apply-templates mode="self-branding"/>
					<xsl:comment/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:copy>
	</xsl:template>

</xsl:stylesheet>
