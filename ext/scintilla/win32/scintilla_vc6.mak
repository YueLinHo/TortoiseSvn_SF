# Make file for Scintilla on Windows Visual C++ version
# Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile is for using Visual C++ with nmake.
# Usage for Microsoft:
#     nmake -f scintilla.mak
# For debug versions define DEBUG on the command line:
#     nmake DEBUG=1 -f scintilla.mak
# The main makefile uses mingw32 gcc and may be more current than this file.

.SUFFIXES: .cxx

DIR_O=.
DIR_BIN=..\bin

COMPONENT=$(DIR_BIN)\Scintilla.dll
LEXCOMPONENT=$(DIR_BIN)\SciLexer.dll

CC=cl
RC=rc
LD=link

#-Zc:forScope -Zc:wchar_t
CXXFLAGS=-Zi -TP -W3 -EHsc
# For something scary:-Wp64
CXXDEBUG=-Od -MTd -DDEBUG
CXXNDEBUG=-O1 -MT -DNDEBUG
NAME=-Fo
# If you have problems with lexers being linked, try removing -OPT:REF and replacing with -OPT:NOREF
LDFLAGS=-OPT:NOWIN98 -OPT:REF
LDDEBUG=
LIBS=KERNEL32.lib USER32.lib GDI32.lib IMM32.lib OLE32.LIB
NOLOGO=-nologo

!IFDEF QUIET
CC=@$(CC)
CXXFLAGS=$(CXXFLAGS) $(NOLOGO)
LDFLAGS=$(LDFLAGS) $(NOLOGO)
!ENDIF

!IFDEF DEBUG
CXXFLAGS=$(CXXFLAGS) $(CXXDEBUG)
LDFLAGS=$(LDDEBUG) $(LDFLAGS)
!ELSE
CXXFLAGS=$(CXXFLAGS) $(CXXNDEBUG)
!ENDIF

INCLUDEDIRS=-I../include -I../src -I../lexlib
CXXFLAGS=$(CXXFLAGS) $(INCLUDEDIRS)

ALL:	$(COMPONENT) $(LEXCOMPONENT) $(DIR_O)\ScintillaWinS.obj

clean:
	-del /q $(DIR_O)\*.obj $(DIR_O)\*.pdb $(COMPONENT) $(LEXCOMPONENT) \
	$(DIR_O)\*.res $(DIR_BIN)\*.map $(DIR_BIN)\*.exp $(DIR_BIN)\*.pdb $(DIR_BIN)\*.lib

SOBJS=\
	$(DIR_O)\AutoComplete.obj \
	$(DIR_O)\CallTip.obj \
	$(DIR_O)\CellBuffer.obj \
	$(DIR_O)\CharacterSet.obj \
	$(DIR_O)\CharClassify.obj \
	$(DIR_O)\ContractionState.obj \
	$(DIR_O)\Decoration.obj \
	$(DIR_O)\Document.obj \
	$(DIR_O)\Editor.obj \
	$(DIR_O)\Indicator.obj \
	$(DIR_O)\KeyMap.obj \
	$(DIR_O)\LineMarker.obj \
	$(DIR_O)\PerLine.obj \
	$(DIR_O)\PlatWin.obj \
	$(DIR_O)\PositionCache.obj \
	$(DIR_O)\PropSetSimple.obj \
	$(DIR_O)\RESearch.obj \
	$(DIR_O)\RunStyles.obj \
	$(DIR_O)\ScintillaBase.obj \
	$(DIR_O)\ScintillaWin.obj \
	$(DIR_O)\Selection.obj \
	$(DIR_O)\Style.obj \
	$(DIR_O)\UniConversion.obj \
	$(DIR_O)\ViewStyle.obj \
	$(DIR_O)\XPM.obj

