<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								version="1.1"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
								xmlns:MSHelp="http://msdn.microsoft.com/mshelp"
								xmlns:mshelp="http://msdn.microsoft.com/mshelp"
								xmlns:ddue="http://ddue.schemas.microsoft.com/authoring/2003/5"
								xmlns:mtps="http://msdn2.microsoft.com/mtps"
								xmlns:xhtml="http://www.w3.org/1999/xhtml"
								xmlns:xlink="http://www.w3.org/1999/xlink"
>
	<!-- ======================================================================================== -->

	<xsl:import href="../../shared/transforms/utilities_bibliography.xsl"/>
	<xsl:output method="xml"
							omit-xml-declaration="yes"
							indent="no"
							encoding="utf-8" />

	<!-- ============================================================================================
	Parameters - key parameter is the api identifier string
	============================================================================================= -->

	<xsl:param name="key" />
	<xsl:param name="metadata">false</xsl:param>
	<xsl:param name="languages">false</xsl:param>
	<xsl:param name="bibliographyData"
						 select="'../data/bibliography.xml'"/>
	<xsl:param name="changeHistoryOptions" />

	<!-- ============================================================================================
	Includes
	============================================================================================= -->

	<xsl:include href="xpathFunctions.xsl" />
	<xsl:include href="utilities_dduexml.xsl" />
	<xsl:include href="seealso_dduexml.xsl" />
	<xsl:include href="conceptualMetadataHelp30.xsl"/>
	<xsl:include href="conceptualMetadataHelp20.xsl"/>
	<xsl:include href="codeTemplates.xsl"/>
	<xsl:include href="globalTemplates.xsl"/>

	<!-- ============================================================================================
	Global Variables
	============================================================================================= -->

	<xsl:variable name="g_hasSeeAlsoSection"
								select="boolean(count(/document/topic/*/ddue:relatedTopics/*[local-name()!='sampleRef']) > 0)"/>
	<!--<xsl:variable name="g_hasExamplesSection"
								select="boolean(string-length(/document/topic/*/ddue:codeExample[normalize-space(.)]) > 0)"/>-->
	<!--<xsl:variable name="g_hasLanguageFilterSection"
								select="normalize-space(/document/topic/*/ddue:codeExample) 
                or normalize-space(/document/topic/*//ddue:snippets/ddue:snippet)
                or /document/topic/ddue:developerSampleDocument/ddue:relatedTopics/ddue:sampleRef[@srcID]" />-->

	<xsl:variable name="g_apiGroup"
								select="/document/reference/apidata/@group" />
	<xsl:variable name="g_apiSubGroup"
								select="/document/reference/apidata/@subgroup" />
	<xsl:variable name="g_apiSubSubGroup"
								select="/document/reference/apidata/@subsubgroup" />
	<xsl:variable name="g_topicGroup"
								select="$g_apiGroup" />
	<xsl:variable name="g_topicSubGroup"
								select="$g_apiSubGroup" />
	<xsl:variable name="g_topicSubSubGroup"
								select="$g_apiSubSubGroup" />
	<xsl:variable name="g_apiTopicGroup"
								select="$g_apiGroup" />
	<xsl:variable name="g_apiTopicSubGroup"
								select="$g_apiSubGroup" />
	<xsl:variable name="g_apiTopicSubSubGroup"
								select="$g_apiSubSubGroup" />
	<xsl:variable name="pseudo"
								select="boolean(/document/reference/apidata[@pseudo='true'])"/>

	<!-- ======================================================================================== -->

	<xsl:template match="/document">
		<html>
			<head>
				<meta http-equiv="Content-Type"
							content="text/html; charset=UTF-8"/>
				<xsl:call-template name="t_insertNoIndexNoFollow" />
				<title>
					<xsl:call-template name="t_topicTitlePlain"/>
				</title>
				<xsl:call-template name="t_insertMetadataHelp30" />
				<xsl:call-template name="t_insertMetadataHelp20" />
				<xsl:call-template name="t_insertMetadata" />
			</head>
			<body class="primary-mtps-offline-document">
				<div class="topic">
					<xsl:call-template name="t_bodyTitle"/>
					<xsl:call-template name="t_bodyMain"/>
				</div>
			</body>
		</html>
	</xsl:template>

	<!-- document head -->

	<xsl:template name="t_nestedKeywordText">
		<xsl:for-each select="keyword[@index='K']">
			<xsl:text>, </xsl:text>
			<xsl:value-of select="text()"/>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="t_isCodeLangValid">
		<xsl:param name="p_codeLang" />
		<xsl:value-of select="true()"/>
	</xsl:template>

	<!-- ============================================================================================
	Formatted topic title
	============================================================================================= -->

	<xsl:template name="t_topicTitlePlain">
		<xsl:call-template name="t_topicTitle" />
	</xsl:template>

	<xsl:template name="t_topicTitleDecorated">
		<xsl:call-template name="t_topicTitle" />
	</xsl:template>

	<xsl:template name="t_topicTitle">
		<xsl:choose>
			<xsl:when test="normalize-space(/document/metadata/title)">
				<xsl:value-of select="normalize-space(/document/metadata/title)"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="normalize-space(/document/topic/*/ddue:title)"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- Title in TOC -->

	<!-- Index entry -->

	<!-- main window -->

	<xsl:template name="t_bodyMain">
		<div id="mainSection">

			<div id="mainBody">
				<include item="header" />
				<xsl:call-template name="t_body" />
			</div>
		</div>

	</xsl:template>

	<xsl:template name="t_body">
		<!-- freshness date -->
		<xsl:call-template name="writeFreshnessDate">
			<xsl:with-param name="ChangedHistoryDate"
											select="/document/topic/*//ddue:section[ddue:title = 'Change History']/ddue:content/ddue:table/ddue:row[1]/ddue:entry[1] |
                      /document/topic/*/ddue:changeHistory/ddue:content/ddue:table/ddue:row[1]/ddue:entry[1]" />
		</xsl:call-template>

		<xsl:apply-templates select="topic" />

		<!-- changed table section -->
		<xsl:call-template name="writeChangeHistorySection" />
	</xsl:template>

	<!-- sections that behave differently in conceptual and reference -->

	<xsl:template match="ddue:title">
		<!-- don't print title -->
	</xsl:template>

	<xsl:template match="ddue:introduction">
		<xsl:apply-templates select="@address" />
		<div class="introduction">
			<xsl:apply-templates />
		</div>
	</xsl:template>

	<xsl:template match="ddue:parameters">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'parametersTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:returnValue">
		<xsl:if test="normalize-space(.)">
			<xsl:choose>
				<xsl:when test="(normalize-space(ddue:content)='') and ddue:sections/ddue:section[ddue:title='Property Value']">
					<xsl:call-template name="t_putSectionInclude">
						<xsl:with-param name="p_titleInclude"
														select="'propertyValueTitle'" />
						<xsl:with-param name="p_content">
							<xsl:apply-templates select="ddue:sections/ddue:section[ddue:title='Property Value']/*" />
						</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="t_putSectionInclude">
						<xsl:with-param name="p_titleInclude"
														select="'returnValueTitle'" />
						<xsl:with-param name="p_content">
							<xsl:apply-templates />
						</xsl:with-param>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:exceptions">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'exceptionsTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:relatedSections">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'relatedSectionsTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:relatedTopics">
		<xsl:if test="$g_hasSeeAlsoSection">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'relatedTopicsTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates select="/document/topic/*/ddue:relatedTopics"
															 mode="seeAlso" />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:codeExample">
		<!-- create Example section for the first codeExample node -->
		<xsl:if test="not(preceding-sibling::ddue:codeExample) and ../ddue:codeExample[normalize-space(.)!='']">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'Example'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
					<!-- if there are additional codeExample nodes, put them inside this section -->
					<xsl:for-each select="following-sibling::ddue:codeExample">
						<xsl:apply-templates />
					</xsl:for-each>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:codeReference">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template name="t_runningHeader">
		<xsl:variable name="runningHeaderText">
			<xsl:value-of select="/document/metadata/runningHeaderText/@uscid"/>
		</xsl:variable>
		<include item="{$runningHeaderText}" />
	</xsl:template>

	<!-- ============================================================================================
	<autoOutline /> or <autoOutline>[#]</autoOutline>
  
	Inserts a bullet list of links to the topic's sections or a section's
  sub-sections with optional support for limiting the expansion down to a
  specific level.  Authors can use the tag directly or specify a token
  (defined in a token file) in a topic's introduction to get a bullet list of
  the sections; or in a ddue:section/ddue:content to get a bullet list of the
  section's sub-sections.  If the token is used, the shared content component
  replaces <token>autoOutline</token> with an <autoOutline/> node that you
  specify.  This was the old way of doing it but this version allows it to
  be specified directly like any other MAML tag. Examples:

  <autoOutline/>                Show only top-level topic titles
  <autoOutline>1</autoOutline>  Show top-level titles and titles for one level down
  <autoOutline>3</autoOutline>  Show titles from the top down to three levels
	============================================================================================= -->

	<xsl:template match="autoOutline|ddue:autoOutline">
		<xsl:variable name="maxDepth">
			<xsl:choose>
				<xsl:when test="normalize-space(.)">
					<xsl:value-of select="number(normalize-space(.))" />
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="number(0)" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="intro"
									select="@lead"/>
		<xsl:variable name="outlineType">
			<xsl:choose>
				<xsl:when test="@excludeRelatedTopics = 'true'">
					<xsl:value-of select="string('topNoRelated')" />
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="string('toplevel')" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<!--if <autoOutline/> is in introduction, it outlines the topic's toplevel sections-->
			<xsl:when test="ancestor::ddue:introduction">
				<xsl:for-each select="ancestor::ddue:introduction/parent::*">
					<xsl:call-template name="insertAutoOutline">
						<xsl:with-param name="intro">
							<xsl:value-of select="$intro"/>
						</xsl:with-param>
						<xsl:with-param name="outlineType">
							<xsl:value-of select="$outlineType" />
						</xsl:with-param>
						<xsl:with-param name="depth">
							<xsl:value-of select="number(0)"/>
						</xsl:with-param>
						<xsl:with-param name="maxDepth">
							<xsl:value-of select="$maxDepth"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:when>
			<!--if <autoOutline/> is in section/content, it outlines the section's subsections-->
			<xsl:when test="ancestor::ddue:content[parent::ddue:section]">
				<xsl:for-each select="ancestor::ddue:content/parent::ddue:section/ddue:sections">
					<xsl:call-template name="insertAutoOutline">
						<xsl:with-param name="intro">
							<xsl:value-of select="$intro"/>
						</xsl:with-param>
						<xsl:with-param name="outlineType">subsection</xsl:with-param>
						<xsl:with-param name="depth">
							<xsl:value-of select="number(0)"/>
						</xsl:with-param>
						<xsl:with-param name="maxDepth">
							<xsl:value-of select="$maxDepth"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="insertAutoOutline">
		<xsl:param name="intro"/>
		<xsl:param name="outlineType"/>
		<xsl:param name="depth"/>
		<xsl:param name="maxDepth"/>
		<!--insert an outline if there are sections with title and address-->
		<xsl:if test="ddue:section[ddue:title[normalize-space(.)!='']]">
			<!--insert a boilerplate intro-->
			<xsl:choose>
				<xsl:when test="normalize-space($intro) = 'none'">
					<xsl:text></xsl:text>
				</xsl:when>
				<xsl:when test="normalize-space($intro)">
					<p>
						<xsl:value-of select="normalize-space($intro)"/>
					</p>
				</xsl:when>
				<xsl:when test="$outlineType='toplevel' or $outlineType='topNoRelated'">
					<p>
						<include item="autoOutlineTopLevelIntro"/>
					</p>
				</xsl:when>
				<xsl:when test="$outlineType='subsection'">
					<p>
						<include item="autoOutlineSubsectionIntro"/>
					</p>
				</xsl:when>
			</xsl:choose>
			<ul>
				<xsl:for-each select="ddue:section[ddue:title[normalize-space(.)!='']]">
					<xsl:call-template name="outlineSectionEntry" />

					<!-- Expand sub-sections too if wanted -->
					<xsl:if test="$depth &lt; $maxDepth">
						<xsl:for-each select="ddue:sections">
							<xsl:call-template name="insertAutoOutline">
								<xsl:with-param name="outlineType">subsubsection</xsl:with-param>
								<xsl:with-param name="depth">
									<xsl:value-of select="$depth + 1"/>
								</xsl:with-param>
								<xsl:with-param name="maxDepth">
									<xsl:value-of select="$maxDepth"/>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:for-each>
					</xsl:if>
				</xsl:for-each>
				<!--for toplevel outlines include a link to See Also-->
				<!-- xsl:if test="starts-with($outlineType,'toplevel') and //ddue:relatedTopics[normalize-space(.)!='']"  -->
				<xsl:if test="starts-with($outlineType,'toplevel') and count(//ddue:relatedTopics/*) > 0">
					<li>
						<a>
							<xsl:attribute name="href">#seeAlsoSection</xsl:attribute>
							<include item="RelatedTopicsLinkText"/>
						</a>
					</li>
				</xsl:if>
			</ul>
		</xsl:if>
	</xsl:template>

	<!--a list item in the outline's bullet list-->
	<xsl:template name="outlineSectionEntry">
		<xsl:if test="descendant::ddue:content[normalize-space(.)] or count(ddue:content/*) &gt; 0">
			<li class="outlineSectionEntry">
				<a>
					<xsl:if test="@address">
						<!-- Keep this on one line or the spaces preceeding the "#" end up in the anchor name -->
						<xsl:attribute name="href"
													 xml:space="preserve">#<xsl:value-of select="@address"/></xsl:attribute>
					</xsl:if>
					<xsl:value-of select="ddue:title" />
				</a>
				<xsl:if test="normalize-space(ddue:summary)">
					<div class="outlineSectionEntrySummary">
						<xsl:apply-templates select="ddue:summary/node()"/>
					</div>
				</xsl:if>
			</li>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	Bibliography
	============================================================================================= -->
	<xsl:key name="citations"
					 match="//ddue:cite"
					 use="text()" />

	<xsl:variable name="g_hasCitations"
								select="boolean(count(//ddue:cite) > 0)"/>

	<xsl:template match="ddue:cite">
		<xsl:variable name="v_currentCitation"
									select="text()"/>
		<xsl:for-each select="//ddue:cite[generate-id(.)=generate-id(key('citations',text()))]">
			<!-- Distinct citations only -->
			<xsl:if test="$v_currentCitation=.">
				<xsl:choose>
					<xsl:when test="document($bibliographyData)/bibliography/reference[@name=$v_currentCitation]">
						<sup class="citation">
							<a>
								<xsl:attribute name="href">
									#cite<xsl:value-of select="position()"/>
								</xsl:attribute>[<xsl:value-of select="position()"/>]
							</a>
						</sup>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="ddue:bibliography">
		<xsl:if test="$g_hasCitations">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'bibliographyTitle'"/>
				<xsl:with-param name="p_content">
					<xsl:call-template name="autogenBibliographyLinks"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template name="autogenBibliographyLinks">
		<xsl:for-each select="//ddue:cite[generate-id(.)=generate-id(key('citations',text()))]">
			<!-- Distinct citations only -->
			<xsl:variable name="v_citation"
										select="."/>
			<xsl:variable name="entry"
										select="document($bibliographyData)/bibliography/reference[@name=$v_citation]"/>

			<xsl:call-template name="bibliographyReference">
				<xsl:with-param name="number"
												select="position()"/>
				<xsl:with-param name="data"
												select="$entry"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>

</xsl:stylesheet>
