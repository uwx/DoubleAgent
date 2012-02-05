<?xml version="1.0"?>
<xsl:stylesheet version="1.1" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:ms="urn:schemas-microsoft-com:xslt">

	<xsl:import href="CreateVSToc.xsl"/>

	<!-- for each namespace, create namespace entry and type sub-entries -->
	<xsl:template match="api[apidata/@group='namespace']">
		<topic id="{@id}" project="{$projectName}_Namespaces" file="{file/@name}">
			<xsl:if test="/reflection/apis/api[apidata/@group='type'][topicdata/@group='api'][containers/namespace/@api=current()/@id][apidata/@subgroup='class']">
				<topic id="Classes">
					<xsl:apply-templates select="key('index',elements/element/@api)" mode="class">
						<xsl:sort select="@id" />
					</xsl:apply-templates>
				</topic>
			</xsl:if>
      <xsl:if test="/reflection/apis/api[apidata/@group='type'][topicdata/@group='api'][containers/namespace/@api=current()/@id][apidata/@subgroup='structure']">
        <topic id="Structures">
          <xsl:apply-templates select="key('index',elements/element/@api)" mode="structure">
            <xsl:sort select="@id" />
          </xsl:apply-templates>
        </topic>
      </xsl:if>
      <xsl:if test="/reflection/apis/api[apidata/@group='type'][topicdata/@group='api'][containers/namespace/@api=current()/@id][apidata/@subgroup='interface']">
				<topic id="Interfaces">
					<xsl:apply-templates select="key('index',elements/element/@api)" mode="interface">
						<xsl:sort select="@id" />
					</xsl:apply-templates>
				</topic>
			</xsl:if>
			<xsl:if test="/reflection/apis/api[apidata/@group='type'][topicdata/@group='api'][containers/namespace/@api=current()/@id][apidata/@subgroup='enumeration']">
				<topic id="Enumerations">
					<xsl:apply-templates select="key('index',elements/element/@api)" mode="enumeration">
						<xsl:sort select="@id" />
					</xsl:apply-templates>
				</topic>
			</xsl:if>
			<xsl:if test="/reflection/apis/api[apidata/@group='type'][topicdata/@group='api'][containers/namespace/@api=current()/@id][apidata/@subgroup='delegate']">
				<topic id="Delegates">
					<xsl:apply-templates select="key('index',elements/element/@api)" mode="delegate">
						<xsl:sort select="@id" />
					</xsl:apply-templates>
				</topic>
			</xsl:if>
			<xsl:apply-templates select="key('index',elements/element/@api)" mode="other">
				<xsl:sort select="@id" />
			</xsl:apply-templates>
		</topic>
	</xsl:template>

	<!-- general type template -->
	<xsl:template match="api[apidata/@group='type'][topicdata[@group='api']]" mode="other">
		<xsl:call-template name="AddTypeNode"/>
	</xsl:template>

	<!-- class type template -->
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='class'][topicdata[@group='api']]" mode="class">
		<xsl:call-template name="AddTypeNode"/>
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup!='class'][topicdata[@group='api']]" mode="class">
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='class'][topicdata[@group='api']]" mode="other">
	</xsl:template>

  <!-- structure type template -->
  <xsl:template match="api[apidata/@group='type'][apidata/@subgroup='structure'][topicdata[@group='api']]" mode="structure">
    <xsl:call-template name="AddTypeNode"/>
  </xsl:template>
  <xsl:template match="api[apidata/@group='type'][apidata/@subgroup!='structure'][topicdata[@group='api']]" mode="structure">
  </xsl:template>
  <xsl:template match="api[apidata/@group='type'][apidata/@subgroup='structure'][topicdata[@group='api']]" mode="other">
  </xsl:template>

  <!-- interface type template -->
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='interface'][topicdata[@group='api']]" mode="interface">
		<xsl:call-template name="AddTypeNode"/>
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup!='interface'][topicdata[@group='api']]" mode="interface">
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='interface'][topicdata[@group='api']]" mode="other">
	</xsl:template>

	<!-- enumeration type template -->
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='enumeration'][topicdata[@group='api']]" mode="enumeration">
		<xsl:call-template name="AddTypeNode"/>
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup!='enumeration'][topicdata[@group='api']]" mode="enumeration">
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='enumeration'][topicdata[@group='api']]" mode="other">
	</xsl:template>

	<!-- delegate type template -->
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='delegate'][topicdata[@group='api']]" mode="delegate">
		<xsl:call-template name="AddTypeNode"/>
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup!='delegate'][topicdata[@group='api']]" mode="delegate">
	</xsl:template>
	<xsl:template match="api[apidata/@group='type'][apidata/@subgroup='delegate'][topicdata[@group='api']]" mode="other">
	</xsl:template>

	<!-- for each type, create type entry and either overload entries or member entries as sub-entries -->
	<xsl:template name="AddTypeNode">
		<xsl:variable name="componentName">
			<xsl:call-template name="GetComponentName">
				<xsl:with-param name="initialName" select="concat($projectPrefix,containers/library/@assembly)" />
			</xsl:call-template>
		</xsl:variable>
		<topic id="{@id}" project="{$componentName}" file="{file/@name}" subgroup="{apidata/@subgroup}">
			<xsl:call-template name="AddMemberListTopics"/>
		</topic>
	</xsl:template>

</xsl:stylesheet>
