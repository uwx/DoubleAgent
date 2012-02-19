<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
								exclude-result-prefixes="msxsl"
								xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
	                    xmlns:xhtml="http://www.w3.org/1999/xhtml"
							xmlns:mtps="http://msdn2.microsoft.com/mtps"
								>

	<!-- ============================================================================================
	Fixes for codesnippet selection
	============================================================================================= -->

	<xsl:key name="code-snippet-use"
					 use="'snippet-group-first'"
					 match="//mtps:CodeSnippet[parent::xhtml:div[starts-with(@id,'snippetGroup') and count(mtps:CodeSnippet) > 1] and (not(preceding-sibling::*[last()][self::mtps:CodeSnippet]))]"/>
	<xsl:key name="code-snippet-use"
					 use="'snippet-group-next'"
					 match="//mtps:CodeSnippet[parent::xhtml:div[starts-with(@id,'snippetGroup') and count(mtps:CodeSnippet) > 1] and (preceding-sibling::*[last()][self::mtps:CodeSnippet])]"/>

	<xsl:template match="key('code-snippet-use', 'snippet-group-first')"
								name="snippet-group-first"
								priority="2">
		<xsl:call-template name="leadsnippet"/>
	</xsl:template>
	<xsl:template match="key('code-snippet-use', 'snippet-group-next')"
								name="snippet-group-next"
								priority="2">
	</xsl:template>

</xsl:stylesheet>