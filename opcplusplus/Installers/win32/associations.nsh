;=====================================================
; Contains association-related install scripting code.
;=====================================================


var /global extension
var /global icon
var /global typename
var /global description
var /global program



; note: pass in the extension via $(extension), ie .doh
;       pass in the icon via $(icon)
;       pass in the type name via $(typename)
;       pass in the description via $(description)
;       pass in the program file via $(program)
Function AddIcon

  ReadRegStr $1 HKCR "$extension" ""
  StrCmp $1 "" NoBackup
    StrCmp $1 "$typename" NoBackup
    WriteRegStr HKCR "$extension" "backup_val" $1
NoBackup:
  WriteRegStr HKCR "$extension" "" "$typename"
  ReadRegStr $0 HKCR "$typename" ""
  StrCmp $0 "" 0 Skip
	WriteRegStr HKCR "$typename" "" "$description"
;	WriteRegStr HKCR "$typename\shell" "" "open"
	WriteRegStr HKCR "$typename\DefaultIcon" "" "$INSTDIR\$icon,0"
Skip:
;  WriteRegStr HKCR "$typename\shell\open\command" "" '$program "%1"'

  
FunctionEnd

Function un.RemoveIcon

  ;start of restore script
  ReadRegStr $1 HKCR "$extension" ""
  StrCmp $1 "$typename" 0 NoOwn ; only do this if we own it
    ReadRegStr $1 HKCR "$extension" "backup_val"
    StrCmp $1 "" 0 Restore ; if backup="" then delete the whole key
      DeleteRegKey HKCR "$extension"
    Goto NoOwn
Restore:
      WriteRegStr HKCR "$extension" "" $1
      DeleteRegValue HKCR "$extension" "backup_val"

    DeleteRegKey HKCR "$typename" ;Delete key with association settings

NoOwn:

FunctionEnd

Function AddAssociations

        StrCpy $extension ".doh"
        StrCpy $icon "opDOHIcon.ico"
        StrCpy $typename "opCppDialect"
        StrCpy $description "opC++ Dialect File"
        Call AddIcon

        StrCpy $extension ".oh"
        StrCpy $icon "opCppIcon.ico"
        StrCpy $typename "opCppCode"
        StrCpy $description "opC++ Code File"
        Call AddIcon

        StrCpy $extension ".ooh"
        StrCpy $icon "opOOHIcon.ico"
        StrCpy $typename "opCppGenHeader"
        StrCpy $description "opC++ Generated Header File"
        Call AddIcon

        StrCpy $extension ".ocpp"
        StrCpy $icon "opOCPPIcon.ico"
        StrCpy $typename "opCppGenSource"
        StrCpy $description "opC++ Generated Source File"
        Call AddIcon

        StrCpy $extension ".oohindex"
        StrCpy $icon "opIndexIcon.ico"
        StrCpy $typename "opCppIndexHeader"
        StrCpy $description "opC++ Header Index File"
        Call AddIcon

        StrCpy $extension ".ocppindex"
        StrCpy $icon "opIndexIcon.ico"
        StrCpy $typename "opCppIndexSource"
        StrCpy $description "opC++ Source Index File"
        Call AddIcon

    System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
FunctionEnd

Function un.RemoveAssociations

        StrCpy $extension ".doh"
        StrCpy $typename "opCppDialect"
        Call un.RemoveIcon

        StrCpy $extension ".oh"
        StrCpy $typename "opCppCode"
        Call un.RemoveIcon

        StrCpy $extension ".ooh"
        StrCpy $typename "opCppGenHeader"
        Call un.RemoveIcon

        StrCpy $extension ".ocpp"
        StrCpy $typename "opCppGenSource"
        Call un.RemoveIcon

        StrCpy $extension ".oohindex"
        StrCpy $typename "opCppIndexHeader"
        Call un.RemoveIcon

        StrCpy $extension ".ocppindex"
        StrCpy $typename "opCppIndexSource"
        Call un.RemoveIcon

    System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
FunctionEnd

Section "Associate File Types" SEC05

Call AddAssociations

SectionEnd


