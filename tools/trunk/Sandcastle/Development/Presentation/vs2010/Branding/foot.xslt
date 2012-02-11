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

	<!-- ============================================================================================
	The footer is composed of values specified in the document metadata
	============================================================================================= -->
	<xsl:variable name="ft_copyrightText">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingCopyrightText']/@content"/>
	</xsl:variable>
	<xsl:variable name="ft_copyrightLink">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingCopyrightLink']/@content"/>
	</xsl:variable>
	<xsl:variable name="ft_copyrightInfo"
								select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingCopyrightInfo']/child::node()"/>
	<xsl:variable name="ft_footerText"
								select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFooterText']/child::node()"/>
	<xsl:variable name="ft_feedbackSubject">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFeedbackSubject']/@content"/>
	</xsl:variable>
	<xsl:variable name="ft_feedbackTopic">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:title"/>
	</xsl:variable>
	<xsl:variable name="ft_feedbackAlias">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFeedbackAlias']/@content"/>
	</xsl:variable>
	<xsl:variable name="ft_feedbackText"
								select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFeedbackText']/child::node()"/>
	<xsl:variable name="ft_feedbackFooterTo">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFeedbackFooterTo']/@content"/>
	</xsl:variable>
	<xsl:variable name="ft_feedbackFooterText"
								select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFeedbackFooterText']/child::node()"/>
	<xsl:variable name="ft_feedbackFooterTextTo"
								select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFeedbackFooterTextTo']/child::node()"/>
	<xsl:variable name="ft_feedbackBody">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='BrandingFeedbackBody']/child::text()"
									disable-output-escaping="yes"/>
	</xsl:variable>

	<xsl:variable name="ft_mailtoTopic"
								select="concat($ft_feedbackSubject,' ',$ft_feedbackTopic,' ',$version,' ',$locale)" />

	<!-- ============================================================================================
	Remove the metadata used above so it won't show up in the document.
	============================================================================================= -->

	<xsl:template match="xhtml:meta[@name='BrandingCopyrightInfo']" >
	</xsl:template>
	<xsl:template match="xhtml:meta[@name='BrandingFooterText']" >
	</xsl:template>
	<xsl:template match="xhtml:meta[@name='BrandingFeedbackText']" >
	</xsl:template>
	<xsl:template match="xhtml:meta[@name='BrandingFeedbackFooterText']" >
	</xsl:template>
	<xsl:template match="xhtml:meta[@name='BrandingFeedbackFooterTextTo']" >
	</xsl:template>
	<xsl:template match="xhtml:meta[@name='BrandingFeedbackBody']" >
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="footer">
		<xsl:element name="div"
								 namespace="{$xhtml}">
			<xsl:attribute name="class">OH_footer</xsl:attribute>
			<xsl:element name="p"
									 namespace="{$xhtml}">
				<xsl:apply-templates select="$ft_footerText"/>
				<xsl:if test="normalize-space($ft_copyrightLink)=''">
					<xsl:text>&#160;</xsl:text>
					<xsl:copy-of select="$ft_copyrightInfo" />
				</xsl:if>
			</xsl:element>
			<xsl:if test="normalize-space($ft_copyrightLink)!=''">
				<xsl:element name="p"
										 namespace="{$xhtml}">
					<xsl:element name="a"
											 namespace="{$xhtml}">
						<xsl:attribute name="href">
							<xsl:value-of select="$ft_copyrightLink"/>
						</xsl:attribute>
						<xsl:attribute name="target">
							<xsl:if test="starts-with($ft_copyrightLink,'http:') or starts-with($ft_copyrightLink,'https:')">
								<xsl:text>_blank</xsl:text>
							</xsl:if>
						</xsl:attribute>
						<xsl:if test="starts-with($ft_copyrightLink,'http:') or starts-with($ft_copyrightLink,'https:')">
							<xsl:element name="img"
													 namespace="{$xhtml}">
								<xsl:attribute name="src">
									<xsl:call-template name="ms-xhelp">
										<xsl:with-param name="ref"
																		select="'online_icon.gif'"/>
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
						<xsl:copy-of select="$ft_copyrightInfo" />
					</xsl:element>
				</xsl:element>
			</xsl:if>
			<xsl:call-template name="feedback-link">
				<xsl:with-param name="prolog">
					<xsl:choose>
						<xsl:when test="normalize-space($ft_feedbackFooterTo)!=''">
							<xsl:apply-templates select="$ft_feedbackFooterTextTo"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:apply-templates select="$ft_feedbackFooterText"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:element>
	</xsl:template>

	<xsl:template name="feedback-link">
		<xsl:param name="prolog"
							 select="''"/>
		<xsl:element name="div"
								 namespace="{$xhtml}">
			<xsl:attribute name="class">OH_feedbacklink</xsl:attribute>
			<xsl:element name="a"
									 namespace="{$xhtml}">
				<xsl:attribute name="href">
					<xsl:value-of select="'mailto:'"/>
					<xsl:value-of select="$ft_feedbackAlias"/>
					<xsl:value-of select="'?subject='" />
					<xsl:value-of select="branding:GetUrlEncode2($ft_mailtoTopic)" />
					<xsl:value-of select="'&amp;body='" />
					<xsl:value-of select="branding:replace(branding:GetUrlEncode2($ft_feedbackBody),'+','%20')" />
				</xsl:attribute>
				<xsl:if test="normalize-space($ft_feedbackText)">
					<xsl:apply-templates select="$ft_feedbackText" />
				</xsl:if>
			</xsl:element>
			<xsl:if test="normalize-space($prolog)">
				<xsl:text>&#160;</xsl:text>
				<xsl:copy-of select="$prolog"/>
			</xsl:if>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>
