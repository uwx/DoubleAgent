<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								version="1.1"
								xmlns:MSHelp="http://msdn.microsoft.com/mshelp"
								xmlns:mshelp="http://msdn.microsoft.com/mshelp"
								xmlns:ddue="http://ddue.schemas.microsoft.com/authoring/2003/5"
								xmlns:mtps="http://msdn2.microsoft.com/mtps"
								xmlns:xlink="http://www.w3.org/1999/xlink"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
   >
	<!-- ======================================================================================== -->

	<xsl:import href="../../shared/transforms/utilities_dduexml.xsl" />

	<!-- ======================================================================================== -->

	<!-- the Remarks section includes content from these nodes, excluding the xaml sections are captured in the xaml syntax processing -->
	<xsl:template name="HasRemarksContent">
		<xsl:param name="node" />

		<xsl:choose>
			<xsl:when test="/document/reference/attributes/attribute/type[@api='T:System.Security.Permissions.HostProtectionAttribute']">true</xsl:when>
			<xsl:when test="normalize-space($node/ddue:remarks/ddue:content)">true</xsl:when>
			<xsl:when test="normalize-space($node/ddue:notesForImplementers)">true</xsl:when>
			<xsl:when test="normalize-space($node/ddue:notesForCallers)">true</xsl:when>
			<xsl:when test="normalize-space($node/ddue:notesForInheritors)">true</xsl:when>
			<xsl:when test="normalize-space($node/ddue:platformNotes)">true</xsl:when>
			<xsl:when test="normalize-space($node/ddue:remarks/ddue:sections/ddue:section[not(
                starts-with(@address,'xamlValues') or 
                starts-with(@address,'xamlTextUsage') or 
                starts-with(@address,'xamlAttributeUsage') or 
                starts-with(@address,'xamlPropertyElementUsage') or 
                starts-with(@address,'xamlImplicitCollectionUsage') or 
                starts-with(@address,'xamlObjectElementUsage') or 
                starts-with(@address,'dependencyPropertyInfo') or 
                starts-with(@address,'routedEventInfo')
                )])">true</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:remarks">
		<xsl:call-template name="WriteRemarksSection">
			<xsl:with-param name="node"
											select=".." />
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="ddue:codeExamples">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'examplesTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
					<xsl:call-template name="moreCodeSection"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- 
  **************************************************************
  CODE EXAMPLES section
  **************************************************************
  -->
	<!-- tasks/task nodes are inserted by TaskGrabberComponent which gets content from HowTo topics -->
	<!-- these nodes are handled below in the moreCodeSection -->
	<xsl:template match="ddue:codeExamples/ddue:codeExample/ddue:legacy/ddue:content/tasks"/>

	<xsl:template name="moreCodeSection">
		<xsl:variable name="gotCodeAlready"
									select="boolean(
            (ddue:codeExample/ddue:legacy/ddue:content[ddue:codeReference[ddue:sampleCode] | ddue:code | ddue:snippets/ddue:snippet]) or
            (ddue:codeExample[ddue:codeReference[ddue:sampleCode] | ddue:code | ddue:snippets/ddue:snippet])
            )"/>

		<xsl:variable name="gotMoreCode"
									select="(count(ddue:codeExample/ddue:legacy/ddue:content/tasks/task)&gt;1) or 
                           ($gotCodeAlready and count(ddue:codeExample/ddue:legacy/ddue:content/tasks/task)&gt;0)"/>

		<!-- if no preceding code in the code examples section, display the tasks[1]/task[1] -->
		<xsl:if test="not($gotCodeAlready)">
			<xsl:for-each select="ddue:codeExample/ddue:legacy/ddue:content/tasks[1]/task[1]">
				<xsl:apply-templates select="ddue:introduction | ddue:codeExample"/>
			</xsl:for-each>
		</xsl:if>

		<xsl:if test="$gotMoreCode">
			<sections>
				<h4 class="subHeading">
					<include item="mrefTaskMoreCodeHeading" />
				</h4>
				<div class="subsection">
					<div class="tableSection">
						<table width="100%"
									 cellspacing="2"
									 cellpadding="5">
							<xsl:for-each select="ddue:codeExample/ddue:legacy/ddue:content/tasks/task">
								<xsl:choose>
									<xsl:when test="not($gotCodeAlready) and position()=1"/>
									<xsl:otherwise>
										<tr valign="top">
											<td>
												<conceptualLink target="{@topicId}">
													<xsl:value-of select="ddue:title"/>
												</conceptualLink>
											</td>
											<td>
												<xsl:choose>
													<xsl:when test="ddue:introduction/ddue:para[1][normalize-space(.)!='']">
														<xsl:apply-templates select="ddue:introduction/ddue:para[1]/node()"/>
													</xsl:when>
													<xsl:when test="ddue:codeExample/ddue:legacy/ddue:content/ddue:para[1][normalize-space(.)!='']">
														<xsl:apply-templates select="ddue:codeExample/ddue:legacy/ddue:content/ddue:para[1]/node()"/>
													</xsl:when>
												</xsl:choose>
											</td>
										</tr>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:for-each>
						</table>
					</div>
				</div>
			</sections>
		</xsl:if>
	</xsl:template>

	<xsl:template name="threadSafety">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="p_titleInclude"
											select="'threadSafetyTitle'" />
			<xsl:with-param name="p_content">
				<xsl:choose>
					<xsl:when test="/document/comments/ddue:dduexml/ddue:threadSafety">
						<xsl:apply-templates select="/document/comments/ddue:dduexml/ddue:threadSafety"/>
					</xsl:when>
					<xsl:otherwise>
						<include item="ThreadSafetyBP"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="ddue:notesForImplementers">
		<p/>
		<span class="label">
			<include item="NotesForImplementers"/>
		</span>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:notesForCallers">
		<p/>
		<span class="label">
			<include item="NotesForCallers"/>
		</span>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:notesForInheritors">
		<p/>
		<span class="label">
			<include item="NotesForInheritors"/>
		</span>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:platformNotes">
		<xsl:for-each select="ddue:platformNote[normalize-space(ddue:content)]">
			<p>
				<include item="PlatformNote">
					<parameter>
						<xsl:for-each select="ddue:platforms/ddue:platform">
							<xsl:variable name="platformName">
								<xsl:value-of select="."/>
							</xsl:variable>
							<include item="{$platformName}"/>
							<xsl:if test="position() != last()">, </xsl:if>
						</xsl:for-each>
					</parameter>
					<parameter>
						<xsl:apply-templates select="ddue:content"/>
					</parameter>
				</include>
			</p>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="ddue:platformNotes/ddue:platformNote/ddue:content/ddue:para">
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match="ddue:schemaHierarchy">
		<xsl:for-each select="ddue:link">
			<xsl:call-template name="t_putIndent">
				<xsl:with-param name="p_count"
												select="position()"/>
			</xsl:call-template>
			<xsl:apply-templates select="."/>
			<br/>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="ddue:syntaxSection">
		<div id="syntaxSection"
				 class="section">
			<div id="snippetGroup_Syntax"
					 class="code">
				<xsl:for-each select="ddue:legacySyntax">
					<xsl:variable name="v_codeLang">
						<xsl:call-template name="t_codeLang">
							<xsl:with-param name="p_codeLang"
															select="@language"/>
						</xsl:call-template>
					</xsl:variable>

					<span codeLanguage="{$v_codeLang}">
						<table width="100%"
									 cellspacing="0"
									 cellpadding="0">
							<tr>
								<th align="left">
									<include item="{$v_codeLang}"/>
								</th>
							</tr>
							<tr>
								<td>
									<pre xml:space="preserve"><xsl:apply-templates xml:space="preserve"/></pre>
								</td>
							</tr>
						</table>
					</span>

				</xsl:for-each>
			</div>
		</div>
	</xsl:template>

	<!-- just skip over these -->
	<xsl:template match="ddue:content | ddue:legacy">
		<xsl:apply-templates />
	</xsl:template>

	<!-- overrides of shared templates -->

	<xsl:template match="ddue:subscript | ddue:subscriptType">
		<xsl:if test="normalize-space(.)">
			<small>
				<small>
					<sub>
						<xsl:apply-templates />
					</sub>
				</small>
			</small>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:superscript | ddue:superscriptType">
		<xsl:if test="normalize-space(.)">
			<small>
				<small>
					<sup>
						<xsl:apply-templates />
					</sup>
				</small>
			</small>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:literal">
		<xsl:if test="normalize-space(.)">
			<span class="literal">
				<xsl:apply-templates />
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:parameterReference">
		<xsl:if test="normalize-space(.)">
			<span class="parameter">
				<xsl:apply-templates />
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:replaceable | ddue:placeholder">
		<xsl:if test="normalize-space(.)">
			<span class="placeholder">
				<xsl:apply-templates />
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:ui">
		<xsl:if test="normalize-space(.)">
			<span class="ui">
				<xsl:apply-templates />
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:newTerm">
		<xsl:if test="normalize-space(.)">
			<span class="term">
				<xsl:apply-templates />
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:errorInline|ddue:fictitiousUri|ddue:localUri">
		<xsl:if test="normalize-space(.)">
			<em>
				<xsl:apply-templates />
			</em>
		</xsl:if>
	</xsl:template>

	<!-- block elements -->

	<xsl:template match="ddue:list">
		<xsl:choose>
			<xsl:when test="@class='bullet'">
				<ul>
					<xsl:apply-templates select="ddue:listItem | ddue:list" />
				</ul>
			</xsl:when>
			<xsl:when test="@class='ordered'">
				<ol>
					<xsl:if test="@start">
						<xsl:attribute name="start">
							<xsl:value-of select="@start"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:apply-templates select="ddue:listItem | ddue:list" />
				</ol>
			</xsl:when>
			<xsl:otherwise>
				<ul style="list-style-type:none;">
					<xsl:apply-templates select="ddue:listItem | ddue:list" />
				</ul>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:table">
		<div class="tableSection">
			<xsl:if test="normalize-space(ddue:title)">
				<div class="caption">
					<xsl:value-of select="ddue:title"/>
				</div>
			</xsl:if>
			<table>
				<xsl:apply-templates />
			</table>
		</div>
	</xsl:template>

	<xsl:template match="ddue:tableHeader">
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match="ddue:row">
		<tr>
			<xsl:apply-templates />
		</tr>
	</xsl:template>

	<xsl:template match="ddue:entry">
		<td>
			<xsl:apply-templates select="@address" />
			<xsl:apply-templates />
		</td>
	</xsl:template>

	<xsl:template match="ddue:tableHeader/ddue:row/ddue:entry">
		<th>
			<xsl:apply-templates />
		</th>
	</xsl:template>

	<xsl:template match="ddue:definitionTable">
		<dl class="authored">
			<xsl:apply-templates />
		</dl>
	</xsl:template>

	<xsl:template match="ddue:definedTerm">
		<dt>
			<xsl:apply-templates select="@address" />
			<xsl:apply-templates />
		</dt>
	</xsl:template>

	<xsl:template match="ddue:definition">
		<dd>
			<xsl:apply-templates />
		</dd>
	</xsl:template>

	<xsl:template match="ddue:code">
		<xsl:variable name="titleName">
			<xsl:value-of select="../../ddue:title"/>
		</xsl:variable>
		<xsl:variable name="currentName"
									select="local-name()"/>
		<xsl:variable name="currentPos"
									select="position()"/>

		<xsl:choose>
			<xsl:when test="local-name(../*[$currentPos - 1]) != $currentName">
				<xsl:choose>
					<xsl:when test="local-name(../*[$currentPos + 1]) != $currentName">
						<xsl:call-template name="t_putCodeSection" />
					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name="endNodePos">
							<xsl:for-each select="../*">
								<xsl:if test="(position() &gt; $currentPos) and (local-name() != $currentName)">
									<xsl:value-of select="position()"/>
								</xsl:if>
							</xsl:for-each>
						</xsl:variable>
						<xsl:variable name="codeCount">
							<xsl:choose>
								<xsl:when test="$endNodePos &gt; 0">
									<xsl:value-of select="$endNodePos - $currentPos"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="count(../*) - $currentPos + 1"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<xsl:variable name="v_codeNodes"
													select="../*[not(position() &lt; $currentPos) and (position() &lt; ($currentPos + $codeCount))]"/>

						<xsl:call-template name="t_putCodeSections">
							<xsl:with-param name="p_codeNodes"
															select="$v_codeNodes" />
							<xsl:with-param name="p_nodeCount"
															select="$codeCount" />
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:sampleCode">
		<div>
			<span class="label">
				<xsl:value-of select="@language"/>
			</span>
		</div>
		<div class="code">
			<pre xml:space="preserve"><xsl:apply-templates /></pre>
		</div>
	</xsl:template>

	<xsl:template name="composeCode">
		<xsl:copy-of select="." />
		<xsl:variable name="next"
									select="following-sibling::*[1]" />
		<xsl:if test="boolean($next/@language) and boolean(local-name($next)=local-name())">
			<xsl:for-each select="$next">
				<xsl:call-template name="composeCode" />
			</xsl:for-each>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:alert">
		<xsl:variable name="title">
			<xsl:choose>
				<xsl:when test="@class='note'">
					<xsl:text>noteTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@class='tip'">
					<xsl:text>tipTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@class='caution' or @class='warning'">
					<xsl:text>cautionTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@class='security' or @class='security note'">
					<xsl:text>securityTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@class='important'">
					<xsl:text>importantTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@class='vb' or @class='VB' or @class='VisualBasic' or @class='visual basic note'">
					<xsl:text>visualBasicTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@class='cs' or @class='CSharp' or @class='c#' or @class='C#' or @class='visual c# note'">
					<xsl:text>visualC#Title</xsl:text>
				</xsl:when>
				<xsl:when test="@class='cpp' or @class='c++' or @class='C++' or @class='CPP' or @class='visual c++ note'">
					<xsl:text>visualC++Title</xsl:text>
				</xsl:when>
				<xsl:when test="@class='JSharp' or @class='j#' or @class='J#' or @class='visual j# note'">
					<xsl:text>visualJ#Title</xsl:text>
				</xsl:when>
				<xsl:when test="@class='implement'">
					<xsl:text>NotesForImplementers</xsl:text>
				</xsl:when>
				<xsl:when test="@class='caller'">
					<xsl:text>NotesForCallers</xsl:text>
				</xsl:when>
				<xsl:when test="@class='inherit'">
					<xsl:text>NotesForInheritors</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>noteTitle</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="altTitle">
			<xsl:choose>
				<xsl:when test="@class='note' or @class='implement' or @class='caller' or @class='inherit'">
					<xsl:text>noteAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='tip'">
					<xsl:text>tipAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='caution' or @class='warning'">
					<xsl:text>cautionAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='security' or @class='security note'">
					<xsl:text>securityAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='important'">
					<xsl:text>importantAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='vb' or @class='VB' or @class='VisualBasic' or @class='visual basic note'">
					<xsl:text>visualBasicAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='cs' or @class='CSharp' or @class='c#' or @class='C#' or @class='visual c# note'">
					<xsl:text>visualC#AltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='cpp' or @class='c++' or @class='C++' or @class='CPP' or @class='visual c++ note'">
					<xsl:text>visualC++AltText</xsl:text>
				</xsl:when>
				<xsl:when test="@class='JSharp' or @class='j#' or @class='J#' or @class='visual j# note'">
					<xsl:text>visualJ#AltText</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>noteAltText</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="noteImg">
			<xsl:choose>
				<xsl:when test="@class='note' or @class='tip' or @class='implement' or @class='caller' or @class='inherit'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@class='caution' or @class='warning'">
					<xsl:text>alert_caution.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@class='security' or @class='security note'">
					<xsl:text>alert_security.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@class='important'">
					<xsl:text>alert_caution.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@class='vb' or @class='VB' or @class='VisualBasic' or @class='visual basic note'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@class='cs' or @class='CSharp' or @class='c#' or @class='C#' or @class='visual c# note'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@class='cpp' or @class='c++' or @class='C++' or @class='CPP' or @class='visual c++ note'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@class='JSharp' or @class='j#' or @class='J#' or @class='visual j# note'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<div class="alert">
			<table>
				<tr>
					<th>
						<img class="mtps-img-src">
							<includeAttribute item="iconPath"
																name="src">
								<parameter>
									<xsl:value-of select="$noteImg"/>
								</parameter>
							</includeAttribute>
							<includeAttribute name="alt"
																item="{$altTitle}" />
						</img>
						<xsl:text> </xsl:text>
						<include item="{$title}" />
					</th>
				</tr>
				<tr>
					<td>
						<xsl:apply-templates />
					</td>
				</tr>
			</table>
		</div>
	</xsl:template>

	<xsl:template match="ddue:sections">
		<xsl:apply-templates select="ddue:section" />
	</xsl:template>

	<xsl:template match="ddue:section">
		<!-- display the section only if it has content (text or media)-->
		<xsl:if test="descendant::ddue:content[normalize-space(.)] or count(ddue:content/*) &gt; 0">

			<xsl:apply-templates select="@address" />
			<!-- Count all the possible ancestor root nodes -->
			<xsl:variable name="a1"
										select="count(ancestor::ddue:attributesandElements)" />
			<xsl:variable name="a2"
										select="count(ancestor::ddue:codeExample)" />
			<xsl:variable name="a3"
										select="count(ancestor::ddue:dotNetFrameworkEquivalent)" />
			<xsl:variable name="a4"
										select="count(ancestor::ddue:elementInformation)" />
			<xsl:variable name="a5"
										select="count(ancestor::ddue:exceptions)" />
			<xsl:variable name="a6"
										select="count(ancestor::ddue:introduction)" />
			<xsl:variable name="a7"
										select="count(ancestor::ddue:languageReferenceRemarks)" />
			<xsl:variable name="a8"
										select="count(ancestor::ddue:nextSteps)" />
			<xsl:variable name="a9"
										select="count(ancestor::ddue:parameters)" />
			<xsl:variable name="a10"
										select="count(ancestor::ddue:prerequisites)" />
			<xsl:variable name="a11"
										select="count(ancestor::ddue:procedure)" />
			<xsl:variable name="a12"
										select="count(ancestor::ddue:relatedTopics)" />
			<xsl:variable name="a13"
										select="count(ancestor::ddue:remarks)" />
			<xsl:variable name="a14"
										select="count(ancestor::ddue:requirements)" />
			<xsl:variable name="a15"
										select="count(ancestor::ddue:schemaHierarchy)" />
			<xsl:variable name="a16"
										select="count(ancestor::ddue:syntaxSection)" />
			<xsl:variable name="a17"
										select="count(ancestor::ddue:textValue)" />
			<xsl:variable name="a18"
										select="count(ancestor::ddue:type)" />
			<xsl:variable name="a19"
										select="count(ancestor::ddue:section)" />
			<xsl:variable name="total"
										select="$a1+$a2+$a3+$a4+$a5+$a6+$a7+$a8+$a9+$a10+$a11+$a12+$a13+$a14+$a15+$a16+$a17+$a18+$a19" />
			<xsl:choose>
				<!-- Don't render the 'Change History' section here; it's handled in the writeChangeHistorySection template. -->
				<xsl:when test="ddue:title = 'Change History'" />

				<xsl:when test="$total = 0">
					<xsl:variable name="sectionCount">
						<xsl:value-of select="generate-id(.)"/>
					</xsl:variable>

					<xsl:call-template name="t_putSection">
						<xsl:with-param name="p_title">
							<xsl:apply-templates select="ddue:title"
																	 mode="section"/>
						</xsl:with-param>
						<xsl:with-param name="p_content">
							<xsl:apply-templates select="ddue:content"/>
							<xsl:apply-templates select="ddue:sections" />
						</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<xsl:when test="$total = 1">
					<h3 class="subHeading">
						<xsl:apply-templates select="ddue:title"
																 mode="section"/>
					</h3>
					<div class="subsection">
						<xsl:apply-templates select="ddue:content"/>
						<xsl:apply-templates select="ddue:sections" />
					</div>
				</xsl:when>
				<xsl:otherwise>
					<h4 class="subHeading">
						<xsl:apply-templates select="ddue:title"
																 mode="section"/>
					</h4>
					<div class="subsection">
						<xsl:apply-templates select="ddue:content"/>
						<xsl:apply-templates select="ddue:sections" />
					</div>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:title"
								mode="section">
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match="@address">
		<a id="{string(.)}" />
	</xsl:template>

	<xsl:template match="ddue:mediaLink">
		<div>
			<xsl:choose>
				<xsl:when test="ddue:image[@placement='center']">
					<xsl:attribute name="class">mediaCenter</xsl:attribute>
				</xsl:when>
				<xsl:when test="ddue:image[@placement='far']">
					<xsl:attribute name="class">mediaFar</xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">mediaNear</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:if test="ddue:caption and not(ddue:caption[@placement='after'])">
				<div class="caption">
					<xsl:if test="ddue:caption[@lead]">
						<span class="captionLead">
							<xsl:value-of select="normalize-space(ddue:caption/@lead)" />:
						</span>
					</xsl:if>
					<xsl:apply-templates select="ddue:caption" />
				</div>
			</xsl:if>
			<artLink target="{ddue:image/@xlink:href}" />
			<xsl:if test="ddue:caption and ddue:caption[@placement='after']">
				<div class="caption">
					<xsl:if test="ddue:caption[@lead]">
						<span class="captionLead">
							<xsl:value-of select="normalize-space(ddue:caption/@lead)" />:
						</span>
					</xsl:if>
					<xsl:apply-templates select="ddue:caption" />
				</div>
			</xsl:if>
		</div>
	</xsl:template>

	<xsl:template match="ddue:mediaLinkInline">
		<span class="media">
			<artLink target="{ddue:image/@xlink:href}" />
		</span>
	</xsl:template>

	<xsl:template match="ddue:procedure">
		<xsl:apply-templates select="@address" />
		<xsl:if test="normalize-space(ddue:title)">
			<h3 class="procedureSubHeading">
				<xsl:value-of select="ddue:title"/>
			</h3>
		</xsl:if>
		<div class="subSection">
			<xsl:apply-templates select="ddue:steps"/>
			<xsl:apply-templates select="ddue:conclusion"/>
		</div>
	</xsl:template>

	<xsl:template match="ddue:steps">
		<xsl:choose>
			<xsl:when test="@class = 'ordered'">
				<xsl:variable name="temp">
					<xsl:value-of select="count(ddue:step)"/>
				</xsl:variable>
				<xsl:if test="$temp = 1">
					<ul>
						<xsl:apply-templates select="ddue:step"/>
					</ul>
				</xsl:if>
				<xsl:if test="$temp &gt; 1">
					<ol>
						<xsl:apply-templates select="ddue:step"/>
					</ol>
				</xsl:if>
			</xsl:when>
			<xsl:when test="@class='bullet'">
				<ul>
					<xsl:apply-templates select="ddue:step" />
				</ul>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:step">
		<li>
			<xsl:apply-templates select="@address" />
			<xsl:apply-templates />
		</li>
	</xsl:template>


	<xsl:template match="ddue:inThisSection">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="p_titleInclude"
											select="'inThisSectionTitle'" />
			<xsl:with-param name="p_content">
				<xsl:apply-templates />
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="ddue:buildInstructions">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'buildInstructionsTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:nextSteps">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'nextStepsTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:requirements">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'requirementsTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- inline elements -->

	<xsl:template match="ddue:languageKeyword">
		<xsl:variable name="word"
									select="." />
		<span sdata="langKeyword"
					value="{$word}">
			<xsl:choose>
				<!-- mref topics get special handling for keywords like null, etc. -->
				<xsl:when test="/document/reference/apidata">
					<span class="keyword">
						<xsl:choose>
							<xsl:when test="$word='null'">
								<span class="languageSpecificText">
									<span class="cs">null</span>
									<span class="vb">Nothing</span>
									<span class="cpp">nullptr</span>
									<span class="fs">unit</span>
								</span>
							</xsl:when>
							<!-- need to comment out special handling for static, virtual, true, and false 
                 until UE teams review authored content to make sure the auto-text works with the authored text.
                 For example, auto-text with authored content like the following will result in bad customer experience. 
                   <languageKeyword>static</languageKeyword> (<languageKeyword>Shared</languageKeyword> in Visual Basic)
                  
                 This also needs to have F# added should it be uncommented. 
                   -->
							<!--
            <xsl:when test="$word='static' or $word='Shared'">
              <span class="cs">static</span>
              <span class="vb">Shared</span>
              <span class="cpp">static</span>
            </xsl:when>
            <xsl:when test="$word='virtual' or $word='Overridable'">
              <span class="cs">virtual</span>
              <span class="vb">Overridable</span>
              <span class="cpp">virtual</span>
            </xsl:when>
            <xsl:when test="$word='true' or $word='True'">
              <span class="cs">true</span>
              <span class="vb">True</span>
              <span class="cpp">true</span>
            </xsl:when>
            <xsl:when test="$word='false' or $word='False'">
              <span class="cs">false</span>
              <span class="vb">False</span>
              <span class="cpp">false</span>
            </xsl:when>
            -->
							<xsl:otherwise>
								<xsl:value-of select="." />
							</xsl:otherwise>
						</xsl:choose>
					</span>
					<xsl:choose>
						<xsl:when test="$word='null'">
							<span class="languageSpecificText">
								<span class="nu">
									<include item="nullKeyword"/>
								</span>
							</span>
						</xsl:when>
						<!-- need to comment out special handling for static, virtual, true, and false: see note above  -->
						<!--
          <xsl:when test="$word='static' or $word='Shared'">
            <span class="nu"><include item="staticKeyword"/></span>
          </xsl:when>
          <xsl:when test="$word='virtual' or $word='Overridable'">
            <span class="nu"><include item="virtualKeyword"/></span>
          </xsl:when>
          <xsl:when test="$word='true' or $word='True'">
            <span class="nu"><include item="trueKeyword"/></span>
          </xsl:when>
          <xsl:when test="$word='false' or $word='False'">
            <span class="nu"><include item="falseKeyword"/></span>
          </xsl:when>
          -->
					</xsl:choose>
				</xsl:when>
				<!-- conceptual and other non-mref topics do not get special handling for keywords like null, etc. -->
				<xsl:otherwise>
					<span class="code">
						<xsl:value-of select="." />
					</span>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>

	<!-- links -->

	<xsl:template match="ddue:externalLink">
		<a class="mtps-external-link">
			<xsl:attribute name="href">
				<xsl:value-of select="normalize-space(ddue:linkUri)" />
			</xsl:attribute>
			<xsl:if test="normalize-space(ddue:linkAlternateText)">
				<xsl:attribute name="title">
					<xsl:value-of select="normalize-space(ddue:linkAlternateText)" />
				</xsl:attribute>
			</xsl:if>
			<xsl:attribute name="target">
				<xsl:choose>
					<xsl:when test="normalize-space(ddue:linkTarget)">
						<xsl:value-of select="normalize-space(ddue:linkTarget)" />
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>_blank</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:value-of select="normalize-space(ddue:linkText)" />
		</a>
	</xsl:template>

	<xsl:template match="ddue:link">
		<span sdata="link">
			<xsl:choose>
				<xsl:when test="starts-with(@xlink:href,'#')">
					<!-- in-page link -->
					<a href="{@xlink:href}">
						<xsl:apply-templates />
					</a>
				</xsl:when>
				<xsl:otherwise>
					<!-- verified, external link -->
					<conceptualLink class="mtps-internal-link"
													target="{@xlink:href}">
						<xsl:apply-templates />
					</conceptualLink>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>

	<xsl:template match="ddue:codeEntityReference">
		<span sdata="cer"
					target="{normalize-space(string(.))}">
			<referenceLink class="mtps-internal-link"
										 target="{normalize-space(string(.))}">
				<xsl:if test="@qualifyHint">
					<xsl:attribute name="show-container">
						<xsl:value-of select="@qualifyHint" />
					</xsl:attribute>
					<xsl:attribute name="show-parameters">
						<xsl:value-of select="@qualifyHint" />
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="@autoUpgrade">
					<xsl:attribute name="prefer-overload">
						<xsl:value-of select="@autoUpgrade" />
					</xsl:attribute>
				</xsl:if>
			</referenceLink>
		</span>
	</xsl:template>

	<!-- -->

	<xsl:template match="ddue:codeFeaturedElement">
		<xsl:if test="normalize-space(.)">
			<span class="label">
				<xsl:apply-templates/>
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:languageReferenceRemarks">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'remarksTitle'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:attributesandElements">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'attributesAndElements'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:attributes">
		<xsl:if test="normalize-space(.)">
			<h4 class="subHeading">
				<include item="attributes"/>
			</h4>
			<xsl:apply-templates/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:attribute">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:attribute/ddue:title">
		<h4 class="subHeading">
			<xsl:apply-templates/>
		</h4>
	</xsl:template>

	<xsl:template match="ddue:childElement">
		<xsl:if test="normalize-space(.)">
			<h4 class="subHeading">
				<include item="childElement"/>
			</h4>
			<xsl:apply-templates/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:parentElement">
		<xsl:if test="normalize-space(.)">
			<h4 class="subHeading">
				<include item="parentElement"/>
			</h4>
			<xsl:apply-templates/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:textValue">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'textValue'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:elementInformation">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'elementInformation'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:dotNetFrameworkEquivalent">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'dotNetFrameworkEquivalent'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:prerequisites">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'prerequisites'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:type">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:robustProgramming">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'robustProgramming'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:security">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'securitySection'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:externalResources">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'externalResources'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:demonstrates">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'demonstrates'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:appliesTo">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'appliesTo'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:conclusion">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:background">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'background'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:whatsNew">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'whatsNew'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:reference">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'reference'" />
				<xsl:with-param name="p_content">
					<xsl:apply-templates />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:developerErrorMessageDocument">
		<xsl:for-each select="*">
			<xsl:choose>
				<xsl:when test="name() = 'secondaryErrorTitle'">
					<xsl:if test="not(../ddue:nonLocErrorTitle)">
						<xsl:apply-templates select=".">
							<xsl:with-param name="newSection">yes</xsl:with-param>
						</xsl:apply-templates>
					</xsl:if>
				</xsl:when>

				<xsl:otherwise>
					<xsl:apply-templates select="." />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>

	</xsl:template>

	<xsl:template match="ddue:nonLocErrorTitle">
		<xsl:if test="string-length(../ddue:nonLocErrorTitle[normalize-space(.)]) > 0 or string-length(../ddue:secondaryErrorTitle[normalize-space(.)]) > 0">
			<div id="errorTitleSection"
					 class="section">
				<xsl:if test="../ddue:secondaryErrorTitle">
					<h4 class="subHeading">
						<include item="errorMessage"/>
					</h4>
					<xsl:apply-templates select="../ddue:secondaryErrorTitle">
						<xsl:with-param name="newSection">no</xsl:with-param>
					</xsl:apply-templates>
				</xsl:if>
				<xsl:apply-templates/>
				<p/>
			</div>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:secondaryErrorTitle">
		<xsl:param name="newSection"/>
		<xsl:if test="string-length(../ddue:secondaryErrorTitle[normalize-space(.)]) > 0">
			<xsl:choose>
				<xsl:when test="$newSection = 'yes'">
					<div id="errorTitleSection"
							 class="section">
						<xsl:apply-templates/>
						<p/>
					</div>
				</xsl:when>
				<xsl:otherwise>
					<xsl:apply-templates/>
					<br/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:snippets">
		<xsl:if test="ddue:snippet">
			<xsl:element name="div">
				<xsl:attribute name="id">
					<xsl:value-of select="concat('snippetGroup_',generate-id())"/>
				</xsl:attribute>
				<xsl:for-each select="ddue:snippet">
					<xsl:call-template name="t_putCodeSection" />
				</xsl:for-each>
			</xsl:element>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:developerSampleDocument">
		<!-- show the topic intro -->
		<xsl:apply-templates select="ddue:introduction"/>

		<!-- the sample download list section from dsSample -->
		<xsl:if test="ddue:relatedTopics/ddue:sampleRef">
			<include item="{ddue:relatedTopics/ddue:sampleRef/@srcID}"/>
		</xsl:if>

		<!-- then the rest of the topic's content -->
		<xsl:for-each select="*">
			<xsl:choose>
				<!-- introduction was already captured above -->
				<xsl:when test="name() = 'introduction'"/>

				<xsl:otherwise>
					<xsl:apply-templates select="." />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>

	</xsl:template>

	<xsl:template name="hostProtectionContent">
		<!-- HostProtectionAttribute boilerplate -->
		<div class="alert">
			<table width="100%"
						 cellspacing="0"
						 cellpadding="0">
				<tr>
					<th align="left">
						<img class="note">
							<includeAttribute name="alt"
																item="noteAltText" />
							<includeAttribute name="title"
																item="noteAltText" />
							<includeAttribute item="iconPath"
																name="src">
								<parameter>alert_note.gif</parameter>
							</includeAttribute>
						</img>
						<include item="noteTitle" />
					</th>
				</tr>
				<tr>
					<td>
						<p>
							<include item="hostProtectionAttributeLong">
								<parameter>
									<xsl:value-of select="concat($g_apiTopicSubGroup, 'Lower')"/>
								</parameter>
								<parameter>
									<span class="label">
										<xsl:for-each select="/document/reference/attributes/attribute[type[@api='T:System.Security.Permissions.HostProtectionAttribute']]/assignment">
											<xsl:value-of select="@name"/>
											<xsl:if test="position() != last()">
												<xsl:text> | </xsl:text>
											</xsl:if>
										</xsl:for-each>
									</span>
								</parameter>
							</include>
						</p>
					</td>
				</tr>
			</table>
		</div>
	</xsl:template>

	<!-- Display a date to show when the topic was last updated. -->
	<xsl:template name="writeFreshnessDate">
		<!-- The $ChangedHistoryDate param is from the authored changeHistory table, if any. -->
		<xsl:param name="ChangedHistoryDate" />
		<!-- Determine whether the authored date is a valid date string.  -->
		<xsl:variable name="validChangeHistoryDate">
			<xsl:choose>
				<xsl:when test="normalize-space($ChangedHistoryDate)=''"/>
				<xsl:when test="ddue:IsValidDate(normalize-space($ChangedHistoryDate)) = 'true'">
					<xsl:value-of select="normalize-space($ChangedHistoryDate)"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<!-- display nothing if the 'changeHistoryOptions' argument is set to 'omit' -->
			<xsl:when test="$changeHistoryOptions = 'omit'"/>

			<!-- if it's a valid date, display the freshness line. -->
			<xsl:when test="normalize-space($validChangeHistoryDate)">
				<p>
					<include item="UpdateTitle">
						<parameter>
							<xsl:value-of select="normalize-space($validChangeHistoryDate)"/>
						</parameter>
					</include>
				</p>
			</xsl:when>

			<!-- use a default date if no ChangedHistoryDate and the 'changeHistoryOptions' argument is set to 'showDefaultFreshnessDate' -->
			<xsl:when test="$changeHistoryOptions = 'showDefaultFreshnessDate'">
				<p>
					<include item="UpdateTitle">
						<parameter>
							<include item="defaultFreshnessDate"/>
						</parameter>
					</include>
				</p>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="writeChangeHistorySection">
		<xsl:if test="$changeHistoryOptions!='omit'">
			<!-- conceptual authored content is in /document/topic/*; mref content is in /document/comments/ddue:dduexml. -->
			<xsl:for-each select="/document/comments/ddue:dduexml | /document/topic/*">
				<!-- Get the change history section content, which can be in changeHistory or a section with title='Change History'. -->
				<xsl:variable name="changeHistoryContent">
					<xsl:choose>
						<xsl:when test="ddue:changeHistory/ddue:content/ddue:table/ddue:row/ddue:entry[normalize-space(.)]">
							<xsl:apply-templates select="ddue:changeHistory/ddue:content"/>
						</xsl:when>
						<xsl:when test=".//ddue:section[ddue:title = 'Change History']/ddue:content/ddue:table/ddue:row/ddue:entry[normalize-space(.)]">
							<xsl:apply-templates select=".//ddue:section[ddue:title = 'Change History']/ddue:content"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:if test="normalize-space($changeHistoryContent)">
					<xsl:call-template name="t_putSectionInclude">
						<xsl:with-param name="p_titleInclude"
														select="'changeHistory'" />
						<xsl:with-param name="p_content">
							<xsl:copy-of select="$changeHistoryContent" />
						</xsl:with-param>
					</xsl:call-template>
				</xsl:if>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:span">
		<xsl:choose>
			<!-- Process the markup added by MTMarkup tool -->
			<xsl:when test="@class='tgtSentence' or @class='srcSentence'">
				<span>
					<xsl:copy-of select="@*" />
					<xsl:apply-templates />
				</span>
			</xsl:when>
			<!-- fix bug 361746 - use copy-of, so that span class="keyword", "literal" and "comment" 
            nodes are copied to preserve code colorization in snippets -->
			<xsl:when test="@class='keyword' or @class='identifier' or @class='literal' or @class='parameter' or @class='typeparameter' or @class='comment'">
				<xsl:copy-of select="."/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!-- Don't render the changeHistory section here; it's handled in the writeChangeHistorySection template. -->
	<xsl:template match="ddue:changeHistory"/>

	<xsl:template name="WriteRemarksSection">
		<xsl:param name="node" />

		<xsl:variable name="hasRemarks">
			<xsl:call-template name="HasRemarksContent">
				<xsl:with-param name="node"
												select="$node" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$hasRemarks='true'">
			<xsl:choose>
				<xsl:when test="not($g_apiTopicGroup = 'namespace')">
					<xsl:call-template name="t_putSectionInclude">
						<xsl:with-param name="p_titleInclude"
														select="'remarksTitle'" />
						<xsl:with-param name="p_content">
							<xsl:apply-templates select="$node/ddue:remarks/*" />
							<!-- HostProtectionAttribute -->
							<xsl:if test="/document/reference/attributes/attribute/type[@api='T:System.Security.Permissions.HostProtectionAttribute']">
								<xsl:call-template name="hostProtectionContent" />
							</xsl:if>
							<xsl:apply-templates select="$node/ddue:notesForImplementers"/>
							<xsl:apply-templates select="$node/ddue:notesForCallers"/>
							<xsl:apply-templates select="$node/ddue:notesForInheritors"/>
							<xsl:apply-templates select="$node/ddue:platformNotes"/>
							<include item="mshelpKTable">
								<parameter>
									<xsl:text>tt_</xsl:text>
									<xsl:value-of select="$key"/>
								</parameter>
							</include>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:apply-templates select="$node/ddue:remarks/*" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>
