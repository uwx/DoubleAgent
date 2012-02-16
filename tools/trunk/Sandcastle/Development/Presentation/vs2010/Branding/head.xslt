<?xml version="1.0" encoding="utf-8"?>
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

	<!-- use the default head transform (note - mode="self-branding" will no longer apply to it's contents) -->
	<xsl:template match="xhtml:head"
								mode="self-branding">
		<xsl:call-template name="head"/>
	</xsl:template>

	<xsl:template match="xhtml:head/*[last()]"
								name="appendToHead">
		<xsl:apply-templates/>

		<!-- tweak some of the styles for targets other than MS Help Viewer -->
		<xsl:if test="$downscale-browser">
			<xsl:element name="style"
									 namespace="{$xhtml}">
				<xsl:attribute name="type">text/css</xsl:attribute>
				body
				{
				background-color:#e6e6e6;
				overflow-x:visible;
				overflow-y:scroll;
				}
				.OH_outerDiv
				{
				margin-left:4px;
				background-color:#ffffff;
				width:100%;
				height:100%;
				overflow-x:auto;
				}
				.OH_outerContent
				{
				background-color:#ffffff;
				}
				.OH_footer
				{
				background-color:#ffffff;
				}
			</xsl:element>
		</xsl:if>
	</xsl:template>

	<!-- update script paths -->
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

	<!-- remove branding data from the header - it's no longer required -->
	<xsl:template match="/xhtml:html/xhtml:head/xhtml:xml[@id='BrandingData']"/>

</xsl:stylesheet>
