<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
								xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
								exclude-result-prefixes="msxsl"
>
	<xsl:param name="Configuration">Any</xsl:param>
	<xsl:param name="Platform">Any</xsl:param>
	<xsl:output method="xml"
							indent="yes"
							omit-xml-declaration="yes"/>

	<xsl:template match="ProjectConfiguration">
		<ProjectConfiguration Project="{@Project}"
													AbsolutePath="{@AbsolutePath}">
			<xsl:value-of select="$Configuration"/>|<xsl:value-of select="$Platform"/>
		</ProjectConfiguration>
	</xsl:template>

	<xsl:template match="@* | node()">
		<xsl:copy>
			<xsl:apply-templates select="@* | node()"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