#++Autogenerated -- run src/LexGen.py to regenerate
#**LEXOBJS=\\\n\(\t$(DIR_O)\\\*.obj \\\n\)
LEXOBJS=\
	$(DIR_O)\LexA68k.obj \
	$(DIR_O)\LexAbaqus.obj \
	$(DIR_O)\LexAda.obj \
	$(DIR_O)\LexAPDL.obj \
	$(DIR_O)\LexAsm.obj \
	$(DIR_O)\LexAsn1.obj \
	$(DIR_O)\LexASY.obj \
	$(DIR_O)\LexAU3.obj \
	$(DIR_O)\LexAVE.obj \
	$(DIR_O)\LexAVS.obj \
	$(DIR_O)\LexBaan.obj \
	$(DIR_O)\LexBash.obj \
	$(DIR_O)\LexBasic.obj \
	$(DIR_O)\LexBullant.obj \
	$(DIR_O)\LexCaml.obj \
	$(DIR_O)\LexCLW.obj \
	$(DIR_O)\LexCmake.obj \
	$(DIR_O)\LexCOBOL.obj \
	$(DIR_O)\LexCoffeeScript.obj \
	$(DIR_O)\LexConf.obj \
	$(DIR_O)\LexCPP.obj \
	$(DIR_O)\LexCrontab.obj \
	$(DIR_O)\LexCsound.obj \
	$(DIR_O)\LexCSS.obj \
	$(DIR_O)\LexD.obj \
	$(DIR_O)\LexECL.obj \
	$(DIR_O)\LexEiffel.obj \
	$(DIR_O)\LexErlang.obj \
	$(DIR_O)\LexEScript.obj \
	$(DIR_O)\LexFlagship.obj \
	$(DIR_O)\LexForth.obj \
	$(DIR_O)\LexFortran.obj \
	$(DIR_O)\LexGAP.obj \
	$(DIR_O)\LexGui4Cli.obj \
	$(DIR_O)\LexHaskell.obj \
	$(DIR_O)\LexHTML.obj \
	$(DIR_O)\LexInno.obj \
	$(DIR_O)\LexKix.obj \
	$(DIR_O)\LexLisp.obj \
	$(DIR_O)\LexLout.obj \
	$(DIR_O)\LexLua.obj \
	$(DIR_O)\LexMagik.obj \
	$(DIR_O)\LexMarkdown.obj \
	$(DIR_O)\LexMatlab.obj \
	$(DIR_O)\LexMetapost.obj \
	$(DIR_O)\LexMMIXAL.obj \
	$(DIR_O)\LexModula.obj \
	$(DIR_O)\LexMPT.obj \
	$(DIR_O)\LexMSSQL.obj \
	$(DIR_O)\LexMySQL.obj \
	$(DIR_O)\LexNimrod.obj \
	$(DIR_O)\LexNsis.obj \
	$(DIR_O)\LexOpal.obj \
	$(DIR_O)\LexOScript.obj \
	$(DIR_O)\LexOthers.obj \
	$(DIR_O)\LexPascal.obj \
	$(DIR_O)\LexPB.obj \
	$(DIR_O)\LexPerl.obj \
	$(DIR_O)\LexPLM.obj \
	$(DIR_O)\LexPOV.obj \
	$(DIR_O)\LexPowerPro.obj \
	$(DIR_O)\LexPowerShell.obj \
	$(DIR_O)\LexProgress.obj \
	$(DIR_O)\LexPS.obj \
	$(DIR_O)\LexPython.obj \
	$(DIR_O)\LexR.obj \
	$(DIR_O)\LexRebol.obj \
	$(DIR_O)\LexRuby.obj \
	$(DIR_O)\LexScriptol.obj \
	$(DIR_O)\LexSmalltalk.obj \
	$(DIR_O)\LexSML.obj \
	$(DIR_O)\LexSorcus.obj \
	$(DIR_O)\LexSpecman.obj \
	$(DIR_O)\LexSpice.obj \
	$(DIR_O)\LexSQL.obj \
	$(DIR_O)\LexTACL.obj \
	$(DIR_O)\LexTADS3.obj \
	$(DIR_O)\LexTAL.obj \
	$(DIR_O)\LexTCL.obj \
	$(DIR_O)\LexTCMD.obj \
	$(DIR_O)\LexTeX.obj \
	$(DIR_O)\LexTxt2tags.obj \
	$(DIR_O)\LexVB.obj \
	$(DIR_O)\LexVerilog.obj \
	$(DIR_O)\LexVHDL.obj \
	$(DIR_O)\LexVisualProlog.obj \
	$(DIR_O)\LexYAML.obj \

