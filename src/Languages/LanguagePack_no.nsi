; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "TSVN Norsk (bokm�l)"
!include "PRODUCT.nsh"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE "Norwegian"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\..\bin\LanguagePack_${PRODUCT_VERSION}_no.exe"
ShowInstDetails show

Section "Hauptgruppe" SEC01
  ReadRegStr $0 HKLM "software\TortoiseSVN" "Directory"
  StrCmp $0 "" NotInstalled Installed
Installed:
  Goto Proceed
NotInstalled:
  MessageBox MB_OK "TortoiseSVN not installed!"
  Abort
Proceed:
;  StrCpy $INSTDIR "$0"
;  SetOutPath "$INSTDIR"
;  File "..\..\doc\output\TortoiseSVN_no.chm"
  StrCpy $INSTDIR "$0\Languages"
  SetOutPath $INSTDIR
  SetOverwrite ifnewer
  File "..\..\bin\*1044.dll"
SectionEnd

Section -Post
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_FUNCTION_DESCRIPTION_END
