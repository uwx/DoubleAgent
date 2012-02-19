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
	<xsl:import href="head.xslt"/>

	<xsl:template match="xhtml:head"
								mode="self-branding"
								name="ps-head-self-branding">
		<xsl:copy>
			<xsl:call-template name="head-favicon"/>
			<xsl:if test="not(boolean($pre-branding))">
				<xsl:call-template name="head-stylesheet"/>
			</xsl:if>
			<xsl:call-template name="head-style-urls"/>
			<xsl:if test="$downscale-browser">
				<xsl:call-template name="head-style-adjustments"/>
			</xsl:if>
			<xsl:if test="not(boolean($pre-branding))">
				<xsl:call-template name="head-script"/>
			</xsl:if>

			<xsl:apply-templates select="@*"/>
			<xsl:apply-templates select="node()"
													 mode="self-branding"/>
		</xsl:copy>
	</xsl:template>

	<!-- Remove branding data from the header - it's no longer required -->
	<xsl:template match="/xhtml:html/xhtml:head/xhtml:xml[@id='BrandingData']"/>
	<xsl:template match="/xhtml:html/xhtml:head/xhtml:xml[@id='BrandingData']"
								mode="self-branding"/>

	<!-- ============================================================================================
	Header Parts
	============================================================================================= -->

	<xsl:template name="head-favicon">
		<xsl:element name="link"
								 namespace="{$xhtml}">
			<xsl:attribute name="rel">
				<xsl:value-of select="'SHORTCUT ICON'"/>
			</xsl:attribute>
			<xsl:attribute name="href">
				<xsl:call-template name="ms-xhelp">
					<xsl:with-param name="ref"
													select="'favicon.ico'"/>
				</xsl:call-template>
			</xsl:attribute>
		</xsl:element>
	</xsl:template>

	<xsl:template name="head-stylesheet">
		<xsl:element name="link"
								 namespace="{$xhtml}">
			<xsl:attribute name="rel">
				<xsl:value-of select="'stylesheet'"/>
			</xsl:attribute>
			<xsl:attribute name="type">
				<xsl:value-of select="'text/css'"/>
			</xsl:attribute>
			<xsl:attribute name="href">
				<xsl:call-template name="ms-xhelp">
					<xsl:with-param name="ref"
													select="$css-file"/>
				</xsl:call-template>
			</xsl:attribute>
		</xsl:element>
	</xsl:template>

	<xsl:template name="head-style-urls">
		<xsl:element name="style"
								 namespace="{$xhtml}">
			<xsl:attribute name="type">text/css</xsl:attribute>
			<xsl:text>.OH_CodeSnippetContainerTabLeftActive, .OH_CodeSnippetContainerTabLeft,.OH_CodeSnippetContainerTabLeftDisabled {background-image: url('</xsl:text>
			<xsl:call-template name="ms-xhelp">
				<xsl:with-param name="ref"
												select="'tabLeftBG.gif'"/>
			</xsl:call-template>
			<xsl:text>')}</xsl:text>
			<xsl:text>.OH_CodeSnippetContainerTabRightActive, .OH_CodeSnippetContainerTabRight,.OH_CodeSnippetContainerTabRightDisabled {background-image: url('</xsl:text>
			<xsl:call-template name="ms-xhelp">
				<xsl:with-param name="ref"
												select="'tabRightBG.gif'"/>
			</xsl:call-template>
			<xsl:text>')}</xsl:text>
			<xsl:text>.OH_footer { background-image: url('</xsl:text>
			<xsl:call-template name="ms-xhelp">
				<xsl:with-param name="ref"
												select="'footer_slice.gif'"/>
			</xsl:call-template>
			<xsl:text>'); background-position:top; background-repeat:repeat-x}</xsl:text>
		</xsl:element>
	</xsl:template>

	<xsl:template name="head-style-adjustments">
		<xsl:element name="style"
								 namespace="{$xhtml}">
			<xsl:attribute name="type">text/css</xsl:attribute>
			body
			{
			border-left:5px solid #e6e6e6;
			overflow-x:visible;
			overflow-y:scroll;
			}
			.OH_outerDiv
			{
			overflow-x:auto;
			}
		</xsl:element>
	</xsl:template>

	<xsl:template name="head-script">
		<xsl:element name="script"
								 namespace="{$xhtml}">
			<xsl:attribute name="src">
				<xsl:call-template name="ms-xhelp">
					<xsl:with-param name="ref"
													select="$js-file"/>
				</xsl:call-template>
			</xsl:attribute>
			<xsl:attribute name="type">
				<xsl:value-of select="'text/javascript'"/>
			</xsl:attribute>
			<xsl:comment/>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>
