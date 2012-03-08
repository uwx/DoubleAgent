REM ********** Set path for .net framework2.0, sandcastle****************************

setlocal
set PATH=%windir%\Microsoft.NET\Framework\v2.0.50727;%DXROOT%\ProductionTools;%PATH%

if exist output rmdir output /s /q

XslTransform /xsl:"%DXROOT%\ProductionTransforms\dsmanifesttomanifest.xsl" aspnet_howto.buildmanifest.proj.xml /out:manifest.xml
XslTransform /xsl:"%DXROOT%\ProductionTransforms\dstoctotoc.xsl" extractedfiles\aspnet_howto.toc.xml /out:toc.xml

call "%DXROOT%\Presentation\vs2005\copyOutput.bat"

BuildAssembler /config:conceptual-2005-mshc.config manifest.xml

