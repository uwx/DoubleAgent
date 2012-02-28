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
			<xsl:when test="$v_codeLangLC = 'vb' or $v_codeLangLC = 'vb#' or $v_codeLangLC = 'kblangvb' or $v_codeLangLC = 'visualbasic'" >
				<xsl:text>VisualBasic</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangLC = 'visualbasicdeclaration'" >
				<xsl:text>VisualBasicDeclaration</xsl:text>
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
			<xsl:when test="$v_codeLangLC = 'aspnet'">
				<xsl:text>AspNet</xsl:text>
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

	<xsl:template name="t_codeLangTitle">
		<xsl:param name="p_codeLang"/>
		<xsl:variable name="v_codeLangUnique">
			<xsl:call-template name="t_codeLang">
				<xsl:with-param name="p_codeLang"
												select="$p_codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$v_codeLangUnique = 'VisualBasic'" >
				<xsl:text>Visual Basic</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'VBScript'">
				<xsl:text>Visual Basic Script</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'VisualBasicDeclaration'" >
				<xsl:text>Visual Basic Declaration</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'VisualBasicUsage'" >
				<xsl:text>Visual Basic Usage</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'CSharp'" >
				<xsl:text>C#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'visualbasicANDcsharp'" >
				<xsl:text>Visual Basic and C#</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'ManagedCPlusPlus'" >
				<xsl:text>Visual C++</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'JSharp'">
				<xsl:text>J#</xsl:text>
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
			<xsl:when test="$v_codeLangUnique = 'XAML'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'xmlLang'">
				<xsl:text>XML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'html'">
				<xsl:text>HTML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'AspNet'">
				<xsl:text>ASP.NET</xsl:text>
			</xsl:when>
			<xsl:otherwise/>
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
			<xsl:when test="$v_codeLangUnique = 'VisualBasic' or $v_codeLangUnique = 'VisualBasicUsage' or $v_codeLangUnique = 'VisualBasicDeclaration'" >
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
			<xsl:when test="$v_codeLangUnique = 'XAML'">
				<xsl:text>XAML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'xmlLang'">
				<xsl:text>XML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'html'">
				<xsl:text>HTML</xsl:text>
			</xsl:when>
			<xsl:when test="$v_codeLangUnique = 'AspNet'">
				<xsl:text>ASP.NET</xsl:text>
			</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="t_mshelpCodelangAttributes">
		<xsl:param name="snippets"/>
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
					<xsl:when test="$v_codeLang='other'"/>
					<!-- If $v_codeLang is already authored, then do nothing -->
					<xsl:when test="/document/metadata/attribute[@name='codelang']/text() = $v_codeLang"/>
					<xsl:otherwise>
						<xsl:call-template name="t_codeLangAttr">
							<xsl:with-param name="p_codeLang"
															select="$v_codeLang"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:if>

		</xsl:for-each>
	</xsl:template>

	<xsl:template name="t_codeLangAttr">
		<xsl:param name="p_codeLang"/>
		<MSHelp:Attr Name="codelang"
								 Value="{$p_codeLang}"/>
	</xsl:template>

	<!-- ============================================================================================
	Code sections
	============================================================================================= -->

	<xsl:template name="t_putCodeSection">
		<xsl:param name="p_codeLang"
							 select="@language"/>
		<xsl:param name="p_codeTitle"
							 select="@title"/>
		<xsl:param name="p_formatCode"
							 select="false()"/>
		<xsl:param name="p_enableCopyCode"
							 select="true()"/>

		<xsl:variable name="v_codeLangUnique">
			<xsl:call-template name="t_codeLang">
				<xsl:with-param name="p_codeLang"
												select="$p_codeLang"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="v_codeLangTitle">
			<xsl:choose>
				<xsl:when test="(($v_codeLangUnique='other') or ($v_codeLangUnique='none')) and (normalize-space($p_codeTitle)!='')">
					<xsl:value-of select="$p_codeTitle"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="t_codeLangTitle">
						<xsl:with-param name="p_codeLang"
														select="$v_codeLangUnique"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="v_code">
			<xsl:choose>
				<xsl:when test="$p_formatCode">
					<xsl:choose>
						<xsl:when test="local-name()='code' or local-name()='pre'">
							<xsl:apply-templates select="node()"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:apply-templates/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="local-name()='code' or local-name()='pre'">
							<xsl:copy-of select="node()"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:copy-of select="."/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:element name="mtps:CodeSnippet"
								 namespace="http://msdn2.microsoft.com/mtps">
			<xsl:attribute name="runat">
				<xsl:value-of select="'server'"/>
			</xsl:attribute>
			<!-- Post-branding treats 'other' and 'none' alike, so avoid using 'other' as the Language. Thus: -->
			<!--   If Language is 'other' a tab is formatted using DisplayLanguage. -->
			<!--   If Language is 'none' no tab is formatted. -->
			<xsl:attribute name="Language">
				<xsl:choose>
					<xsl:when test="($v_codeLangUnique!='other') and ($v_codeLangUnique!='none')">
						<xsl:value-of select="$v_codeLangUnique"/>
					</xsl:when>
					<xsl:when test="($v_codeLangUnique!='none')">
						<xsl:value-of select="$v_codeLangTitle"/>
					</xsl:when>
				</xsl:choose>
			</xsl:attribute>
			<xsl:attribute name="DisplayLanguage">
				<xsl:value-of select="$v_codeLangTitle"/>
			</xsl:attribute>
			<xsl:attribute name="ContainsMarkup">
				<xsl:choose>
					<xsl:when test="starts-with(normalize-space($v_code),'@@_')">
						<xsl:value-of select="'true'"/>
					</xsl:when>
					<!--<xsl:when test="msxsl:node-set($v_code)//*[(local-name()!='code') and (local-name()!='pre') and (local-name()!='div')]">
						<xsl:value-of select="'true'"/>
					</xsl:when>-->
					<xsl:otherwise>
						<xsl:value-of select="'false'"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:attribute name="EnableCopyCode">
				<xsl:value-of select="string($p_enableCopyCode)"/>
			</xsl:attribute>
			<xsl:choose>
				<xsl:when test="starts-with(normalize-space($v_code),'@@_')">
					<!-- MS Help Viewer has code to show the code colorized or plain.  We'll ignore their colorizer and insert our own colorized text later. -->
					<xsl:element name="div">
						<xsl:attribute name="class">
							<xsl:value-of select="'code'"/>
						</xsl:attribute>
						<xsl:value-of select="$v_code"/>
					</xsl:element>
				</xsl:when>
				<xsl:otherwise>
					<xsl:copy-of select="$v_code"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>

	<xsl:template name="t_putCodeSections">
		<xsl:param name="p_codeNodes"/>
		<xsl:param name="p_nodeCount"/>
		<xsl:param name="p_codeLangAttr"
							 select="'language'"/>
		<xsl:param name="p_formatCode"
							 select="true()"/>
		<xsl:param name="p_enableCopyCode"
							 select="true()"/>

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

			<xsl:call-template name="t_putCodeSection">
				<xsl:with-param name="p_codeLang"
												select="$v_codeLang"/>
				<xsl:with-param name="p_formatCode"
												select="$p_formatCode"/>
				<xsl:with-param name="p_enableCopyCode"
												select="$p_enableCopyCode"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>

	<!-- ============================================================================================
	Code sections (direct to branded)
	
	The following templates bypass the content branding by formatting code sections directly to
	the branded format.  They're not used in this presentation style, but they're available for
	use in derived styles.
	============================================================================================= -->

	<xsl:template name="t_putCodeSectionBranded">
		<xsl:param name="p_codeLang"
							 select="@title"/>
		<xsl:param name="p_codeTitle"
							 select="@language"/>
		<xsl:param name="p_formatCode"
							 select="false()"/>
		<xsl:param name="p_enableCopyCode"
							 select="true()"/>

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
				<xsl:when test="(($v_codeLangUnique = 'other') or ($v_codeLangUnique = 'none')) and (normalize-space($p_codeTitle) != '')">
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

	<xsl:template name="t_putCodeSectionsBranded">
		<xsl:param name="p_codeNodes"/>
		<xsl:param name="p_nodeCount"/>
		<xsl:param name="p_codeLangAttr"
							 select="'language'"/>
		<xsl:param name="p_formatCode"
							 select="true()"/>
		<xsl:param name="p_enableCopyCode"
							 select="true()"/>

		<xsl:variable name="sectionId">
			<xsl:value-of select="generate-id()"/>
		</xsl:variable>

		<div class="OH_clear"></div>
		<a id="{$sectionId}_syntaxToggle">
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
		<xsl:param name="sectionId"/>
		<xsl:param name="p_codeLangName"/>
		<xsl:param name="p_enableCopyCode"
							 select="true()"/>

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
		<xsl:param name="sectionId"/>
		<xsl:param name="p_codeNodes"/>
		<xsl:param name="p_nodeCount"/>
		<xsl:param name="p_codeLangAttr"
							 select="'language'"/>
		<xsl:param name="p_enableCopyCode"
							 select="true()"/>

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
					<xsl:when test="(($v_codeLang = 'other') or ($v_codeLang = 'none')) and (normalize-space($v_codeTitle) != '')">
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
			<xsl:variable name="v_codeLangTitle">
				<xsl:choose>
					<xsl:when test="(($v_codeLang = 'other') or ($v_codeLang = 'none')) and (normalize-space($v_codeTitle) != '')">
						<xsl:value-of select="$v_codeTitle"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="t_codeLangTitle">
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
				<a href="javascript:ChangeTab('{$sectionId}','{$v_codeLangTitle}','{$nodeIndex}','{$p_nodeCount}')">
					<xsl:value-of select="$v_codeLangName"/>
				</a>
			</div>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="t_putCodeDiv">
		<xsl:param name="sectionId"/>
		<xsl:param name="p_codeLang"/>
		<xsl:param name="p_codeDivIndex"
							 select="1"/>
		<xsl:param name="p_formatCode"
							 select="true()"/>

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
					<xsl:when test="$p_formatCode">
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
		<xsl:param name="sectionId"/>
		<xsl:param name="p_codeNodes"/>
		<xsl:param name="p_nodeCount"/>
		<xsl:param name="p_codeLangAttr"
							 select="'language'"/>
		<xsl:param name="p_formatCode"
							 select="true()"/>

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

</xsl:stylesheet>
