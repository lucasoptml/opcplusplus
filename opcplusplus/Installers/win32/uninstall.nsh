;===================================================
; Contains uninstall-related install scripting code.
;===================================================

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP

  SetShellVarContext all

  ; TODO: make uninstall based on registry keys to make it conditional...
  
  


  ;delete url
  Delete "$INSTDIR\${PRODUCT_NAME}.url"

  ;delete uninstaller
  Delete "$INSTDIR\uninst.exe"

  ; main path
  RMDir /r "$INSTDIR\opcpp"
  
  RMDir /r "$INSTDIR\examples"
  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\css"
  
  Delete "$INSTDIR\*.ico"
  Delete "$INSTDIR\*.txt"

  ;delete program file group
  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall opC++.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\opC++ Website.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\opC++ Examples.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\opC++ Standard Dialect Reference.lnk"
  
  ; other folders
  RMDir "$SMPROGRAMS\$ICONS_GROUP"

  ;delete directory if empty
  RMDir "$INSTDIR\"


;TODO: rollback the registry keys
   Call un.RemoveRegistryKeys



MessageBox MB_YESNO "Do you want to remove all opC++ file associations?" /SD IDNO IDYES RemoveAssoc
Goto PostAssociations
RemoveAssoc:
   Call un.RemoveAssociations

PostAssociations:



MessageBox MB_YESNO "Do you want to reset visual studio toolbars, completely removing all traces of opC++ integration?  This is not required." /SD IDNO IDYES Reset
Goto PostReset
Reset:

DetailPrint "---------------------------------------------------------------------"
DetailPrint "Resetting Toolbars, Registering Splash Screen, Running devenv /setup."
DetailPrint "---------------------------------------------------------------------"

ReadRegStr $devenv HKLM "Software\Microsoft\VisualStudio\8.0" "InstallDir"
ExecWait '"$devenv\devenv.exe" /setup' $0

PostReset:

;  Push "$INSTDIR\bin"
;  Call un.RemoveFromPath

;  Push "INCLUDE"
;  Push "$INSTDIR\inc"
;  Call un.RemoveFromEnvVar


  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
;  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd