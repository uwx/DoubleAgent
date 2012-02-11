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

	<xsl:import href="../../shared/transforms/utilities_reference.xsl"/>

	<!-- ============================================================================================
	Parameters - key parameter is the api identifier string
	============================================================================================= -->

	<xsl:param name="key" />
	<xsl:param name="metadata">false</xsl:param>
	<xsl:param name="languages">false</xsl:param>
	<xsl:param name="componentizeBy">namespace</xsl:param>

	<!-- ============================================================================================
	Includes
	============================================================================================= -->

	<xsl:include href="metadataHelp30.xsl" />
	<xsl:include href="metadataHelp20.xsl"/>
	<xsl:include href="xamlSyntax.xsl"/>

	<!-- ============================================================================================
	Global Variables
	============================================================================================= -->

	<xsl:variable name="g_topicTypeId">
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

	<xsl:variable name="g_topicGroup"
								select="/document/reference/topicdata/@group" />
	<xsl:variable name="g_apiGroup"
								select="/document/reference/apidata/@group" />
	<xsl:variable name="g_apiTopicGroup">
		<xsl:choose>
			<xsl:when test="/document/reference/topicdata/@group = 'api'">
				<xsl:value-of select="/document/reference/apidata/@group" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/document/reference/topicdata/@group" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="g_topicSubGroup"
								select="/document/reference/topicdata/@subgroup" />
	<xsl:variable name="g_apiSubGroup"
								select="/document/reference/apidata/@subgroup" />
	<xsl:variable name="g_apiTopicSubGroup">
		<xsl:choose>
			<xsl:when test="/document/reference/topicdata/@group = 'api'">
				<xsl:value-of select="/document/reference/apidata/@subgroup" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/document/reference/topicdata/@subgroup" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="g_apiSubSubGroup"
								select="/document/reference/apidata/@subsubgroup" />
	<xsl:variable name="g_apiTopicSubSubGroup">
		<xsl:choose>
			<xsl:when test="/document/reference/topicdata/@group = 'api'">
				<xsl:value-of select="/document/reference/apidata/@subsubgroup" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/document/reference/topicdata/@subsubgroup" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="namespaceName"
								select="/document/reference/containers/namespace/apidata/@name" />

	<!-- ============================================================================================
	Document header
	============================================================================================= -->

	<xsl:template match="/">
		<html>
			<head>
				<meta http-equiv="Content-Type"
							content="text/html; charset=UTF-8"/>
				<xsl:call-template name="t_insertNoIndexNoFollow" />
				<title>
					<xsl:call-template name="t_topicTitlePlain"/>
				</title>
				<xsl:call-template name="t_insertMetadata" />
				<xsl:call-template name="t_insertMetadataHelp30" />
				<xsl:call-template name="t_insertFilename" />
				<xsl:call-template name="t_insertMetadataHelp20" />
			</head>
			<body class="primary-mtps-offline-document">
				<div class="topic">
					<xsl:call-template name="t_bodyTitle"/>
					<xsl:call-template name="t_bodyMain"/>
				</div>
			</body>
		</html>
	</xsl:template>

	<!-- ============================================================================================
	Main body div
	============================================================================================= -->

	<xsl:template name="t_bodyMain">
		<div id="mainSection">

			<div id="mainBody">
				<include item="header" />
				<xsl:call-template name="t_body" />
			</div>
		</div>
	</xsl:template>

	<!-- ============================================================================================
	The plain-text title used in the TOC 
	============================================================================================= -->

	<xsl:template name="t_topicTitlePlain">
		<xsl:param name="qualifyMembers"
							 select="false()" />
		<include>
			<xsl:attribute name="item">
				<xsl:choose>
					<!-- api topic titles -->
					<xsl:when test="$g_topicGroup='api'">
						<!-- the subsubgroup, subgroup, or group determines the title -->
						<xsl:choose>
							<xsl:when test="string($g_apiSubSubGroup)">
								<xsl:choose>
									<!-- topic title for op_explicit and op_implicit members -->
									<xsl:when test="$g_apiSubSubGroup='operator' and (document/reference/apidata/@name = 'Explicit' or document/reference/apidata/@name = 'Implicit')">
										<xsl:value-of select="'typeConversion'"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$g_apiSubSubGroup" />
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:when test="string($g_apiSubGroup)">
								<xsl:value-of select="$g_apiSubGroup"/>
							</xsl:when>
							<xsl:when test="string($g_apiGroup)">
								<xsl:value-of select="$g_apiGroup"/>
							</xsl:when>
						</xsl:choose>
					</xsl:when>
					<!-- overload topic titles -->
					<xsl:when test="$g_topicSubGroup='overload'">
						<!-- the api subgroup (e.g. "property") determines the title; do we want to use the subsubgoup name when it is available? -->
						<xsl:choose>
							<!-- topic title for overload op_explicit and op_implicit members -->
							<xsl:when test="$g_apiSubSubGroup = 'operator' and (document/reference/apidata/@name='Explicit' or document/reference/apidata/@name = 'Implicit')">
								<xsl:value-of select="'conversionOperator'"/>
							</xsl:when>
							<!-- topic title for overload operator members -->
							<xsl:when test="$g_apiSubSubGroup='operator'">
								<xsl:value-of select="$g_apiSubSubGroup" />
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$g_apiSubGroup"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<!-- list topic titles -->
					<xsl:when test="$g_topicGroup='list'">
						<!-- the topic subgroup (e.g. "methods") determines the title -->
						<xsl:choose>
							<xsl:when test="$g_topicSubGroup='Operators'">
								<xsl:variable name="v_operators"
															select="document/reference/elements/element[not(apidata[@name='Explicit' or @name='Implicit'])]"/>
								<xsl:variable name="v_conversions"
															select="document/reference/elements/element[apidata[@name='Explicit' or @name='Implicit']]" />
								<xsl:choose>
									<!-- v_operators + type v_conversions -->
									<xsl:when test="count($v_operators) &gt; 0 and count($v_conversions) &gt; 0">
										<xsl:value-of select="'OperatorsAndTypeConversions'" />
									</xsl:when>
									<!-- no v_operators + type v_conversions -->
									<xsl:when test="not(count($v_operators) &gt; 0) and count($v_conversions) &gt; 0">
										<xsl:value-of select="'TypeConversions'" />
									</xsl:when>
									<!-- v_operators + no type v_conversions -->
									<xsl:otherwise>
										<xsl:value-of select="$g_topicSubGroup" />
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$g_topicSubGroup" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<!-- overload root titles  -->
					<xsl:when test="$g_topicGroup='root'">
						<xsl:value-of select="$g_topicGroup" />
					</xsl:when>
				</xsl:choose>
				<xsl:text>TopicTitle</xsl:text>
			</xsl:attribute>
			<parameter>
				<xsl:call-template name="shortNamePlain">
					<xsl:with-param name="qualifyMembers"
													select="$qualifyMembers" />
				</xsl:call-template>
			</parameter>
			<parameter>
				<!-- show parameters only for overloaded members -->
				<xsl:if test="document/reference/memberdata/@overload or ($g_apiSubSubGroup = 'operator' and (document/reference/apidata/@name='Explicit' or document/reference/apidata/@name='Implicit'))">
					<xsl:choose>
						<xsl:when test="$g_apiSubSubGroup = 'operator' and (document/reference/apidata/@name='Explicit' or document/reference/apidata/@name='Implicit')">
							<xsl:for-each select="/document/reference">
								<xsl:call-template name="operatorTypesPlain" />
							</xsl:for-each>
						</xsl:when>
						<xsl:otherwise>
							<xsl:for-each select="/document/reference">
								<xsl:call-template name="parameterTypesPlain" />
							</xsl:for-each>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
			</parameter>
		</include>
	</xsl:template>

	<!-- ============================================================================================
	The language-variant, marked-up topic title	used as the big title at the top of the page
	============================================================================================= -->

	<xsl:template name="t_topicTitleDecorated">
		<include>
			<xsl:attribute name="item">
				<xsl:choose>

					<!-- api topic titles -->
					<xsl:when test="$g_topicGroup='api'">
						<xsl:choose>
							<xsl:when test="string($g_apiSubSubGroup)">
								<xsl:choose>
									<!-- topic title for op_explicit and op_implicit members -->
									<xsl:when test="$g_apiSubSubGroup='operator' and (document/reference/apidata/@name = 'Explicit' or document/reference/apidata/@name = 'Implicit')">
										<xsl:value-of select="'typeConversion'"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$g_apiSubSubGroup" />
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:when test="string($g_apiSubGroup)">
								<xsl:value-of select="$g_apiSubGroup" />
							</xsl:when>
							<xsl:when test="string($g_apiGroup)">
								<xsl:value-of select="$g_apiGroup" />
							</xsl:when>
						</xsl:choose>
					</xsl:when>

					<!-- overload topic titles -->
					<xsl:when test="$g_topicSubGroup='overload'">
						<!-- the api subgroup (e.g. "property") determines the title; do we want to use the subsubgoup name when it is available? -->
						<xsl:choose>
							<!-- topic title for overload op_explicit and op_implicit members -->
							<xsl:when test="$g_apiSubSubGroup = 'operator' and (document/reference/apidata/@name= 'Explicit' or document/reference/apidata/@name= 'Implicit')">
								<xsl:value-of select="'conversionOperator'"/>
							</xsl:when>
							<!-- topic title for overload operator members -->
							<xsl:when test="$g_apiSubSubGroup='operator'">
								<xsl:value-of select="$g_apiSubSubGroup" />
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$g_apiSubGroup"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>

					<!-- list topic titles -->
					<xsl:when test="$g_topicGroup='list'">
						<!-- the topic subgroup (e.g. "methods") determines the title -->
						<xsl:choose>
							<xsl:when test="$g_topicSubGroup='Operators'">
								<xsl:variable name="v_operators"
															select="document/reference/elements/element[not(apidata[@name='Explicit' or @name='Implicit'])]"/>
								<xsl:variable name="v_conversions"
															select="document/reference/elements/element[apidata[@name='Explicit' or @name='Implicit']]" />
								<xsl:choose>
									<!-- operators + type conversions -->
									<xsl:when test="count($v_operators) &gt; 0 and count($v_conversions) &gt; 0">
										<xsl:value-of select="'OperatorsAndTypeConversions'" />
									</xsl:when>
									<!-- no operators + type conversions -->
									<xsl:when test="not(count($v_operators) &gt; 0) and count($v_conversions) &gt; 0">
										<xsl:value-of select="'TypeConversions'" />
									</xsl:when>
									<!-- operators + no type conversions -->
									<xsl:otherwise>
										<xsl:value-of select="$g_topicSubGroup" />
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$g_topicSubGroup" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>

					<!-- overload root titles  -->
					<xsl:when test="$g_topicGroup='root'">
						<xsl:value-of select="$g_topicGroup" />
					</xsl:when>
				</xsl:choose>

				<xsl:text>TopicTitle</xsl:text>
			</xsl:attribute>
			<parameter>
				<xsl:call-template name="shortNameDecorated" />
			</parameter>
			<parameter>
				<!-- show parameters only from overloaded members -->
				<xsl:if test="document/reference/memberdata/@overload or ($g_apiSubSubGroup= 'operator' and (document/reference/apidata/@name='Explicit' or document/reference/apidata/@name='Implicit'))">
					<xsl:choose>
						<xsl:when test="$g_apiSubSubGroup = 'operator' and (document/reference/apidata/@name='Explicit' or document/reference/apidata/@name='Implicit')">
							<xsl:for-each select="/document/reference">
								<xsl:call-template name="operatorTypesDecorated" />
							</xsl:for-each>
						</xsl:when>
						<xsl:otherwise>
							<xsl:for-each select="/document/reference">
								<xsl:call-template name="parameterTypesDecorated" />
							</xsl:for-each>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
			</parameter>
		</include>
	</xsl:template>

	<xsl:template name="t_insertFilename">
		<meta name="container">
			<xsl:attribute name="content">
				<xsl:choose>
					<xsl:when test="$componentizeBy='assembly'">
						<xsl:choose>
							<xsl:when test="normalize-space(/document/reference/containers/library/@assembly)">
								<xsl:value-of select="normalize-space(/document/reference/containers/library/@assembly)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>Namespaces</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<!-- the default is to componentize by namespace. For non-componentized builds, the <meta name="container"> value is ignored. -->
					<xsl:otherwise>
						<xsl:choose>
							<!-- get the namespace name from containers/namespace/@api for most members -->
							<xsl:when test="normalize-space(substring-after(/document/reference/containers/namespace/@api,':'))">
								<xsl:value-of select="normalize-space(substring-after(/document/reference/containers/namespace/@api,':'))"/>
							</xsl:when>
							<!-- use 'default_namespace' for members in the default namespace (where namespace/@api == 'N:') -->
							<xsl:when test="normalize-space(/document/reference/containers/namespace/@api)">
								<xsl:text>default_namespace</xsl:text>
							</xsl:when>
							<!-- for the default namespace topic, use 'default_namespace' -->
							<xsl:when test="/document/reference/apidata[@group='namespace' and @name='']">
								<xsl:text>default_namespace</xsl:text>
							</xsl:when>
							<!-- for other namespace topics, get the name from apidata/@name -->
							<xsl:when test="/document/reference/apidata/@group='namespace'">
								<xsl:value-of select="normalize-space(/document/reference/apidata/@name)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>unknown</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</meta>
		<meta name="file"
					content="{/document/reference/file/@name}" />
		<meta name="guid">
			<xsl:attribute name="content">
				<xsl:value-of select="/document/reference/file/@name" />
			</xsl:attribute>
		</meta>
	</xsl:template>

	<!-- ============================================================================================
	elements processing
	============================================================================================= -->

	<xsl:template match="elements"
								mode="root">
		<xsl:if test="count(element) > 0">

			<xsl:call-template name="t_putSection">
				<xsl:with-param name="toggleSwitch"
												select="'namespaces'"/>
				<xsl:with-param name="title">
					<include item="namespacesTitle" />
				</xsl:with-param>
				<xsl:with-param name="content">
					<table class="members"
								 id="memberList"
								 frame="lhs"
								 cellpadding="2">
						<tr>
							<th class="nameColumn">
								<include item="namespaceNameHeader"/>
							</th>
							<th class="descriptionColumn">
								<include item="namespaceDescriptionHeader" />
							</th>
						</tr>
						<xsl:apply-templates select="element"
																 mode="root">
							<xsl:sort select="apidata/@name" />
						</xsl:apply-templates>
					</table>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="elements"
								mode="namespace">

		<xsl:if test="element/apidata/@subgroup = 'class'">
			<xsl:call-template name="t_putNamespaceSection">
				<xsl:with-param name="p_listSubgroup"
												select="'class'" />
			</xsl:call-template>
		</xsl:if>

		<xsl:if test="element/apidata/@subgroup = 'structure'">
			<xsl:call-template name="t_putNamespaceSection">
				<xsl:with-param name="p_listSubgroup"
												select="'structure'" />
			</xsl:call-template>
		</xsl:if>

		<xsl:if test="element/apidata/@subgroup = 'interface'">
			<xsl:call-template name="t_putNamespaceSection">
				<xsl:with-param name="p_listSubgroup"
												select="'interface'" />
			</xsl:call-template>
		</xsl:if>

		<xsl:if test="element/apidata/@subgroup = 'delegate'">
			<xsl:call-template name="t_putNamespaceSection">
				<xsl:with-param name="p_listSubgroup"
												select="'delegate'" />
			</xsl:call-template>
		</xsl:if>

		<xsl:if test="element/apidata/@subgroup = 'enumeration'">
			<xsl:call-template name="t_putNamespaceSection">
				<xsl:with-param name="p_listSubgroup"
												select="'enumeration'" />
			</xsl:call-template>
		</xsl:if>

	</xsl:template>

	<xsl:template match="elements"
								mode="enumeration">
		<div id="enumerationSection">
			<xsl:if test="count(element) > 0">
				<xsl:call-template name="t_putSection">
					<xsl:with-param name="toggleSwitch"
													select="'members'"/>
					<xsl:with-param name="title">
						<include item="enumMembersTitle" />
					</xsl:with-param>
					<xsl:with-param name="content">
						<table class="members"
									 id="memberList"
									 frame="lhs"
									 cellpadding="2">
							<col width="10%"/>
							<tr>
								<th class="iconColumn"></th>
								<th class="nameColumn">
									<include item="memberNameHeader"/>
								</th>
								<th class="valueColumn">
									<include item="memberValueHeader" />
								</th>
								<th class="descriptionColumn">
									<include item="memberDescriptionHeader" />
								</th>
							</tr>
							<xsl:apply-templates select="element"
																	 mode="enumeration"/>
						</table>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:if>
		</div>
	</xsl:template>

	<xsl:template match="elements"
								mode="member">

		<!-- Filter out the Overload pages created by ApplyVSDocModel.xsl. These
         pages (and the need for this filter) will go away once the full Brighton spec is implemented. -->
		<xsl:variable name="filteredOverloadElements"
									select="element[starts-with(@api, 'Overload:')]/element | element[not(starts-with(@api, 'Overload:'))]" />

		<xsl:call-template name="t_putMemberIntro" />

		<!-- TODO: factor out these duplicated Xpaths by a new conditional in t_putMemberListSection:
         count($members) &gt; 0 -->

		<xsl:if test="$filteredOverloadElements[apidata[@subgroup='constructor']][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup"
												select="'constructor'" />
				<xsl:with-param name="p_members"
												select="$filteredOverloadElements[apidata[@subgroup='constructor']][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]" />
			</xsl:call-template>
		</xsl:if>

		<!-- method table -->
		<xsl:if test="$filteredOverloadElements[apidata[@subgroup='method' and not(@subsubgroup)]][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">method</xsl:with-param>
				<xsl:with-param name="p_members"
												select="$filteredOverloadElements[apidata[@subgroup='method' and not(@subsubgroup)]][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]" />
			</xsl:call-template>
		</xsl:if>

		<!-- operator table -->
		<xsl:if test="$filteredOverloadElements[apidata[@subsubgroup='operator']][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">operator</xsl:with-param>
				<xsl:with-param name="p_members"
												select="$filteredOverloadElements[apidata[@subsubgroup='operator']][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]" />
			</xsl:call-template>
		</xsl:if>

		<!-- extension method table -->
		<xsl:if test="$filteredOverloadElements[apidata[@subsubgroup='extension']]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">extensionMethod</xsl:with-param>
				<xsl:with-param name="p_members"
												select="$filteredOverloadElements[apidata[@subsubgroup='extension']]" />
			</xsl:call-template>
		</xsl:if>

		<!-- field table -->
		<xsl:if test="element[apidata[@subgroup='field']][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">field</xsl:with-param>
				<xsl:with-param name="p_members"
												select="element[apidata[@subgroup='field']][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]" />
			</xsl:call-template>
		</xsl:if>

		<!-- property table -->
		<xsl:if test="$filteredOverloadElements[apidata[@subgroup='property' and not(@subsubgroup)]][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">property</xsl:with-param>
				<xsl:with-param name="p_members"
												select="$filteredOverloadElements[apidata[@subgroup='property' and not(@subsubgroup)]][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]" />
			</xsl:call-template>
		</xsl:if>

		<!-- attached property table -->
		<xsl:if test="element/apidata[@subsubgroup='attachedProperty']">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">attachedProperty</xsl:with-param>
				<xsl:with-param name="p_members"
												select="element[apidata[@subsubgroup='attachedProperty']]" />
			</xsl:call-template>
		</xsl:if>

		<!-- event table -->
		<xsl:if test="element[apidata[@subgroup='event' and not(@subsubgroup)]][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">event</xsl:with-param>
				<xsl:with-param name="p_members"
												select="element[apidata[@subgroup='event' and not(@subsubgroup)]][.//memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly'] or (.//memberdata[@visibility='private'] and not(.//proceduredata[@virtual = 'true']))]" />
			</xsl:call-template>
		</xsl:if>

		<!-- attached event table -->
		<xsl:if test="element/apidata[@subsubgroup='attachedEvent']">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">attachedEvent</xsl:with-param>
				<xsl:with-param name="p_members"
												select="element[apidata[@subsubgroup='attachedEvent']]" />
			</xsl:call-template>
		</xsl:if>

		<!-- eii table -->
		<xsl:if test="$filteredOverloadElements[memberdata[@visibility='private'] and proceduredata[@virtual = 'true']]">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup">ExplicitInterfaceImplementation</xsl:with-param>
				<xsl:with-param name="p_members"
												select="$filteredOverloadElements[.//memberdata[@visibility='private'] and .//proceduredata[@virtual = 'true']]" />
			</xsl:call-template>
		</xsl:if>

	</xsl:template>

	<xsl:template match="elements"
								mode="type">

	</xsl:template>

	<xsl:template match="elements"
								mode="derivedType">
		<xsl:if test="count(element) > 0">
			<xsl:call-template name="t_putSection">
				<xsl:with-param name="toggleSwitch"
												select="'DerivedClasses'"/>
				<xsl:with-param name="title">
					<include item="derivedClasses" />
				</xsl:with-param>
				<xsl:with-param name="content">
					<table class="members"
								 id="memberList"
								 frame="lhs"
								 cellpadding="2">
						<tr>
							<th class="nameColumn">
								<include item="memberNameHeader"/>
							</th>
							<th class="descriptionColumn">
								<include item="memberDescriptionHeader" />
							</th>
						</tr>
						<xsl:apply-templates select="element"
																 mode="derivedType">
							<xsl:sort select="apidata/@name" />
						</xsl:apply-templates>
					</table>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="elements"
								mode="overload">
		<xsl:if test="count(element) > 0">
			<xsl:call-template name="t_putMemberListSection">
				<xsl:with-param name="p_headerGroup"
												select="'overloadMembers'" />
				<xsl:with-param name="p_members"
												select="element" />
				<xsl:with-param name="p_showParameters"
												select="'true'" />
				<xsl:with-param name="p_sort"
												select="false()" />
			</xsl:call-template>
		</xsl:if>
		<xsl:apply-templates select="element"
												 mode="overloadSections">
			<xsl:sort select="apidata/@name" />
		</xsl:apply-templates>
	</xsl:template>

	<xsl:template match="elements"
								mode="overloadSummary">
		<xsl:apply-templates select="element"
												 mode="overloadSummary" >
			<xsl:sort select="apidata/@name"/>
		</xsl:apply-templates>
	</xsl:template>

	<!-- ============================================================================================
	elements helpers
	============================================================================================= -->

	<xsl:template name="t_putNamespaceSection">
		<xsl:param name="p_listSubgroup" />

		<xsl:variable name="header"
									select="concat($p_listSubgroup, 'TypesFilterLabel')"/>
		<xsl:call-template name="t_putSection">
			<xsl:with-param name="toggleSwitch"
											select="$p_listSubgroup"/>
			<xsl:with-param name="title">
				<include item="{$header}" />
			</xsl:with-param>
			<xsl:with-param name="content">
				<xsl:call-template name="t_putNamespaceList">
					<xsl:with-param name="p_listSubgroup"
													select="$p_listSubgroup" />
				</xsl:call-template>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="t_putNamespaceList">
		<xsl:param name="p_listSubgroup" />

		<table id="typeList"
					 class="members"
					 frame="lhs"
					 cellpadding="2">
			<col width="10%"/>
			<tr>
				<th class="iconColumn">
					&#160;
				</th>
				<th class="nameColumn">
					<include item="{$p_listSubgroup}NameHeader"/>
				</th>
				<th class="descriptionColumn">
					<include item="typeDescriptionHeader" />
				</th>
			</tr>
			<xsl:apply-templates select="element[apidata/@subgroup=$p_listSubgroup]"
													 mode="namespace">
				<xsl:sort select="@api" />
			</xsl:apply-templates>
		</table>

	</xsl:template>

	<xsl:template name="t_memberIntroBoilerplate">
		<xsl:if test="/document/reference/elements/element/memberdata[@visibility='public' or @visibility='family' or @visibility='family or assembly' or @visibility='assembly']">
			<!-- if there are exposed members, show a boilerplate intro p -->
			<xsl:variable name="introTextItemId">
				<xsl:choose>
					<xsl:when test="/document/reference/containers/type/templates">genericExposedMembersTableText</xsl:when>
					<xsl:otherwise>exposedMembersTableText</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<p>
				<include item="{$introTextItemId}">
					<parameter>
						<referenceLink target="{$g_topicTypeId}" />
					</parameter>
					<parameter>
						<xsl:value-of select="$g_apiTopicSubGroup"/>
						<xsl:text>Subgroup</xsl:text>
					</parameter>
				</include>
			</p>
		</xsl:if>
	</xsl:template>

	<xsl:template name="t_putMemberListSection">
		<xsl:param name="p_members"/>
		<xsl:param name="p_headerGroup" />
		<xsl:param name="p_showParameters"
							 select="false()" />
		<xsl:param name="p_sort"
							 select="true()" />

		<xsl:variable name="header">
			<xsl:value-of select="concat($p_headerGroup, 'Table')"/>
		</xsl:variable>

		<xsl:call-template name="t_putSection">
			<xsl:with-param name="toggleSwitch"
											select="$header" />
			<xsl:with-param name="title">
				<include item="{$header}" />
			</xsl:with-param>
			<xsl:with-param name="toplink"
											select="true()"/>
			<xsl:with-param name="content">
				<table id="memberList"
							 class="members"
							 frame="lhs"
							 cellpadding="2">
					<col width="10%"/>
					<tr>
						<th class="iconColumn">
							&#160;
						</th>
						<th class="nameColumn">
							<include item="typeNameHeader"/>
						</th>
						<th class="descriptionColumn">
							<include item="typeDescriptionHeader" />
						</th>
					</tr>

					<!-- add a row for each member of the current subgroup-visibility -->
					<xsl:choose>
						<xsl:when test="boolean($p_sort)">
							<xsl:apply-templates select="$p_members"
																	 mode="memberlistRow">
								<xsl:with-param name="p_showParameters"
																select="$p_showParameters" />
								<xsl:sort select="topicdata/@eiiName | apidata/@name"/>
								<xsl:sort select="count(templates/*)" />
							</xsl:apply-templates>
						</xsl:when>
						<xsl:otherwise>
							<xsl:apply-templates select="$p_members"
																	 mode="memberlistRow">
								<xsl:with-param name="p_showParameters"
																select="$p_showParameters" />
							</xsl:apply-templates>
						</xsl:otherwise>
					</xsl:choose>
				</table>
			</xsl:with-param>
		</xsl:call-template>

	</xsl:template>

	<!-- ============================================================================================
	element processing
	============================================================================================= -->

	<xsl:template match="element"
								mode="root">
		<tr>
			<td>
				<xsl:choose>
					<xsl:when test="apidata/@name = ''">
						<referenceLink target="{@api}"
													 qualified="false">
							<include item="defaultNamespace" />
						</referenceLink>
					</xsl:when>
					<xsl:otherwise>
						<referenceLink target="{@api}"
													 qualified="false" />
					</xsl:otherwise>
				</xsl:choose>
			</td>
			<td>
				<xsl:call-template name="t_getElementDescription" />
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="element"
								mode="namespace">

		<xsl:variable name="v_typeVisibility">
			<xsl:choose>
				<xsl:when test="typedata/@visibility='family' or typedata/@visibility='family or assembly' or typedata/@visibility='assembly'">prot</xsl:when>
				<xsl:when test="typedata/@visibility='private'">priv</xsl:when>
				<xsl:otherwise>pub</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<tr>
			<xsl:attribute name="data">
				<xsl:value-of select="apidata/@subgroup" />
				<xsl:text>; public</xsl:text>
			</xsl:attribute>
			<td>
				<xsl:call-template name="t_putTypeIcon">
					<xsl:with-param name="p_typeVisibility"
													select="$v_typeVisibility" />
				</xsl:call-template>
			</td>
			<td>
				<referenceLink target="{@api}"
											 qualified="false" />
			</td>
			<td>
				<xsl:call-template name="t_getInternalOnlyDescription" />
				<xsl:if test="attributes/attribute/type[@api='T:System.ObsoleteAttribute']">
					<xsl:text> </xsl:text>
					<include item="obsoleteRed" />
				</xsl:if>
				<xsl:call-template name="t_getElementDescription" />
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="element"
								mode="enumeration">

		<xsl:variable name="v_supportedOnXna">
			<xsl:call-template name="t_isMemberSupportedOnXna"/>
		</xsl:variable>
		<xsl:variable name="v_supportedOnCf">
			<xsl:call-template name="t_isMemberSupportedOnCf"/>
		</xsl:variable>
		<xsl:variable name="v_supportedOnSilverlightMobile">
			<xsl:call-template name="t_isMemberSupportedOnSilverlightMobile" />
		</xsl:variable>
		<tr>
			<td>
				<!-- platform icons -->
				<xsl:if test="normalize-space($v_supportedOnCf)!=''">
					<img data="netcfw">
						<includeAttribute name="src"
															item="iconPath">
							<parameter>CFW.gif</parameter>
						</includeAttribute>
						<includeAttribute name="alt"
															item="CompactFrameworkAltText" />
						<includeAttribute name="title"
															item="CompactFrameworkAltText" />
					</img>
				</xsl:if>

				<xsl:if test="normalize-space($v_supportedOnXna)!=''">
					<img data="xnafw">
						<includeAttribute name="src"
															item="iconPath">
							<parameter>xna.gif</parameter>
						</includeAttribute>
						<includeAttribute name="alt"
															item="XNAFrameworkAltText" />
						<includeAttribute name="title"
															item="XNAFrameworkAltText" />
					</img>
				</xsl:if>
				<xsl:if test="normalize-space($v_supportedOnSilverlightMobile)!=''">
					<img data="silverlight_mobile">
						<includeAttribute name="src"
															item="iconPath">
							<parameter>slMobile.gif</parameter>
						</includeAttribute>
						<includeAttribute name="alt"
															item="SilverlightMobileAltText" />
						<includeAttribute name="title"
															item="SilverlightMobileAltText" />
					</img>
				</xsl:if>
			</td>
			<xsl:variable name="id"
										select="@api" />
			<td target="{$id}">
				<span class="selflink">
					<xsl:value-of select="apidata/@name"/>
				</span>
			</td>
			<td>
				<xsl:value-of select="value"/>
			</td>
			<td>
				<xsl:if test="attributes/attribute/type[@api='T:System.ObsoleteAttribute']">
					<xsl:text> </xsl:text>
					<include item="obsoleteRed" />
				</xsl:if>
				<xsl:call-template name="t_getEnumMemberDescription" />
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="element"
								mode="derivedType">
		<tr>
			<td>
				<xsl:choose>
					<xsl:when test="@display-api">
						<referenceLink target="{@api}"
													 display-target="{@display-api}" />
					</xsl:when>
					<xsl:otherwise>
						<referenceLink target="{@api}" />
					</xsl:otherwise>
				</xsl:choose>
			</td>
			<td>

				<xsl:call-template name="t_getInternalOnlyDescription" />
				<xsl:if test="attributes/attribute/type[@api='T:System.ObsoleteAttribute']">
					<xsl:text> </xsl:text>
					<include item="obsoleteRed" />
				</xsl:if>
				<xsl:call-template name="t_getElementDescription" />
			</td>
		</tr>
	</xsl:template>

	<xsl:template match="element"
								mode="members">
		<xsl:param name="subgroup"/>
		<xsl:if test="memberdata[@visibility='public'] and apidata[@subgroup=$g_apiTopicSubGroup]">
			public;
		</xsl:if>
		<xsl:if test="memberdata[@visibility='family' or @visibility='family or assembly' or @visibility='assembly'] and apidata[@subgroup=$g_apiTopicSubGroup]">
			protected;
		</xsl:if>
		<xsl:if test="memberdata[@visibility='private'] and apidata[@subgroup=$g_apiTopicSubGroup] and not(proceduredata[@virtual = 'true'])">
			private;
		</xsl:if>
		<xsl:if test="memberdata[@visibility='private'] and proceduredata[@virtual = 'true']">
			explicit;
		</xsl:if>
	</xsl:template>

	<xsl:template match="element"
								mode="memberlistRow">
		<xsl:param name="p_showParameters"
							 select="'false'" />
		<xsl:variable name="notsupportedOnNetfw">
			<xsl:call-template name="t_isMemberUnsupportedOnNetfw"/>
		</xsl:variable>
		<xsl:variable name="v_supportedOnXna">
			<xsl:call-template name="t_isMemberSupportedOnXna"/>
		</xsl:variable>
		<xsl:variable name="v_supportedOnCf">
			<xsl:call-template name="t_isMemberSupportedOnCf"/>
		</xsl:variable>
		<xsl:variable name="supportedOnSilverlight">
			<xsl:call-template name="IsMemberSupportedOnSilverlight"/>
		</xsl:variable>
		<xsl:variable name="v_supportedOnSilverlightMobile">
			<xsl:call-template name="t_isMemberSupportedOnSilverlightMobile" />
		</xsl:variable>
		<xsl:variable name="v_staticMember">
			<xsl:call-template name="t_isMemberStatic"/>
		</xsl:variable>
		<xsl:variable name="inheritedMember">
			<xsl:call-template name="t_isMemberInherited"/>
		</xsl:variable>
		<xsl:variable name="declaredMember">
			<xsl:call-template name="t_isMemberDeclared"/>
		</xsl:variable>
		<xsl:variable name="protectedMember">
			<xsl:call-template name="t_isMemberProtected"/>
		</xsl:variable>
		<xsl:variable name="publicMember">
			<xsl:call-template name="t_isMemberPublic"/>
		</xsl:variable>
		<xsl:variable name="privateMember">
			<xsl:call-template name="t_isMemberPrivate"/>
		</xsl:variable>
		<xsl:variable name="explicitMember">
			<xsl:call-template name="t_isMemberExplicit" />
		</xsl:variable>
		<xsl:variable name="conversionOperator">
			<xsl:call-template name="t_isConversionOperator" />
		</xsl:variable>
		<!-- do not show non-static members of static types -->
		<xsl:if test=".//memberdata/@static='true' or not(/document/reference/typedata[@abstract='true' and @sealed='true'])">
			<tr>
				<xsl:attribute name="data">
					<!-- it's possible to include both public and protected for overload topics -->
					<xsl:if test="normalize-space($publicMember)!=''">
						<xsl:text>public;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($protectedMember)!=''">
						<xsl:text>protected;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($privateMember)!=''">
						<xsl:text>private;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($explicitMember) != ''">
						<xsl:text>explicit;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($v_staticMember)!=''">
						<xsl:text>static;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($declaredMember)!=''">
						<xsl:text>declared;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($inheritedMember)!=''">
						<xsl:text>inherited;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($v_supportedOnCf)!=''">
						<xsl:text>netcfw;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($v_supportedOnXna)!=''">
						<xsl:text>xnafw;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($supportedOnSilverlight) != ''">
						<xsl:text>silverlight;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($v_supportedOnSilverlightMobile) != ''">
						<xsl:text>silverlight_mobile;</xsl:text>
					</xsl:if>
					<xsl:if test="normalize-space($notsupportedOnNetfw)!=''">
						<xsl:text>notNetfw;</xsl:text>
					</xsl:if>
				</xsl:attribute>
				<!--
        <xsl:if test="normalize-space($declaredMember)=''">
          <xsl:attribute name="name">inheritedMember</xsl:attribute>
        </xsl:if>
        <xsl:if test="normalize-space($protectedMember)!=''">
          <xsl:attribute name="protected">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="normalize-space($notsupportedOnNetfw)!=''">
          <xsl:attribute name="notsupportedOnNetfw">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="normalize-space($v_supportedOnXna)=''">
          <xsl:attribute name="notSupportedOnXna">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="normalize-space($v_supportedOnCf)=''">
          <xsl:attribute name="notSupportedOn">netcf</xsl:attribute>
        </xsl:if>
