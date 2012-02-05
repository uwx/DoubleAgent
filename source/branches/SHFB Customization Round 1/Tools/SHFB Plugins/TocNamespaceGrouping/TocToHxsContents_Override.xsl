<?xml version="1.0"?>
<xsl:stylesheet version="1.1" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:import href="TocToHxsContents.xsl"/>
	<xsl:param name="SiteMap" />
	<!--<xsl:variable name="includeIds" select="'true'" />-->

	<xsl:template match="topic">
		<HelpTOCNode>
			<xsl:if test="boolean($includeIds)">
				<xsl:attribute name="Id">
					<xsl:value-of select="@id"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="@file">
					<xsl:variable name="UrlPrefix">
						<xsl:call-template name="GetUrlPrefix">
							<xsl:with-param name="FilePath" select="@file" />
						</xsl:call-template>
					</xsl:variable>
					<xsl:variable name="UrlSuffix">
						<xsl:call-template name="GetUrlSuffix">
							<xsl:with-param name="FilePath" select="@file" />
						</xsl:call-template>
					</xsl:variable>

					<xsl:variable name="SiteMapRef" xmlns:sm="http://schemas.microsoft.com/AspNet/SiteMap-File-1.0">
						<xsl:if test="$SiteMap">
							<xsl:value-of select="document($SiteMap)//sm:siteMapNode[@title=current()/@id]/@href"/>
						</xsl:if>
					</xsl:variable>
					<xsl:attribute name="Url">
						<xsl:choose>
							<xsl:when test="boolean($segregated)">
								<xsl:value-of select="concat('ms-help:/../',@project,'/',$UrlPrefix,@file,$UrlSuffix,$SiteMapRef)" />
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="concat($UrlPrefix,@file,$UrlSuffix,$SiteMapRef)" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>

					<xsl:if test="not(@project)">
						<xsl:attribute name="Title">
							<xsl:value-of select="@id" />
						</xsl:attribute>
					</xsl:if>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="Title">
						<xsl:value-of select="@id" />
					</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>

			<xsl:apply-templates />
		</HelpTOCNode>
	</xsl:template>

	<xsl:template name="GetUrlPrefix">
		<xsl:param name="FilePath" />
		<xsl:choose>
			<xsl:when test="substring-before($FilePath, '/')">
				<xsl:value-of select="''" />
			</xsl:when>
			<xsl:when test="substring-before($FilePath, '\')">
				<xsl:value-of select="''" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="'html/'" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="GetUrlSuffix">
		<xsl:param name="FilePath" />
		<xsl:choose>
			<xsl:when test="substring-after($FilePath, '.')">
				<xsl:value-of select="''" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="'.htm'" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>