#--Autogenerated -- end of automatically generated section

LOBJS=\
	$(DIR_O)\Accessor.obj \
	$(DIR_O)\AutoComplete.obj \
	$(DIR_O)\CallTip.obj \
	$(DIR_O)\Catalogue.obj \
	$(DIR_O)\CellBuffer.obj \
	$(DIR_O)\CharacterSet.obj \
	$(DIR_O)\CharClassify.obj \
	$(DIR_O)\ContractionState.obj \
	$(DIR_O)\Decoration.obj \
	$(DIR_O)\Document.obj \
	$(DIR_O)\Editor.obj \
	$(DIR_O)\ExternalLexer.obj \
	$(DIR_O)\Indicator.obj \
	$(DIR_O)\KeyMap.obj \
	$(DIR_O)\LexerBase.obj \
	$(DIR_O)\LexerModule.obj \
	$(DIR_O)\LexerSimple.obj \
	$(DIR_O)\LineMarker.obj \
	$(DIR_O)\PerLine.obj \
	$(DIR_O)\PlatWin.obj \
	$(DIR_O)\PositionCache.obj \
	$(DIR_O)\PropSetSimple.obj \
	$(DIR_O)\RESearch.obj \
	$(DIR_O)\RunStyles.obj \
	$(DIR_O)\ScintillaBaseL.obj \
	$(DIR_O)\ScintillaWinL.obj \
	$(DIR_O)\Selection.obj \
	$(DIR_O)\Style.obj \
	$(DIR_O)\StyleContext.obj \
	$(DIR_O)\UniConversion.obj \
	$(DIR_O)\ViewStyle.obj \
	$(DIR_O)\WordList.obj \
	$(DIR_O)\XPM.obj \
	$(LEXOBJS)

$(DIR_O)\ScintRes.res : ScintRes.rc
	$(RC) -fo$@ $**

$(COMPONENT): $(SOBJS) $(DIR_O)\ScintRes.res
	$(LD) $(LDFLAGS) -DEF:Scintilla.def -DLL -OUT:$@ $** $(LIBS)

$(LEXCOMPONENT): $(LOBJS) $(DIR_O)\ScintRes.res
	$(LD) $(LDFLAGS) -DEF:Scintilla.def -DLL -OUT:$@ $** $(LIBS)

# Define how to build all the objects and what they depend on

{..\src}.cxx{$(DIR_O)}.obj:
	$(CC) $(CXXFLAGS) -c $(NAME)$@ $<
{..\lexlib}.cxx{$(DIR_O)}.obj:
	$(CC) $(CXXFLAGS) -c $(NAME)$@ $<
{..\lexers}.cxx{$(DIR_O)}.obj:
	$(CC) $(CXXFLAGS) -c $(NAME)$@ $<
{.}.cxx{$(DIR_O)}.obj:
	$(CC) $(CXXFLAGS) -c $(NAME)$@ $<

# Some source files are compiled into more than one object because of different conditional compilation
$(DIR_O)\ScintillaBaseL.obj: ..\src\ScintillaBase.cxx
	$(CC) $(CXXFLAGS) -DSCI_LEXER -c $(NAME)$@ ..\src\ScintillaBase.cxx

$(DIR_O)\ScintillaWinL.obj: ScintillaWin.cxx
	$(CC) $(CXXFLAGS) -DSCI_LEXER -c $(NAME)$@ ScintillaWin.cxx

$(DIR_O)\ScintillaWinS.obj: ScintillaWin.cxx
	$(CC) $(CXXFLAGS) -DSTATIC_BUILD -c $(NAME)$@ ScintillaWin.cxx

# Dependencies

