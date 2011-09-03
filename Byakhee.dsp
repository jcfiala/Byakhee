# Microsoft Developer Studio Project File - Name="Byakhee" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Byakhee - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Byakhee.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Byakhee.mak" CFG="Byakhee - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Byakhee - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Byakhee - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Byakhee - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Byakhee - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Byakhee - Win32 Release"
# Name "Byakhee - Win32 Debug"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddSkillDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddSkillDlg.h
# End Source File
# Begin Source File

SOURCE=.\Byakhee.cpp
# End Source File
# Begin Source File

SOURCE=.\Byakhee.h
# End Source File
# Begin Source File

SOURCE=.\res\Byakhee.ico
# End Source File
# Begin Source File

SOURCE=.\Byakhee.rc
# End Source File
# Begin Source File

SOURCE=.\res\ByakheeDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\byakheescript.ico
# End Source File
# Begin Source File

SOURCE=.\ByakheeScriptExecutor.cpp
# End Source File
# Begin Source File

SOURCE=.\ByakheeScriptExecutor.h
# End Source File
# Begin Source File

SOURCE=.\CharacterAgeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterAgeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChooseSkillDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChooseSkillDlg.h
# End Source File
# Begin Source File

SOURCE=.\CImage\Debug\cimage.lib

!IF  "$(CFG)" == "Byakhee - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Byakhee - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CImage\Release\cimage.lib

!IF  "$(CFG)" == "Byakhee - Win32 Release"

!ELSEIF  "$(CFG)" == "Byakhee - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\closedbook.ico
# End Source File
# Begin Source File

SOURCE=.\ColourConversion.h
# End Source File
# Begin Source File

SOURCE=.\ColouredListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ColouredListBox.h
# End Source File
# Begin Source File

SOURCE=.\DiceRoller.cpp
# End Source File
# Begin Source File

SOURCE=.\DiceRoller.h
# End Source File
# Begin Source File

SOURCE=.\res\dragdrop.cur
# End Source File
# Begin Source File

SOURCE=.\DragSwapEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DragSwapEdit.h
# End Source File
# Begin Source File

SOURCE=.\EditSkillDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSkillDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditSkillsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSkillsDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditWeaponsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditWeaponsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Field.cpp
# End Source File
# Begin Source File

SOURCE=.\Field.h
# End Source File
# Begin Source File

SOURCE=.\FieldEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldEdit.h
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\handout.ico
# End Source File
# Begin Source File

SOURCE=.\HandoutDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HandoutDoc.h
# End Source File
# Begin Source File

SOURCE=.\HandoutView.cpp
# End Source File
# Begin Source File

SOURCE=.\HandoutView.h
# End Source File
# Begin Source File

SOURCE=.\Hyperlink.cpp
# End Source File
# Begin Source File

SOURCE=.\Hyperlink.h
# End Source File
# Begin Source File

SOURCE=.\res\info.ico
# End Source File
# Begin Source File

SOURCE=.\Investigator.cpp
# End Source File
# Begin Source File

SOURCE=.\Investigator.h
# End Source File
# Begin Source File

SOURCE=.\res\investigator_wizard.ico
# End Source File
# Begin Source File

SOURCE=.\investigatordoc.cpp
# End Source File
# Begin Source File

SOURCE=.\InvestigatorDoc.h
# End Source File
# Begin Source File

SOURCE=.\InvestigatorPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\InvestigatorPopupWnd.h
# End Source File
# Begin Source File

SOURCE=.\investigatorview.cpp
# End Source File
# Begin Source File

SOURCE=.\investigatorview.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MDIWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MDIWnd.h
# End Source File
# Begin Source File

SOURCE=.\MultiFileMultiDocTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiFileMultiDocTemplate.h
# End Source File
# Begin Source File

SOURCE=.\NamedOptionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\NamedOptionManager.h
# End Source File
# Begin Source File

SOURCE=.\NewDocumentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewDocumentDlg.h
# End Source File
# Begin Source File

SOURCE=.\res\newhando.ico
# End Source File
# Begin Source File

SOURCE=.\res\newscript.ico
# End Source File
# Begin Source File

SOURCE=.\res\newsheetscript.ico
# End Source File
# Begin Source File

SOURCE=.\NewWeaponDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewWeaponDlg.h
# End Source File
# Begin Source File

SOURCE=.\res\openbook.ico
# End Source File
# Begin Source File

SOURCE=.\OptionalSkillsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionalSkillsDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsPages.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsPages.h
# End Source File
# Begin Source File

SOURCE=.\Output.cpp
# End Source File
# Begin Source File

SOURCE=.\Output.h
# End Source File
# Begin Source File

SOURCE=.\res\page.ico
# End Source File
# Begin Source File

SOURCE=.\ParseBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ParseBuffer.h
# End Source File
# Begin Source File

SOURCE=.\res\penandpaper.ico
# End Source File
# Begin Source File

SOURCE=.\PopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupWnd.h
# End Source File
# Begin Source File

SOURCE=.\QuickInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QuickInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\res\random.ico
# End Source File
# Begin Source File

SOURCE=.\RandomNameGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomNameGenerator.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\runscript.ico
# End Source File
# Begin Source File

SOURCE=.\ScriptEditDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptEditDoc.h
# End Source File
# Begin Source File

SOURCE=.\ScriptEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptEditView.h
# End Source File
# Begin Source File

SOURCE=.\ScriptEngine\Release\ScriptEngine.lib

!IF  "$(CFG)" == "Byakhee - Win32 Release"

!ELSEIF  "$(CFG)" == "Byakhee - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\scripthandout.ico
# End Source File
# Begin Source File

SOURCE=.\ScriptSheetView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptSheetView.h
# End Source File
# Begin Source File

SOURCE=.\SectionFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionFile.h
# End Source File
# Begin Source File

SOURCE=.\SheetCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\SheetCommand.h
# End Source File
# Begin Source File

SOURCE=.\SheetDrawInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SheetDrawInfo.h
# End Source File
# Begin Source File

SOURCE=.\res\sheetscript.ico
# End Source File
# Begin Source File

SOURCE=.\SIZECBAR.CPP
# End Source File
# Begin Source File

SOURCE=.\SIZECBAR.H
# End Source File
# Begin Source File

SOURCE=.\Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\Skill.h
# End Source File
# Begin Source File

SOURCE=.\SkillEditListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillEditListBox.h
# End Source File
# Begin Source File

SOURCE=.\SkillInformationPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillInformationPopupWnd.h
# End Source File
# Begin Source File

SOURCE=.\SkillManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillManager.h
# End Source File
# Begin Source File

SOURCE=.\SpellbookDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellbookDlg.h
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\res\undo.ico
# End Source File
# Begin Source File

SOURCE=.\Wizard_NewInvestigator.cpp
# End Source File
# Begin Source File

SOURCE=.\Wizard_NewInvestigator.h
# End Source File
# Begin Source File

SOURCE=.\Wizard_NewInvestigatorPages.cpp
# End Source File
# Begin Source File

SOURCE=.\Wizard_NewInvestigatorPages.h
# End Source File
# End Target
# End Project
