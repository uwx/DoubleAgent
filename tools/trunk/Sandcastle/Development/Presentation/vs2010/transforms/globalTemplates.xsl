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
	Parameters
	============================================================================================= -->

	<xsl:param name="metadata">false</xsl:param>
	<xsl:param name="languages">false</xsl:param>
	<xsl:param name="minimal-spacing">code;alert;listItem;table;tableHeader;definedTerm;definition;</xsl:param>

	<!-- ============================================================================================
	Globals
	============================================================================================= -->

	<xsl:variable name="xhtml"
								select="'http://www.w3.org/1999/xhtml'"/>
	<xsl:variable name="ddue"
								select="'http://ddue.schemas.microsoft.com/authoring/2003/5'"/>
	<xsl:variable name="mtps"
								select="'http://msdn2.microsoft.com/mtps'"/>
	<xsl:variable name="g_allUpperCaseLetters">ABCDEFGHIJKLMNOPQRSTUVWXYZ</xsl:variable>
	<xsl:variable name="g_allLowerCaseLetters">abcdefghijklmnopqrstuvwxyz</xsl:variable>

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
	Minimal spacing tests
	============================================================================================= -->

	<xsl:template name="t_checkMinimalSpacing">
		<xsl:param name="p_spacingType"
							 select="'any'"/>
		<xsl:param name="p_parentLevel"
							 select="0"/>
		<xsl:if test="$p_spacingType='any' or contains($minimal-spacing,$p_spacingType)">
			<xsl:variable name="v_minimalSpacing">
				<xsl:call-template name="t_getMinimalSpacing"/>
			</xsl:variable>
			<xsl:if test="$v_minimalSpacing='' or starts-with($v_minimalSpacing,'true')">
				<xsl:variable name="v_levelCheck">
					<xsl:call-template name="t_checkMinimalParent">
						<xsl:with-param name="p_parentLevel"
														select="$p_parentLevel"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="$v_levelCheck=''">
						<xsl:value-of select="'true'"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="$v_levelCheck"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:if>
		</xsl:if>
	</xsl:template>

	<xsl:template name="t_getMinimalSpacing">
		<xsl:choose>
			<xsl:when test="@minimal-spacing">
				<xsl:value-of select="@minimal-spacing"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:for-each select="parent::*">
					<xsl:call-template name="t_getMinimalSpacing"/>
				</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_checkMinimalParent">
		<xsl:param name="p_parentLevel"/>
		<xsl:param name="p_childLevel"
							 select="0"/>
		<xsl:choose>
			<xsl:when test="$p_parentLevel = 1">
				<xsl:for-each select="parent::*">
					<xsl:call-template name="t_checkMinimalChild">
						<xsl:with-param name="p_childLevel"
														select="$p_childLevel"/>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:when>
			<xsl:when test="$p_parentLevel &gt; 1">
				<xsl:for-each select="parent::*">
					<xsl:call-template name="t_checkMinimalParent">
						<xsl:with-param name="p_parentLevel"
														select="$p_parentLevel - 1"/>
						<xsl:with-param name="p_childLevel"
														select="$p_childLevel + 1"/>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_checkMinimalChild">
		<xsl:param name="p_childLevel"/>
		<xsl:choose>
			<xsl:when test="$p_childLevel = 0">
				<xsl:if test="@minimal-spacing!='true'">
					<xsl:choose>
						<xsl:when test="child::*[local-name()!='para']">
							<xsl:value-of select="'sibling!=para'"/>
						</xsl:when>
						<xsl:when test="(child::*[local-name()='para'])[2]">
							<xsl:value-of select="'sibling para[2]'"/>
						</xsl:when>
					</xsl:choose>
				</xsl:if>
			</xsl:when>
			<xsl:when test="$p_childLevel &gt; 0">
				<xsl:for-each select="child::*">
					<xsl:call-template name="t_checkMinimalChild">
						<xsl:with-param name="p_childLevel"
														select="$p_childLevel - 1"/>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:when>
		</xsl:choose>
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

			<xsl:if test="$languages/language">
				<list id="BrandingLanguages">
					<xsl:for-each select="$languages/language">
						<xsl:variable name="v_devlang">
							<xsl:call-template name="t_codeLangTitle">
								<xsl:with-param name="p_codeLang"
																select="@name"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:if test="normalize-space($v_devlang)!=''">
							<xsl:element name="value">
								<xsl:value-of select="$v_devlang"/>
							</xsl:element>
						</xsl:if>
					</xsl:for-each>
				</list>
			</xsl:if>
			<xsl:if test="/document/syntax/div[@codeLanguage]">
				<list id="BrandingSyntaxLanguages">
					<xsl:for-each select="/document/syntax/div[@codeLanguage and not(div[@class='nonXamlAssemblyBoilerplate'])]">
						<xsl:variable name="v_codeLang">
							<xsl:call-template name="t_codeLangTitle">
								<xsl:with-param name="p_codeLang"
																select="@codeLanguage"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:if test="normalize-space($v_codeLang)!=''">
							<xsl:element name="value">
								<xsl:value-of select="$v_codeLang"/>
							</xsl:element>
						</xsl:if>
					</xsl:for-each>
				</list>
			</xsl:if>
		</xsl:element>
	</xsl:template>

	<xsl:template name="t_insertNoIndexNoFollow">
		<xsl:if test="/document/metadata/attribute[@name='NoSearch']">
			<META NAME="ROBOTS"
						CONTENT="NOINDEX, NOFOLLOW" />
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	Branding fixups
	============================================================================================= -->

	<xsl:template name="t_fixupStylesheets"
								xml:space="preserve">
		<div id="dummyImageDivProvidesImageUrls"
				 style="display: none">
			<img id="tabLeftBG"
					 alt=""
					 style="display: none">
				<includeAttribute name="src"
													item="iconPath">
					<parameter>tabLeftBG.gif</parameter>
				</includeAttribute>
			</img>
			<img id="tabRightBG"
					 alt=""
					 style="display: none">
				<includeAttribute name="src"
													item="iconPath">
					<parameter>tabRightBG.gif</parameter>
				</includeAttribute>
			</img>
			<img id="footer_slice"
					 alt=""
					 style="display: none">
				<includeAttribute name="src"
													item="iconPath">
					<parameter>footer_slice.gif</parameter>
				</includeAttribute>
			</img>
		</div>
		<script type="text/javascript">
			try
			{
				var styleSheetEnum = new Enumerator(document.styleSheets);
				var styleSheet;
				var ruleNdx;
				var rule;

				for (; !styleSheetEnum.atEnd(); styleSheetEnum.moveNext())
				{
					styleSheet = styleSheetEnum.item();
					if (styleSheet.rules)
					{
						if (styleSheet.rules.length != 0)
						{
							for (ruleNdx = 0; ruleNdx != styleSheet.rules.length; ruleNdx++)
							{
								rule = styleSheet.rules.item(ruleNdx);

								var bgUrl = rule.style.backgroundImage;
								if (bgUrl != "")
								{
									bgUrl = bgUrl.substring(bgUrl.indexOf("(")+1,bgUrl.lastIndexOf(")"));
									if (bgUrl != "")
									{
										if (tabLeftBG.src.indexOf (bgUrl) != -1)
										{
											bgUrl = rule.style.backgroundImage.replace (bgUrl, tabLeftBG.src);
											rule.style.backgroundImage = bgUrl;
										}
										else if (tabRightBG.src.indexOf (bgUrl) != -1)
										{
											bgUrl = rule.style.backgroundImage.replace (bgUrl, tabRightBG.src);
											rule.style.backgroundImage = bgUrl;
										}
										else if (footer_slice.src.indexOf (bgUrl) != -1)
										{
											bgUrl = rule.style.backgroundImage.replace (bgUrl, footer_slice.src);
											rule.style.backgroundImage = bgUrl;
										}
									}
								}
							}
						}
					}
				}
			}
			catch (e) {}
			finally {}
		</script>
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
		<!-- It basically fakes the formatting of older presentation styles so common logic can be used in -->
		<!-- the PostTransformComponent. -->
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

		<xsl:element name="mtps:CollapsibleArea">
			<xsl:attribute name="runat">
				<xsl:value-of select="'server'" />
			</xsl:attribute>
			<xsl:attribute name="Title">
				<xsl:value-of select="$p_title" />
			</xsl:attribute>
			<xsl:element name="xml">
				<xsl:element name="string">
					<xsl:attribute name="id">
						<xsl:value-of select="'Title'" />
					</xsl:attribute>
					<xsl:copy-of select="$p_title" />
				</xsl:element>
			</xsl:element>

			<xsl:copy-of select="$p_content" />

			<xsl:if test="boolean($p_toplink)">
				<xsl:element name="a">
					<xsl:attribute name="href">
						<xsl:value-of select="'#mainBody'" />
					</xsl:attribute>
					<include item="top"/>
				</xsl:element>
			</xsl:if>
		</xsl:element>
	</xsl:template>

	<xsl:template name="t_putSectionInclude">
		<xsl:param name="p_titleInclude" />
		<xsl:param name="p_content" />
		<xsl:param name="p_toplink"
							 select="false()" />

		<xsl:element name="mtps:CollapsibleArea">
			<xsl:attribute name="runat">
				<xsl:value-of select="'server'" />
			</xsl:attribute>
			<includeAttribute name="Title"
												item="{$p_titleInclude}"/>
			<xsl:element name="xml">
				<xsl:element name="string">
					<xsl:attribute name="id">
						<xsl:value-of select="'Title'" />
					</xsl:attribute>
					<include item="{$p_titleInclude}"/>
				</xsl:element>
			</xsl:element>

			<xsl:copy-of select="$p_content" />

			<xsl:if test="boolean($p_toplink)">
				<xsl:element name="a">
					<xsl:attribute name="href">
						<xsl:value-of select="'#mainBody'" />
					</xsl:attribute>
					<include item="top"/>
				</xsl:element>
			</xsl:if>
		</xsl:element>
	</xsl:template>

	<xsl:template name="t_putSubSection">
		<xsl:param name="p_title" />
		<xsl:param name="p_content" />

		<xsl:element name="h4">
			<xsl:attribute name="class">
				<xsl:value-of select="'subHeading'"/>
			</xsl:attribute>
			<xsl:copy-of select="$p_title" />
		</xsl:element>
		<xsl:copy-of select="$p_content" />
	</xsl:template>

	<!-- ============================================================================================
	Debugging template for showing an element in comments
	============================================================================================= -->

	<xsl:template name="t_dumpContent">
		<xsl:param name="indent"
							 select="''"/>
		<xsl:choose>
			<xsl:when test="self::text()">
				<xsl:comment>
					<xsl:value-of select="$indent"/>
					<xsl:value-of select="."/>
				</xsl:comment>
			</xsl:when>
			<xsl:otherwise>
				<xsl:comment>
					<xsl:value-of select="$indent"/>
					<xsl:value-of select="'«'"/>
					<xsl:value-of select="name()"/>
					<xsl:for-each select="@*">
						<xsl:text xml:space="preserve"> </xsl:text>
						<xsl:value-of select="name()"/>
						<xsl:value-of select="'='"/>
						<xsl:value-of select="."/>
					</xsl:for-each>
					<xsl:choose>
						<xsl:when test="./node()">
							<xsl:value-of select="'»'"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="'/»'"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:comment>
				<xsl:for-each select="node()">
					<xsl:call-template name="t_dumpContent">
						<xsl:with-param name="indent"
														select="concat($indent,'  ')"/>
					</xsl:call-template>
				</xsl:for-each>
				<xsl:if test="./node()">
					<xsl:comment>
						<xsl:value-of select="$indent"/>
						<xsl:value-of select="'«/'"/>
						<xsl:value-of select="name()"/>
						<xsl:value-of select="'»'"/>
					</xsl:comment>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>
