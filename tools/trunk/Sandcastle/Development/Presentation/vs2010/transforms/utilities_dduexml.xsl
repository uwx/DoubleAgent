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

	<xsl:import href="globalTemplates.xsl"/>
	<xsl:import href="codeTemplates.xsl"/>
	<xsl:import href="../../shared/transforms/utilities_dduexml.xsl"/>

	<!-- ============================================================================================
	The Remarks section includes content from these nodes, excluding the xaml sections which are captured in the xaml syntax processing
	============================================================================================= -->

	<xsl:template name="t_hasRemarksContent">
		<xsl:param name="p_node"/>
		<xsl:choose>
			<xsl:when test="/document/reference/attributes/attribute/type[@api='T:System.Security.Permissions.HostProtectionAttribute']">true</xsl:when>
			<xsl:when test="normalize-space($p_node/ddue:remarks/ddue:content)">true</xsl:when>
			<xsl:when test="normalize-space($p_node/ddue:notesForImplementers)">true</xsl:when>
			<xsl:when test="normalize-space($p_node/ddue:notesForCallers)">true</xsl:when>
			<xsl:when test="normalize-space($p_node/ddue:notesForInheritors)">true</xsl:when>
			<xsl:when test="normalize-space($p_node/ddue:platformNotes)">true</xsl:when>
			<xsl:when test="normalize-space($p_node/ddue:remarks/ddue:sections/ddue:section[not(
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

	<!-- ============================================================================================
	Remarks
	============================================================================================= -->

	<xsl:template match="ddue:remarks"
								name="t_ddue_Remarks">
		<xsl:call-template name="t_writeRemarksSection">
			<xsl:with-param name="p_node"
											select=".."/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="t_writeRemarksSection">
		<xsl:param name="p_node"/>

		<xsl:variable name="v_hasRemarks">
			<xsl:call-template name="t_hasRemarksContent">
				<xsl:with-param name="p_node"
												select="$p_node"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$v_hasRemarks='true'">
			<xsl:choose>
				<xsl:when test="not($g_apiTopicGroup = 'namespace')">
					<xsl:call-template name="t_putSectionInclude">
						<xsl:with-param name="p_titleInclude"
														select="'remarksTitle'"/>
						<xsl:with-param name="p_content">
							<xsl:apply-templates select="$p_node/ddue:remarks/*"/>
							<!-- HostProtectionAttribute -->
							<xsl:if test="/document/reference/attributes/attribute/type[@api='T:System.Security.Permissions.HostProtectionAttribute']">
								<xsl:call-template name="t_hostProtectionContent"/>
							</xsl:if>
							<xsl:apply-templates select="$p_node/ddue:notesForImplementers"/>
							<xsl:apply-templates select="$p_node/ddue:notesForCallers"/>
							<xsl:apply-templates select="$p_node/ddue:notesForInheritors"/>
							<xsl:apply-templates select="$p_node/ddue:platformNotes"/>
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
					<xsl:apply-templates select="$p_node/ddue:remarks/*"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name="t_hostProtectionContent">
		<!-- HostProtectionAttribute boilerplate -->
		<div class="alert">
			<table>
				<tr>
					<th align="left">
						<img class="note">
							<includeAttribute name="alt"
																item="noteAltText"/>
							<includeAttribute name="title"
																item="noteAltText"/>
							<includeAttribute item="iconPath"
																name="src">
								<parameter>alert_note.gif</parameter>
							</includeAttribute>
						</img>
						<include item="noteTitle"/>
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

	<!-- ============================================================================================
	Sections
	============================================================================================= -->

	<xsl:template match="ddue:sections"
								name="t_ddue_sections">
		<xsl:apply-templates select="ddue:section"/>
	</xsl:template>

	<xsl:template match="ddue:section"
								name="t_ddue_section">
		<!-- display the section only if it has content (text or media)-->
		<xsl:if test="descendant::ddue:content[normalize-space(.)] or descendant::ddue:content/*">

			<xsl:apply-templates select="@address"/>
			<!-- Count all the possible ancestor root nodes -->
			<xsl:variable name="a1"
										select="count(ancestor::ddue:attributesandElements)"/>
			<xsl:variable name="a2"
										select="count(ancestor::ddue:codeExample)"/>
			<xsl:variable name="a3"
										select="count(ancestor::ddue:dotNetFrameworkEquivalent)"/>
			<xsl:variable name="a4"
										select="count(ancestor::ddue:elementInformation)"/>
			<xsl:variable name="a5"
										select="count(ancestor::ddue:exceptions)"/>
			<xsl:variable name="a6"
										select="count(ancestor::ddue:introduction)"/>
			<xsl:variable name="a7"
										select="count(ancestor::ddue:languageReferenceRemarks)"/>
			<xsl:variable name="a8"
										select="count(ancestor::ddue:nextSteps)"/>
			<xsl:variable name="a9"
										select="count(ancestor::ddue:parameters)"/>
			<xsl:variable name="a10"
										select="count(ancestor::ddue:prerequisites)"/>
			<xsl:variable name="a11"
										select="count(ancestor::ddue:procedure)"/>
			<xsl:variable name="a12"
										select="count(ancestor::ddue:relatedTopics)"/>
			<xsl:variable name="a13"
										select="count(ancestor::ddue:remarks)"/>
			<xsl:variable name="a14"
										select="count(ancestor::ddue:requirements)"/>
			<xsl:variable name="a15"
										select="count(ancestor::ddue:schemaHierarchy)"/>
			<xsl:variable name="a16"
										select="count(ancestor::ddue:syntaxSection)"/>
			<xsl:variable name="a17"
										select="count(ancestor::ddue:textValue)"/>
			<xsl:variable name="a18"
										select="count(ancestor::ddue:type)"/>
			<xsl:variable name="a19"
										select="count(ancestor::ddue:section)"/>
			<xsl:variable name="total"
										select="$a1+$a2+$a3+$a4+$a5+$a6+$a7+$a8+$a9+$a10+$a11+$a12+$a13+$a14+$a15+$a16+$a17+$a18+$a19"/>
			<xsl:choose>
				<!-- Don't render the 'Change History' section here; it's handled in the t_writeChangeHistorySection template. -->
				<xsl:when test="ddue:title = 'Change History'"/>

				<xsl:when test="$total = 0">
					<xsl:call-template name="t_putSection">
						<xsl:with-param name="p_title">
							<xsl:apply-templates select="ddue:title"
																	 mode="section"/>
						</xsl:with-param>
						<xsl:with-param name="p_content">
							<xsl:apply-templates select="ddue:content"/>
							<xsl:apply-templates select="ddue:sections"/>
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
						<xsl:apply-templates select="ddue:sections"/>
					</div>
				</xsl:when>
				<xsl:otherwise>
					<h4 class="subHeading">
						<xsl:apply-templates select="ddue:title"
																 mode="section"/>
					</h4>
					<div class="subsection">
						<xsl:apply-templates select="ddue:content"/>
						<xsl:apply-templates select="ddue:sections"/>
					</div>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:title"
								mode="section"
								name="t_ddue_sectionTitle">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="@address"
								name="t_ddue_address">
		<a id="{string(.)}"/>
	</xsl:template>

	<!-- ============================================================================================
	Block Elements
	============================================================================================= -->

	<xsl:template name="t_threadSafety">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="p_titleInclude"
											select="'threadSafetyTitle'"/>
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

	<xsl:template match="ddue:notesForImplementers"
								name="t_ddue_notesForImplementers">
		<p/>
		<span class="label">
			<include item="NotesForImplementers"/>
		</span>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:notesForCallers"
								name="t_ddue_notesForCallers">
		<p/>
		<span class="label">
			<include item="NotesForCallers"/>
		</span>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:notesForInheritors"
								name="t_ddue_notesForInheritors">
		<p/>
		<span class="label">
			<include item="NotesForInheritors"/>
		</span>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:platformNotes"
								name="t_ddue_platformNotes">
		<xsl:for-each select="ddue:platformNote[normalize-space(ddue:content)]">
			<p>
				<include item="PlatformNote">
					<parameter>
						<xsl:for-each select="ddue:platforms/ddue:platform">
							<xsl:variable name="v_platformName">
								<xsl:value-of select="."/>
							</xsl:variable>
							<include item="{$v_platformName}"/>
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

	<xsl:template match="ddue:platformNotes/ddue:platformNote/ddue:content/ddue:para"
								name="t_ddue_platformNote_para">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:schemaHierarchy"
								name="t_ddue_schemaHierarchy">
		<xsl:for-each select="ddue:link">
			<xsl:call-template name="t_putIndent">
				<xsl:with-param name="p_count"
												select="position()"/>
			</xsl:call-template>
			<xsl:apply-templates select="."/>
			<br/>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="ddue:syntaxSection"
								name="t_ddue_syntaxSection">
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
						<table>
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

	<!-- just pass these through -->
	<xsl:template match="ddue:content"
								name="t_ddue_content">
		<xsl:apply-templates/>
	</xsl:template>
	<xsl:template match="ddue:legacy"
								name="t_ddue_legacy">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:procedure"
								name="t_ddue_procedure">
		<xsl:apply-templates select="@address"/>
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

	<xsl:template match="ddue:steps"
								name="t_ddue_steps">
		<xsl:choose>
			<xsl:when test="@class = 'ordered'">
				<xsl:variable name="v_temp">
					<xsl:value-of select="count(ddue:step)"/>
				</xsl:variable>
				<xsl:if test="$v_temp = 1">
					<ul>
						<xsl:apply-templates select="ddue:step"/>
					</ul>
				</xsl:if>
				<xsl:if test="$v_temp &gt; 1">
					<ol>
						<xsl:apply-templates select="ddue:step"/>
					</ol>
				</xsl:if>
			</xsl:when>
			<xsl:when test="@class='bullet'">
				<ul>
					<xsl:apply-templates select="ddue:step"/>
				</ul>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:step"
								name="t_ddue_step">
		<li>
			<xsl:apply-templates select="@address"/>
			<xsl:apply-templates/>
		</li>
	</xsl:template>


	<xsl:template match="ddue:inThisSection"
								name="t_ddue_inThisSection">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="p_titleInclude"
											select="'inThisSectionTitle'"/>
			<xsl:with-param name="p_content">
				<xsl:apply-templates/>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="ddue:buildInstructions"
								name="t_ddue_buildInstructions">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'buildInstructionsTitle'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:nextSteps"
								name="t_ddue_nextSteps">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'nextStepsTitle'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:requirements"
								name="t_ddue_requirements">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'requirementsTitle'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:languageReferenceRemarks"
								name="t_ddue_languageReferenceRemarks">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'remarksTitle'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:attributesandElements"
								name="t_ddue_attributesandElements">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'attributesAndElements'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:attributes"
								name="t_ddue_attributes">
		<xsl:if test="normalize-space(.)">
			<h4 class="subHeading">
				<include item="attributes"/>
			</h4>
			<xsl:apply-templates/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:attribute"
								name="t_ddue_attribute">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:attribute/ddue:title"
								name="t_ddue_attributeTitle">
		<h4 class="subHeading">
			<xsl:apply-templates/>
		</h4>
	</xsl:template>

	<xsl:template match="ddue:childElement"
								name="t_ddue_childElement">
		<xsl:if test="normalize-space(.)">
			<h4 class="subHeading">
				<include item="childElement"/>
			</h4>
			<xsl:apply-templates/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:parentElement"
								name="t_ddue_parentElement">
		<xsl:if test="normalize-space(.)">
			<h4 class="subHeading">
				<include item="parentElement"/>
			</h4>
			<xsl:apply-templates/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:textValue"
								name="t_ddue_textValue">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'textValue'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:elementInformation"
								name="t_ddue_elementInformation">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'elementInformation'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:dotNetFrameworkEquivalent"
								name="t_ddue_dotNetFrameworkEquivalent">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'dotNetFrameworkEquivalent'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:prerequisites"
								name="t_ddue_prerequisites">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'prerequisites'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:type"
								name="t_ddue_type">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:robustProgramming"
								name="t_ddue_robustProgramming">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'robustProgramming'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:security"
								name="t_ddue_security">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'securitySection'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:externalResources"
								name="t_ddue_externalResources">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'externalResources'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:demonstrates"
								name="t_ddue_demonstrates">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'demonstrates'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:appliesTo"
								name="t_ddue_appliesTo">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'appliesTo'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:conclusion"
								name="t_ddue_conclusion">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:background"
								name="t_ddue_background">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'background'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:whatsNew"
								name="t_ddue_whatsNew">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'whatsNew'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:reference"
								name="t_ddue_reference">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'reference'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:developerErrorMessageDocument"
								name="t_ddue_developerErrorMessageDocument">
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
					<xsl:apply-templates select="."/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>

	</xsl:template>

	<xsl:template match="ddue:nonLocErrorTitle"
								name="t_ddue_nonLocErrorTitle">
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

	<xsl:template match="ddue:secondaryErrorTitle"
								name="t_ddue_secondaryErrorTitle">
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

	<xsl:template match="ddue:developerSampleDocument"
								name="t_ddue_developerSampleDocument">
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
					<xsl:apply-templates select="."/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>

	</xsl:template>

	<!-- ============================================================================================
	Lists and Tables
	============================================================================================= -->

	<xsl:template match="ddue:list"
								name="t_ddue_list">
		<xsl:choose>
			<xsl:when test="@class='bullet'">
				<ul>
					<xsl:apply-templates select="ddue:listItem | ddue:list"/>
				</ul>
			</xsl:when>
			<xsl:when test="@class='ordered'">
				<ol>
					<xsl:if test="@start">
						<xsl:attribute name="start">
							<xsl:value-of select="@start"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:apply-templates select="ddue:listItem | ddue:list"/>
				</ol>
			</xsl:when>
			<xsl:otherwise>
				<ul style="list-style-type:none;">
					<xsl:apply-templates select="ddue:listItem | ddue:list"/>
				</ul>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:table"
								name="t_ddue_table">
		<div class="tableSection">
			<xsl:if test="normalize-space(ddue:title)">
				<div class="caption">
					<xsl:value-of select="ddue:title"/>
				</div>
			</xsl:if>
			<table>
				<xsl:apply-templates/>
			</table>
		</div>
	</xsl:template>

	<xsl:template match="ddue:tableHeader"
								name="t_ddue_tableHeader">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ddue:row"
								name="t_ddue_row">
		<tr>
			<xsl:apply-templates/>
		</tr>
	</xsl:template>

	<xsl:template match="ddue:entry"
								name="t_ddue_entry">
		<td>
			<xsl:apply-templates select="@address"/>
			<xsl:apply-templates/>
		</td>
	</xsl:template>

	<xsl:template match="ddue:tableHeader/ddue:row/ddue:entry"
								name="t_ddue_tableHeaderRowEntry">
		<th>
			<xsl:apply-templates/>
		</th>
	</xsl:template>

	<xsl:template match="ddue:definitionTable"
								name="t_ddue_definitionTable">
		<dl class="authored">
			<xsl:apply-templates/>
		</dl>
	</xsl:template>

	<xsl:template match="ddue:definedTerm"
								name="t_ddue_definedTerm">
		<dt>
			<xsl:apply-templates select="@address"/>
			<xsl:apply-templates/>
		</dt>
	</xsl:template>

	<xsl:template match="ddue:definition"
								name="t_ddue_definition">
		<dd>
			<xsl:apply-templates/>
		</dd>
	</xsl:template>

	<!-- ============================================================================================
	Code
	============================================================================================= -->

	<xsl:template match="ddue:snippets"
								name="t_ddue_snippets">
		<xsl:if test="ddue:snippet">
			<xsl:element name="div">
				<xsl:attribute name="id">
					<xsl:value-of select="concat('snippetGroup_',generate-id())"/>
				</xsl:attribute>
				<xsl:for-each select="ddue:snippet">
					<xsl:call-template name="t_putCodeSection"/>
				</xsl:for-each>
			</xsl:element>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:code"
								name="t_ddue_code">
		<xsl:variable name="v_currentId">
			<xsl:value-of select="generate-id(.)"/>
		</xsl:variable>
		<xsl:variable name="v_prevPosition">
			<xsl:for-each select="parent::*/child::*">
				<xsl:if test="generate-id(.)=$v_currentId">
					<xsl:number value="position()-1"/>
				</xsl:if>
			</xsl:for-each>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="name(parent::*/child::*[position()=$v_prevPosition])=name()">
			</xsl:when>
			<xsl:when test="following-sibling::*[1]/self::ddue:code">
				<xsl:variable name="v_codeNodes"
											select=". | following-sibling::ddue:code[not(preceding-sibling::*[generate-id(.)=generate-id((current()/following-sibling::*[not(self::ddue:code)])[1])])]"/>
				<xsl:call-template name="t_putCodeSections">
					<xsl:with-param name="p_codeNodes"
													select="$v_codeNodes"/>
					<xsl:with-param name="p_nodeCount"
													select="count($v_codeNodes)"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="t_putCodeSection"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:sampleCode"
								name="t_ddue_sampleCode">
		<div>
			<span class="label">
				<xsl:value-of select="@language"/>
			</span>
		</div>
		<div class="code">
			<pre xml:space="preserve"><xsl:apply-templates/></pre>
		</div>
	</xsl:template>

	<xsl:template match="ddue:codeExamples"
								name="t_ddue_codeExamples">
		<xsl:if test="normalize-space(.)">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'examplesTitle'"/>
				<xsl:with-param name="p_content">
					<xsl:apply-templates/>
					<xsl:call-template name="t_moreCodeSection"/>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
  tasks/task nodes are inserted by TaskGrabberComponent which gets content from HowTo topics
	these nodes are handled below in the t_moreCodeSection
	============================================================================================= -->
	<xsl:template match="ddue:codeExamples/ddue:codeExample/ddue:legacy/ddue:content/tasks"/>

	<xsl:template name="t_moreCodeSection">
		<xsl:variable name="v_gotCodeAlready"
									select="boolean(
													(ddue:codeExample/ddue:legacy/ddue:content[ddue:codeReference[ddue:sampleCode] | ddue:code | ddue:snippets/ddue:snippet]) or
													(ddue:codeExample[ddue:codeReference[ddue:sampleCode] | ddue:code | ddue:snippets/ddue:snippet])
													)"/>

		<xsl:variable name="v_gotMoreCode"
									select="(count(ddue:codeExample/ddue:legacy/ddue:content/tasks/task)&gt;1) or 
                           ($v_gotCodeAlready and count(ddue:codeExample/ddue:legacy/ddue:content/tasks/task)&gt;0)"/>

		<!-- if no preceding code in the code examples section, display the tasks[1]/task[1] -->
		<xsl:if test="not($v_gotCodeAlready)">
			<xsl:for-each select="ddue:codeExample/ddue:legacy/ddue:content/tasks[1]/task[1]">
				<xsl:apply-templates select="ddue:introduction | ddue:codeExample"/>
			</xsl:for-each>
		</xsl:if>

		<xsl:if test="$v_gotMoreCode">
			<sections>
				<h4 class="subHeading">
					<include item="mrefTaskMoreCodeHeading"/>
				</h4>
				<div class="subsection">
					<div class="tableSection">
						<table>
							<xsl:for-each select="ddue:codeExample/ddue:legacy/ddue:content/tasks/task">
								<xsl:choose>
									<xsl:when test="not($v_gotCodeAlready) and position()=1"/>
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

	<!-- ============================================================================================
	Alerts
	============================================================================================= -->

	<xsl:template match="ddue:alert"
								name="t_ddue_alert">
		<xsl:variable name="v_title">
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
		<xsl:variable name="v_altTitle">
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
		<xsl:variable name="v_noteImg">
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
									<xsl:value-of select="$v_noteImg"/>
								</parameter>
							</includeAttribute>
							<includeAttribute name="alt"
																item="{$v_altTitle}"/>
						</img>
						<xsl:text> </xsl:text>
						<include item="{$v_title}"/>
					</th>
				</tr>
				<tr>
					<td>
						<xsl:apply-templates/>
					</td>
				</tr>
			</table>
		</div>
	</xsl:template>

	<!-- ============================================================================================
	Media
	============================================================================================= -->

	<xsl:template match="ddue:mediaLink" name="t_ddue_mediaLink">
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
							<xsl:value-of select="normalize-space(ddue:caption/@lead)"/>:
						</span>
					</xsl:if>
					<xsl:apply-templates select="ddue:caption"/>
				</div>
			</xsl:if>
			<artLink target="{ddue:image/@xlink:href}"/>
			<xsl:if test="ddue:caption and ddue:caption[@placement='after']">
				<div class="caption">
					<xsl:if test="ddue:caption[@lead]">
						<span class="captionLead">
							<xsl:value-of select="normalize-space(ddue:caption/@lead)"/>:
						</span>
					</xsl:if>
					<xsl:apply-templates select="ddue:caption"/>
				</div>
			</xsl:if>
		</div>
	</xsl:template>

	<xsl:template match="ddue:mediaLinkInline" name="t_ddue_mediaLinkInline">
		<span class="media">
			<artLink target="{ddue:image/@xlink:href}"/>
		</span>
	</xsl:template>

	<!-- ============================================================================================
	Inline elements
	============================================================================================= -->

	<xsl:template match="ddue:span" name="t_ddue_span">
		<xsl:choose>
			<!-- Process the markup added by MTMarkup tool -->
			<xsl:when test="@class='tgtSentence' or @class='srcSentence'">
				<span>
					<xsl:copy-of select="@*"/>
					<xsl:apply-templates/>
				</span>
			</xsl:when>
			<!-- fix bug 361746 - use copy-of, so that span class="keyword", "literal" and "comment" 
            nodes are copied to preserve code colorization in snippets -->
			<xsl:when test="@class='keyword' or @class='identifier' or @class='literal' or @class='parameter' or @class='typeparameter' or @class='comment'">
				<xsl:copy-of select="."/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="ddue:languageKeyword" name="t_ddue_languageKeyword">
		<xsl:variable name="v_keyword"
									select="."/>
		<span sdata="langKeyword"
					value="{$v_keyword}">
			<xsl:choose>
				<!-- mref topics get special handling for keywords like null, etc. -->
				<xsl:when test="/document/reference/apidata">
					<span class="keyword">
						<xsl:choose>
							<xsl:when test="$v_keyword='null'">
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
            <xsl:when test="$v_keyword='static' or $v_keyword='Shared'">
              <span class="cs">static</span>
              <span class="vb">Shared</span>
              <span class="cpp">static</span>
            </xsl:when>
            <xsl:when test="$v_keyword='virtual' or $v_keyword='Overridable'">
              <span class="cs">virtual</span>
              <span class="vb">Overridable</span>
              <span class="cpp">virtual</span>
            </xsl:when>
            <xsl:when test="$v_keyword='true' or $v_keyword='True'">
              <span class="cs">true</span>
              <span class="vb">True</span>
              <span class="cpp">true</span>
            </xsl:when>
            <xsl:when test="$v_keyword='false' or $v_keyword='False'">
              <span class="cs">false</span>
              <span class="vb">False</span>
              <span class="cpp">false</span>
            </xsl:when>
            -->
							<xsl:otherwise>
								<xsl:value-of select="."/>
							</xsl:otherwise>
						</xsl:choose>
					</span>
					<xsl:choose>
						<xsl:when test="$v_keyword='null'">
							<span class="languageSpecificText">
								<span class="nu">
									<include item="nullKeyword"/>
								</span>
							</span>
						</xsl:when>
						<!-- need to comment out special handling for static, virtual, true, and false: see note above  -->
						<!--
          <xsl:when test="$v_keyword='static' or $v_keyword='Shared'">
            <span class="nu"><include item="staticKeyword"/></span>
          </xsl:when>
          <xsl:when test="$v_keyword='virtual' or $v_keyword='Overridable'">
            <span class="nu"><include item="virtualKeyword"/></span>
          </xsl:when>
          <xsl:when test="$v_keyword='true' or $v_keyword='True'">
            <span class="nu"><include item="trueKeyword"/></span>
          </xsl:when>
          <xsl:when test="$v_keyword='false' or $v_keyword='False'">
            <span class="nu"><include item="falseKeyword"/></span>
          </xsl:when>
          -->
					</xsl:choose>
				</xsl:when>
				<!-- conceptual and other non-mref topics do not get special handling for keywords like null, etc. -->
				<xsl:otherwise>
					<span class="code">
						<xsl:value-of select="."/>
					</span>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>

	<xsl:template match="ddue:codeFeaturedElement" name="t_ddue_codeFeaturedElement">
		<xsl:if test="normalize-space(.)">
			<span class="label">
				<xsl:apply-templates/>
			</span>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	Overrides of shared templates
	============================================================================================= -->

	<xsl:template match="ddue:subscript | ddue:subscriptType">
		<xsl:if test="normalize-space(.)">
			<small>
				<small>
					<sub>
						<xsl:apply-templates/>
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
						<xsl:apply-templates/>
					</sup>
				</small>
			</small>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:literal">
		<xsl:if test="normalize-space(.)">
			<span class="literal">
				<xsl:apply-templates/>
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:parameterReference">
		<xsl:if test="normalize-space(.)">
			<span class="parameter">
				<xsl:apply-templates/>
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:replaceable | ddue:placeholder">
		<xsl:if test="normalize-space(.)">
			<span class="placeholder">
				<xsl:apply-templates/>
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:ui">
		<xsl:if test="normalize-space(.)">
			<span class="ui">
				<xsl:apply-templates/>
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:newTerm">
		<xsl:if test="normalize-space(.)">
			<span class="term">
				<xsl:apply-templates/>
			</span>
		</xsl:if>
	</xsl:template>

	<xsl:template match="ddue:errorInline|ddue:fictitiousUri|ddue:localUri">
		<xsl:if test="normalize-space(.)">
			<em>
				<xsl:apply-templates/>
			</em>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	Links
	============================================================================================= -->

	<xsl:template match="ddue:externalLink" name="t_ddue_externalLink">
		<a class="mtps-external-link">
			<xsl:attribute name="href">
				<xsl:value-of select="normalize-space(ddue:linkUri)"/>
			</xsl:attribute>
			<xsl:if test="normalize-space(ddue:linkAlternateText)">
				<xsl:attribute name="title">
					<xsl:value-of select="normalize-space(ddue:linkAlternateText)"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:attribute name="target">
				<xsl:choose>
					<xsl:when test="normalize-space(ddue:linkTarget)">
						<xsl:value-of select="normalize-space(ddue:linkTarget)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>_blank</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
			<xsl:value-of select="normalize-space(ddue:linkText)"/>
		</a>
	</xsl:template>

	<xsl:template match="ddue:link" name="t_ddue_link">
		<span sdata="link">
			<xsl:choose>
				<xsl:when test="starts-with(@xlink:href,'#')">
					<!-- in-page link -->
					<a href="{@xlink:href}">
						<xsl:apply-templates/>
					</a>
				</xsl:when>
				<xsl:otherwise>
					<!-- verified, external link -->
					<conceptualLink class="mtps-internal-link"
													target="{@xlink:href}">
						<xsl:apply-templates/>
					</conceptualLink>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>

	<xsl:template match="ddue:codeEntityReference" name="t_ddue_codeEntityReference">
		<span sdata="cer"
					target="{normalize-space(string(.))}">
			<referenceLink class="mtps-internal-link"
										 target="{normalize-space(string(.))}">
				<xsl:if test="@qualifyHint">
					<xsl:attribute name="show-container">
						<xsl:value-of select="@qualifyHint"/>
					</xsl:attribute>
					<xsl:attribute name="show-parameters">
						<xsl:value-of select="@qualifyHint"/>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="@autoUpgrade">
					<xsl:attribute name="prefer-overload">
						<xsl:value-of select="@autoUpgrade"/>
					</xsl:attribute>
				</xsl:if>
			</referenceLink>
		</span>
	</xsl:template>

	<!-- ============================================================================================
	Change history
	============================================================================================= -->

	<!-- Don't render the changeHistory section here; it's handled in the t_writeChangeHistorySection template. -->
	<xsl:template match="ddue:changeHistory"
								name="t_ddue_changeHistory"/>

	<!-- Display a date to show when the topic was last updated. -->
	<xsl:template name="t_writeFreshnessDate">
		<!-- The $p_changedHistoryDate param is from the authored changeHistory table, if any. -->
		<xsl:param name="p_changedHistoryDate"/>
		<!-- Determine whether the authored date is a valid date string.  -->
		<xsl:variable name="v_validChangeHistoryDate">
			<xsl:choose>
				<xsl:when test="normalize-space($p_changedHistoryDate)=''"/>
				<xsl:when test="ddue:IsValidDate(normalize-space($p_changedHistoryDate)) = 'true'">
					<xsl:value-of select="normalize-space($p_changedHistoryDate)"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<!-- display nothing if the 'changeHistoryOptions' argument is set to 'omit' -->
			<xsl:when test="$changeHistoryOptions = 'omit'"/>

			<!-- if it's a valid date, display the freshness line. -->
			<xsl:when test="normalize-space($v_validChangeHistoryDate)">
				<p>
					<include item="UpdateTitle">
						<parameter>
							<xsl:value-of select="normalize-space($v_validChangeHistoryDate)"/>
						</parameter>
					</include>
				</p>
			</xsl:when>

			<!-- use a default date if no p_changedHistoryDate and the 'changeHistoryOptions' argument is set to 'showDefaultFreshnessDate' -->
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

	<xsl:template name="t_writeChangeHistorySection">
		<xsl:if test="$changeHistoryOptions!='omit'">
			<!-- conceptual authored content is in /document/topic/*; mref content is in /document/comments/ddue:dduexml. -->
			<xsl:for-each select="/document/comments/ddue:dduexml | /document/topic/*">
				<!-- Get the change history section content, which can be in changeHistory or a section with title='Change History'. -->
				<xsl:variable name="v_changeHistoryContent">
					<xsl:choose>
						<xsl:when test="ddue:changeHistory/ddue:content/ddue:table/ddue:row/ddue:entry[normalize-space(.)]">
							<xsl:apply-templates select="ddue:changeHistory/ddue:content"/>
						</xsl:when>
						<xsl:when test=".//ddue:section[ddue:title = 'Change History']/ddue:content/ddue:table/ddue:row/ddue:entry[normalize-space(.)]">
							<xsl:apply-templates select=".//ddue:section[ddue:title = 'Change History']/ddue:content"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:if test="normalize-space($v_changeHistoryContent)">
					<xsl:call-template name="t_putSectionInclude">
						<xsl:with-param name="p_titleInclude"
														select="'changeHistory'"/>
						<xsl:with-param name="p_content">
							<xsl:copy-of select="$v_changeHistoryContent"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:if>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>
