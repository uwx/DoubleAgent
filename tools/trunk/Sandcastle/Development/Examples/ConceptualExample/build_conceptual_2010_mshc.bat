REM ********** Set path for .net framework2.0, sandcastle****************************

set PATH=%windir%\Microsoft.NET\Framework\v2.0.50727;%DXROOT%\ProductionTools;%PATH%

if exist output rmdir output /s /q
if exist branding rmdir branding /s /q
if exist Output.mshc del Output.mshc
if exist OutputBranding.mshc del OutputBranding.mshc

XslTransform /xsl:"%DXROOT%\ProductionTransforms\dsmanifesttomanifest.xsl" aspnet_howto.buildmanifest.proj.xml /out:manifest.xml
XslTransform /xsl:"%DXROOT%\ProductionTransforms\dstoctotoc.xsl" extractedfiles\aspnet_howto.toc.xml /out:toc.xml

call "%DXROOT%\Presentation\vs2010\copyOutput.bat"

MSHCPackager /extract /r "%DXROOT%\Presentation\vs2010\branding\dev10.mshc" "branding" "/manifest:%DXROOT%\Presentation\vs2010\branding\dev10.manifest" /arg:noIcons 
MSHCPackager /extract /r "%DXROOT%\Presentation\vs2010\branding\dev10.mshc" "output\icons" "/manifest:%DXROOT%\Presentation\vs2010\branding\dev10.manifest" /arg:onlyIcons 
MSHCPackager /copy /r "branding" "/manifest:%DXROOT%\Presentation\vs2010\branding\branding.manifest" /arg:noIcons 
MSHCPackager /copy /r "output\icons" "/manifest:%DXROOT%\Presentation\vs2010\branding\branding.manifest" /arg:onlyIcons 
XslTransform /xsl:"%DXROOT%\Presentation\vs2010\copyBranding.xsl" "%DXROOT%\Presentation\vs2010\branding\branding.xml" /out:branding\branding.xml /w /arg:catalogProductFamily=VS,catalogProductVersion=100,catalogLocale=en-US

BuildAssembler /config:conceptual-2010-mshc.config manifest.xml

MSHCPackager /save /r "output" "Output.mshc"
@rem Create a branding package only if not using the default catalog. Remember to update	HelpContentSetup.msha accordingly.
@rem MSHCPackager /save /r "branding" "OutputBranding.mshc"