# All lexers depend on this set of headers
LEX_HEADERS= ..\include\ILexer.h ..\include\Scintilla.h ..\include\SciLexer.h \
 ..\lexlib\Accessor.h ..\lexlib\CharacterSet.h ..\lexlib\LexAccessor.h \
 ..\lexlib\LexerModule.h ..\lexlib\StyleContext.h

$(DIR_O)\AutoComplete.obj: ../src/AutoComplete.cxx ../include/Platform.h \
  ../src/AutoComplete.h
$(DIR_O)\Accessor.obj: ../lexlib/Accessor.cxx ../lexlib/Accessor.h
$(DIR_O)\CallTip.obj: ../src/CallTip.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/CallTip.h
$(DIR_O)\CellBuffer.obj: ../src/CellBuffer.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/SVector.h ../src/SplitVector.h \
  ../src/Partitioning.h ../src/CellBuffer.h
$(DIR_O)\CharacterSet.obj: ../lexlib/CharacterSet.cxx ../lexlib/CharacterSet.h
$(DIR_O)\CharClassify.obj: ../src/CharClassify.cxx ../src/CharClassify.h
$(DIR_O)\ContractionState.obj: ../src/ContractionState.cxx ../include/Platform.h \
  ../src/ContractionState.h
$(DIR_O)\Decoration.obj: ../src/Decoration.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/SplitVector.h ../src/Partitioning.h \
  ../src/RunStyles.h ../src/Decoration.h
$(DIR_O)\Document.obj: ../src/Document.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/SVector.h ../src/SplitVector.h \
  ../src/Partitioning.h ../src/RunStyles.h ../src/CellBuffer.h \
  ../src/CharClassify.h ../src/Decoration.h ../src/Document.h \
  ../src/RESearch.h ../src/PerLine.h
$(DIR_O)\Editor.obj: ../src/Editor.cxx ../include/Platform.h ../include/Scintilla.h \
  ../src/ContractionState.h ../src/SVector.h ../src/SplitVector.h \
  ../src/Partitioning.h ../src/CellBuffer.h ../src/KeyMap.h \
  ../src/RunStyles.h ../src/Indicator.h ../src/XPM.h ../src/LineMarker.h \
  ../src/Style.h ../src/ViewStyle.h ../src/CharClassify.h \
  ../src/Decoration.h ../src/Document.h ../src/Editor.h ../src/Selection.h ../src/PositionCache.h
$(DIR_O)\ExternalLexer.obj: ../src/ExternalLexer.cxx ../include/Platform.h \
  ../include/Scintilla.h ../include/SciLexer.h \
  ../lexlib/Accessor.h ../src/ExternalLexer.h
$(DIR_O)\Indicator.obj: ../src/Indicator.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/Indicator.h
$(DIR_O)\KeyMap.obj: ../src/KeyMap.cxx ../include/Platform.h ../include/Scintilla.h \
  ../src/KeyMap.h

#++Autogenerated -- run src/LexGen.py to regenerate
#**\n\($(DIR_O)\\\*.obj: ..\\lexers\\\*.cxx $(LEX_HEADERS)\n\n\)

$(DIR_O)\LexA68k.obj: ..\lexers\LexA68k.cxx $(LEX_HEADERS)

$(DIR_O)\LexAbaqus.obj: ..\lexers\LexAbaqus.cxx $(LEX_HEADERS)

$(DIR_O)\LexAda.obj: ..\lexers\LexAda.cxx $(LEX_HEADERS)

$(DIR_O)\LexAPDL.obj: ..\lexers\LexAPDL.cxx $(LEX_HEADERS)

$(DIR_O)\LexAsm.obj: ..\lexers\LexAsm.cxx $(LEX_HEADERS)

$(DIR_O)\LexAsn1.obj: ..\lexers\LexAsn1.cxx $(LEX_HEADERS)

$(DIR_O)\LexASY.obj: ..\lexers\LexASY.cxx $(LEX_HEADERS)

$(DIR_O)\LexAU3.obj: ..\lexers\LexAU3.cxx $(LEX_HEADERS)

