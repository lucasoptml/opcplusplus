;==================================================
; Contains registry-related install scripting code.
;==================================================





Function RefreshShellIcons
  ; This Gets Windows To Refresh Its Shell Icons
  System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'

  !define SHCNE_ASSOCCHANGED 0x08000000
  !define SHCNF_IDLIST 0
  System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (${SHCNE_ASSOCCHANGED}, ${SHCNF_IDLIST}, 0, 0)'
FunctionEnd



; Write all the registry keys.
Function AddRegistryKeys

; uninstall information in the registry
        WriteUninstaller "$INSTDIR\uninst.exe"
        WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
        WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
        WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR/opCpp Icon.ico"
        WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
        WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
        WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

; define visual studios GUID for c++
        !define cppvalue "{B2F072B0-ABC1-11D0-9D62-00C04FD9DFD9}"

        ; TODO: make a macro or something to do this:
        ;       copying code is getting ridiculous

; associate our extensions with c++ (non-visualassist)
        WriteRegStr HKLM "Software\Microsoft\VisualStudio\8.0\Languages\File Extensions\.oh" "" ${cppvalue}
        WriteRegStr HKLM "Software\Microsoft\VisualStudio\8.0\Languages\File Extensions\.ooh" "" ${cppvalue}
        WriteRegStr HKLM "Software\Microsoft\VisualStudio\8.0\Languages\File Extensions\.ocpp" "" ${cppvalue}
        WriteRegStr HKLM "Software\Microsoft\VisualStudio\8.0\Languages\File Extensions\.doh" "" ${cppvalue}
        WriteRegStr HKLM "Software\Microsoft\VisualStudio\8.0\Languages\File Extensions\.oohindex" "" ${cppvalue}
        WriteRegStr HKLM "Software\Microsoft\VisualStudio\8.0\Languages\File Extensions\.ocppindex" "" ${cppvalue}

FunctionEnd

Function AddIntegrationKeys
        ;TODO: add the visual studio package registry keys
        ;TODO: parameterize on the GUID, the path, etc
        ;TODO: need to know where system32 is...

        !define opCppGUID "{42231b8e-2a34-43c0-8504-96b74d38af4a}"

        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\InstalledProducts\opCpp2005" "Package" "${opCppGUID}"
        WriteRegDword HKLM "Software\Microsoft\VisualStudio\8.0\InstalledProducts\opCpp2005" "UseInterface" 1

        ;auto load the package
        WriteRegDword HKLM "Software\Microsoft\VisualStudio\8.0\AutoLoadPackages\{F1536EF8-92EC-443C-9ED7-FDADF150DA82}" "${opCppGUID}" 0

        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "" "opC++ 2005 Addin"
        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "InprocServer32" "C:\WINDOWS\system32\mscoree.dll"
        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "Class" "opGamesLLC.opCpp2005.opCpp2005"
        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "CodeBase" "$INSTDIR\bin\win32\release\opCpp2005.dll"

        WriteRegDword HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "ID" 0x71
        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "MinEdition" "Standard"
        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "ProductVersion" "1.0"
        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "ProductName" "opC++ Compiler"
        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}" "CompanyName" "opGames LLC"

        WriteRegStr   HKLM "Software\Microsoft\VisualStudio\8.0\Menus" "${opCppGUID}" ", 1000, 1"


FunctionEnd



; Delete all the registry keys.
Function un.RemoveRegistryKeys

DeleteRegKey HKLM "Software\Microsoft\VisualStudio\8.0\Packages\${opCppGUID}"
DeleteRegKey HKLM "Software\Microsoft\VisualStudio\8.0\InstalledProducts\opCpp2005"


FunctionEnd

!undef opCppGUID

