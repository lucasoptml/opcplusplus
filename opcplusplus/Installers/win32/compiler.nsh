;==================================================
; Contains compiler-related install scripting code.
;==================================================

; The opC++ Compiler Section
Section "opC++ Compiler" SEC01
  SetOutPath "$INSTDIR\"
  SetOverwrite on
  SetShellVarContext all

  SetOutPath "$INSTDIR\bin\win32\release"
  File "..\..\Distribution\bin\win32\release\opCpp.exe"

  SetOutPath "$INSTDIR\"
  File "..\..\Art\opCppIcon.ico"
  File "..\..\Art\opDOHIcon.ico"
  File "..\..\Art\opOOHIcon.ico"
  File "..\..\Art\opOCPPIcon.ico"
  File "..\..\Art\opIndexIcon.ico"
  File "${LICENSE_FILE}"

  SetOutPath "$INSTDIR\opcpp\"
  File /r "..\..\Distribution\opcpp\*.doh"
  File /r "..\..\Distribution\opcpp\*.h"
  File /r "..\..\Distribution\opcpp\*.cpp"
  File /r "..\..\Distribution\opcpp\*.txt"
  File /r "..\..\Distribution\opcpp\*.sln"
  File /r "..\..\Distribution\opcpp\*.vcproj"
  File /r "..\..\Distribution\opcpp\*Makefile"
  File /r "..\..\Distribution\opcpp\*.dsp"
  File /r "..\..\Distribution\opcpp\*.gif"
  File /r "..\..\Distribution\opcpp\*.xml"
  File /r "..\..\Distribution\opcpp\*.dsp"
  File /r "..\..\Distribution\opcpp\*.html"
  File /r "..\..\Distribution\opcpp\*.gif"
  File /r "..\..\Distribution\opcpp\*.png"
  File /r "..\..\Distribution\opcpp\*.dsp"
  File /r "..\..\Distribution\opcpp\*.css"

  SetOutPath "$INSTDIR\Css\"
  File /r "..\..\Distribution\Css\*.css"
  File /r "..\..\Distribution\Css\*.xsl"
  File /r "..\..\Distribution\Css\*.js"

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
        CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\opC++ Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url" "" "$INSTDIR\opCppIcon.ico"
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall opC++.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd


;TODO: move this stuff to mydocs!

var /global WinVersion

var /global examplesdir
var /global benchdir


; The opCpp Examples Section
Section "Examples" SEC02

  SetOverwrite on
SetShellVarContext all


; if its winxp, use the allusers path, if not, use the
ReadRegStr $WinVersion HKLM "Software\Microsoft\Windows NT\CurrentVersion" "VersionNumber"

StrCmp "$WinVersion" "5.0" Win2k
StrCmp "$WinVersion" "4.0" Win2k

;WinXP:
;set the output path to the allusers mydocuments folder...


StrCpy $examplesdir "$DOCUMENTS\opC++\Examples\"
StrCpy $benchdir "$DOCUMENTS\opC++\Benchmarks\"

goto Continue

Win2k:

  StrCpy $examplesdir "$INSTDIR\Examples\"
  StrCpy $benchdir "$INSTDIR\Benchmarks\"
  

Continue:

 SetOutPath "$examplesdir"

; if win2k or less, use installdir
; if winxp or greater, use mydocuments

  File /r "..\..\Distribution\Examples\*.h"
  File /r "..\..\Distribution\Examples\*.cpp"
  File /r "..\..\Distribution\Examples\*.oh"
  File /r "..\..\Distribution\Examples\*.doh"
  File /r "..\..\Distribution\Examples\*.sln"
  File /r "..\..\Distribution\Examples\*.vcproj"

 SetOutPath "$benchdir"

  File /r "..\..\Distribution\Benchmarks\*.h"
  File /r "..\..\Distribution\Benchmarks\*.cpp"
  File /r "..\..\Distribution\Benchmarks\*.oh"
  File /r "..\..\Distribution\Benchmarks\*.doh"
  File /r "..\..\Distribution\Benchmarks\*.sln"
  File /r "..\..\Distribution\Benchmarks\*.vcproj"
  File /r "..\..\Distribution\Benchmarks\*.txt"
  File /r "..\..\Distribution\Benchmarks\*.bat"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\opC++ Examples.lnk" "$examplesdir\"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