$(DIR_O)\LexAVE.obj: ..\lexers\LexAVE.cxx $(LEX_HEADERS)

$(DIR_O)\LexAVS.obj: ..\lexers\LexAVS.cxx $(LEX_HEADERS)

$(DIR_O)\LexBaan.obj: ..\lexers\LexBaan.cxx $(LEX_HEADERS)

$(DIR_O)\LexBash.obj: ..\lexers\LexBash.cxx $(LEX_HEADERS)

$(DIR_O)\LexBasic.obj: ..\lexers\LexBasic.cxx $(LEX_HEADERS)

$(DIR_O)\LexBullant.obj: ..\lexers\LexBullant.cxx $(LEX_HEADERS)

$(DIR_O)\LexCaml.obj: ..\lexers\LexCaml.cxx $(LEX_HEADERS)

$(DIR_O)\LexCLW.obj: ..\lexers\LexCLW.cxx $(LEX_HEADERS)

$(DIR_O)\LexCmake.obj: ..\lexers\LexCmake.cxx $(LEX_HEADERS)

$(DIR_O)\LexCOBOL.obj: ..\lexers\LexCOBOL.cxx $(LEX_HEADERS)

$(DIR_O)\LexCoffeeScript.obj: ..\lexers\LexCoffeeScript.cxx $(LEX_HEADERS)

$(DIR_O)\LexConf.obj: ..\lexers\LexConf.cxx $(LEX_HEADERS)

$(DIR_O)\LexCPP.obj: ..\lexers\LexCPP.cxx $(LEX_HEADERS)

$(DIR_O)\LexCrontab.obj: ..\lexers\LexCrontab.cxx $(LEX_HEADERS)

$(DIR_O)\LexCsound.obj: ..\lexers\LexCsound.cxx $(LEX_HEADERS)

$(DIR_O)\LexCSS.obj: ..\lexers\LexCSS.cxx $(LEX_HEADERS)

$(DIR_O)\LexD.obj: ..\lexers\LexD.cxx $(LEX_HEADERS)

$(DIR_O)\LexECL.obj: ..\lexers\LexECL.cxx $(LEX_HEADERS)

$(DIR_O)\LexEiffel.obj: ..\lexers\LexEiffel.cxx $(LEX_HEADERS)

$(DIR_O)\LexErlang.obj: ..\lexers\LexErlang.cxx $(LEX_HEADERS)

$(DIR_O)\LexEScript.obj: ..\lexers\LexEScript.cxx $(LEX_HEADERS)

$(DIR_O)\LexFlagship.obj: ..\lexers\LexFlagship.cxx $(LEX_HEADERS)

$(DIR_O)\LexForth.obj: ..\lexers\LexForth.cxx $(LEX_HEADERS)

$(DIR_O)\LexFortran.obj: ..\lexers\LexFortran.cxx $(LEX_HEADERS)

$(DIR_O)\LexGAP.obj: ..\lexers\LexGAP.cxx $(LEX_HEADERS)

$(DIR_O)\LexGui4Cli.obj: ..\lexers\LexGui4Cli.cxx $(LEX_HEADERS)

$(DIR_O)\LexHaskell.obj: ..\lexers\LexHaskell.cxx $(LEX_HEADERS)

$(DIR_O)\LexHTML.obj: ..\lexers\LexHTML.cxx $(LEX_HEADERS)

$(DIR_O)\LexInno.obj: ..\lexers\LexInno.cxx $(LEX_HEADERS)

$(DIR_O)\LexKix.obj: ..\lexers\LexKix.cxx $(LEX_HEADERS)

$(DIR_O)\LexLisp.obj: ..\lexers\LexLisp.cxx $(LEX_HEADERS)

$(DIR_O)\LexLout.obj: ..\lexers\LexLout.cxx $(LEX_HEADERS)

$(DIR_O)\LexLua.obj: ..\lexers\LexLua.cxx $(LEX_HEADERS)

