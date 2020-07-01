// CommandIds.h
// Command IDs used in defining command bars
//

// do not use #pragma once - used by ctc compiler
#ifndef __COMMANDIDS_H_
#define __COMMANDIDS_H_

///////////////////////////////////////////////////////////////////////////////
// Menu IDs

#define opCppMenu           0x1000
#define opCppToolbar        0x1001
#define opCppAdditionalMenu 0x1002
#define opCppArgumentMenu	0x1003

///////////////////////////////////////////////////////////////////////////////
// Menu Group IDs

//#define MyMenuGroup         0x1020
#define opCppCompilerGroup       0x1030
#define opCppSettingsGroup       0x1031
#define opCppGlobalGroup         0x1032
#define opCppToolbarGroup        0x1033
#define opCppCodeContextGroup    0x1034
#define opCppProjectContextGroup 0x1035
#define opCppGotoGroup			 0x1036
#define opCppAdditionalGroup     0x1037

#define opCppCodeToolsGroup		 0x1038
#define opCppArgumentGroup		 0x1039

#define VSTopLevelGroup       0x1040

///////////////////////////////////////////////////////////////////////////////
// Command IDs

// Menu and Toolbar Commands
#define cmdopProjectSettings 0x100
#define cmdopGlobalSettings  0x101
#define cmdopBuildSolution	 0x102
#define cmdopBuildProject    0x103
#define cmdopCleanSolution   0x104
#define cmdopFeatureManager  0x105

// Context Menu Commands
#define cmdGotoOriginal      0x115
#define cmdGotoDefinition    0x116 //NOTE: doesnt know enough to work yet.
#define cmdGotoNote			 0x117
#define cmdOpenInclude       0x118
#define cmdGotoOOH			 0x119
#define cmdGotoOCPP			 0x120

#define cmdVisualize		 0x121
#define cmdArgumentLabel     0x122

#define cmdStop				 0x123


#define cmdArgumentStart	 0x300


#define cmdModifiersStart	 0x400


///////////////////////////////////////////////////////////////////////////////
// Bitmap IDs


#endif // __COMMANDIDS_H_
