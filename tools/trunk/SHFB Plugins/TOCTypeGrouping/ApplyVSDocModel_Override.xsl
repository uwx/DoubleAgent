<?xml version="1.0"?>
<xsl:stylesheet version="1.1"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                xmlns:ms="urn:schemas-microsoft-com:xslt"
                exclude-result-prefixes="ms">

  <xsl:import href="ApplyVSDocModel.xsl"/>

  <xsl:template name="projectTopic">
    <api id="R:{$project}">
      <topicdata group="root" />
      <elements>
        <xsl:for-each select="/*/apis/api[apidata/@group='namespace' and not(apidata/@subgroup)]">
          <element api="{@id}" />
        </xsl:for-each>
      </elements>
    </api>
  </xsl:template>

</xsl:stylesheet>