$(DIR_O)\LexMagik.obj: ..\lexers\LexMagik.cxx $(LEX_HEADERS)

$(DIR_O)\LexMarkdown.obj: ..\lexers\LexMarkdown.cxx $(LEX_HEADERS)

$(DIR_O)\LexMatlab.obj: ..\lexers\LexMatlab.cxx $(LEX_HEADERS)

$(DIR_O)\LexMetapost.obj: ..\lexers\LexMetapost.cxx $(LEX_HEADERS)

$(DIR_O)\LexMMIXAL.obj: ..\lexers\LexMMIXAL.cxx $(LEX_HEADERS)

$(DIR_O)\LexModula.obj: ..\lexers\LexModula.cxx $(LEX_HEADERS)

$(DIR_O)\LexMPT.obj: ..\lexers\LexMPT.cxx $(LEX_HEADERS)

$(DIR_O)\LexMSSQL.obj: ..\lexers\LexMSSQL.cxx $(LEX_HEADERS)

$(DIR_O)\LexMySQL.obj: ..\lexers\LexMySQL.cxx $(LEX_HEADERS)

$(DIR_O)\LexNimrod.obj: ..\lexers\LexNimrod.cxx $(LEX_HEADERS)

$(DIR_O)\LexNsis.obj: ..\lexers\LexNsis.cxx $(LEX_HEADERS)

$(DIR_O)\LexOpal.obj: ..\lexers\LexOpal.cxx $(LEX_HEADERS)

$(DIR_O)\LexOScript.obj: ..\lexers\LexOScript.cxx $(LEX_HEADERS)

$(DIR_O)\LexOthers.obj: ..\lexers\LexOthers.cxx $(LEX_HEADERS)

$(DIR_O)\LexPascal.obj: ..\lexers\LexPascal.cxx $(LEX_HEADERS)

$(DIR_O)\LexPB.obj: ..\lexers\LexPB.cxx $(LEX_HEADERS)

$(DIR_O)\LexPerl.obj: ..\lexers\LexPerl.cxx $(LEX_HEADERS)

$(DIR_O)\LexPLM.obj: ..\lexers\LexPLM.cxx $(LEX_HEADERS)

$(DIR_O)\LexPOV.obj: ..\lexers\LexPOV.cxx $(LEX_HEADERS)

$(DIR_O)\LexPowerPro.obj: ..\lexers\LexPowerPro.cxx $(LEX_HEADERS)

$(DIR_O)\LexPowerShell.obj: ..\lexers\LexPowerShell.cxx $(LEX_HEADERS)

$(DIR_O)\LexProgress.obj: ..\lexers\LexProgress.cxx $(LEX_HEADERS)

$(DIR_O)\LexPS.obj: ..\lexers\LexPS.cxx $(LEX_HEADERS)

$(DIR_O)\LexPython.obj: ..\lexers\LexPython.cxx $(LEX_HEADERS)

$(DIR_O)\LexR.obj: ..\lexers\LexR.cxx $(LEX_HEADERS)

$(DIR_O)\LexRebol.obj: ..\lexers\LexRebol.cxx $(LEX_HEADERS)

$(DIR_O)\LexRuby.obj: ..\lexers\LexRuby.cxx $(LEX_HEADERS)

$(DIR_O)\LexScriptol.obj: ..\lexers\LexScriptol.cxx $(LEX_HEADERS)

$(DIR_O)\LexSmalltalk.obj: ..\lexers\LexSmalltalk.cxx $(LEX_HEADERS)

$(DIR_O)\LexSML.obj: ..\lexers\LexSML.cxx $(LEX_HEADERS)

$(DIR_O)\LexSorcus.obj: ..\lexers\LexSorcus.cxx $(LEX_HEADERS)

$(DIR_O)\LexSpecman.obj: ..\lexers\LexSpecman.cxx $(LEX_HEADERS)

$(DIR_O)\LexSpice.obj: ..\lexers\LexSpice.cxx $(LEX_HEADERS)

$(DIR_O)\LexSQL.obj: ..\lexers\LexSQL.cxx $(LEX_HEADERS)

