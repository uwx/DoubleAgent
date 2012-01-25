<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								version="1.0"
								xmlns:MSHelp="http://msdn.microsoft.com/mshelp"
								xmlns:mshelp="http://msdn.microsoft.com/mshelp"
								xmlns:ddue="http://ddue.schemas.microsoft.com/authoring/2003/5"
								xmlns:xlink="http://www.w3.org/1999/xlink"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
        >

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

	<!-- Code -->

	<xsl:template name="codeSection">
		<xsl:param name="codeLang" />
		<xsl:param name="codeTitle" />

		<xsl:variable name="sectionId">
			<xsl:value-of select="generate-id()"/>
		</xsl:variable>
		<xsl:variable name="nameLang">
			<xsl:choose>
				<xsl:when test="($codeLang = 'none') and (normalize-space($codeTitle) != '')">
					<xsl:value-of select="$codeTitle"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="codeLangName">
						<xsl:with-param name="codeLang"
														select="$codeLang"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="labelLang">
			<xsl:choose>
				<xsl:when test="($codeLang = 'none') and (normalize-space($codeTitle) != '')">
					<xsl:value-of select="$codeTitle"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="codeLangLabel">
						<xsl:with-param name="codeLang"
														select="$codeLang"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<div class="OH_clear"></div>
		<a id="syntaxToggle">
			<!---->
			<!---->
		</a>
		<div id="{$sectionId}"
				 class="OH_CodeSnippetContainer">

			<xsl:call-template name="codeTabsSolo">
				<xsl:with-param name="sectionId"
												select="$sectionId"/>
				<xsl:with-param name="nameLang"
												select="$nameLang"/>
				<xsl:with-param name="labelLang"
												select="$labelLang"/>
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

				<xsl:call-template name="codeDiv">
					<xsl:with-param name="sectionId"
													select="$sectionId"/>
				</xsl:call-template>
			</div>
		</div>
	</xsl:template>

	<xsl:template name="codeSections">
		<xsl:param name="codeNodes" />
		<xsl:param name="nodeCount" />

		<xsl:variable name="sectionId">
			<xsl:value-of select="generate-id()"/>
		</xsl:variable>

		<div class="OH_clear"></div>
		<a id="syntaxToggle">
			<!---->
			<!---->
		</a>
		<div id="{$sectionId}"
				 class="OH_CodeSnippetContainer">

			<div class="OH_CodeSnippetContainerTabs"
					 id="{$sectionId}_tabs">
				<div class="OH_CodeSnippetContainerTabLeftActive"
						 id="{$sectionId}_tabimgleft"></div>

				<xsl:call-template name="codeTabs">
					<xsl:with-param name="sectionId"
													select="$sectionId"/>
					<xsl:with-param name="codeNodes"
													select="$codeNodes"/>
					<xsl:with-param name="nodeCount"
													select="$nodeCount"/>
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
							 href="javascript:CopyToClipboard('{$sectionId}','{$nodeCount}')"
							 title="Copy to Clipboard"
							 style="display: none">Copy to Clipboard</a>
						<a id="{$sectionId}_PrintText"
							 class="OH_PrintText"
							 href="javascript:Print('{$sectionId}','{$nodeCount}')"
							 title="Print">Print</a>
					</div>
				</div>

				<xsl:call-template name="codeDivs">
					<xsl:with-param name="sectionId"
													select="$sectionId"/>
					<xsl:with-param name="codeNodes"
													select="$codeNodes"/>
					<xsl:with-param name="nodeCount"
													select="$nodeCount"/>
				</xsl:call-template>
			</div>
		</div>
	</xsl:template>

	<xsl:template name="codeTabsSolo">
		<xsl:param name="sectionId" />
		<xsl:param name="nameLang" />
		<xsl:param name="labelLang" />
		<xsl:param name="enableCopyCode"
							 select="'true'" />

		<div class="OH_CodeSnippetContainerTabs"
				 id="{$sectionId}_tabs">
			<div class="OH_CodeSnippetContainerTabLeftActive"
					 id="{$sectionId}_tabimgleft"></div>
			<div id="{$sectionId}_tab1"
					 class="OH_CodeSnippetContainerTabSolo"
					 EnableCopyCode="{$enableCopyCode}">
				<!--<a href="#"
					 title="{$labelLang}">
					<xsl:value-of select="$nameLang"/>
				</a>-->
				<a href="#">
					<xsl:value-of select="$nameLang"/>
				</a>
			</div>
			<div class="OH_CodeSnippetContainerTabRightActive"
					 id="{$sectionId}_tabimgright"></div>
		</div>
	</xsl:template>

	<xsl:template name="codeTabs">
		<xsl:param name="sectionId" />
		<xsl:param name="codeNodes" />
		<xsl:param name="nodeCount" />
		<xsl:param name="enableCopyCode"
							 select="'true'" />

		<xsl:for-each select="msxsl:node-set($codeNodes)">
			<xsl:variable name="nodeIndex"
										select="position()"/>
			<xsl:variable name="codeLang">
				<xsl:call-template name="codeLang">
					<xsl:with-param name="codeLang"
													select="@language"/>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name="codeTitle"
										select="@title"/>
			<xsl:variable name="nameLang">
				<xsl:choose>
					<xsl:when test="($codeLang = 'none') and (normalize-space($codeTitle) != '')">
						<xsl:value-of select="$codeTitle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="codeLangName">
							<xsl:with-param name="codeLang"
															select="$codeLang"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="labelLang">
				<xsl:choose>
					<xsl:when test="($codeLang = 'none') and (normalize-space($codeTitle) != '')">
						<xsl:value-of select="$codeTitle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="codeLangLabel">
							<xsl:with-param name="codeLang"
															select="$codeLang"/>
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
					 EnableCopyCode="{$enableCopyCode}">
				<a href="javascript:ChangeTab('{$sectionId}','{$labelLang}','{$nodeIndex}','{$nodeCount}')">
					<xsl:value-of select="$nameLang"/>
				</a>
			</div>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="codeDiv">
		<xsl:param name="sectionId" />
		<xsl:param name="divIndex"
							 select="1" />

		<div id="{$sectionId}_code_Div{$divIndex}"
				 class="OH_CodeSnippetContainerCode"
				 style="">
			<div class="code">
				<xsl:choose>
					<xsl:when test="substring(normalize-space(.),1,8)='@@_SHFB_'">
						<!-- MS Help Viewer has code to show the code colorized or plain.  We'll ignore their colorizer and insert our colorized text -->
						<pre xml:space="preserve"><xsl:text/><xsl:value-of select="."/><xsl:text/></pre>
					</xsl:when>
					<xsl:otherwise>
						<pre xml:space="preserve"><xsl:text/><xsl:apply-templates select="."/><xsl:text/></pre>
					</xsl:otherwise>
				</xsl:choose>
			</div>
		</div>
		<div id="{$sectionId}_code_Plain_Div{$divIndex}"
				 class="OH_CodeSnippetContainerCode"
				 style="display: none">
			<div class="code">
				<xsl:choose>
					<xsl:when test="substring(normalize-space(.),1,8)='@@_SHFB_'">
						<!-- The hidden plain (uncolorized) text is for copying to the clipboard and printing -->
						<pre xml:space="preserve"><xsl:text/>@@_ORIGINAL_<xsl:value-of select="."/><xsl:text/></pre>
					</xsl:when>
					<xsl:otherwise>
						<pre xml:space="preserve"><xsl:text/><xsl:apply-templates select="."/><xsl:text/></pre>
					</xsl:otherwise>
				</xsl:choose>
			</div>
		</div>

	</xsl:template>

	<xsl:template name="codeDivs">
		<xsl:param name="sectionId" />
		<xsl:param name="codeNodes" />
		<xsl:param name="nodeCount" />

		<xsl:for-each select="msxsl:node-set($codeNodes)">
			<xsl:call-template name="codeDiv">
				<xsl:with-param name="sectionId"
												select="$sectionId"/>
				<xsl:with-param name="divIndex"
												select="position()"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>

	<!-- Code Language -->

	<xsl:template name="codeLang">
		<xsl:param name="codeLang"/>
		<xsl:variable name="codeLangLC"
									select="translate($codeLang,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','abcdefghijklmnopqrstuvwxyz ')"/>
		<xsl:choose>
			<xsl:when test="$codeLangLC = 'vbs' or $codeLangLC = 'vbscript'">
				<xsl:text>VBScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'vb' or $codeLangLC = 'vb#' or $codeLangLC = 'kblangvb' or $codeLangLC = 'visualbasic'" >
				<xsl:text>VisualBasic</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'visualbasicusage'" >
				<xsl:text>VisualBasicUsage</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'c#' or $codeLangLC = 'cs' or $codeLangLC = 'csharp'" >
				<xsl:text>CSharp</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'cpp' or $codeLangLC = 'cpp#' or $codeLangLC = 'c' or $codeLangLC = 'c++' or $codeLangLC = 'kblangcpp' or $codeLangLC = 'managedcplusplus'" >
				<xsl:text>ManagedCPlusPlus</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'j#' or $codeLangLC = 'jsharp'">
				<xsl:text>JSharp</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'f#' or $codeLangLC = 'fs' or $codeLangLC = 'fsharp'">
				<xsl:text>FSharp</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'js' or $codeLangLC = 'jscript#' or $codeLangLC = 'jscript' or $codeLangLC = 'kbjscript'">
				<xsl:text>JScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'javascript'">
				<xsl:text>JavaScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'xml' or $codeLangLC = 'xmlLang'">
				<xsl:text>xmlLang</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'html'">
				<xsl:text>html</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'xaml'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'vb-c#' or $codeLangLC = 'visualbasicandcsharp'">
				<xsl:text>visualbasicANDcsharp</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangLC = 'none'">
				<xsl:text>{$codeLangLC}</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>other</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="codeLangLabel">
		<xsl:param name="codeLang"/>
		<xsl:variable name="codeLangUnique">
			<xsl:call-template name="codeLang">
				<xsl:with-param name="codeLang"
												select="$codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$codeLangUnique = 'VBScript'">
				<xsl:text>Visual Basic Script</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'VisualBasic'" >
				<xsl:text>Visual Basic</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'VisualBasicUsage'" >
				<xsl:text>Visual Basic Usage</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'CSharp'" >
				<xsl:text>Visual C#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'visualbasicANDcsharp'" >
				<xsl:text>Visual Basic and C#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'ManagedCPlusPlus'" >
				<xsl:text>Visual C++</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'JSharp'">
				<xsl:text>Visual J#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'FSharp'" >
				<xsl:text>Visual F#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'JScript'">
				<xsl:text>JScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'JavaScript'">
				<xsl:text>JavaScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'xaml'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'xmlLang'">
				<xsl:text>XML</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'html'">
				<xsl:text>HTML</xsl:text>
			</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="codeLangName">
		<xsl:param name="codeLang"/>
		<xsl:variable name="codeLangUnique">
			<xsl:call-template name="codeLang">
				<xsl:with-param name="codeLang"
												select="$codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$codeLangUnique = 'VBScript'">
				<xsl:text>VBScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'VisualBasic' or $codeLangUnique = 'VisualBasicUsage'" >
				<xsl:text>VB</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'CSharp'" >
				<xsl:text>C#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'visualbasicANDcsharp'" >
				<xsl:text>VB and C#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'ManagedCPlusPlus'" >
				<xsl:text>C++</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'JSharp'">
				<xsl:text>VJ#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'FSharp'" >
				<xsl:text>F#</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'JScript'">
				<xsl:text>JScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'JavaScript'">
				<xsl:text>JavaScript</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'xaml'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'xmlLang'">
				<xsl:text>XML</xsl:text>
			</xsl:when>
			<xsl:when test="$codeLangUnique = 'html'">
				<xsl:text>HTML</xsl:text>
			</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="mshelpCodelangAttributes">
		<xsl:param name="snippets" />
		<xsl:for-each select="$snippets">

			<xsl:if test="not(@language=preceding::*/@language)">
				<xsl:variable name="codeLang">
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
					<xsl:when test="$codeLang='other'" />
					<!-- If $codeLang is already authored, then do nothing -->
					<xsl:when test="/document/metadata/attribute[@name='codelang']/text() = $codeLang" />
					<xsl:otherwise>
						<xsl:call-template name="codeLangAttr">
							<xsl:with-param name="codeLang"
															select="$codeLang" />
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:if>

		</xsl:for-each>
	</xsl:template>

	<xsl:template name="codeLangAttr">
		<xsl:param name="codeLang" />
		<MSHelp:Attr Name="codelang"
								 Value="{$codeLang}" />
	</xsl:template>

	<!-- String -->

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

</xsl:stylesheet>