; Documentation Section
Section "Documentation" SEC03
  SetOutPath "$INSTDIR\"
  SetOverwrite on
  SetShellVarContext all

  SetOutPath "$INSTDIR\docs\"
  File /r "..\..\Documentation\docs\*.html"
  File /r "..\..\Documentation\docs\*.png"
  File /r "..\..\Documentation\docs\*.css"
  File /r "..\..\Documentation\docs\*.gif"


;  File "..\..\Documentation\Manual\manual.pdf"
;  File "..\..\Documentation\opCpp-Getting-Started.pdf"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
               CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\opC++ Standard Dialect Reference.lnk" "$INSTDIR\docs\html\index.html"
;               CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\opC++ Getting Started.lnk" "$INSTDIR\docs\opCpp-Getting-Started.pdf"
;               CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\opC++ Manual.lnk" "$INSTDIR\docs\manual.pdf"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

SectionGroup "VS 2005 Addin" SEC04






; Integration Section
Section "VS 2005 Package" SUBPACKAGE
  SetOutPath "$INSTDIR\"
  SetOverwrite on

  SetOutPath "$INSTDIR\bin\win32\release"
  File "..\..\Distribution\bin\win32\final\opCpp2005.dll"
  ;File "..\..\Distribution\bin\win32\release\opCpp2005.dll"

  ;move the registry stuff here...
  Call AddIntegrationKeys
  
SectionEnd

var /global devenv

Section "Reset Toolbars" SUBTOOLBAR

MessageBox MB_YESNO "Do you really want to reset visual studio toolbars, updating opC++ integration?  This is necessary on the first run only usually." /SD IDNO IDYES Reset
Goto PostReset
Reset:

DetailPrint "---------------------------------------------------------------------"
DetailPrint "Resetting Toolbars, Registering Splash Screen, Running devenv /setup."
DetailPrint "---------------------------------------------------------------------"

ReadRegStr $devenv HKLM "Software\Microsoft\VisualStudio\8.0" "InstallDir"
ExecWait '"$devenv\devenv.exe" /setup' $0

PostReset:


SectionEnd

Section "Wizards" SUBWIZARDS

; grabs the location of visual studio
;=========================================
ReadRegStr $devenv HKLM "Software\Microsoft\VisualStudio\8.0" "InstallDir"
;===========================================

SetOutPath "$devenv..\..\VC\vcwizards\"

File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcwizards\*.vcproj"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcwizards\*.js"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcwizards\*.oh"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcwizards\*.doh"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcwizards\*.cpp"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcwizards\*.h"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcwizards\*.inf"

SetOutPath "$devenv..\..\VC\vcprojects\"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcprojects\*.ico"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcprojects\*.vsdir"
File /r "..\..\Addins\opC++Wizard\opC++Wizard\vcprojects\*.vsz"

SetOutPath "$devenv..\..\VC\vcprojectitems\opC++\"
File /r "..\..\Addins\ItemWizards\opC++\*.doh"
File /r "..\..\Addins\ItemWizards\opC++\*.oh"
File /r "..\..\Addins\ItemWizards\opC++\*.vsdir"

SetOutPath "$devenv..\..\VC\vcprojectitems\"
File /r "..\..\Addins\ItemWizards\*.vsdir"


SectionEnd

SectionGroupEnd


Section "Visual Assist Support" VISUALASSIST

        ; add visual assist file extension
        ; headers
        ; oh
        ; ooh
        ; oohindex

        ; source
        ; ocppindex
        ; ocpp

        ;TODO: add visual assist option!
        ;retrieve vax keys
        ReadRegStr $R0 HKCU "Software\Whole Tomato\Visual Assist X\VANet8" "ExtHeader"

        IfErrors noVAX 0
         ${WordAdd} $R0 "." "+oh;" $R0
         ${WordAdd} $R0 "." "+ooh;" $R0
         ${WordAdd} $R0 "." "+oohindex;" $R0
         ${WordAdd} $R0 "." "+doh;" $R0

        WriteRegStr HKCU "Software\Whole Tomato\Visual Assist X\VANet8" "ExtHeader" $R0

        ReadRegStr $R0 HKCU "Software\Whole Tomato\Visual Assist X\VANet8" "ExtSource"

        IfErrors noVAX 0
         ${WordAdd} $R0 "." "+ocpp;" $R0
         ${WordAdd} $R0 "." "+ocppindex;" $R0

        WriteRegStr HKCU "Software\Whole Tomato\Visual Assist X\VANet8" "ExtSource" $R0

        noVAX:

SectionEnd