$(DIR_O)\LexTACL.obj: ..\lexers\LexTACL.cxx $(LEX_HEADERS)

$(DIR_O)\LexTADS3.obj: ..\lexers\LexTADS3.cxx $(LEX_HEADERS)

$(DIR_O)\LexTAL.obj: ..\lexers\LexTAL.cxx $(LEX_HEADERS)

$(DIR_O)\LexTCL.obj: ..\lexers\LexTCL.cxx $(LEX_HEADERS)

$(DIR_O)\LexTCMD.obj: ..\lexers\LexTCMD.cxx $(LEX_HEADERS)

$(DIR_O)\LexTeX.obj: ..\lexers\LexTeX.cxx $(LEX_HEADERS)

$(DIR_O)\LexTxt2tags.obj: ..\lexers\LexTxt2tags.cxx $(LEX_HEADERS)

$(DIR_O)\LexVB.obj: ..\lexers\LexVB.cxx $(LEX_HEADERS)

$(DIR_O)\LexVerilog.obj: ..\lexers\LexVerilog.cxx $(LEX_HEADERS)

$(DIR_O)\LexVHDL.obj: ..\lexers\LexVHDL.cxx $(LEX_HEADERS)

$(DIR_O)\LexVisualProlog.obj: ..\lexers\LexVisualProlog.cxx $(LEX_HEADERS)

$(DIR_O)\LexYAML.obj: ..\lexers\LexYAML.cxx $(LEX_HEADERS)


#--Autogenerated -- end of automatically generated section

$(DIR_O)\LexerBase.obj: ../lexlib/LexerBase.cxx ../lexlib/LexerBase.h
$(DIR_O)\LexerModule.obj: ../lexlib/LexerModule.cxx ../lexlib/LexerModule.h
$(DIR_O)\LexerSimple.obj: ../lexlib/LexerSimple.cxx ../lexlib/LexerSimple.h
$(DIR_O)\LineMarker.obj: ../src/LineMarker.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/XPM.h ../src/LineMarker.h
$(DIR_O)\PerLine.obj: ../src/PerLine.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/SVector.h ../src/SplitVector.h \
  ../src/Partitioning.h ../src/RunStyles.h ../src/PerLine.h
$(DIR_O)\PlatWin.obj: PlatWin.cxx ../include/Platform.h \
  ../src/UniConversion.h ../src/XPM.h
$(DIR_O)\PositionCache.obj: ../src/Editor.cxx ../include/Platform.h ../include/Scintilla.h \
  ../src/ContractionState.h ../src/SVector.h ../src/SplitVector.h \
  ../src/Partitioning.h ../src/CellBuffer.h ../src/KeyMap.h \
  ../src/RunStyles.h ../src/Indicator.h ../src/XPM.h ../src/LineMarker.h \
  ../src/Style.h ../src/ViewStyle.h ../src/CharClassify.h \
  ../src/Decoration.h ../src/Document.h ../src/Editor.h ../src/Selection.h ../src/PositionCache.h
$(DIR_O)\PropSetSimple.obj: ../lexlib/PropSetSimple.cxx ../include/Platform.h
$(DIR_O)\RESearch.obj: ../src/RESearch.cxx ../src/CharClassify.h ../src/RESearch.h
$(DIR_O)\RunStyles.obj: ../src/RunStyles.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/SplitVector.h ../src/Partitioning.h \
  ../src/RunStyles.h
$(DIR_O)\ScintillaBase.obj: ../src/ScintillaBase.cxx ../include/Platform.h \
  ../include/Scintilla.h \
  ../src/ContractionState.h ../src/SVector.h ../src/SplitVector.h \
  ../src/Partitioning.h ../src/RunStyles.h ../src/CellBuffer.h \
  ../src/CallTip.h ../src/KeyMap.h ../src/Indicator.h ../src/XPM.h \
  ../src/LineMarker.h ../src/Style.h ../src/ViewStyle.h \
  ../src/AutoComplete.h ../src/CharClassify.h ../src/Decoration.h \
  ../src/Document.h ../src/Editor.h ../src/Selection.h ../src/ScintillaBase.h
