;================================
; The base installer script file.
;================================

; TODO: test on win2k
;       add a page for devenv? at least add a dialog thats ALWAYS shown
;       add a page for current user/all user selection (registry is easy, and mydocs is easy...)


; Installer defines.
!define PRODUCT_NAME "opC++"
!define PRODUCT_VERSION "0.9.1"
!define PRODUCT_PUBLISHER "opGames LLC"
!define PRODUCT_WEB_SITE "http://www.opcpp.com/"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"
!define LICENSE_FILE "License-0.9.txt"

LicenseForceSelection checkbox "I Agree"

BrandingText "Copyright 2007 opGames LLC"

; Compression algorithm used to build the installer.
SetCompressor lzma

RequestExecutionLevel admin

; Used to get windows version.
!include "WordFunc.nsh"
!insertmacro WordAdd

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\..\Art\opCpp Icon.ico"
!define MUI_UNICON "..\..\Art\opCpp Uninstall Icon.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME

; License page
!insertmacro MUI_PAGE_LICENSE "License-0.9.txt"

; Components page
!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!insertmacro MUI_PAGE_DIRECTORY

; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "opC++"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP

; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!define MUI_FINISHPAGE_SHOWREADME "http://www.opcpp.com/?n=OpCpp.Beta"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; build opcpp release mode
!system '"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" ..\..\opCpp.sln /Build Release /Project opCpp.vcproj' = 0

; build opCpp2005 release mode
!system '"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" ..\..\Addins\opCpp2005\opCpp2005.sln /Build Release' = 0

;build smartassembly addin version
!system '"..\..\..\Utilities\smartassembly\{smartassembly}.com" /build ..\..\Addins\opCpp2005\opCpp2005\smartproject.{sa}proj' = 0

;build the doxygen project
;!system '"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" ..\..\Documentation\Doxygen\Doxygen.sln /Build Release /Project Doxygen.vcproj' = 0

;doesn't quite work
;run the doxygen thing
;!system '"..\..\Documentation\GenerateDox.bat' = 0


; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "../builds/Windows-${PRODUCT_NAME}${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\opC++"
;InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

!include "registry.nsh"

; Install the compiler
!include "compiler.nsh"


Function .onInit
  IntOp $0 ${SF_SELECTED} | ${SF_RO}
  SectionSetFlags ${SEC01} $0
FunctionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR

SectionEnd

; File Associations
!include "associations.nsh"


var /global devenvpath

Section -Post

   Call AddRegistryKeys
   
   Call RefreshShellIcons
   
;  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR/bin\../../Release/opcpp.exe"

SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN

  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "opC++ Compiler with Standard Dialects and Features."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Sample Projects and Implementations for Various Development Environments."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} "opC++ Help Links and Documentation Pdfs."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC04} "Visual Studio 2005 opC++ Integration."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC05} "Associate File Types and Icons."
  !insertmacro MUI_DESCRIPTION_TEXT ${SUBPACKAGE} "Installs the Visual Studio 2005 opC++ Addin"
  !insertmacro MUI_DESCRIPTION_TEXT ${SUBTOOLBAR} "Resets Visual Studio Toolbars: not always necessary, but is on initial install - runs devenv /setup"
  !insertmacro MUI_DESCRIPTION_TEXT ${SUBWIZARDS} "Installs Visual Studio Project and Item Wizards"
  !insertmacro MUI_DESCRIPTION_TEXT ${VISUALASSIST} "Installs Visual Assist Support for opC++ File Extensions."

!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Uninstallation Process
!include "uninstall.nsh"


