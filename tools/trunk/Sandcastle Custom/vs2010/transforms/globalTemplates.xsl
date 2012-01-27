<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								version="1.0"
								xmlns:MSHelp="http://msdn.microsoft.com/mshelp"
								xmlns:mshelp="http://msdn.microsoft.com/mshelp"
								xmlns:ddue="http://ddue.schemas.microsoft.com/authoring/2003/5"
								xmlns:xlink="http://www.w3.org/1999/xlink"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
        >

	<!-- ======================================================================================== -->

	<xsl:template name="autogenSeeAlsoLinks">

		<!-- a link to the containing type on all list and member topics -->
		<xsl:if test="($group='member' or $group='list')">
			<xsl:variable name="typeTopicId">
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
				<referenceLink target="{$typeTopicId}"
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
		<xsl:variable name="allMembersTopicId">
			<xsl:choose>
				<xsl:when test="/document/reference/topicdata/@allMembersTopicId">
					<xsl:value-of select="/document/reference/topicdata/@allMembersTopicId"/>
				</xsl:when>
				<xsl:when test="$group='member' or ($group='list' and $subgroup='overload')">
					<xsl:value-of select="/document/reference/containers/type/topicdata/@allMembersTopicId"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="normalize-space($allMembersTopicId) and not($allMembersTopicId=$key)">
			<div class="seeAlsoStyle">
				<referenceLink target="{$allMembersTopicId}"
											 display-target="format">
					<include item="SeeAlsoMembersLinkText">
						<parameter>{0}</parameter>
					</include>
				</referenceLink>
			</div>
		</xsl:if>

		<xsl:if test="/document/reference/memberdata/@overload">
			<!-- a link to the overload topic -->
			<div class="seeAlsoStyle">
				<referenceLink target="{/document/reference/memberdata/@overload}"
											 display-target="format"
											 show-parameters="false">
					<include item="SeeAlsoOverloadLinkText">
						<parameter>{0}</parameter>
					</include>
				</referenceLink>
			</div>
		</xsl:if>

		<!-- a link to the namespace topic -->
		<xsl:variable name="namespaceId">
			<xsl:value-of select="/document/reference/containers/namespace/@api"/>
		</xsl:variable>
		<xsl:if test="normalize-space($namespaceId)">
			<div class="seeAlsoStyle">
				<referenceLink target="{$namespaceId}"
											 display-target="format">
					<include item="SeeAlsoNamespaceLinkText">
						<parameter>{0}</parameter>
					</include>
				</referenceLink>
			</div>
		</xsl:if>

	</xsl:template>

	<!-- ============================================================================================
	String formatting
	============================================================================================= -->

	<xsl:variable name="typeId">
		<xsl:choose>
			<xsl:when test="/document/reference/topicdata[@group='api'] and /document/reference/apidata[@group='type']">
				<xsl:value-of select="$key"/>
			</xsl:when>
			<xsl:when test="/document/reference/topicdata/@typeTopicId">
				<xsl:value-of select="/document/reference/topicdata/@typeTopicId"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/document/reference/containers/type/@api"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<!-- indent by 2*n spaces -->
	<xsl:template name="indent">
		<xsl:param name="count" />
		<xsl:if test="$count &gt; 1">
			<xsl:text>&#160;&#160;</xsl:text>
			<xsl:call-template name="indent">
				<xsl:with-param name="count"
												select="$count - 1" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- Gets the substring after the last occurence of a period in a given string -->
	<xsl:template name="subString">
		<xsl:param name="name" />

		<xsl:choose>
			<xsl:when test="contains($name, '.')">
				<xsl:call-template name="subString">
					<xsl:with-param name="name"
													select="substring-after($name, '.')" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$name" />
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

	<xsl:template name="trimAtPeriod">
		<xsl:param name="string" />

		<xsl:variable name="trimmedString"
									select="substring(normalize-space($string), 1, 256)" />
		<xsl:choose>
			<xsl:when test="normalize-space($string) != $trimmedString">
				<xsl:choose>
					<xsl:when test="not(contains($trimmedString, '.'))">
						<xsl:value-of select="$trimmedString"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="substringAndLastPeriod">
							<xsl:with-param name="string"
															select="$trimmedString" />
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="normalize-space($string)"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="substringAndLastPeriod">
		<xsl:param name="string" />

		<xsl:if test="contains($string, '.')">
			<xsl:variable name="after"
										select="substring-after($string, '.')" />
			<xsl:value-of select="concat(substring-before($string, '.'),'.')" />
			<xsl:if test="contains($after, '.')">
				<xsl:call-template name="substringAndLastPeriod">
					<xsl:with-param name="string"
													select="$after" />
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	Reference links
	============================================================================================= -->

	<xsl:template name="createReferenceLink">
		<xsl:param name="id" />
		<xsl:param name="qualified"
							 select="false()" />
		<referenceLink class="mtps-internal-link"
									 target="{$id}"
									 qualified="{$qualified}" />
	</xsl:template>

	<xsl:template name="decoratedNameSep">
		<xsl:variable name="v_LangSpecId">
			<xsl:value-of select="generate-id()"/>
		</xsl:variable>
		<xsl:variable name="v_ScriptText">
			<xsl:value-of disable-output-escaping="yes"
										select="concat('addToLanSpecTextIdSet(&quot;',$v_LangSpecId,'?cs=.|vb=.|cpp=::|nu=.&quot;);')"/>
		</xsl:variable>
		<span id="{$v_LangSpecId}"></span>
		<script type="text/javascript">
			<xsl:value-of select="$v_ScriptText"/>
		</script>
	</xsl:template>

	<!-- ============================================================================================
	Section headers
	============================================================================================= -->

	<xsl:template name="t_section">
		<xsl:param name="toggleSwitch" />
		<xsl:param name="title" />
		<xsl:param name="content" />
		<xsl:param name="toplink"
							 select="false()" />

		<xsl:variable name="toggleSection"
									select="concat($toggleSwitch,'Section')" />

		<div class="OH_CollapsibleAreaRegion">
			<div class="OH_regiontitle">
				<xsl:copy-of select="$title" />
			</div>
			<div class="OH_CollapsibleArea_HrDiv">
				<hr class="OH_CollapsibleArea_Hr"
						xmlns="" />
			</div>
		</div>
		<div class="OH_clear">
		</div>
		<a id="{$toggleSection}">
			<!---->
			<!---->
		</a>
		<xsl:copy-of select="$content" />
		<xsl:if test="boolean($toplink)">
			<a href="#mainBody">
				<include item="top"/>
			</a>
		</xsl:if>
	</xsl:template>

	<xsl:template name="t_subSection">
		<xsl:param name="title" />
		<xsl:param name="content" />

		<h4 class="subHeading">
			<xsl:copy-of select="$title" />
		</h4>
		<xsl:copy-of select="$content" />
	</xsl:template>

	<!-- ============================================================================================
	Code formatting
	============================================================================================= -->

	<xsl:template name="t_codeSection">
		<xsl:param name="p_codeLang" />
		<xsl:param name="p_CodeTitle" />
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
				<xsl:when test="($v_codeLangUnique = 'none') and (normalize-space($p_CodeTitle) != '')">
					<xsl:value-of select="$p_CodeTitle"/>
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

			<xsl:call-template name="t_codeTabsSolo">
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

				<xsl:call-template name="t_codeDiv">
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

	<xsl:template name="t_codeSections">
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

				<xsl:call-template name="t_codeTabs">
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

				<xsl:call-template name="t_codeDivs">
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

	<xsl:template name="t_codeTabsSolo">
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

	<xsl:template name="t_codeTabs">
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

			<xsl:variable name="t_isCodeLangValid">
				<xsl:call-template name="t_isCodeLangValid">
					<xsl:with-param name="p_codeLang"
													select="$v_codeLang"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="p_CodeTitle"
										select="@title"/>
			<xsl:variable name="v_codeLangName">
				<xsl:choose>
					<xsl:when test="($v_codeLang = 'none') and (normalize-space($p_CodeTitle) != '')">
						<xsl:value-of select="$p_CodeTitle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="t_codeLangName">
							<xsl:with-param name="p_codeLang"
															select="$v_codeLang"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="labelLang">
				<xsl:choose>
					<xsl:when test="($v_codeLang = 'none') and (normalize-space($p_CodeTitle) != '')">
						<xsl:value-of select="$p_CodeTitle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="t_codeLangLabel">
							<xsl:with-param name="t_codeLang"
															select="$v_codeLang"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>

			<xsl:variable name="tabClass">
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
					 class="{$tabClass}"
					 EnableCopyCode="{$p_enableCopyCode}">
				<a href="javascript:ChangeTab('{$sectionId}','{$labelLang}','{$nodeIndex}','{$p_nodeCount}')">
					<xsl:value-of select="$v_codeLangName"/>
				</a>
			</div>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="t_codeDiv">
		<xsl:param name="sectionId" />
		<xsl:param name="p_codeLang" />
		<xsl:param name="p_codeDivIndex"
							 select="1" />
		<xsl:param name="p_formatCode"
							 select="true()" />

		<xsl:variable name="t_isCodeLangValid">
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

	<xsl:template name="t_codeDivs">
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

			<xsl:call-template name="t_codeDiv">
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
			<xsl:when test="$v_codeLangLC = 'vb6' or $v_codeLangLC = 'visualbasic6'" >
				<xsl:text>VisualBasic6</xsl:text>
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
			<xsl:when test="$v_codeLangUnique = 'VisualBasic6'" >
				<xsl:text>VB6</xsl:text>
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
			<xsl:when test="$v_codeLangUnique = 'VisualBasic6'" >
				<xsl:text>Visual Basic 6</xsl:text>
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
	Footer
	============================================================================================= -->
	<xsl:template name="t_footer">
		<div class="OH_footer"
				 id="footer">
			<include item="footer">
				<parameter>
					<xsl:value-of select="$key"/>
				</parameter>
				<parameter>
					<xsl:call-template name="topicTitlePlain"/>
				</parameter>
				<parameter>
					<xsl:value-of select="/document/metadata/item[@id='PBM_FileVersion']" />
				</parameter>
				<parameter>
					<xsl:value-of select="/document/metadata/attribute[@name='TopicVersion']" />
				</parameter>
			</include>
		</div>
	</xsl:template>

</xsl:stylesheet>
