<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								version="1.1"
								xmlns:ddue="http://ddue.schemas.microsoft.com/authoring/2003/5">
	<!-- ======================================================================================== -->

	<xsl:import href="../../shared/transforms/utilities_bibliography.xsl"/>
	<xsl:output method="xml"
							omit-xml-declaration="yes"
							indent="no"
							encoding="utf-8" />

	<!-- ============================================================================================
	Parameters
	============================================================================================= -->

	<xsl:param name="bibliographyData"
						 select="'../Data/bibliography.xml'"/>
	<xsl:param name="omitXmlnsBoilerplate"
						 select="'false'" />
	<xsl:param name="omitVersionInformation"
						 select="'false'" />

	<!-- ============================================================================================
	Includes
	============================================================================================= -->

	<xsl:include href="xpathFunctions.xsl" />
	<xsl:include href="utilities_reference.xsl" />
	<xsl:include href="codeTemplates.xsl"/>
	<xsl:include href="globalTemplates.xsl"/>

	<!-- ============================================================================================
	Global Variables
	============================================================================================= -->

	<!--<xsl:variable name="g_summary"
								select="normalize-space(/document/comments/summary)" />-->
	<xsl:variable name="g_abstractSummary"
								select="/document/comments/summary" />
	<xsl:variable name="g_hasSeeAlsoSection"
								select="boolean((count(/document/comments//seealso | /document/reference/elements/element/overloads//seealso) > 0)  or 
                           ($g_apiTopicGroup='type' or $g_apiTopicGroup='member' or $g_apiTopicGroup='list'))"/>
	<!--<xsl:variable name="g_hasExamplesSection"
								select="boolean(string-length(/document/comments/example[normalize-space(.)]) > 0)"/>-->
	<!--<xsl:variable name="g_hasLanguageFilterSection"
								select="boolean(string-length(/document/comments/example[normalize-space(.)]) > 0)" />-->

	<!-- ============================================================================================
	Body
	============================================================================================= -->

	<xsl:template name="t_body">

		<!-- auto-inserted info -->
		<xsl:apply-templates select="/document/comments/preliminary" />
		<xsl:apply-templates select="/document/comments/summary" />
		<xsl:if test="$g_apiTopicSubGroup='overload'">
			<xsl:apply-templates select="/document/reference/elements"
													 mode="overloadSummary" />
		</xsl:if>

		<!-- inheritance -->
		<xsl:apply-templates select="/document/reference/family" />

		<!-- assembly information -->
		<xsl:if test="not($g_apiTopicGroup='list' or $g_apiTopicGroup='root' or $g_apiTopicGroup='namespace')">
			<xsl:call-template name="t_putRequirementsInfo"/>
		</xsl:if>

		<!-- syntax -->
		<xsl:if test="not($g_apiTopicGroup='list' or $g_apiTopicGroup='namespace')">
			<xsl:apply-templates select="/document/syntax" />
		</xsl:if>

		<!-- members -->
		<xsl:choose>
			<xsl:when test="$g_apiTopicGroup='root'">
				<xsl:apply-templates select="/document/reference/elements"
														 mode="root" />
			</xsl:when>
			<xsl:when test="$g_apiTopicGroup='namespace'">
				<xsl:apply-templates select="/document/reference/elements"
														 mode="namespace" />
			</xsl:when>
			<xsl:when test="$g_apiTopicSubGroup='enumeration'">
				<xsl:apply-templates select="/document/reference/elements"
														 mode="enumeration" />
			</xsl:when>
			<xsl:when test="$g_apiTopicGroup='type'">
				<xsl:apply-templates select="/document/reference/elements"
														 mode="type" />
			</xsl:when>
			<xsl:when test="$g_apiTopicGroup='list'">
				<xsl:choose>
					<xsl:when test="$g_apiTopicSubGroup='overload'">
						<xsl:apply-templates select="/document/reference/elements"
																 mode="overload" />
					</xsl:when>
					<xsl:when test="$g_apiTopicSubGroup='DerivedTypeList'">
						<xsl:apply-templates select="/document/reference/elements"
																 mode="derivedType" />
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="/document/reference/elements"
																 mode="member" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
		</xsl:choose>

		<!-- remarks -->
		<xsl:apply-templates select="/document/comments/remarks" />
		<!-- examples -->
		<xsl:apply-templates select="/document/comments/example" />

		<!-- other comment sections -->
		<!-- exceptions -->
		<xsl:call-template name="exceptions" />
		<!-- permissions -->
		<xsl:call-template name="permissions" />
		<!-- contracts -->
		<xsl:call-template name="contracts" />
		<!--versions-->
		<xsl:if test="not($g_apiTopicGroup='list' or $g_apiTopicGroup='namespace' or $g_apiTopicGroup='root' )">
			<xsl:apply-templates select="/document/reference/versions" />
		</xsl:if>
		<!-- threadsafety -->
		<xsl:apply-templates select="/document/comments/threadsafety" />

		<!-- bibliography -->
		<xsl:call-template name="bibliography" />
		<!-- see also -->
		<xsl:call-template name="t_putSeeAlsoSection" />

	</xsl:template>

	<!-- ============================================================================================
	Inline tags
	============================================================================================= -->

	<!-- pass through html tags -->
	<xsl:template match="p|ol|ul|li|dl|dt|dd|table|tr|th|td|a|img|b|i|strong|em|del|sub|sup|br|hr|h1|h2|h3|h4|h5|h6|pre|div|span|blockquote|abbr|acronym|u|font|map|area">
		<xsl:copy>
			<xsl:copy-of select="@*" />
			<xsl:apply-templates />
		</xsl:copy>
	</xsl:template>

	<xsl:template match="para">
		<p>
			<xsl:apply-templates />
		</p>
	</xsl:template>

	<xsl:template match="c">
		<span class="code">
			<xsl:apply-templates/>
		</span>
	</xsl:template>

	<xsl:template match="preliminary">
		<div class="preliminary">
			<include item="preliminaryText" />
		</div>
	</xsl:template>

	<xsl:template match="paramref">
		<span class="parameter">
			<xsl:value-of select="@name" />
		</span>
	</xsl:template>

	<xsl:template match="typeparamref">
		<span class="typeparameter">
			<xsl:value-of select="@name" />
		</span>
	</xsl:template>

	<!-- ============================================================================================
	Block sections
	============================================================================================= -->

	<xsl:template match="summary">
		<div class="summary">
			<xsl:apply-templates />
		</div>
	</xsl:template>

	<xsl:template match="value">
		<xsl:call-template name="t_putSubSection">
			<xsl:with-param name="p_title">
				<include item="fieldValueTitle" />
			</xsl:with-param>
			<xsl:with-param name="p_content">
				<xsl:apply-templates />
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="returns">
		<xsl:call-template name="t_putSubSection">
			<xsl:with-param name="p_title">
				<include item="methodValueTitle" />
			</xsl:with-param>
			<xsl:with-param name="p_content">
				<xsl:apply-templates />
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="remarks">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="p_titleInclude"
											select="'remarksTitle'" />
			<xsl:with-param name="p_content">
				<xsl:apply-templates />
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="example">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="title"
											select="'examplesTitle'" />
			<xsl:with-param name="p_content">
				<xsl:apply-templates />
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="code">
		<xsl:call-template name="t_putCodeSection" />
	</xsl:template>

	<!-- Details (nonstandard) -->

	<xsl:template match="details">
		<xsl:variable name="sectionTitle">
			<xsl:value-of select="(child::h1 | child::h2 | child::h3 | child::h4 | child::h5 | child::h6)[1]"/>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$sectionTitle = ''">
				<xsl:apply-templates />
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="sectionAddress">
					<xsl:choose>
						<xsl:when test="@name != ''">
							<xsl:value-of select="@name"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="generate-id(.)"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:call-template name="t_putSection">
					<xsl:with-param name="p_title">
						<xsl:value-of select="$sectionTitle" />
					</xsl:with-param>
					<xsl:with-param name="p_content">
						<xsl:choose>
							<xsl:when test="$sectionTitle != ''">
								<xsl:apply-templates select="(child::h1 | child::h2 | child::h3 | child::h4 | child::h5 | child::h6)[1]/following-sibling::*" />
							</xsl:when>
							<xsl:otherwise>
								<xsl:apply-templates />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template match="syntax">
		<xsl:if test="count(*) > 0">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'syntaxTitle'"/>
				<xsl:with-param name="p_content">
					<div id="snippetGroup_Syntax"
							 class="code">
						<xsl:call-template name="syntaxBlocks" />
					</div>
					<!-- parameters & return value -->
					<xsl:apply-templates select="/document/reference/parameters" />
					<xsl:apply-templates select="/document/reference/templates" />
					<xsl:apply-templates select="/document/comments/value" />
					<xsl:apply-templates select="/document/comments/returns" />
					<xsl:apply-templates select="/document/reference/implements" />
					<!-- usage note for extension methods -->
					<xsl:if test="/document/reference/attributes/attribute/type[@api='T:System.Runtime.CompilerServices.ExtensionAttribute'] and boolean($g_apiSubGroup='method')">
						<xsl:call-template name="t_putSubSection">
							<xsl:with-param name="p_title">
								<include item="extensionUsageTitle" />
							</xsl:with-param>
							<xsl:with-param name="p_content">
								<include item="extensionUsageText">
									<parameter>
										<xsl:apply-templates select="/document/reference/parameters/parameter[1]/type"
																				 mode="link" />
									</parameter>
								</include>
							</xsl:with-param>
						</xsl:call-template>
					</xsl:if>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template match="overloads"
								mode="summary">
		<xsl:choose>
			<xsl:when test="count(summary) > 0">
				<xsl:apply-templates select="summary" />
			</xsl:when>
			<xsl:otherwise>
				<div class="summary">
					<xsl:apply-templates/>
				</div>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="overloads"
								mode="sections">
		<xsl:apply-templates select="remarks" />
		<xsl:apply-templates select="example"/>
	</xsl:template>

	<xsl:template match="templates">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="p_titleInclude"
											select="'templatesTitle'" />
			<xsl:with-param name="p_content">
				<dl>
					<xsl:for-each select="template">
						<xsl:variable name="templateName"
													select="@name" />
						<dt>
							<span class="parameter">
								<xsl:value-of select="$templateName"/>
							</span>
						</dt>
						<dd>
							<xsl:apply-templates select="/document/comments/typeparam[@name=$templateName]" />
						</dd>
					</xsl:for-each>
				</dl>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="exceptions">
		<xsl:if test="count(/document/comments/exception) &gt; 0">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'exceptionsTitle'" />
				<xsl:with-param name="p_content">
					<div class="tableSection">
						<table>
							<tr>
								<th class="exceptionNameColumn">
									<include item="exceptionNameHeader" />
								</th>
								<th class="exceptionConditionColumn">
									<include item="exceptionConditionHeader" />
								</th>
							</tr>
							<xsl:for-each select="/document/comments/exception">
								<tr>
									<td>
										<referenceLink target="{@cref}"
																	 qualified="true" />
									</td>
									<td>
										<xsl:apply-templates select="." />
									</td>
								</tr>
							</xsl:for-each>
						</table>
					</div>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="threadsafety">
		<xsl:call-template name="t_putSectionInclude">
			<xsl:with-param name="p_titleInclude"
											select="'threadSafetyTitle'" />
			<xsl:with-param name="p_content">
				<xsl:choose>
					<xsl:when test="normalize-space(.)">
						<xsl:apply-templates />
					</xsl:when>
					<xsl:otherwise>
						<xsl:if test="@static='true'">
							<include item="staticThreadSafe" />
						</xsl:if>
						<xsl:if test="@static='false'">
							<include item="staticNotThreadSafe" />
						</xsl:if>
						<xsl:if test="@instance='true'">
							<include item="instanceThreadSafe" />
						</xsl:if>
						<xsl:if test="@instance='false'">
							<include item="instanceNotThreadSafe" />
						</xsl:if>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="permissions">
		<xsl:if test="count(/document/comments/permission) &gt; 0">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'permissionsTitle'" />
				<xsl:with-param name="p_content">
					<div class="tableSection">
						<table>
							<tr>
								<th class="permissionNameColumn">
									<include item="permissionNameHeader" />
								</th>
								<th class="permissionDescriptionColumn">
									<include item="permissionDescriptionHeader" />
								</th>
							</tr>
							<xsl:for-each select="/document/comments/permission">
								<tr>
									<td>
										<referenceLink target="{@cref}"
																	 qualified="true" />
									</td>
									<td>
										<xsl:apply-templates select="." />
									</td>
								</tr>
							</xsl:for-each>
						</table>
					</div>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="contracts">
		<xsl:variable name="requires"
									select="/document/comments/requires" />
		<xsl:variable name="ensures"
									select="/document/comments/ensures" />
		<xsl:variable name="ensuresOnThrow"
									select="/document/comments/ensuresOnThrow" />
		<xsl:variable name="invariants"
									select="/document/comments/invariant" />
		<xsl:variable name="setter"
									select="/document/comments/setter" />
		<xsl:variable name="getter"
									select="/document/comments/getter" />
		<xsl:variable name="pure"
									select="/document/comments/pure" />
		<xsl:if test="$requires or $ensures or $ensuresOnThrow or $invariants or $setter or $getter or $pure">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'contractsTitle'" />
				<xsl:with-param name="p_content">
					<!--Purity-->
					<xsl:if test="$pure">
						<xsl:text>This method is pure.</xsl:text>
					</xsl:if>
					<!--Contracts-->
					<div class="tableSection">
						<xsl:if test="$getter">
							<xsl:variable name="getterRequires"
														select="$getter/requires"/>
							<xsl:variable name="getterEnsures"
														select="$getter/ensures"/>
							<xsl:variable name="getterEnsuresOnThrow"
														select="$getter/ensuresOnThrow"/>
							<xsl:call-template name="t_putSubSection">
								<xsl:with-param name="p_title">
									<include item="getterTitle" />
								</xsl:with-param>
								<xsl:with-param name="p_content">
									<xsl:if test="$getterRequires">
										<xsl:call-template name="contractsTable">
											<xsl:with-param name="title">
												<include item="requiresNameHeader"/>
											</xsl:with-param>
											<xsl:with-param name="contracts"
																			select="$getterRequires"/>
										</xsl:call-template>
									</xsl:if>
									<xsl:if test="$getterEnsures">
										<xsl:call-template name="contractsTable">
											<xsl:with-param name="title">
												<include item="ensuresNameHeader"/>
											</xsl:with-param>
											<xsl:with-param name="contracts"
																			select="$getterEnsures"/>
										</xsl:call-template>
									</xsl:if>
									<xsl:if test="$getterEnsuresOnThrow">
										<xsl:call-template name="contractsTable">
											<xsl:with-param name="title">
												<include item="ensuresOnThrowNameHeader"/>
											</xsl:with-param>
											<xsl:with-param name="contracts"
																			select="$getterEnsuresOnThrow"/>
										</xsl:call-template>
									</xsl:if>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="$setter">
							<xsl:variable name="setterRequires"
														select="$setter/requires"/>
							<xsl:variable name="setterEnsures"
														select="$setter/ensures"/>
							<xsl:variable name="setterEnsuresOnThrow"
														select="$setter/ensuresOnThrow"/>
							<xsl:call-template name="t_putSubSection">
								<xsl:with-param name="p_title">
									<include item="setterTitle" />
								</xsl:with-param>
								<xsl:with-param name="p_content">
									<xsl:if test="$setterRequires">
										<xsl:call-template name="contractsTable">
											<xsl:with-param name="title">
												<include item="requiresNameHeader"/>
											</xsl:with-param>
											<xsl:with-param name="contracts"
																			select="$setterRequires"/>
										</xsl:call-template>
									</xsl:if>
									<xsl:if test="$setterEnsures">
										<xsl:call-template name="contractsTable">
											<xsl:with-param name="title">
												<include item="ensuresNameHeader"/>
											</xsl:with-param>
											<xsl:with-param name="contracts"
																			select="$setterEnsures"/>
										</xsl:call-template>
									</xsl:if>
									<xsl:if test="$setterEnsuresOnThrow">
										<xsl:call-template name="contractsTable">
											<xsl:with-param name="title">
												<include item="ensuresOnThrowNameHeader"/>
											</xsl:with-param>
											<xsl:with-param name="contracts"
																			select="$setterEnsuresOnThrow"/>
										</xsl:call-template>
									</xsl:if>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="$requires">
							<xsl:call-template name="contractsTable">
								<xsl:with-param name="title">
									<include item="requiresNameHeader"/>
								</xsl:with-param>
								<xsl:with-param name="contracts"
																select="$requires"/>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="$ensures">
							<xsl:call-template name="contractsTable">
								<xsl:with-param name="title">
									<include item="ensuresNameHeader"/>
								</xsl:with-param>
								<xsl:with-param name="contracts"
																select="$ensures"/>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="$ensuresOnThrow">
							<xsl:call-template name="contractsTable">
								<xsl:with-param name="title">
									<include item="ensuresOnThrowNameHeader"/>
								</xsl:with-param>
								<xsl:with-param name="contracts"
																select="$ensuresOnThrow"/>
							</xsl:call-template>
						</xsl:if>
						<xsl:if test="$invariants">
							<xsl:call-template name="contractsTable">
								<xsl:with-param name="title">
									<include item="invariantsNameHeader"/>
								</xsl:with-param>
								<xsl:with-param name="contracts"
																select="$invariants"/>
							</xsl:call-template>
						</xsl:if>
					</div>
					<!--Contracts link-->
					<div class="contractsLink">
						<a>
							<xsl:attribute name="target">
								<xsl:text>_blank</xsl:text>
							</xsl:attribute>
							<xsl:attribute name="href">
								<xsl:text>http://msdn.microsoft.com/en-us/devlabs/dd491992.aspx</xsl:text>
							</xsl:attribute>
							<xsl:text>Learn more about contracts</xsl:text>
						</a>
					</div>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template name="contractsTable">
		<xsl:param name="title"/>
		<xsl:param name="contracts"/>
		<table>
			<tr>
				<th class="contractsNameColumn">
					<xsl:copy-of select="$title"/>
				</th>
			</tr>
			<xsl:for-each select="$contracts">
				<tr>
					<td>
						<div class="code"
								 style="margin-bottom: 0pt; white-space: pre-wrap;">
							<pre xml:space="preserve"
									 style="margin-bottom: 0pt"><xsl:value-of select="."/></pre>
						</div>
						<xsl:if test="@description or @inheritedFrom or @exception">
							<div style="font-size:95%; margin-left: 10pt;
                        margin-bottom: 0pt">
								<table
									class="contractaux"
									width="100%"
									frame="void"
									rules="none"
									border="0">
									<colgroup>
										<col width="10%"/>
										<col width="90%"/>
									</colgroup>
									<xsl:if test="@description">
										<tr style="border-bottom: 0px none;">
											<td style="border-bottom: 0px none;">
												<i>
													<xsl:text>Description: </xsl:text>
												</i>
											</td>
											<td style="border-bottom: 0px none;">
												<xsl:value-of select="@description"/>
											</td>
										</tr>
									</xsl:if>
									<xsl:if test="@inheritedFrom">
										<tr style="border-bottom: 0px none;">
											<td style="border-bottom: 0px none;">
												<i>
													<xsl:text>Inherited From: </xsl:text>
												</i>
											</td>
											<td style="border-bottom: 0px none;">
												<referenceLink target="{@inheritedFrom}">
													<xsl:value-of select="@inheritedFromTypeName"/>
												</referenceLink>
											</td>
										</tr>
									</xsl:if>
									<xsl:if test="@exception">
										<tr style="border-bottom: 0px none;">
											<td style="border-bottom: 0px none;">
												<i>
													<xsl:text>Exception: </xsl:text>
												</i>
											</td>
											<td style="border-bottom: 0px none;">
												<referenceLink target="{@exception}"
																			 qualified="true" />
											</td>
										</tr>
									</xsl:if>
								</table>
							</div>
						</xsl:if>
					</td>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="t_putSeeAlsoSection">
		<xsl:if test="$g_hasSeeAlsoSection">
			<xsl:call-template name="t_putSectionInclude">
				<xsl:with-param name="p_titleInclude"
												select="'relatedTopicsTitle'" />
				<xsl:with-param name="p_content">
					<xsl:call-template name="t_autogenSeeAlsoLinks"/>
					<xsl:for-each select="/document/comments//seealso | /document/reference/elements/element/overloads//seealso">
						<div class="seeAlsoStyle">
							<xsl:apply-templates select=".">
								<xsl:with-param name="displaySeeAlso"
																select="true()" />
							</xsl:apply-templates>
						</div>
					</xsl:for-each>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	lists
	============================================================================================= -->

	<xsl:template match="list[@type='nobullet' or @type='']">
		<ul style="list-style-type:none;">
			<xsl:for-each select="item">
				<li>
					<xsl:choose>
						<xsl:when test="term or description">
							<xsl:if test="term">
								<xsl:apply-templates select="term" />
							</xsl:if>
							<p>
								<xsl:apply-templates select="description" />
							</p>
						</xsl:when>
						<xsl:otherwise>
							<xsl:apply-templates />
						</xsl:otherwise>
					</xsl:choose>
				</li>
			</xsl:for-each>
		</ul>
	</xsl:template>

	<xsl:template match="list[@type='bullet']">
		<ul>
			<xsl:for-each select="item">
				<li>
					<xsl:choose>
						<xsl:when test="term or description">
							<xsl:if test="term">
								<xsl:apply-templates select="term" />
							</xsl:if>
							<p>
								<xsl:apply-templates select="description" />
							</p>
						</xsl:when>
						<xsl:otherwise>
							<xsl:apply-templates />
						</xsl:otherwise>
					</xsl:choose>
				</li>
			</xsl:for-each>
		</ul>
	</xsl:template>

	<xsl:template match="list[@type='number']">
		<ol>
			<xsl:if test="@start">
				<xsl:attribute name="start">
					<xsl:value-of select="@start"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:for-each select="item">
				<li>
					<xsl:choose>
						<xsl:when test="term or description">
							<xsl:if test="term">
								<xsl:apply-templates select="term" />
								<xsl:text> - </xsl:text>
							</xsl:if>
							<p>
								<xsl:apply-templates select="description" />
							</p>
						</xsl:when>
						<xsl:otherwise>
							<xsl:apply-templates />
						</xsl:otherwise>
					</xsl:choose>
				</li>
			</xsl:for-each>
		</ol>
	</xsl:template>

	<xsl:template match="list[@type='table']">
		<div class="tableSection">
			<table>
				<xsl:for-each select="listheader">
					<tr>
						<xsl:for-each select="*">
							<th>
								<xsl:apply-templates />
							</th>
						</xsl:for-each>
					</tr>
				</xsl:for-each>
				<xsl:for-each select="item">
					<tr>
						<xsl:for-each select="*">
							<td>
								<xsl:apply-templates />
							</td>
						</xsl:for-each>
					</tr>
				</xsl:for-each>
			</table>
		</div>
	</xsl:template>

	<xsl:template match="list[@type='definition']">
		<dl class="authored">
			<xsl:for-each select="item">
				<dt>
					<xsl:apply-templates select="term" />
				</dt>
				<dd>
					<xsl:apply-templates select="description" />
				</dd>
			</xsl:for-each>
		</dl>
	</xsl:template>

	<!-- ============================================================================================
	inline tags
	============================================================================================= -->

	<xsl:template match="see[@cref]">
		<xsl:variable name="v_ref">
			<xsl:choose>
				<xsl:when test="contains(@cref,'#')">
					<xsl:value-of select="substring-before(@cref,'#')"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="@cref"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="normalize-space(translate($v_ref,'0123456789abcdefABCDEF-',''))">
				<xsl:choose>
					<xsl:when test="normalize-space(.)">
						<referenceLink target="{@cref}"
													 class="mtps-internal-link">
							<xsl:apply-templates />
						</referenceLink>
					</xsl:when>
					<xsl:otherwise>
						<referenceLink target="{@cref}"
													 class="mtps-internal-link"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="normalize-space(.)">
						<conceptualLink class="mtps-internal-link"
														target="{@cref}">
							<xsl:apply-templates />
						</conceptualLink>
					</xsl:when>
					<xsl:otherwise>
						<conceptualLink class="mtps-internal-link"
														target="{@cref}"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="see[@href]">
		<xsl:call-template name="hyperlink">
			<xsl:with-param name="p_content"
											select="."/>
			<xsl:with-param name="href"
											select="@href"/>
			<xsl:with-param name="target"
											select="@target"/>
			<xsl:with-param name="alt"
											select="@alt"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="see[@langword]">
		<xsl:choose>
			<xsl:when test="@langword='null' or @langword='Nothing' or @langword='nullptr'">
				<xsl:call-template name="t_nullKeyword"/>
			</xsl:when>
			<xsl:when test="@langword='static' or @langword='Shared'">
				<xsl:call-template name="t_staticKeyword"/>
			</xsl:when>
			<xsl:when test="@langword='virtual' or @langword='Overridable'">
				<xsl:call-template name="t_virtualKeyword"/>
			</xsl:when>
			<xsl:when test="@langword='true' or @langword='True'">
				<xsl:call-template name="t_trueKeyword"/>
			</xsl:when>
			<xsl:when test="@langword='false' or @langword='False'">
				<xsl:call-template name="t_falseKeyword"/>
			</xsl:when>
			<xsl:when test="@langword='abstract'">
				<xsl:call-template name="t_abstractKeyword"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="@langword" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="seealso[@href]">
		<xsl:param name="displaySeeAlso"
							 select="false()" />
		<xsl:if test="$displaySeeAlso">
			<xsl:call-template name="hyperlink">
				<xsl:with-param name="p_content"
												select="."/>
				<xsl:with-param name="href"
												select="@href"/>
				<xsl:with-param name="target"
												select="@target"/>
				<xsl:with-param name="alt"
												select="@alt"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="seealso">
		<xsl:param name="displaySeeAlso"
							 select="false()" />
		<xsl:if test="$displaySeeAlso">
			<xsl:choose>
				<xsl:when test="normalize-space(.)">
					<referenceLink target="{@cref}"
												 qualified="true">
						<xsl:value-of select="." />
					</referenceLink>
				</xsl:when>
				<xsl:otherwise>
					<referenceLink target="{@cref}"
												 qualified="true" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="hyperlink">
		<xsl:param name="p_content"/>
		<xsl:param name="href"/>
		<xsl:param name="target"/>
		<xsl:param name="alt"/>

		<a>
			<xsl:choose>
				<xsl:when test="starts-with($href,'ms.help?')">
					<xsl:attribute name="class">
						mtps-internal-link
					</xsl:attribute>
				</xsl:when>
				<xsl:when test="starts-with($href,'http:')">
					<xsl:attribute name="class">
						mtps-external-link
					</xsl:attribute>
				</xsl:when>
			</xsl:choose>
			<xsl:attribute name="href">
				<xsl:value-of select="$href"/>
			</xsl:attribute>
			<xsl:choose>
				<xsl:when test="normalize-space($target)">
					<xsl:attribute name="target">
						<xsl:value-of select="normalize-space($target)"/>
					</xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="target">_blank</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:if test="normalize-space($alt)">
				<xsl:attribute name="title">
					<xsl:value-of select="normalize-space($alt)"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="normalize-space($content)">
					<xsl:value-of select="$content" />
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$href" />
				</xsl:otherwise>
			</xsl:choose>
		</a>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template match="note">
		<xsl:variable name="title">
			<xsl:choose>
				<xsl:when test="@type='note'">
					<xsl:text>noteTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@type='tip'">
					<xsl:text>tipTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@type='caution' or @type='warning'">
					<xsl:text>cautionTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@type='security' or @type='security note'">
					<xsl:text>securityTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@type='important'">
					<xsl:text>importantTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@type='vb' or @type='VB' or @type='VisualBasic' or @type='visual basic note'">
					<xsl:text>visualBasicTitle</xsl:text>
				</xsl:when>
				<xsl:when test="@type='cs' or @type='CSharp' or @type='c#' or @type='C#' or @type='visual c# note'">
					<xsl:text>visualC#Title</xsl:text>
				</xsl:when>
				<xsl:when test="@type='cpp' or @type='c++' or @type='C++' or @type='CPP' or @type='visual c++ note'">
					<xsl:text>visualC++Title</xsl:text>
				</xsl:when>
				<xsl:when test="@type='JSharp' or @type='j#' or @type='J#' or @type='visual j# note'">
					<xsl:text>visualJ#Title</xsl:text>
				</xsl:when>
				<xsl:when test="@type='implement'">
					<xsl:text>NotesForImplementers</xsl:text>
				</xsl:when>
				<xsl:when test="@type='caller'">
					<xsl:text>NotesForCallers</xsl:text>
				</xsl:when>
				<xsl:when test="@type='inherit'">
					<xsl:text>NotesForInheritors</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>noteTitle</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="altTitle">
			<xsl:choose>
				<xsl:when test="@type='note' or @type='implement' or @type='caller' or @type='inherit'">
					<xsl:text>noteAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='tip'">
					<xsl:text>tipAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='caution' or @type='warning'">
					<xsl:text>cautionAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='security' or @type='security note'">
					<xsl:text>securityAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='important'">
					<xsl:text>importantAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='vb' or @type='VB' or @type='VisualBasic' or @type='visual basic note'">
					<xsl:text>visualBasicAltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='cs' or @type='CSharp' or @type='c#' or @type='C#' or @type='visual c# note'">
					<xsl:text>visualC#AltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='cpp' or @type='c++' or @type='C++' or @type='CPP' or @type='visual c++ note'">
					<xsl:text>visualC++AltText</xsl:text>
				</xsl:when>
				<xsl:when test="@type='JSharp' or @type='j#' or @type='J#' or @type='visual j# note'">
					<xsl:text>visualJ#AltText</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>noteAltText</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="noteImg">
			<xsl:choose>
				<xsl:when test="@type='note' or @type='tip' or @type='implement' or @type='caller' or @type='inherit'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@type='caution' or @type='warning'">
					<xsl:text>alert_caution.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@type='security' or @type='security note'">
					<xsl:text>alert_security.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@type='important'">
					<xsl:text>alert_caution.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@type='vb' or @type='VB' or @type='VisualBasic' or @type='visual basic note'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@type='cs' or @type='CSharp' or @type='c#' or @type='C#' or @type='visual c# note'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@type='cpp' or @type='c++' or @type='C++' or @type='CPP' or @type='visual c++ note'">
					<xsl:text>alert_note.gif</xsl:text>
				</xsl:when>
				<xsl:when test="@type='JSharp' or @type='j#' or @type='J#' or @type='visual j# note'">
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

	<!-- ======================================================================================== -->

	<xsl:template name="t_putMemberIntro">
		<xsl:if test="$g_apiTopicSubGroup='members'">
			<p>
				<xsl:apply-templates select="/document/reference/containers/summary"/>
			</p>
		</xsl:if>
		<xsl:call-template name="t_memberIntroBoilerplate"/>
	</xsl:template>

	<xsl:template name="codelangAttributes">
		<xsl:call-template name="mshelpCodelangAttributes">
			<xsl:with-param name="snippets"
											select="/document/comments/example/code" />
		</xsl:call-template>
	</xsl:template>

	<!-- ======================================================================================== -->

	<xsl:template name="t_runningHeader">
		<include item="runningHeaderText" />
	</xsl:template>

	<xsl:template name="t_getParameterDescription">
		<xsl:param name="name" />
		<xsl:apply-templates select="/document/comments/param[@name=$name]" />
	</xsl:template>

	<xsl:template name="getReturnsDescription">
		<xsl:param name="name" />
		<xsl:apply-templates select="/document/comments/param[@name=$name]" />
	</xsl:template>

	<xsl:template name="t_getElementDescription">
		<xsl:apply-templates select="summary[1]" />
	</xsl:template>

	<xsl:template name="t_getOverloadSummary">
		<xsl:apply-templates select="overloads"
												 mode="summary"/>
	</xsl:template>

	<xsl:template name="t_getOverloadSections">
		<xsl:apply-templates select="overloads"
												 mode="sections"/>
	</xsl:template>

	<xsl:template name="t_getInternalOnlyDescription">

	</xsl:template>

	<!-- ============================================================================================
	Bibliography
	============================================================================================= -->
	<xsl:key name="citations"
					 match="//cite"
					 use="text()" />

	<xsl:variable name="g_hasCitations"
								select="boolean(count(//cite) > 0)"/>

	<xsl:template match="cite">
		<xsl:variable name="v_currentCitation"
									select="text()"/>
		<xsl:for-each select="//cite[generate-id(.)=generate-id(key('citations',text()))]">
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

	<xsl:template name="bibliography">
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
		<xsl:for-each select="//cite[generate-id(.)=generate-id(key('citations',text()))]">
			<!-- Distinct citations only -->
			<xsl:variable name="citation"
										select="."/>
			<xsl:variable name="entry"
										select="document($bibliographyData)/bibliography/reference[@name=$citation]"/>

			<xsl:call-template name="bibliographyReference">
				<xsl:with-param name="number"
												select="position()"/>
				<xsl:with-param name="data"
												select="$entry"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>

</xsl:stylesheet>
