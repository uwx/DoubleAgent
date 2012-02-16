﻿<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
								xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
								xmlns:msxsl="urn:schemas-microsoft-com:xslt"
								exclude-result-prefixes="msxsl branding"
								xmlns:mtps="http://msdn2.microsoft.com/mtps"
								xmlns:xhtml="http://www.w3.org/1999/xhtml"
								xmlns:opensearch="http://a9.com/-/spec/opensearch/1.1/"
								xmlns:mshelp="http://help.microsoft.com"
								xmlns:branding="urn:FH-Branding"
								xmlns:xs="http://www.w3.org/2001/XMLSchema">

	<!-- unsupported controls-->
	<xsl:import href="UnsupportedControls.xslt"/>
	<!-- generic support -->
	<xsl:import href="Identity.xslt"/>
	<xsl:import href="body.xslt"/>
	<xsl:import href="head.xslt"/>
	<xsl:import href="foot.xslt"/>

	<xsl:output version ="1.0"
							encoding="utf-8"
							method="xml"
							indent="no"
							omit-xml-declaration="yes"/>

	<!-- ============================================================================================
	Parameters
	============================================================================================= -->

	<xsl:param name="downscale-browser"
						 select="false()"/>
	<xsl:param name="catalogProductFamily"
						 select="''"/>
	<xsl:param name="catalogProductVersion"
						 select="''"/>
	<xsl:param name="catalogLocale"
						 select="''"/>
	<xsl:param name="launchingApp"
						 select="''"/>
	<xsl:param name="branding-package"
						 select="'dev10.mshc'"/>
	<xsl:param name="content-path"
						 select="'./'"/>

	<!-- ============================================================================================
	Global variables
	============================================================================================= -->

	<xsl:variable name="version">
		<xsl:choose>
			<xsl:when test="$catalogProductVersion">
				<xsl:value-of select="$catalogProductVersion"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='Microsoft.Help.TopicVersion']/@content"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>
	<xsl:variable name="topicVersion">
		<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='Microsoft.Help.TopicVersion']/@content"/>
	</xsl:variable>

	<xsl:variable name="product">
		<xsl:choose>
			<xsl:when test="$catalogProductFamily">
				<xsl:value-of select="$catalogProductFamily"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="substring-before(/xhtml:html/xhtml:head/xhtml:meta[@name='Microsoft.Help.TopicVersion']/@content, '.')"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="topiclocale">
		<xsl:value-of select="translate(/xhtml:html/xhtml:head/xhtml:meta[@name='Microsoft.Help.TopicLocale']/@content, 'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
	</xsl:variable>
	<xsl:variable name="locale">
		<xsl:choose>
			<xsl:when test="$catalogLocale">
				<xsl:value-of select="translate($catalogLocale, 'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$topiclocale"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>
	<xsl:variable name="actuallocale"
								select="branding:GetLocale($locale)"></xsl:variable>

	<xsl:variable name="topic-id"
								select="/xhtml:html/xhtml:head/xhtml:meta[@http-equiv='Content-Location']/@content" />
	<xsl:variable name="topic-id2">
		<xsl:value-of select="substring-after($topic-id,'/content/')"/>
	</xsl:variable>
	<xsl:variable name="topic-id1">
		<xsl:value-of select="substring-before($topic-id2,'/')"/>
	</xsl:variable>
	<xsl:variable name="title">
		<xsl:choose>
			<xsl:when test="/xhtml:html/xhtml:head/xhtml:meta[@name='Title']/@content">
				<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='Title']/@content"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:title"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>
	<xsl:variable name="title1">
		<xsl:choose>
			<xsl:when test="contains($title,'#')">
				<xsl:value-of select="concat(substring-before($title,'#'),'%23',substring-after($title,'#'))"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$title"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="self-branded">
		<xsl:choose>
			<xsl:when test="/xhtml:html/xhtml:head/xhtml:meta[@name='SelfBranded']/@content">
				<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='SelfBranded']/@content"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="/xhtml:html/xhtml:head/xhtml:meta[@name='Microsoft.Help.SelfBranded']/@content"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="BrandingPath">
		<xsl:choose>
			<xsl:when test="$downscale-browser">
				<xsl:value-of select="concat('ms.help?',branding:EscapeBackslashes($branding-package),';')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="concat(branding:BackslashesToFrontslashes($contentFolder), '/../branding')"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>
	<xsl:variable name="js-file"
								select="'branding.js'"/>
	<xsl:variable name="css-file"
								select="concat('branding-',branding:GetLocale($locale),'.css')" />
	<xsl:variable name="contentFolder"
								select="branding:GetDirectoryName($content-path)"/>

	<xsl:variable name="collapse-all-class"
								select="'FH-Collapse-All'"/>
	<xsl:variable name="squote">'</xsl:variable>

	<xsl:variable name="mtps"
								select="'http://msdn2.microsoft.com/mtps'"/>
	<xsl:variable name="xhtml"
								select="'http://www.w3.org/1999/xhtml'"/>

	<!-- The following variables are not used for branded content but are retained to ensure all references are resolved -->
	<xsl:variable name="sp1-error-page"
								select="/xhtml:html/xhtml:head/xhtml:meta[@name='SP1ErrorPage']/@content"></xsl:variable>
	<xsl:variable name="error-page"
								select="/xhtml:html/xhtml:body/xhtml:errorLink"/>
	<xsl:variable name="f1-error-page"
								select="/xhtml:html/xhtml:body/xhtml:rss/opensearch:totalResults">
	</xsl:variable>
	<xsl:variable name="localehelp"
								select="concat('help-',$actuallocale,'.htm')" />
	<xsl:variable name="contentnotfound"
								select="concat('contentnotfound-',$actuallocale,'.htm')" />

	<!-- ============================================================================================
	MAIN ENTRY POINT
	============================================================================================= -->

	<xsl:template match="xhtml:html"
								name="html">
		<xsl:copy>
			<xsl:choose>
				<xsl:when test="not($self-branded) or $self-branded = 'true'">
					<!--no branding required-->
					<xsl:apply-templates select="xhtml:head"
															 mode="self-branding"/>
					<xsl:apply-templates select="xhtml:body"
															 mode="self-branding"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:apply-templates select="xhtml:head"/>
					<xsl:apply-templates select="xhtml:body"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:copy>
	</xsl:template>

	<xsl:template match="xhtml:base"
								name="branding-base"/>
	<xsl:template match="mtps:MemberLink"
								name="branding-MemberLink"/>

	<!-- ============================================================================================
	Helper templates
	============================================================================================= -->

	<xsl:template name="ms-xhelp" >
		<xsl:param name="ref"
							 select="@href|@src"/>
		<xsl:value-of select="concat($BrandingPath,'/',branding:BackslashesToFrontslashes($ref))"/>
	</xsl:template>

	<xsl:template match="xhtml:link[@rel='stylesheet']"
								name="branding-stylesheet"/>
	<xsl:template match="xhtml:script"
								name="branding-script"/>

	<!-- generates comment to replace mtps control element
  and keep its original information-->
	<xsl:template name="comment-mtps">
		<xsl:comment>
			<xsl:value-of select="concat('[', local-name(namespace::*[.=namespace-uri(current())]),':', local-name())" />
			<xsl:apply-templates mode="cmt"
													 select="@*" />
			<xsl:value-of select="']'" />
		</xsl:comment>
		<xsl:text> </xsl:text>
	</xsl:template>

	<!-- adds attribute to a comment for an mtps control-->
	<xsl:template match="@*"
								mode="cmt"
								name="cmt-mode">
		<xsl:value-of select="concat(' ', local-name(),'=&quot;', ., '&quot;')" />
	</xsl:template>

	<!-- copies standard attributes from element. It adds a class attribute always, but appends value of existing class attribute -->
	<xsl:template name="copy-std-atrs">
		<xsl:param name="target"
							 select="." />
		<xsl:if test="$target/@id">
			<xsl:copy-of select="$target/@id" />
		</xsl:if>
		<xsl:if test="$target/@title">
			<xsl:copy-of select="$target/@title" />
		</xsl:if>
		<xsl:attribute name="class">
			<xsl:value-of select="normalize-space(concat(local-name($target), ' behavior-removed ', @class))" />
		</xsl:attribute>
	</xsl:template>

	<!-- ============================================================================================
	Function scripts
	============================================================================================= -->

	<msxsl:script language="C#"
								implements-prefix="branding">
		<msxsl:assembly name="System" />
		<msxsl:using namespace="System" />
		<msxsl:using namespace="System.Collections.Generic" />
		<msxsl:using namespace="System.Collections.Specialized" />
		<msxsl:using namespace="System.ComponentModel" />
		<msxsl:using namespace="System.IO" />
		<msxsl:using namespace="System.Globalization" />
		<msxsl:assembly name="System.Web" />
		<msxsl:using namespace="System.Web" />
		<![CDATA[
        string CSKeywordMatch(Match match){
            return " <span xmlns='http://www.w3.org/1999/xhtml' class='CSLanguage'>" + match.Groups[1].Value + " </span>";
        }

        Regex rep =
        new Regex(
            @"(\b(abstract|event|new|struct|as|explicit|null|switch|base|extern|object|this|bool|false|operator|throw|break|finally|out|true|byte|fixed|override|try|case|float|params|typeof|catch|for|private|uint|char|foreach|protected|ulong|checked|goto|public|unchecked|class|if|readonly|unsafe|const|implicit|ref|ushort|continue|in|return|using|decimal|int|sbyte|virtual|default|interface|sealed|volatile|delegate|internal|short|void|do|is|sizeof|while|double|lock|stackalloc|else|long|static|enum|namespace|string)\b)");
        public XPathNodeIterator CSKeyword(string input)
        {
          XmlDocument xdoc = new XmlDocument();
          XmlElement ele = xdoc.CreateElement("branding:Root", "urn:FH-Brandng");
          ele.InnerText = input;
          xdoc.AppendChild(ele);
          String s = ele.OuterXml;
          string s1 = rep.Replace(s, CSKeywordMatch);
          xdoc.LoadXml(s1);
          XPathNavigator nav = xdoc.CreateNavigator();
          return nav.Select("/*/node()");
        }
        public string EscapeBackslashes(string input)
        {
          return input.Replace(@"\",@"\\");
        }
        public string BackslashesToFrontslashes(string input)
        {
          return input.Replace(@"\",@"/");
        }
        public string GetUrlEncode(string input)
        {
            //return HttpUtility.UrlEncode(input);
            return input;
        }
        // Replaces no-op GetUrlEncode - fix this!
        public string GetUrlEncode2(string input)
        {
            return HttpUtility.UrlEncode(input);
        }
        public string GetUrlDecode(string input)
        {
            return HttpUtility.UrlDecode(input);
        }
        public string GetHtmlDecode(string input)
        {
            return HttpUtility.HtmlDecode(input);
        }
        public string replace(string source,string replaceto,string replacewith)
        {
           return source.Replace(replaceto,replacewith);
        }
        public string GetSubString(string source,int index)
        {
          string[] subStrings = source.Split(new String[]{"<a>","</a>"}, StringSplitOptions.None);
          if (index < subStrings.Length)
          {
            return subStrings[index];
          }
          else
          {
            return "";
          }
        }
        public string GetAnchorTagValue(string source,string value)
        {
          MatchCollection matches = Regex.Matches(source, @"<a\shref=""(?<url>.*?)"">(?<text>.*?)</a>");
          if(value == "tag")
             return matches[0].Groups["url"].Value;
          else if (value== "text")
            return matches[0].Groups["text"].Value;
          else if (value== "preText")
             return source.Substring(0, source.IndexOf("<"));
          else
             return "";
              
        }
       public string GetLocale(string locale)
        {
            string[] locales = { "ja-jp", "de-de", "es-es", "fr-fr", "it-it", "ko-kr", "pt-br", "ru-ru", "zh-cn", "zh-tw", "cs-cz", "pl-pl", "tr-tr" };
            int i = 0;
            while (i < locales.Length)
            {
                if (string.Equals(locale, locales[i], StringComparison.CurrentCultureIgnoreCase))
                    return locale;
                i++;
            }
            return "en-US";
        }
        public string GetID(string href)
        {
            string str = String.Empty;
            href = href.ToUpper();
            if (href.ToUpper().Contains("?ID"))
            {
                str = href.Substring(href.IndexOf("?ID=") + 4, href.Length - href.IndexOf("?ID=") - 4);
                if (str.Contains("&"))
                {
                    str = str.Substring(0, str.IndexOf("&"));
                }
            }
           return str;
        }
        public string GetOnlineID(string href)
        {
          if(href.Contains("appId=Dev10IDEF1"))
            return href;
          int idStart = href.LastIndexOf('/');
          if (idStart > -1)
          {
            idStart++;
            string urlPre = href.Substring(0, idStart);
            string urlPost = href.Substring(idStart);

            // Remove type prefix
            int pos = urlPost.IndexOf(':');
            if (pos > -1)
            {
              urlPost = urlPost.Substring(pos + 1);
            }

            // Remove product version
            int verStart = urlPost.LastIndexOf('(');
            int verEnd = urlPost.LastIndexOf(')');
            if (verStart > -1
             && verEnd > -1)
            {
              urlPost = urlPost.Remove(verStart, verEnd - verStart + 1);
            }
            return urlPre + urlPost;
          }
          return href;
        }
        public string GetDirectoryName(string filePath)
        {
          if (String.IsNullOrEmpty(filePath)) return String.Empty;

          return BackslashesToFrontslashes(Path.GetDirectoryName(filePath));
        }
        public string BuildContentPath(string directoryName,string filePath)
        {
          string tempDirName = directoryName;
          if (tempDirName[0] == '/') tempDirName = tempDirName.Substring(1);
          string tempPath = filePath;
          int pathSeparator = tempPath.IndexOf("../");
          while (pathSeparator != -1)
          {
            int dirSeparator = tempDirName.LastIndexOf("/");
            tempDirName = (dirSeparator != -1)?tempDirName.Substring(0,dirSeparator):"";
            string firstPart = tempPath.Substring(0,pathSeparator);
            string secondPart = tempPath.Substring(pathSeparator+2);
            tempPath = firstPart + tempDirName + secondPart;
            pathSeparator = tempPath.IndexOf("../");
          }
          return tempPath;
        }
    ]]>
	</msxsl:script>
</xsl:stylesheet>