$(DIR_O)\ScintillaBaseL.obj: ../src/ScintillaBase.cxx ../include/Platform.h \
  ../include/Scintilla.h \
  ../src/ContractionState.h ../src/SVector.h ../src/SplitVector.h \
  ../src/Partitioning.h ../src/RunStyles.h ../src/CellBuffer.h \
  ../src/CallTip.h ../src/KeyMap.h ../src/Indicator.h ../src/XPM.h \
  ../src/LineMarker.h ../src/Style.h ../src/ViewStyle.h \
  ../src/AutoComplete.h ../src/CharClassify.h ../src/Decoration.h \
  ../src/Document.h ../src/Editor.h ../src/Selection.h ../src/ScintillaBase.h
$(DIR_O)\ScintillaWin.obj: ScintillaWin.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/ContractionState.h \
  ../src/SVector.h ../src/SplitVector.h ../src/Partitioning.h \
  ../src/RunStyles.h ../src/CellBuffer.h ../src/CallTip.h ../src/KeyMap.h \
  ../src/Indicator.h ../src/XPM.h ../src/LineMarker.h ../src/Style.h \
  ../src/AutoComplete.h ../src/ViewStyle.h ../src/CharClassify.h \
  ../src/Decoration.h ../src/Document.h ../src/Editor.h \
  ../src/ScintillaBase.h ../src/Selection.h ../src/UniConversion.h
$(DIR_O)\ScintillaWinS.obj: ScintillaWin.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/ContractionState.h \
  ../src/SVector.h ../src/SplitVector.h ../src/Partitioning.h \
  ../src/RunStyles.h ../src/CellBuffer.h ../src/CallTip.h ../src/KeyMap.h \
  ../src/Indicator.h ../src/XPM.h ../src/LineMarker.h ../src/Style.h \
  ../src/AutoComplete.h ../src/ViewStyle.h ../src/CharClassify.h \
  ../src/Decoration.h ../src/Document.h ../src/Editor.h \
  ../src/ScintillaBase.h ../src/Selection.h ../src/UniConversion.h
$(DIR_O)\ScintillaWinL.obj: ScintillaWin.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/ContractionState.h \
  ../src/SVector.h ../src/SplitVector.h ../src/Partitioning.h \
  ../src/RunStyles.h ../src/CellBuffer.h ../src/CallTip.h ../src/KeyMap.h \
  ../src/Indicator.h ../src/XPM.h ../src/LineMarker.h ../src/Style.h \
  ../src/AutoComplete.h ../src/ViewStyle.h ../src/CharClassify.h \
  ../src/Decoration.h ../src/Document.h ../src/Editor.h \
  ../src/ScintillaBase.h ../src/Selection.h ../src/UniConversion.h
$(DIR_O)\Selection.obj: ../src/Selection.cxx ../include/Platform.h ../include/Scintilla.h \
  ../src/Selection.h
$(DIR_O)\Style.obj: ../src/Style.cxx ../include/Platform.h ../include/Scintilla.h \
  ../src/Style.h
$(DIR_O)\StyleContext.obj: ../lexlib/StyleContext.cxx ../lexlib/Accessor.h \
  ../lexlib/StyleContext.h
$(DIR_O)\UniConversion.obj: ../src/UniConversion.cxx ../src/UniConversion.h
$(DIR_O)\ViewStyle.obj: ../src/ViewStyle.cxx ../include/Platform.h \
  ../include/Scintilla.h ../src/SplitVector.h ../src/Partitioning.h \
  ../src/RunStyles.h ../src/Indicator.h ../src/XPM.h ../src/LineMarker.h \
  ../src/Style.h ../src/ViewStyle.h
$(DIR_O)\WordList.obj: ../lexlib/WordList.cxx ../lexlib/WordList.h
$(DIR_O)\XPM.obj: ../src/XPM.cxx ../include/Platform.h ../src/XPM.h
