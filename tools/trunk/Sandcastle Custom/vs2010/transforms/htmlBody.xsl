<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								version="1.0"
								xmlns:MSHelp="http://msdn.microsoft.com/mshelp"
								xmlns:mshelp="http://msdn.microsoft.com/mshelp"
								xmlns:ddue="http://ddue.schemas.microsoft.com/authoring/2003/5"
								xmlns:xlink="http://www.w3.org/1999/xlink"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
         >

	<xsl:import href="globalTemplates.xsl"/>
	<xsl:template name="upperBodyStuff">
		<!--<span id="HCColorTest"></span>-->
		<!--<input type="hidden"
					 id="userDataCache"
					 class="userDataStyle"/>-->
		<!--<input type="hidden"
					 id="hiddenScrollOffset"/>-->
	</xsl:template>

	<xsl:template name="bodyHeader">
		<div class="OH_topic">
			<div class="OH_title">
				<table>
					<tr>
						<td class="OH_tdTitleColumn">
							<include item="nsrTitle">
								<parameter>
									<xsl:call-template name="topicTitleDecorated"/>
								</parameter>
							</include>
							<xsl:choose>
								<xsl:when test="logoFile">
									<td class="OH_tdLogoColumn">
										<xsl:apply-templates select="logoFile"/>
									</td>
								</xsl:when>
								<xsl:otherwise>
									<td align="right">
										<xsl:call-template name="runningHeader" />
									</td>
								</xsl:otherwise>
							</xsl:choose>
						</td>
					</tr>
				</table>
			</div>
		</div>
		<div class="OH_feedbacklink">
			<include item="headerFeedback"/>
		</div>
	</xsl:template>

</xsl:stylesheet>