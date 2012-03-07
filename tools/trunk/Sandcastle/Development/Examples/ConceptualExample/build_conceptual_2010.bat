REM ********** Set path for .net framework2.0, sandcastle,hhc,hxcomp****************************

set PATH=%windir%\Microsoft.NET\Framework\v2.0.50727;%DXROOT%\ProductionTools;%ProgramFiles(x86)%\HTML Help Workshop;%PATH%

if exist output rmdir output /s /q
if exist branding rmdir branding /s /q
if exist chm rmdir chm /s /q

XslTransform /xsl:"%DXROOT%\ProductionTransforms\dsmanifesttomanifest.xsl" aspnet_howto.buildmanifest.proj.xml /out:manifest.xml
XslTransform /xsl:"%DXROOT%\ProductionTransforms\dstoctotoc.xsl" extractedfiles\aspnet_howto.toc.xml /out:toc.xml

call "%DXROOT%\Presentation\vs2010\copyOutput.bat"

MSHCPackager /extract /r "%DXROOT%\Presentation\vs2010\branding\dev10.mshc" "output\branding" "/manifest:%DXROOT%\Presentation\vs2010\branding\dev10.manifest" /arg:noTransforms 
MSHCPackager /extract /r "%DXROOT%\Presentation\vs2010\branding\dev10.mshc" "branding" "/manifest:%DXROOT%\Presentation\vs2010\branding\dev10.manifest" /arg:onlyTransforms 
MSHCPackager /copy /r "output\branding" "/manifest:%DXROOT%\Presentation\vs2010\branding\branding.manifest" /arg:noTransforms 
MSHCPackager /copy /r "branding" "/manifest:%DXROOT%\Presentation\vs2010\branding\branding.manifest" /arg:onlyTransforms 
XslTransform /xsl:"%DXROOT%\Presentation\vs2010\copyBranding.xsl" "%DXROOT%\Presentation\vs2010\branding\branding.xml" /out:branding\branding.xml /w /arg:catalogProductFamily=VS,catalogProductVersion=100,catalogLocale=en-US

BuildAssembler /config:conceptual-2010.config manifest.xml

if not exist chm mkdir chm
if not exist chm\html mkdir chm\html
if not exist chm\icons mkdir chm\icons

xcopy output\icons\* chm\icons\ /y /r
xcopy output\branding\* chm\branding\ /y /r

ChmBuilder.exe /project:test /html:Output\html /lcid:1033 /toc:Toc.xml /out:Chm

DBCSFix.exe /d:Chm /l:1033 

hhc chm\test.hhp