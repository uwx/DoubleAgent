<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								version="1.0"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
								xmlns:MSHelp="http://msdn.microsoft.com/mshelp"
								xmlns:mshelp="http://msdn.microsoft.com/mshelp"
								xmlns:ddue="http://ddue.schemas.microsoft.com/authoring/2003/5"
								xmlns:mtps="http://msdn2.microsoft.com/mtps"
								xmlns:xhtml="http://www.w3.org/1999/xhtml"
								xmlns:xlink="http://www.w3.org/1999/xlink"
        >

	<!-- ============================================================================================
	String formatting
	============================================================================================= -->

	<!-- Gets the substring after the last occurence of a period in a given string -->
	<xsl:template name="t_getTrimmedLastPeriod">
		<xsl:param name="p_string" />

		<xsl:choose>
			<xsl:when test="contains($p_string, '.')">
				<xsl:call-template name="t_getTrimmedLastPeriod">
					<xsl:with-param name="p_string"
													select="substring-after($p_string, '.')" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$p_string" />
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<xsl:template name="t_getTrimmedAtPeriod">
		<xsl:param name="p_string" />

		<xsl:variable name="v_trimmedString"
									select="substring(normalize-space($p_string), 1, 256)" />
		<xsl:choose>
			<xsl:when test="normalize-space($p_string) != $v_trimmedString">
				<xsl:choose>
					<xsl:when test="not(contains($v_trimmedString, '.'))">
						<xsl:value-of select="$v_trimmedString"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="t_getSubstringAndLastPeriod">
							<xsl:with-param name="p_string"
															select="$v_trimmedString" />
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="normalize-space($p_string)"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_getSubstringAndLastPeriod">
		<xsl:param name="p_string" />

		<xsl:if test="contains($p_string, '.')">
			<xsl:variable name="v_after"
										select="substring-after($p_string, '.')" />
			<xsl:value-of select="concat(substring-before($p_string, '.'),'.')" />
			<xsl:if test="contains($v_after, '.')">
				<xsl:call-template name="t_getSubstringAndLastPeriod">
					<xsl:with-param name="p_string"
													select="$v_after" />
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
	</xsl:template>

	<!-- indent by 2*n spaces -->
	<xsl:template name="t_putIndent">
		<xsl:param name="p_count" />
		<xsl:if test="$p_count &gt; 1">
			<xsl:text>&#160;&#160;</xsl:text>
			<xsl:call-template name="t_putIndent">
				<xsl:with-param name="p_count"
												select="$p_count - 1" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	LanguageSpecific text

	NOTE - the MSHCComponent recognizes these bits and post-processes them into the format used
	       by the MS Help Viewer. 
	============================================================================================= -->

	<xsl:template name="t_decoratedNameSep">
		<span class="languageSpecificText">
			<span class="cs">.</span>
			<span class="vb">.</span>
			<span class="cpp">::</span>
			<span class="fs">.</span>
			<span class="nu">.</span>
		</span>
	</xsl:template>

	<xsl:template name="t_nullKeyword">
		<span class="keyword"
					style="display: none">
			<span class="languageSpecificText">
				<span class="cs">null</span>
				<span class="vb">Nothing</span>
				<span class="cpp">nullptr</span>
			</span>
		</span>
		<span class="nu">
			<include item="nullKeyword"/>
		</span>
	</xsl:template>

	<xsl:template name="t_staticKeyword">
		<span class="keyword"
					style="display: none">
			<span class="languageSpecificText">
				<span class="cs">static</span>
				<span class="vb">Shared</span>
				<span class="cpp">static</span>
			</span>
		</span>
		<span class="nu">
			<include item="staticKeyword"/>
		</span>
	</xsl:template>

	<xsl:template name="t_virtualKeyword">
		<span class="keyword"
					style="display: none">
			<span class="languageSpecificText">
				<span class="cs">virtual</span>
				<span class="vb">Overridable</span>
				<span class="cpp">virtual</span>
			</span>
		</span>
		<span class="nu">
			<include item="virtualKeyword"/>
		</span>
	</xsl:template>

	<xsl:template name="t_trueKeyword">
		<span class="keyword"
					style="display: none">
			<span class="languageSpecificText">
				<span class="cs">true</span>
				<span class="vb">True</span>
				<span class="cpp">true</span>
			</span>
		</span>
		<span class="nu">
			<include item="trueKeyword"/>
		</span>
	</xsl:template>

	<xsl:template name="t_falseKeyword">
		<span class="keyword"
					style="display: none">
			<span class="languageSpecificText">
				<span class="cs">false</span>
				<span class="vb">False</span>
				<span class="cpp">false</span>
			</span>
		</span>
		<span class="nu">
			<include item="falseKeyword"/>
		</span>
	</xsl:template>

	<xsl:template name="t_abstractKeyword">
		<span class="keyword"
					style="display: none">
			<span class="languageSpecificText">
				<span class="cs">abstract</span>
				<span class="vb">MustInherit</span>
				<span class="cpp">abstract</span>
			</span>
		</span>
		<span class="nu">
			<include item="abstractKeyword"/>
		</span>
	</xsl:template>

	<xsl:template name="t_inKeyword">
		<span class="keyword">
			<span class="languageSpecificText">
				<span class="cs">in</span>
				<span class="vb">In</span>
				<span class="cpp">in</span>
				<span class="fs"></span>
				<span class="nu">in</span>
			</span>
		</span>
	</xsl:template>

	<xsl:template name="t_outKeyword">
		<span class="keyword">
			<span class="languageSpecificText">
				<span class="cs">out</span>
				<span class="vb">Out</span>
				<span class="cpp">out</span>
				<span class="fs"></span>
				<span class="nu">out</span>
			</span>
		</span>
	</xsl:template>

	<!-- ============================================================================================
	Common metadata
	============================================================================================= -->

	<xsl:template name="t_insertMetadata">
		<xsl:element name="xml">
			<xsl:attribute name="id">BrandingData</xsl:attribute>
			<string id="BrandingProductTitle">
				<include item="productTitle"/>
			</string>

			<string id="BrandingCopyrightText">
				<include item="copyright_text"/>
			</string>
			<string id="BrandingCopyrightLink">
				<include item="copyright_link"/>
			</string>
			<string id="BrandingCopyrightInfo">
				<include item="copyright_info"/>
			</string>
			<string id="BrandingHeader">
				<include item="nsrTitle">
					<parameter>
						<xsl:call-template name="t_topicTitleDecorated"/>
					</parameter>
				</include>
			</string>

			<string id="BrandingFooterText">
				<include item="footer_text"/>
			</string>
			<string id="BrandingFeedbackAlias">
				<include item="fb_alias"/>
			</string>
			<string id="BrandingFeedbackSubject">
				<include item="fb_product"/>
			</string>
			<string id="BrandingFeedbackText">
				<include item="fb_text"/>
			</string>
			<string id="BrandingFeedbackFooterTo">
				<include item="fb_footer_to"/>
			</string>
			<string id="BrandingFeedbackFooterText">
				<include item="fb_footer_text"/>
			</string>
			<string id="BrandingFeedbackFooterTextTo">
				<include item="fb_footer_text_to"/>
			</string>
			<string id="BrandingFeedbackBody">
				<include item="fb_body"/>
			</string>
		</xsl:element>
	</xsl:template>

	<xsl:template name="t_insertNoIndexNoFollow">
		<xsl:if test="/document/metadata/attribute[@name='NoSearch']">
			<META NAME="ROBOTS"
						CONTENT="NOINDEX, NOFOLLOW" />
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	Running header
	============================================================================================= -->

	<xsl:template name="t_bodyTitle">
		<div class="majorTitle">
			<!---->
		</div>
		<div class="title">
			<include item="nsrTitle">
				<parameter>
					<xsl:call-template name="t_topicTitleDecorated"/>
				</parameter>
			</include>
		</div>

		<!-- This table is required for runningHeader/logo placement. It will be used and removed during branding. -->
		<!-- It basically fakes the formatting of older presentation styles so common logic can be applied. -->
		<table id="topTable"
					 style="display:none">
			<tr>
				<td id="titleColumn"></td>
			</tr>
			<tr>
				<td id="runningHeaderColumn"
						align="right">
					<xsl:call-template name="t_runningHeader" />
				</td>
			</tr>
		</table>
	</xsl:template>

	<!-- ============================================================================================
	SeeAlso links
	============================================================================================= -->

	<xsl:template name="t_autogenSeeAlsoLinks">

		<!-- a link to the containing type on all list and member topics -->
		<xsl:if test="($g_apiTopicGroup='member' or $g_apiTopicGroup='list')">
			<xsl:variable name="v_typeTopicId">
				<xsl:choose>
					<xsl:when test="/document/reference/topicdata/@typeTopicId">
						<xsl:value-of select="/document/reference/topicdata/@typeTopicId"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="/document/reference/containers/type/@api"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<div class="seeAlsoStyle">
				<referenceLink target="{$v_typeTopicId}"
											 display-target="format">
					<include item="SeeAlsoTypeLinkText">
						<parameter>{0}</parameter>
						<parameter>
							<xsl:choose>
								<xsl:when test="/document/reference/topicdata/@typeTopicId">
									<xsl:value-of select="/document/reference/apidata/@subgroup"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="/document/reference/containers/type/apidata/@subgroup"/>
								</xsl:otherwise>
							</xsl:choose>
						</parameter>
					</include>
				</referenceLink>
			</div>
		</xsl:if>

		<!-- a link to the type's All Members list -->
		<xsl:variable name="v_allMembersId">
			<xsl:choose>
				<xsl:when test="/document/reference/topicdata/@allMembersTopicId">
					<xsl:value-of select="/document/reference/topicdata/@allMembersTopicId"/>
				</xsl:when>
				<xsl:when test="$g_apiTopicGroup='member' or ($g_apiTopicGroup='list' and $g_apiTopicSubGroup='overload')">
					<xsl:value-of select="/document/reference/containers/type/topicdata/@allMembersTopicId"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="normalize-space($v_allMembersId) and not($v_allMembersId=$key)">
			<div class="seeAlsoStyle">
				<referenceLink target="{$v_allMembersId}"
											 display-target="format">
					<include item="SeeAlsoMembersLinkText">
						<parameter>{0}</parameter>
					</include>
				</referenceLink>
			</div>
		</xsl:if>

		<!-- a link to the overload topic -->
		<xsl:variable name="v_overloadId">
			<xsl:value-of select="/document/reference/memberdata/@overload"/>
		</xsl:variable>
		<xsl:if test="normalize-space($v_overloadId)">
			<div class="seeAlsoStyle">
				<referenceLink target="{$v_overloadId}"
											 display-target="format"
											 show-parameters="false">
					<include item="SeeAlsoOverloadLinkText">
						<parameter>{0}</parameter>
					</include>
				</referenceLink>
			</div>
		</xsl:if>

		<!-- a link to the namespace topic -->
		<xsl:variable name="v_namespaceId">
			<xsl:value-of select="/document/reference/containers/namespace/@api"/>
		</xsl:variable>
		<xsl:if test="normalize-space($v_namespaceId)">
			<div class="seeAlsoStyle">
				<referenceLink target="{$v_namespaceId}"
											 display-target="format">
					<include item="SeeAlsoNamespaceLinkText">
						<parameter>{0}</parameter>
					</include>
				</referenceLink>
			</div>
		</xsl:if>

	</xsl:template>

	<!-- ============================================================================================
	Section headers
	============================================================================================= -->

	<xsl:template name="t_putSection">
		<xsl:param name="p_title" />
		<xsl:param name="p_content" />
		<xsl:param name="p_toplink"
							 select="false()" />

		<mtps:CollapsibleArea>
			<xsl:attribute name="Title">
				<xsl:value-of select="$p_title" />
			</xsl:attribute>
			<xsl:element name="xml">
				<string id="Title">
					<xsl:copy-of select="$p_title" />
				</string>
			</xsl:element>
			<xsl:copy-of select="$p_content" />
			<xsl:if test="boolean($p_toplink)">
				<a href="#mainBody">
					<include item="top"/>
				</a>
			</xsl:if>
		</mtps:CollapsibleArea>
	</xsl:template>

	<xsl:template name="t_putSectionInclude">
		<xsl:param name="p_titleInclude" />
		<xsl:param name="p_content" />
		<xsl:param name="p_toplink"
							 select="false()" />

		<mtps:CollapsibleArea>
			<includeAttribute name="Title"
												item="{$p_titleInclude}"/>
			<xsl:element name="xml">
				<string id="Title">
					<include item="{$p_titleInclude}"/>
				</string>
			</xsl:element>
			<xsl:copy-of select="$p_content" />
			<xsl:if test="boolean($p_toplink)">
				<a href="#mainBody">
					<include item="top"/>
				</a>
			</xsl:if>
		</mtps:CollapsibleArea>
	</xsl:template>

	<xsl:template name="t_putSubSection">
		<xsl:param name="p_title" />
		<xsl:param name="p_content" />

		<h4 class="subHeading">
			<xsl:copy-of select="$p_title" />
		</h4>
		<xsl:copy-of select="$p_content" />
	</xsl:template>

	<!-- ============================================================================================
	Code formatting
	============================================================================================= -->

	<xsl:template name="t_putCodeSection">
		<xsl:param name="p_codeLang" />
		<xsl:param name="p_codeTitle" />
		<xsl:param name="p_formatCode"
							 select="false()" />
		<xsl:param name="p_enableCopyCode"
							 select="'true'" />

		<xsl:variable name="sectionId">
			<xsl:value-of select="generate-id()"/>
		</xsl:variable>
		<xsl:variable name="v_codeLangUnique">
			<xsl:call-template name="t_codeLang">
				<xsl:with-param name="p_codeLang"
												select="$p_codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="v_codeLangName">
			<xsl:choose>
				<xsl:when test="($v_codeLangUnique = 'none') and (normalize-space($p_codeTitle) != '')">
					<xsl:value-of select="$p_codeTitle"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="t_codeLangName">
						<xsl:with-param name="p_codeLang"
														select="$v_codeLangUnique"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<div class="OH_clear"></div>
		<a id="{$sectionId}_syntaxToggle">
			<!---->
			<!---->
		</a>
		<div id="{$sectionId}"
				 class="OH_CodeSnippetContainer">

			<xsl:call-template name="t_putCodeTabsSolo">
				<xsl:with-param name="sectionId"
												select="$sectionId"/>
				<xsl:with-param name="p_codeLangName"
												select="$v_codeLangName"/>
				<xsl:with-param name="p_enableCopyCode"
												select="$p_enableCopyCode"/>
			</xsl:call-template>

			<div id="{$sectionId}_codecollection"
					 class="OH_CodeSnippetContainerCodeCollection">
				<div class="OH_CodeSnippetToolBar">
					<div class="OH_CodeSnippetToolBarText">
						<a id="{$sectionId}_copycode"
							 href="javascript:CopyToClipboard('{$sectionId}','1')"
							 title="Copy to Clipboard">Copy to Clipboard</a>
						<a id="{$sectionId}_PrintText"
							 class="OH_PrintText"
							 href="javascript:Print('{$sectionId}','1')"
							 title="Print">Print</a>
					</div>
				</div>

				<xsl:call-template name="t_putCodeDiv">
					<xsl:with-param name="sectionId"
													select="$sectionId"/>
					<xsl:with-param name="p_codeLang"
													select="$v_codeLangUnique"/>
					<xsl:with-param name="p_formatCode"
													select="$p_formatCode"/>
				</xsl:call-template>
			</div>
		</div>
	</xsl:template>

	<xsl:template name="t_putCodeSections">
		<xsl:param name="p_codeNodes" />
		<xsl:param name="p_nodeCount" />
		<xsl:param name="p_codeLangAttr"
							 select="'language'" />
		<xsl:param name="p_formatCode"
							 select="true()" />
		<xsl:param name="p_enableCopyCode"
							 select="'true'"/>

		<xsl:variable name="sectionId">
			<xsl:value-of select="generate-id()"/>
		</xsl:variable>

		<div class="OH_clear"></div>
		<a id="{$sectionId}_syntaxToggle">
			<!---->
			<!---->
		</a>
		<div id="{$sectionId}"
				 class="OH_CodeSnippetContainer">

			<div class="OH_CodeSnippetContainerTabs"
					 id="{$sectionId}_tabs">
				<div class="OH_CodeSnippetContainerTabLeftActive"
						 id="{$sectionId}_tabimgleft"></div>

				<xsl:call-template name="t_putCodeTabs">
					<xsl:with-param name="sectionId"
													select="$sectionId"/>
					<xsl:with-param name="p_codeNodes"
													select="$p_codeNodes"/>
					<xsl:with-param name="p_nodeCount"
													select="$p_nodeCount"/>
					<xsl:with-param name="p_codeLangAttr"
													select="$p_codeLangAttr"/>
					<xsl:with-param name="p_enableCopyCode"
													select="$p_enableCopyCode"/>
				</xsl:call-template>
				<!--<div id="{$sectionId}_tab1"
						 class="OH_CodeSnippetContainerTabActive"
						 EnableCopyCode="false">
					<a href="javascript:ChangeTab('{$sectionId}','Visual Basic','1','5')">VB</a>
				</div>
				<div id="{$sectionId}_tab2"
						 class="OH_CodeSnippetContainerTab"
						 EnableCopyCode="false">
					<a href="javascript:ChangeTab('{$sectionId}','C#','2','5')">C#</a>
				</div>
				<div id="{$sectionId}_tab3"
						 class="OH_CodeSnippetContainerTab"
						 EnableCopyCode="false">
					<a href="javascript:ChangeTab('{$sectionId}','Visual C++','3','5')">C++</a>
				</div>
				<div id="{$sectionId}_tab4"
						 class="OH_CodeSnippetContainerTab"
						 EnableCopyCode="false">
					<a href="javascript:ChangeTab('{$sectionId}','F#','4','5')">F#</a>
				</div>
				<div id="{$sectionId}_tab5"
						 class="OH_CodeSnippetContainerTabDisabledNotFirst"
						 EnableCopyCode="false"
						 disabled="true">
					<a>JScript</a>
				</div>-->
				<div class="OH_CodeSnippetContainerTabRight"
						 id="{$sectionId}_tabimgright"></div>
			</div>

			<div id="{$sectionId}_codecollection"
					 class="OH_CodeSnippetContainerCodeCollection">
				<div class="OH_CodeSnippetToolBar">
					<div class="OH_CodeSnippetToolBarText">
						<a id="{$sectionId}_copycode"
							 href="javascript:CopyToClipboard('{$sectionId}','{$p_nodeCount}')"
							 title="Copy to Clipboard"
							 style="display: none">Copy to Clipboard</a>
						<a id="{$sectionId}_PrintText"
							 class="OH_PrintText"
							 href="javascript:Print('{$sectionId}','{$p_nodeCount}')"
							 title="Print">Print</a>
					</div>
				</div>

				<xsl:call-template name="t_putCodeDivs">
					<xsl:with-param name="sectionId"
													select="$sectionId"/>
					<xsl:with-param name="p_codeNodes"
													select="$p_codeNodes"/>
					<xsl:with-param name="p_nodeCount"
													select="$p_nodeCount"/>
					<xsl:with-param name="p_codeLangAttr"
													select="$p_codeLangAttr"/>
					<xsl:with-param name="p_formatCode"
													select="$p_formatCode"/>
				</xsl:call-template>
			</div>
		</div>
	</xsl:template>

	<xsl:template name="t_putCodeTabsSolo">
		<xsl:param name="sectionId" />
		<xsl:param name="p_codeLangName" />
		<xsl:param name="p_enableCopyCode"
							 select="'true'" />

		<div class="OH_CodeSnippetContainerTabs"
				 id="{$sectionId}_tabs">
			<div class="OH_CodeSnippetContainerTabLeftActive"
					 id="{$sectionId}_tabimgleft"></div>
			<div id="{$sectionId}_tab1"
					 class="OH_CodeSnippetContainerTabSolo"
					 EnableCopyCode="{$p_enableCopyCode}">
				<a href="#">
					<xsl:value-of select="$p_codeLangName"/>
				</a>
			</div>
			<div class="OH_CodeSnippetContainerTabRightActive"
					 id="{$sectionId}_tabimgright"></div>
		</div>
	</xsl:template>

	<xsl:template name="t_putCodeTabs">
		<xsl:param name="sectionId" />
		<xsl:param name="p_codeNodes" />
		<xsl:param name="p_nodeCount" />
		<xsl:param name="p_codeLangAttr"
							 select="'language'" />
		<xsl:param name="p_enableCopyCode"
							 select="'true'" />

		<xsl:for-each select="msxsl:node-set($p_codeNodes)">
			<xsl:variable name="nodeIndex"
										select="position()"/>
			<xsl:variable name="v_codeLang">
				<xsl:call-template name="t_codeLang">
					<xsl:with-param name="p_codeLang">
						<xsl:for-each select="@*">
							<xsl:if test="name() = $p_codeLangAttr">
								<xsl:value-of select="."/>
							</xsl:if>
						</xsl:for-each>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>

			<xsl:variable name="v_isCodeLangValid">
				<xsl:call-template name="t_isCodeLangValid">
					<xsl:with-param name="p_codeLang"
													select="$v_codeLang"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="v_codeTitle"
										select="@title"/>
			<xsl:variable name="v_codeLangName">
				<xsl:choose>
					<xsl:when test="($v_codeLang = 'none') and (normalize-space($v_codeTitle) != '')">
						<xsl:value-of select="$v_codeTitle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="t_codeLangName">
							<xsl:with-param name="p_codeLang"
															select="$v_codeLang"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="v_labelLang">
				<xsl:choose>
					<xsl:when test="($v_codeLang = 'none') and (normalize-space($v_codeTitle) != '')">
						<xsl:value-of select="$v_codeTitle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="t_codeLangLabel">
							<xsl:with-param name="p_codeLang"
															select="$v_codeLang"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<xsl:variable name="v_tabClass">
				<xsl:choose>
					<xsl:when test="$nodeIndex = 1">
						<xsl:text>OH_CodeSnippetContainerTabActive</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>OH_CodeSnippetContainerTab</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<div id="{$sectionId}_tab{$nodeIndex}"
					 class="{$v_tabClass}"
					 EnableCopyCode="{$p_enableCopyCode}">
				<a href="javascript:ChangeTab('{$sectionId}','{$v_labelLang}','{$nodeIndex}','{$p_nodeCount}')">
					<xsl:value-of select="$v_codeLangName"/>
				</a>
			</div>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="t_putCodeDiv">
		<xsl:param name="sectionId" />
		<xsl:param name="p_codeLang" />
		<xsl:param name="p_codeDivIndex"
							 select="1" />
		<xsl:param name="p_formatCode"
							 select="true()" />

		<xsl:variable name="v_isCodeLangValid">
			<xsl:call-template name="t_isCodeLangValid">
				<xsl:with-param name="p_codeLang"
												select="$p_codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="v_codeHidden">
			<xsl:if test="$p_codeDivIndex &gt; 1">
				<xsl:value-of select="'display: none'"/>
			</xsl:if>
		</xsl:variable>

		<div id="{$sectionId}_code_Div{$p_codeDivIndex}"
				 class="OH_CodeSnippetContainerCode"
				 style="{$v_codeHidden}">
			<div class="code">
				<xsl:choose>
					<xsl:when test="substring(normalize-space(.),1,8)='@@_SHFB_'">
						<!-- MS Help Viewer has code to show the code colorized or plain.  We'll ignore their colorizer and insert our own colorized text -->
						<pre xml:space="preserve"><xsl:text/><xsl:value-of select="."/><xsl:text/></pre>
					</xsl:when>
					<xsl:when test="boolean($p_formatCode)">
						<pre xml:space="preserve"><xsl:text/><xsl:apply-templates/><xsl:text/></pre>
					</xsl:when>
					<xsl:otherwise>
						<pre xml:space="preserve"><xsl:text/><xsl:copy-of select="."/><xsl:text/></pre>
					</xsl:otherwise>
				</xsl:choose>
			</div>
		</div>
		<!-- The hidden plain (uncolorized) text is for copying to the clipboard and printing -->
		<div id="{$sectionId}_code_Plain_Div{$p_codeDivIndex}"
				 class="OH_CodeSnippetContainerCode"
				 style="display: none">
			<div class="code">
				<!-- Leave it empty - it is filled in by the MSHCComponent -->
				<pre xml:space="preserve"><xsl:text/></pre>
			</div>
		</div>

	</xsl:template>

	<xsl:template name="t_putCodeDivs">
		<xsl:param name="sectionId" />
		<xsl:param name="p_codeNodes" />
		<xsl:param name="p_nodeCount" />
		<xsl:param name="p_codeLangAttr"
							 select="'language'" />
		<xsl:param name="p_formatCode"
							 select="true()" />

		<xsl:for-each select="msxsl:node-set($p_codeNodes)">
			<xsl:variable name="v_codeLang">
				<xsl:call-template name="t_codeLang">
					<xsl:with-param name="p_codeLang">
						<xsl:for-each select="@*">
							<xsl:if test="name() = $p_codeLangAttr">
								<xsl:value-of select="."/>
							</xsl:if>
						</xsl:for-each>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>

			<xsl:call-template name="t_putCodeDiv">
				<xsl:with-param name="sectionId"
												select="$sectionId"/>
				<xsl:with-param name="p_codeLang"
												select="$v_codeLang"/>
				<xsl:with-param name="p_formatCode"
												select="$p_formatCode"/>
				<xsl:with-param name="p_codeDivIndex"
												select="position()"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>

	<!-- ============================================================================================
	Code languages
	============================================================================================= -->

	<xsl:template name="t_codeLang">
		<xsl:param name="p_codeLang"/>
		<xsl:variable name="v_codeLangLC"
									select="translate($p_codeLang,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz')"/>
		<xsl:choose>
			<xsl:when test="$v_codeLangLC = 'vbs' or $v_codeLangLC = 'vbscript'">
				<xsl:text>VBScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'vb' or $v_codeLangLC = 'vb#' or $v_codeLangLC = 'kblangvb' or $v_codeLangLC = 'visualbasicdeclaration' or $v_codeLangLC = 'visualbasic'" >
				<xsl:text>VisualBasic</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'visualbasicusage'" >
				<xsl:text>VisualBasicUsage</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'c#' or $v_codeLangLC = 'cs' or $v_codeLangLC = 'csharp'" >
				<xsl:text>CSharp</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'cpp' or $v_codeLangLC = 'cpp#' or $v_codeLangLC = 'c' or $v_codeLangLC = 'c++' or $v_codeLangLC = 'kblangcpp' or $v_codeLangLC = 'managedcplusplus'" >
				<xsl:text>ManagedCPlusPlus</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'j#' or $v_codeLangLC = 'jsharp'">
				<xsl:text>JSharp</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'f#' or $v_codeLangLC = 'fs' or $v_codeLangLC = 'fsharp'">
				<xsl:text>FSharp</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'js' or $v_codeLangLC = 'jscript#' or $v_codeLangLC = 'jscript' or $v_codeLangLC = 'kbjscript'">
				<xsl:text>JScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'javascript'">
				<xsl:text>JavaScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'xml' or $v_codeLangLC = 'xmlLang'">
				<xsl:text>xmlLang</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'html'">
				<xsl:text>html</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'xaml'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'vb-c#' or $v_codeLangLC = 'visualbasicandcsharp'">
				<xsl:text>visualbasicANDcsharp</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'none'">
				<xsl:value-of select="$v_codeLangLC"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>other</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_codeLangName">
		<xsl:param name="p_codeLang"/>
		<xsl:variable name="v_codeLangUnique">
			<xsl:call-template name="t_codeLang">
				<xsl:with-param name="p_codeLang"
												select="$p_codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$v_codeLangUnique = 'VBScript'">
				<xsl:text>VBScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'VisualBasic' or $v_codeLangUnique = 'VisualBasicUsage'" >
				<xsl:text>VB</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'CSharp'" >
				<xsl:text>C#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'visualbasicANDcsharp'" >
				<xsl:text>VB and C#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'ManagedCPlusPlus'" >
				<xsl:text>C++</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'JSharp'">
				<xsl:text>VJ#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'FSharp'" >
				<xsl:text>F#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'JScript'">
				<xsl:text>JScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'JavaScript'">
				<xsl:text>JavaScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'xaml'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'xmlLang'">
				<xsl:text>XML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'html'">
				<xsl:text>HTML</xsl:text>
			</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_codeLangLabel">
		<xsl:param name="p_codeLang"/>
		<xsl:variable name="v_codeLangUnique">
			<xsl:call-template name="t_codeLang">
				<xsl:with-param name="p_codeLang"
												select="$p_codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$v_codeLangUnique = 'VBScript'">
				<xsl:text>Visual Basic Script</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'VisualBasic'" >
				<xsl:text>Visual Basic</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'VisualBasicUsage'" >
				<xsl:text>Visual Basic Usage</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'CSharp'" >
				<xsl:text>Visual C#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'visualbasicANDcsharp'" >
				<xsl:text>Visual Basic and C#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'ManagedCPlusPlus'" >
				<xsl:text>Visual C++</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'JSharp'">
				<xsl:text>Visual J#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'FSharp'" >
				<xsl:text>Visual F#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'JScript'">
				<xsl:text>JScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'JavaScript'">
				<xsl:text>JavaScript</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'xaml'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'xmlLang'">
				<xsl:text>XML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'html'">
				<xsl:text>HTML</xsl:text>
			</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="mshelpCodelangAttributes">
		<xsl:param name="snippets" />
		<xsl:for-each select="$snippets">

			<xsl:if test="not(@language=preceding::*/@language)">
				<xsl:variable name="v_codeLang">
					<xsl:choose>
						<xsl:when test="@language = 'VBScript' or @language = 'vbs'">
							<xsl:text>VBScript</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'VisualBasic' or @language = 'vb' or @language = 'vb#' or @language = 'VB' or @language = 'kbLangVB'" >
							<xsl:text>kbLangVB</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'CSharp' or @language = 'c#' or @language = 'cs' or @language = 'C#'" >
							<xsl:text>CSharp</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'ManagedCPlusPlus' or @language = 'cpp' or @language = 'cpp#' or @language = 'c' or @language = 'c++' or @language = 'C++' or @language = 'kbLangCPP'" >
							<xsl:text>kbLangCPP</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'JSharp' or @language = 'j#' or @language = 'jsharp' or @language = 'VJ#'">
							<xsl:text>VJ#</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'JScript' or @language = 'js' or @language = 'jscript#' or @language = 'jscript' or @language = 'JScript' or @language = 'kbJScript'">
							<xsl:text>kbJScript</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'XAML' or @language = 'xaml'">
							<xsl:text>XAML</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'JavaScript' or @language = 'javascript'">
							<xsl:text>JavaScript</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'xml'">
							<xsl:text>xml</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'html'">
							<xsl:text>html</xsl:text>
						</xsl:when>
						<xsl:when test="@language = 'vb-c#'">
							<xsl:text>visualbasicANDcsharp</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>other</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$v_codeLang='other'" />
					<!-- If $v_codeLang is already authored, then do nothing -->
					<xsl:when test="/document/metadata/attribute[@name='codelang']/text() = $v_codeLang" />
					<xsl:otherwise>
						<xsl:call-template name="t_codeLangAttr">
							<xsl:with-param name="p_codeLang"
															select="$v_codeLang" />
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:if>

		</xsl:for-each>
	</xsl:template>

	<xsl:template name="t_codeLangAttr">
		<xsl:param name="p_codeLang" />
		<MSHelp:Attr Name="p_codelang"
								 Value="{$p_codeLang}" />
	</xsl:template>

	<!-- ============================================================================================
	Debugging
	============================================================================================= -->

	<xsl:template name="t_Dump">
		<xsl:param name="p_Path"/>
		<xsl:param name="p_Recursive"
							 select="true()"/>

		<div>
			<xsl:value-of select="name($p_Path)"/>
			<ol>
				<xsl:for-each select="msxsl:node-set($p_Path)/@*">
					<li>
						@<xsl:value-of select="name()"/> [<xsl:value-of select="."/>]
					</li>
				</xsl:for-each>
			</ol>
			<ul>
				<xsl:for-each select="msxsl:node-set($p_Path)/*">
					<xsl:choose>
						<xsl:when test="boolean($p_Recursive)">
							<li>
								<xsl:call-template name="t_Dump">
									<xsl:with-param name="p_Path"
																	select="."/>
								</xsl:call-template>
							</li>
						</xsl:when>
						<xsl:otherwise>
							<li>
								<xsl:value-of select="name()"/> [<xsl:value-of select="."/>]
							</li>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:for-each>
			</ul>
		</div>
	</xsl:template>

</xsl:stylesheet>
