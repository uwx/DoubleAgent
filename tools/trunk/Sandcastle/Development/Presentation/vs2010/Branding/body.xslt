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

	<!-- ============================================================================================
	Generic transforms (see Identity.xslt for others)
	============================================================================================= -->

	<!-- strip style attributes by default -->
	<xsl:template match="@style[normalize-space(.)!='display:none']"
								name="style"/>

	<!-- pass through styles for p and h elements -->
	<xsl:template match="//xhtml:p[@style]|xhtml:h1[@style]|xhtml:h2[@style]|xhtml:h3[@style]|xhtml:h4[@style]|xhtml:h5[@style]|xhtml:h6[@style]"
								name="allowStyles">
		<xsl:copy>
			<xsl:apply-templates select="@*"/>
			<xsl:attribute name="style">
				<xsl:value-of select="@style"/>
			</xsl:attribute>
			<xsl:apply-templates/>
		</xsl:copy>
	</xsl:template>

	<!-- pass all self-branded content through transform -->
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
		</xsl:copy>
	</xsl:template>

	<!-- ============================================================================================
	Specific self-branding transforms
	============================================================================================= -->

	<!-- use the default body transform (note - mode="self-branding" will no longer apply to it's contents) -->
	<xsl:template match="xhtml:body"
								mode="self-branding"
								name="body-self-branding">
		<xsl:call-template name="body"/>
	</xsl:template>

	<!-- redirect image sources to the appropriate path -->
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

	<!-- ============================================================================================
	Override for formatting the page title.
	============================================================================================= -->

	<xsl:variable name="hd_productTitle">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingProductTitle']/@content"/>
	</xsl:variable>

	<xsl:template match="*[@class='title']"
								name="body-title">
		<xsl:variable name="id"
									select="generate-id()" />
		<xsl:element name="div"
								 namespace="{$xhtml}">
			<xsl:attribute name="class">OH_topic</xsl:attribute>

			<xsl:element name="div"
									 namespace="{$xhtml}">
				<xsl:attribute name="class">OH_title</xsl:attribute>
				<xsl:element name="table"
										 namespace="{$xhtml}">
					<xsl:element name="tr"
											 namespace="{$xhtml}">
						<xsl:element name="td"
												 namespace="{$xhtml}">
							<xsl:attribute name="class">OH_tdTitleColumn</xsl:attribute>
							<xsl:variable name="bodyTitle"
														select="." />
							<xsl:choose>
								<xsl:when test="$bodyTitle = ''">
									<xsl:value-of select="$title"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:apply-templates />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:element>
						<xsl:element name="td"
												 namespace="{$xhtml}">
							<xsl:attribute name="class">OH_tdLogoColumn</xsl:attribute>
							<xsl:variable name="v_runningHeader"
														select="following-sibling::xhtml:table[@id='topTable']//xhtml:td[@id='runningHeaderColumn']"/>
							<xsl:choose>
								<xsl:when test="following-sibling::xhtml:table[@id='topTable']//xhtml:td//xhtml:img">
									<!--<xsl:if test="following-sibling::xhtml:table[@id='topTable']//xhtml:td/xhtml:img//ancestor::xhtml:td/@align">
										<xsl:attribute name="align">
											<xsl:value-of select="following-sibling::xhtml:table[@id='topTable']//xhtml:td/xhtml:img//ancestor::xhtml:td/@align"/>
										</xsl:attribute>
									</xsl:if>-->
									<!--<xsl:if test="following-sibling::xhtml:table[@id='topTable']//xhtml:td/xhtml:img//ancestor::xhtml:td/@style">
										<xsl:attribute name="style">
											<xsl:value-of select="following-sibling::xhtml:table[@id='topTable']//xhtml:td/xhtml:img//ancestor::xhtml:td/@style"/>
										</xsl:attribute>
									</xsl:if>-->
									<xsl:apply-templates select="following-sibling::xhtml:table[@id='topTable']//xhtml:td/xhtml:img//ancestor::xhtml:td/child::node()"/>
								</xsl:when>
								<xsl:when test="following-sibling::xhtml:table[@id='topTable']//xhtml:td[@id='runningHeaderColumn']">
									<xsl:apply-templates select="following-sibling::xhtml:table[@id='topTable']//xhtml:td[@id='runningHeaderColumn']/child::node()"/>
								</xsl:when>
							</xsl:choose>
						</xsl:element>
					</xsl:element>
				</xsl:element>
			</xsl:element>
		</xsl:element>

		<xsl:call-template name="feedback-link"/>
	</xsl:template>

	<!-- Remove the generated body title table -->
	<xsl:template match="xhtml:table[@id='topTable']">
	</xsl:template>

</xsl:stylesheet>