-->
				<td>
					<xsl:call-template name="t_putMemberIcons">
						<xsl:with-param name="p_memberVisibility">
							<xsl:choose>
								<xsl:when test="normalize-space($publicMember)!=''">pub</xsl:when>
								<xsl:when test="normalize-space($protectedMember)!=''">prot</xsl:when>
								<xsl:when test="memberdata/@visibility='private'">priv</xsl:when>
								<xsl:otherwise>pub</xsl:otherwise>
							</xsl:choose>
						</xsl:with-param>
						<xsl:with-param name="p_staticMember"
														select="normalize-space($v_staticMember)" />
						<xsl:with-param name="p_supportedOnXna"
														select="normalize-space($v_supportedOnXna)"/>
						<xsl:with-param name="p_supportedOnCf"
														select="normalize-space($v_supportedOnCf)"/>
						<xsl:with-param name="p_supportedOnSilverlight"
														select="normalized-space($supportedOnSilverlight)" />
						<xsl:with-param name="p_supportedOnSilverlightMobile"
														select="normalize-space($v_supportedOnSilverlightMobile)" />
					</xsl:call-template>
				</td>
				<td>
					<xsl:choose>
						<xsl:when test="normalize-space($conversionOperator)!=''">
							<referenceLink target="{@api}"
														 show-parameters="true" />
						</xsl:when>
						<xsl:when test="memberdata[@overload] or starts-with(../@api, 'Overload:')">
							<referenceLink target="{@api}"
														 show-parameters="true" />
						</xsl:when>
						<xsl:when test="@source='extension'">
							<xsl:call-template name="t_putExtensionMethodDisplayLink"/>
						</xsl:when>
						<xsl:when test="@display-api">
							<referenceLink target="{@api}"
														 display-target="{@display-api}"
														 show-parameters="{$p_showParameters}" />
						</xsl:when>
						<xsl:otherwise>
							<referenceLink target="{@api}"
														 show-parameters="{$p_showParameters}" />
						</xsl:otherwise>
					</xsl:choose>
				</td>
				<td>
					<xsl:call-template name="t_getInternalOnlyDescription" />
					<xsl:if test="attributes/attribute/type[@api='T:System.ObsoleteAttribute']">
						<xsl:text> </xsl:text>
						<include item="obsoleteRed" />
					</xsl:if>
					<xsl:if test="topicdata[@subgroup='overload'] or @overload='true'">
						<include item="Overloaded"/>
						<xsl:text> </xsl:text>
					</xsl:if>
					<xsl:apply-templates select="element"
															 mode="overloadSummary" />
					<xsl:call-template name="t_getElementDescription" />
					<xsl:choose>
						<xsl:when test="not(topicdata[@subgroup='overload'])">
							<xsl:choose>
								<xsl:when test="@source='extension' and containers/type">
									<xsl:text> </xsl:text>
									<include item="definedBy">
										<parameter>
											<xsl:apply-templates select="containers/type"
																					 mode="link" />
										</parameter>
									</include>
								</xsl:when>
								<xsl:when test="normalize-space($inheritedMember)!=''">
									<xsl:text> </xsl:text>
									<include item="inheritedFrom">
										<parameter>
											<xsl:apply-templates select="containers/type"
																					 mode="link" />
										</parameter>
									</include>
								</xsl:when>
								<xsl:when test="overrides/member">
									<xsl:text> </xsl:text>
									<include item="overridesMember">
										<parameter>
											<xsl:apply-templates select="overrides/member"
																					 mode="link" />
										</parameter>
									</include>
								</xsl:when>
							</xsl:choose>
						</xsl:when>
					</xsl:choose>

					<!-- add boilerplate for other members in the signature set -->
					<xsl:if test="@signatureset and element">
						<xsl:variable name="primaryMember">
							<xsl:copy-of select="."/>
						</xsl:variable>
						<xsl:variable name="primaryFramework"
													select="versions/versions[1]/@name"/>
						<xsl:for-each select="versions/versions[@name!=$primaryFramework]">
							<xsl:variable name="secondaryFramework"
														select="@name"/>
							<xsl:if test="(msxsl:node-set($primaryMember)/*[not(@*[local-name()=$secondaryFramework])]) and (msxsl:node-set($primaryMember)/*[element[@*[local-name()=$secondaryFramework]]])">
								<xsl:for-each select="msxsl:node-set($primaryMember)/*/element[@*[local-name()=$secondaryFramework]][1]">
									<xsl:variable name="inheritedSecondaryMember">
										<xsl:call-template name="t_isMemberInherited"/>
									</xsl:variable>
									<xsl:choose>
										<xsl:when test="overrides">
											<span data="{$secondaryFramework}">
												<include item="secondaryFrameworkOverride">
													<parameter>
														<xsl:value-of select="$secondaryFramework"/>
													</parameter>
													<!--<parameter>
                            <xsl:value-of select="@*[local-name()=$secondaryFramework]"/>
                          </parameter>-->
													<parameter>
														<referenceLink target="{@api}"/>
													</parameter>
												</include>
											</span>
										</xsl:when>
										<xsl:when test="normalize-space($inheritedSecondaryMember)!=''">
											<span data="{$secondaryFramework}">
												<include item="secondaryFrameworkInherited">
													<parameter>
														<xsl:value-of select="$secondaryFramework"/>
													</parameter>
													<parameter>
														<xsl:value-of select="@*[local-name()=$secondaryFramework]"/>
													</parameter>
													<parameter>
														<xsl:call-template name="t_decoratedNameSep"/>
													</parameter>
													<parameter>
														<xsl:apply-templates select="containers/type"
																								 mode="link" />
													</parameter>
													<parameter>
														<referenceLink target="{@api}"/>
													</parameter>
												</include>
											</span>
										</xsl:when>
										<xsl:otherwise>
											<span data="{$secondaryFramework}">
												<include item="secondaryFrameworkMember">
													<parameter>
														<xsl:value-of select="$secondaryFramework"/>
													</parameter>
													<parameter>
														<xsl:value-of select="@*[local-name()=$secondaryFramework]"/>
													</parameter>
													<parameter>
														<referenceLink target="{@api}"/>
													</parameter>
												</include>
											</span>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:if>
						</xsl:for-each>
					</xsl:if>

				</td>
			</tr>
		</xsl:if>
	</xsl:template>

	<xsl:template match="element"
								mode="overloadSummary">
		<xsl:call-template name="t_getOverloadSummary" />
	</xsl:template>

	<xsl:template match="element"
								mode="overloadSections">
		<xsl:call-template name="t_getOverloadSections" />
	</xsl:template>

	<!-- ============================================================================================
	element helpers
	============================================================================================= -->

	<xsl:template name="t_putTypeIcon">
		<xsl:param name="p_typeVisibility" />

		<xsl:variable name="typeSubgroup"
									select="apidata/@subgroup" />
		<img>
			<includeAttribute name="src"
												item="iconPath">
				<parameter>
					<xsl:value-of select="concat($p_typeVisibility,$typeSubgroup,'.gif')" />
				</parameter>
			</includeAttribute>
			<includeAttribute name="alt"
												item="{concat($p_typeVisibility,$typeSubgroup,'AltText')}" />
			<includeAttribute name="title"
												item="{concat($p_typeVisibility,$typeSubgroup,'AltText')}" />
		</img>

	</xsl:template>

	<xsl:template name="t_putMemberIcons">
		<xsl:param name="p_memberVisibility" />
		<xsl:param name="p_staticMember" />
		<xsl:param name="p_supportedOnXna"/>
		<xsl:param name="p_supportedOnCf"/>
		<xsl:param name="p_supportedOnSilverlightMobile" />

		<xsl:variable name="v_memberSubgroup">
			<xsl:choose>
				<xsl:when test="apidata/@subgroup='constructor'">
					<xsl:text>method</xsl:text>
				</xsl:when>
				<xsl:when test="apidata/@subgroup='method'">
					<xsl:choose>
						<xsl:when test="apidata/@subsubgroup='operator'">
							<xsl:text>operator</xsl:text>
						</xsl:when>
						<xsl:when test="apidata/@subsubgroup='extension'">
							<xsl:text>extension</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>method</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="apidata/@subgroup" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<!-- test for explicit interface implementations, which get the interface icon -->
		<xsl:if test="memberdata/@visibility='private' and proceduredata/@virtual='true'">
			<img>
				<includeAttribute name="src"
													item="iconPath">
					<parameter>pubinterface.gif</parameter>
				</includeAttribute>
				<includeAttribute name="alt"
													item="ExplicitInterfaceAltText" />
				<includeAttribute name="title"
													item="ExplicitInterfaceAltText" />
			</img>
		</xsl:if>

		<img>
			<includeAttribute name="src"
												item="iconPath">
				<parameter>
					<xsl:value-of select="concat($p_memberVisibility,$v_memberSubgroup,'.gif')" />
				</parameter>
			</includeAttribute>
			<xsl:choose>
				<xsl:when test="apidata/@subsubgroup">
					<includeAttribute name="alt"
														item="{concat($p_memberVisibility,apidata/@subsubgroup,'AltText')}" />
					<includeAttribute name="title"
														item="{concat($p_memberVisibility,apidata/@subsubgroup,'AltText')}" />
				</xsl:when>
				<xsl:otherwise>
					<includeAttribute name="alt"
														item="{concat($p_memberVisibility,$v_memberSubgroup,'AltText')}" />
					<includeAttribute name="title"
														item="{concat($p_memberVisibility,$v_memberSubgroup,'AltText')}" />
				</xsl:otherwise>
			</xsl:choose>
		</img>

		<xsl:if test="$p_staticMember!=''">
			<img>
				<includeAttribute name="src"
													item="iconPath">
					<parameter>static.gif</parameter>
				</includeAttribute>
				<includeAttribute name="alt"
													item="staticAltText" />
				<includeAttribute name="title"
													item="staticAltText" />
			</img>
		</xsl:if>

		<xsl:if test="$p_supportedOnCf!=''">
			<img data="netcfw">
				<includeAttribute name="src"
													item="iconPath">
					<parameter>CFW.gif</parameter>
				</includeAttribute>
				<includeAttribute name="alt"
													item="CompactFrameworkAltText" />
				<includeAttribute name="title"
													item="CompactFrameworkAltText" />
			</img>
		</xsl:if>

		<xsl:if test="$p_supportedOnXna!=''">
			<img data="xnafw">
				<includeAttribute name="src"
													item="iconPath">
					<parameter>xna.gif</parameter>
				</includeAttribute>
				<includeAttribute name="alt"
													item="XNAFrameworkAltText" />
				<includeAttribute name="title"
													item="XNAFrameworkAltText" />
			</img>
		</xsl:if>

		<xsl:if test="$p_supportedOnSilverlightMobile!=''">
			<img data="silverlight_mobile">
				<includeAttribute name="src"
													item="iconPath">
					<parameter>slMobile.gif</parameter>
				</includeAttribute>
				<includeAttribute name="alt"
													item="SilverlightMobileAltText" />
				<includeAttribute name="title"
													item="SilverlightMobileAltText" />
			</img>
		</xsl:if>

	</xsl:template>

	<xsl:template name="t_getEnumMemberDescription">
		<xsl:apply-templates select="summary[1]/node()" />
		<!-- enum members may have additional authored content in the remarks node -->
		<xsl:apply-templates select="remarks[1]/node()" />
	</xsl:template>

	<xsl:template name="t_putExtensionMethodDisplayLink">
		<xsl:variable name="v_showParameters">
			<xsl:choose>
				<xsl:when test="@overload='true'">true</xsl:when>
				<xsl:otherwise>false</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<referenceLink target="{@api}"
									 display-target="extension"
									 show-parameters="{$v_showParameters}">
			<extensionMethod>
				<xsl:copy-of select="@*"/>
				<xsl:copy-of select="apidata|templates|parameters|containers"/>
			</extensionMethod>
		</referenceLink>
	</xsl:template>

	<!-- ============================================================================================
	Inheritance hierarchy
	============================================================================================= -->

	<xsl:template match="family">

		<xsl:call-template name="t_putSection">
			<xsl:with-param name="toggleSwitch"
											select="'family'"/>
			<xsl:with-param name="title">
				<include item="familyTitle" />
			</xsl:with-param>
			<xsl:with-param name="content">
				<xsl:variable name="ancestorCount"
											select="count(ancestors/*)" />
				<xsl:variable name="childCount"
											select="count(descendents/*)" />

				<xsl:for-each select="ancestors/type">
					<xsl:sort select="position()"
										data-type="number"
										order="descending" />
					<!-- <xsl:sort select="@api"/> -->

					<xsl:call-template name="t_putIndent">
						<xsl:with-param name="p_count"
														select="position()" />
					</xsl:call-template>

					<xsl:apply-templates select="self::type"
															 mode="link">
						<xsl:with-param name="qualified"
														select="true()" />
					</xsl:apply-templates>

					<br/>
				</xsl:for-each>

				<xsl:call-template name="t_putIndent">
					<xsl:with-param name="p_count"
													select="$ancestorCount + 1" />
				</xsl:call-template>
				<referenceLink target="{$key}"
											 qualified="true"/>
				<br/>

				<xsl:choose>
					<xsl:when test="descendents/@derivedTypes">
						<xsl:call-template name="t_putIndent">
							<xsl:with-param name="p_count"
															select="$ancestorCount + 2" />
						</xsl:call-template>
						<referenceLink target="{descendents/@derivedTypes}"
													 qualified="true">
							<include item="derivedClasses"/>
						</referenceLink>
					</xsl:when>
					<xsl:otherwise>

						<xsl:for-each select="descendents/type">
							<xsl:sort select="@api" />

							<xsl:if test="not(self::type/@api=preceding-sibling::*/self::type/@api)">
								<xsl:call-template name="t_putIndent">
									<xsl:with-param name="p_count"
																	select="$ancestorCount + 2" />
								</xsl:call-template>

								<xsl:apply-templates select="self::type"
																		 mode="link">
									<xsl:with-param name="qualified"
																	select="true()" />
								</xsl:apply-templates>

								<br/>
							</xsl:if>
						</xsl:for-each>
					</xsl:otherwise>
				</xsl:choose>

			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="implements">
		<xsl:if test="member">
			<xsl:call-template name="t_putSubSection">
				<xsl:with-param name="title">
					<include item="implementsTitle" />
				</xsl:with-param>
				<xsl:with-param name="content">
					<xsl:for-each select="member">
						<referenceLink target="{@api}"
													 qualified="true" />
						<br />
					</xsl:for-each>
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- ============================================================================================
	Member attribute tests
	============================================================================================= -->

	<xsl:template name="t_isMemberUnsupportedOnNetfw">
		<xsl:if test="boolean(not(@netfw) and not(element/@netfw))">
			<xsl:text>unsupported</xsl:text>
		</xsl:if>
	</xsl:template>

	<!-- for testing CF and XNA support, check the signature variations of @signatureset elements -->
	<!-- for testing inherited/protected/etc, do not check the @signatureset variations; just go with the primary .NET Framework value -->
	<xsl:template name="t_isMemberSupportedOnXna">
		<xsl:choose>
			<xsl:when test="element">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberSupportedOnXna"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="v_platformFilterExcludesXna"
											select="boolean(platforms and not(platforms/platform[.='Xbox360']))" />
				<xsl:if test="boolean(not($v_platformFilterExcludesXna) and (@xnafw or element/@xnafw))">
					<xsl:text>supported</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_isMemberSupportedOnCf">
		<xsl:choose>
			<xsl:when test="element">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberSupportedOnCf"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="v_platformFilterExcludesCF"
											select="boolean( platforms and not(platforms[platform[.='PocketPC'] or platform[.='SmartPhone'] or platform[.='WindowsCE']]) )" />
				<xsl:if test="boolean(not($v_platformFilterExcludesCF) and (@netcfw or element/@netcfw))">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_isMemberSupportedOnSilverlightMobile">
		<xsl:choose>
			<xsl:when test="element">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberSupportedOnSilverlightMobile"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="platformFilterExcludesSilverlightMobile"
											select="boolean( platforms and not(platforms[platform[.='SilverlightPlatforms']]) )" />
				<xsl:if test="boolean(not($platformFilterExcludesSilverlightMobile) and (@silverlight_mobile or element/@silverlight_mobile))">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="IsMemberSupportedOnSilverlight">
		<xsl:choose>
			<xsl:when test="element">
				<xsl:for-each select="element">
					<xsl:call-template name="IsMemberSupportedOnSilverlight"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="v_platformFilterExcludesSilverlight"
											select="boolean( platforms and not(platforms[platform[.='SilverlightPlatforms']]) )" />
				<xsl:if test="boolean(not($v_platformFilterExcludesSilverlight) and (@silverlight or element/@silverlight))">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>


	<xsl:template name="t_isMemberStatic">
		<xsl:choose>
			<xsl:when test="element and not(@signatureset)">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberStatic"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:when test="apidata[@subsubgroup='attachedProperty' or @subsubgroup='attachedEvent']"/>
			<xsl:otherwise>
				<xsl:if test="memberdata/@static='true'">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- returns a non-empty string if the element is inherited, or for overloads if any of the overloads is inherited -->
	<xsl:template name="t_isMemberInherited">
		<xsl:choose>
			<xsl:when test="element and not(@signatureset)">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberInherited"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="containers/type[@api!=$g_topicTypeId]">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- returns a non-empty string if the element is declared, or for overloads if any of the overloads is declared -->
	<xsl:template name="t_isMemberDeclared">
		<xsl:choose>
			<xsl:when test="element and not(@signatureset)">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberDeclared"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="containers/type[@api=$g_topicTypeId]">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_isMemberPublic">
		<xsl:choose>
			<xsl:when test="element and not(@signatureset)">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberPublic"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="memberdata[@visibility='public']">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_isMemberProtected">
		<xsl:choose>
			<xsl:when test="element and not(@signatureset)">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberProtected"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="memberdata[@visibility='family' or @visibility='family or assembly' or @visibility='assembly']">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_isMemberPrivate">
		<xsl:choose>
			<xsl:when test="element and not(@signatureset)">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberPrivate"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="memberdata[@visibility='private'] and not(proceduredata[@virtual = 'true'])">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_isMemberExplicit">
		<xsl:choose>
			<xsl:when test="element">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isMemberExplicit"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="memberdata[@visibility='private'] and proceduredata[@virtual = 'true']">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_isConversionOperator">
		<xsl:choose>
			<xsl:when test="element">
				<xsl:for-each select="element">
					<xsl:call-template name="t_isConversionOperator"/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:if test="apidata/@subsubgroup='operator' and (apidata/@name='Explicit' or apidata/@name='Implicit') and not(memberdata/@overload)">
					<xsl:text>yes</xsl:text>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- ============================================================================================
	Syntax
	============================================================================================= -->

	<xsl:template name="syntaxBlocks">

		<xsl:call-template name="t_putCodeSections">
			<xsl:with-param name="p_codeNodes"
											select="/document/syntax/div[@codeLanguage]" />
			<xsl:with-param name="p_nodeCount"
											select="count(/document/syntax/div[@codeLanguage])" />
			<xsl:with-param name="p_codeLangAttr"
											select="'codeLanguage'" />
			<xsl:with-param name="p_formatCode"
											select="false()" />
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="t_isCodeLangValid">
		<xsl:param name="p_codeLang" />
		<xsl:value-of select="true()"/>

		<xsl:choose>
			<xsl:when test="$p_codeLang='JSharp'">
				<xsl:choose>
					<xsl:when test="not(/document/reference/versions) or boolean(/document/reference/versions/versions[@name='netfw']//version[not(@name='netfw35')])">
						<xsl:value-of select="false()"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="true()"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="true()"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="parameters">
		<div id="parameters">
			<xsl:call-template name="t_putSubSection">
				<xsl:with-param name="title">
					<include item="parametersTitle"/>
				</xsl:with-param>
				<xsl:with-param name="content">
					<xsl:for-each select="parameter">

						<!-- Use the reflection-generated parameter name when non-empty, otherwise use the authored parameter name. -->
						<xsl:variable name="paramPosition"
													select="position()" />
						<xsl:variable name="paramName">
							<xsl:choose>
								<xsl:when test="normalize-space(@name) != ''">
									<xsl:value-of select="normalize-space(@name)" />
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="normalize-space(/document/comments/ddue:dduexml/ddue:parameters[1]/ddue:parameter[$paramPosition]/ddue:parameterReference)" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>

						<dl paramName="{$paramName}">
							<dt>
								<span class="parameter">
									<xsl:value-of select="$paramName"/>
								</span>
							</dt>
							<dd>
								<include item="typeLink">
									<parameter>
										<xsl:apply-templates select="*[1]"
																				 mode="link">
											<xsl:with-param name="qualified"
																			select="true()" />
										</xsl:apply-templates>
									</parameter>
								</include>
								<br/>
								<span>
									<xsl:call-template name="t_getParameterDescription">
										<xsl:with-param name="name"
																		select="$paramName" />
									</xsl:call-template>
								</span>
							</dd>
						</dl>
					</xsl:for-each>
				</xsl:with-param>
			</xsl:call-template>
		</div>
	</xsl:template>

	<!-- ======================================================================================== -->

	<!-- fix for template in shared utilities_reference -->
	<xsl:template match="arrayOf"
								mode="link">
		<xsl:param name="qualified"
							 select="false()" />
		<span class="languageSpecificText">
			<span class="cpp">array&lt;</span>
			<span class="cs"></span>
			<span class="vb"></span>
			<span class="fs"></span>
			<span class="nu"></span>
		</span>
		<xsl:apply-templates mode="link">
			<xsl:with-param name="qualified"
											select="$qualified" />
		</xsl:apply-templates>
		<span class="languageSpecificText">
			<span class="cpp">
				<xsl:if test="number(@rank) &gt; 1">
					<xsl:text>,</xsl:text>
					<xsl:value-of select="@rank"/>
				</xsl:if>
				<xsl:text>&gt;</xsl:text>
			</span>
			<span class="cs">
				<xsl:text>[</xsl:text>
				<xsl:if test="number(@rank) &gt; 1">,</xsl:if>
				<xsl:text>]</xsl:text>
			</span>
			<span class="vb">
				<xsl:text>(</xsl:text>
				<xsl:if test="number(@rank) &gt; 1">,</xsl:if>
				<xsl:text>)</xsl:text>
			</span>
			<span class="nu">
				<xsl:text>[</xsl:text>
				<xsl:if test="number(@rank) &gt; 1">,</xsl:if>
				<xsl:text>]</xsl:text>
			</span>
			<span class="fs">
				<xsl:text>[</xsl:text>
				<xsl:if test="number(@rank) &gt; 1">,</xsl:if>
				<xsl:text>]</xsl:text>
			</span>
		</span>
	</xsl:template>

	<!-- fix for template in shared utilities_reference -->
	<xsl:template match="pointerTo"
								mode="decorated">
		<xsl:apply-templates select="type|arrayOf|pointerTo|referenceTo|template|specialization|templates"
												 mode="decorated"/>
		<span class="languageSpecificText">
			<span class="cpp">
				<xsl:text>*</xsl:text>
			</span>
			<span class="cs"></span>
			<span class="vb"></span>
			<span class="fs"></span>
			<span class="nu"></span>
		</span>
	</xsl:template>

	<!-- fix for template in shared utilities_reference -->
	<xsl:template match="referenceTo"
								mode="decorated">
		<xsl:apply-templates select="type|arrayOf|pointerTo|referenceTo|template|specialization|templates"
												 mode="decorated"/>
		<span class="languageSpecificText">
			<span class="cpp">%</span>
			<span class="cs"></span>
			<span class="vb"></span>
			<span class="fs"></span>
			<span class="nu"></span>
		</span>
	</xsl:template>

	<!-- fix for template in shared utilities_reference -->
	<xsl:template match="referenceTo"
								mode="link">
		<xsl:param name="qualified"
							 select="false()" />
		<xsl:apply-templates mode="link">
			<xsl:with-param name="qualified"
											select="$qualified" />
		</xsl:apply-templates>
		<span class="languageSpecificText">
			<span class="cpp">%</span>
			<span class="cs"></span>
			<span class="vb"></span>
			<span class="fs"></span>
			<span class="nu"></span>
		</span>
	</xsl:template>

	<!-- ============================================================================================
	Assembly information
	============================================================================================= -->

	<xsl:template name="t_putRequirementsInfo">
		<p/>
		<include item="requirementsNamespaceLayout" />
		<xsl:text>&#xa0;</xsl:text>
		<referenceLink target="{/document/reference/containers/namespace/@api}" />
		<br/>
		<xsl:call-template name="t_putAssembliesInfo"/>

		<!-- some apis display a XAML xmlns uri -->
		<xsl:if test="$omitXmlnsBoilerplate != 'true'">
			<xsl:call-template name="xamlXmlnsInfo"/>
		</xsl:if>
	</xsl:template>

	<xsl:template name="t_putAssembliesInfo">
		<xsl:choose>
			<xsl:when test="count(/document/reference/containers/library)&gt;1">
				<include item="requirementsAssembliesLabel"/>
				<xsl:for-each select="/document/reference/containers/library">
					<xsl:text>&#xa0;&#xa0;</xsl:text>
					<xsl:call-template name="t_putAssemblyNameAndModule">
						<xsl:with-param name="library"
														select="."/>
					</xsl:call-template>
					<br/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<include item="requirementsAssemblyLabel"/>
				<xsl:text>&#xa0;</xsl:text>
				<xsl:call-template name="t_putAssemblyNameAndModule"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="t_putAssemblyNameAndModule">
		<xsl:param name="library"
							 select="/document/reference/containers/library"/>
		<include item="assemblyNameAndModule">
			<parameter>
				<span sdata="assembly">
					<xsl:value-of select="$library/@assembly"/>
				</span>
			</parameter>
			<parameter>
				<xsl:value-of select="$library/@module"/>
			</parameter>
			<parameter>
				<xsl:choose>
					<xsl:when test="$library/@kind = 'DynamicallyLinkedLibrary'">
						<xsl:text>dll</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>exe</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</parameter>
		</include>
	</xsl:template>

	<!-- ============================================================================================
	Platform information
	============================================================================================= -->

	<xsl:template match="platforms[platform]">
		<xsl:call-template name="t_putSection">
			<xsl:with-param name="toggleSwitch"
											select="'platformsTitle'"/>
			<xsl:with-param name="title">
				<include item="platformsTitle" />
			</xsl:with-param>
			<xsl:with-param name="content">
				<xsl:choose>
					<xsl:when test="/document/reference/versions/versions[@name='silverlight']//version">
						<p>
							<include item="silverlightplatforms"/>
						</p>
					</xsl:when>
					<xsl:otherwise>
						<p>
							<xsl:for-each select="platform">
								<include item="{.}" />
								<xsl:if test="position()!=last()">
									<xsl:text>, </xsl:text>
								</xsl:if>
							</xsl:for-each>
						</p>
						<xsl:if test="/document/reference/versions/versions[@name='netfw' or @name='netcfw']//version">
							<p>
								<include item="SystemRequirementsLinkBoilerplate"/>
							</p>
						</xsl:if>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<!-- ============================================================================================
	Version information
	============================================================================================= -->

	<xsl:template match="versions">
		<xsl:if test="$omitVersionInformation != 'true'">
			<xsl:call-template name="t_putSection">
				<xsl:with-param name="toggleSwitch"
												select="'versionsTitle'"/>
				<xsl:with-param name="title">
					<include item="versionsTitle" />
				</xsl:with-param>
				<xsl:with-param name="content">
					<xsl:call-template name="t_processVersions" />
				</xsl:with-param>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template name="t_processVersions">
		<xsl:param name="p_frameworkGroup"
							 select="true()"/>
		<xsl:choose>
			<xsl:when test="versions and $p_frameworkGroup">
				<xsl:for-each select="versions">
					<!-- $v_platformFilterExcluded is based on platform filtering information -->
					<xsl:variable name="v_platformFilterExcluded"
												select="boolean(/document/reference/platforms and ( (@name='netcfw' and not(/document/reference/platforms/platform[.='PocketPC']) and not(/document/reference/platforms/platform[.='SmartPhone']) and not(/document/reference/platforms/platform[.='WindowsCE']) ) or (@name='xnafw' and not(/document/reference/platforms/platform[.='Xbox360']) ) ) )" />
					<xsl:if test="not($v_platformFilterExcluded) and count(.//version) &gt; 0">
						<h4 class ="subHeading">
							<include item="{@name}" />
						</h4>
						<xsl:call-template name="t_processVersions">
							<xsl:with-param name="p_frameworkGroup"
															select="false()"/>
						</xsl:call-template>
					</xsl:if>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<!-- show the versions in which the api is supported, if any -->
				<xsl:variable name="v_supportedCount"
											select="count(version[not(@obsolete)] | versions[version[not(@obsolete)]])"/>
				<xsl:if test="$v_supportedCount &gt; 0">
					<include item="supportedIn_{$v_supportedCount}">
						<xsl:for-each select="version[not(@obsolete)] | versions[version[not(@obsolete)]]">
							<xsl:variable name="versionName">
								<xsl:choose>
									<!-- A versions[version] node at this level is for releases that had subsequent service packs. 
                       For example, versions for .NET 3.0 has version nodes for 3.0 and 3.0 SP1. 
                       We show only the first node, which is the one in which the api was first released, 
                       that is, we show 3.0 SP1 only if the api was introduced in SP1. -->
									<xsl:when test="local-name()='versions'">
										<xsl:value-of select="version[not(@obsolete)][not(preceding-sibling::version[not(@obsolete)])]/@name"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="@name"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:variable>
							<parameter>
								<include item="{$versionName}" />
							</parameter>
						</xsl:for-each>
					</include>
					<br/>
				</xsl:if>
				<!-- show the versions in which the api is obsolete with a compiler warning, if any -->
				<xsl:for-each select=".//version[@obsolete='warning']">
					<include item="obsoleteWarning">
						<parameter>
							<include item="{@name}" />
						</parameter>
					</include>
					<br/>
				</xsl:for-each>
				<!-- show the versions in which the api is obsolete and does not compile, if any -->
				<xsl:for-each select=".//version[@obsolete='error']">
					<xsl:if test="position()=last()">
						<include item="obsoleteError">
							<parameter>
								<include item="{@name}" />
							</parameter>
						</include>
						<br/>
					</xsl:if>
				</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- ============================================================================================
	Decorated names
	============================================================================================= -->

	<xsl:template name="typeNameDecorated">
		<xsl:if test="type|(containers/type)">
			<xsl:apply-templates select="type|(containers/type)"
													 mode="decorated" />
			<xsl:call-template name="t_decoratedNameSep"/>
		</xsl:if>
		<xsl:value-of select="apidata/@name" />
		<xsl:choose>
			<xsl:when test="specialization">
				<xsl:apply-templates select="specialization"
														 mode="decorated" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:apply-templates select="templates"
														 mode="decorated" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="shortNameDecorated">
		<xsl:choose>
			<!-- type overview pages and member list pages get the type name -->
			<xsl:when test="($g_topicGroup='api' and $g_apiGroup='type') or ($g_topicGroup='list' and not($g_topicSubGroup='overload'))">
				<xsl:for-each select="/document/reference[1]">
					<xsl:call-template name="typeNameDecorated" />
				</xsl:for-each>
			</xsl:when>
			<!-- constructors and member list pages also use the type name -->
			<xsl:when test="($g_topicGroup='api' and $g_apiSubGroup='constructor') or ($g_topicSubGroup='overload' and $g_apiSubGroup='constructor')">
				<xsl:for-each select="/document/reference/containers/type[1]">
					<xsl:call-template name="typeNameDecorated" />
				</xsl:for-each>
			</xsl:when>
			<!-- eii members -->
			<xsl:when test="document/reference[memberdata[@visibility='private'] and proceduredata[@virtual = 'true']]">
				<xsl:for-each select="/document/reference/containers/type[1]">
					<xsl:call-template name="typeNameDecorated" />
				</xsl:for-each>
				<xsl:call-template name="t_decoratedNameSep"/>
				<xsl:for-each select="/document/reference/implements/member">
					<xsl:for-each select="type">
						<xsl:call-template name="typeNameDecorated" />
					</xsl:for-each>
					<xsl:call-template name="t_decoratedNameSep"/>
					<xsl:value-of select="apidata/@name" />
					<xsl:apply-templates select="templates"
															 mode="decorated" />
				</xsl:for-each>
			</xsl:when>
			<!-- Use just the plain, unadorned type.api name for overload pages with templates -->
			<xsl:when test="$g_topicGroup='list' and $g_topicSubGroup='overload' and /document/reference/templates">
				<xsl:for-each select="/document/reference/containers/type[1]">
					<xsl:call-template name="typeNameDecorated" />
				</xsl:for-each>
				<xsl:call-template name="t_decoratedNameSep"/>
				<xsl:value-of select="/document/reference/apidata/@name" />
			</xsl:when>
			<!-- normal member pages use the qualified member name -->
			<xsl:when test="($g_topicGroup='api' and $g_apiGroup='member') or ($g_topicSubGroup='overload' and $g_apiGroup='member')">
				<xsl:for-each select="/document/reference/containers/type[1]">
					<xsl:call-template name="typeNameDecorated" />
				</xsl:for-each>
				<xsl:if test="not($g_apiSubSubGroup='operator'and (document/reference/apidata/@name='Explicit' or document/reference/apidata/@name='Implicit'))">
					<xsl:call-template name="t_decoratedNameSep"/>
				</xsl:if>
				<xsl:for-each select="/document/reference[1]">
					<xsl:choose>
						<xsl:when test="$g_apiSubSubGroup='operator' and (apidata/@name='Explicit' or apidata/@name='Implicit')">
							<xsl:text>&#xa0;</xsl:text>
							<span class="languageSpecificText">
								<span class="cs">
									<xsl:value-of select="apidata/@name" />
								</span>
								<span class="vb">
									<xsl:choose>
										<xsl:when test="apidata/@name='Explicit'">
											<xsl:text>Narrowing</xsl:text>
										</xsl:when>
										<xsl:when test="apidata/@name='Implicit'">
											<xsl:text>Widening</xsl:text>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="apidata/@name" />
										</xsl:otherwise>
									</xsl:choose>
								</span>
								<span class="cpp">
									<xsl:value-of select="apidata/@name" />
								</span>
								<span class="fs">
									<xsl:value-of select="apidata/@name" />
								</span>
								<span class="nu">
									<xsl:value-of select="apidata/@name" />
								</span>
							</span>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="apidata/@name" />
						</xsl:otherwise>
					</xsl:choose>
					<xsl:apply-templates select="templates"
															 mode="decorated" />
				</xsl:for-each>
			</xsl:when>
			<!-- namespace (and any other) topics just use the name -->
			<xsl:when test="/document/reference/apidata/@name = ''">
				<include item="defaultNamespace" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/document/reference/apidata/@name" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- ============================================================================================
	Plain names
	============================================================================================= -->

	<xsl:template name="shortNamePlain">
		<xsl:param name="qualifyMembers"
							 select="false()" />
		<xsl:choose>
			<!-- type overview pages and member list pages get the type name -->
			<xsl:when test="($g_topicGroup='api' and $g_apiGroup='type') or ($g_topicGroup='list' and not($g_topicSubGroup='overload'))">
				<xsl:for-each select="/document/reference[1]">
					<xsl:call-template name="typeNamePlain" />
				</xsl:for-each>
			</xsl:when>
			<!-- constructors and member list pages also use the type name -->
			<xsl:when test="($g_topicGroup='api' and $g_apiSubGroup='constructor') or ($g_topicSubGroup='overload' and $g_apiSubGroup='constructor')">
				<xsl:for-each select="/document/reference/containers/type[1]">
					<xsl:call-template name="typeNamePlain" />
				</xsl:for-each>
			</xsl:when>
			<!-- member pages use the member name, qualified if the qualified flag is set -->
			<xsl:when test="($g_topicGroup='api' and $g_apiGroup='member') or ($g_topicSubGroup='overload' and $g_apiGroup='member')">
				<!-- check for qualify flag and qualify if it is set -->
				<xsl:if test="$qualifyMembers">
					<xsl:for-each select="/document/reference/containers/type[1]">
						<xsl:call-template name="typeNamePlain" />
					</xsl:for-each>
					<xsl:choose>
						<xsl:when test="$g_apiSubSubGroup='operator' and (document/reference/apidata/@name='Explicit' or document/reference/apidata/@name='Implicit')">
							<xsl:text>&#xa0;</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>.</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
				<xsl:choose>
					<!-- EII names are interfaceName.interfaceMemberName, not memberName -->
					<xsl:when test="document/reference[memberdata[@visibility='private'] and proceduredata[@virtual = 'true']]">
						<xsl:for-each select="/document/reference/implements/member">
							<xsl:for-each select="type">
								<xsl:call-template name="typeNamePlain" />
							</xsl:for-each>
							<xsl:text>.</xsl:text>
							<xsl:value-of select="apidata/@name" />
							<xsl:apply-templates select="templates"
																	 mode="plain" />
						</xsl:for-each>
					</xsl:when>
					<!-- Use just the plain, unadorned api name for overload pages with templates -->
					<xsl:when test="$g_topicGroup='list' and $g_topicSubGroup='overload' and /document/reference/templates">
						<xsl:value-of select="/document/reference/apidata/@name" />
					</xsl:when>
					<xsl:otherwise>
						<!-- but other members just use the name -->
						<xsl:for-each select="/document/reference[1]">
							<xsl:value-of select="apidata/@name" />
							<xsl:apply-templates select="templates"
																	 mode="plain" />
						</xsl:for-each>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<!-- namespace, member (and any other) topics just use the name -->
			<xsl:when test="/document/reference/apidata/@name = ''">
				<include item="defaultNamespace" />
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/document/reference/apidata/@name" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:stylesheet>
