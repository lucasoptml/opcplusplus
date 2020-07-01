// PkgCmdID.cs
// MUST match PkgCmdID.h
using System;

namespace opGamesLLC.opCpp2005
{
    static class PkgCmdIDList
    {
		//NOTE: mirror these constants in CommandIds.h
		public const uint cmdopProjectSettings = 0x100;
		public const uint cmdopGlobalSettings  = 0x101;
		public const uint cmdopBuildSolution   = 0x102;
		public const uint cmdopBuildProject    = 0x103;
		public const uint cmdopCleanSolution   = 0x104;
		public const uint cmdopFeatureManager  = 0x105;

		public const uint cmdGotoOriginal      = 0x115;
		public const uint cmdGotoDefinition    = 0x116;
		public const uint cmdGotoNote		   = 0x117;
		public const uint cmdOpenInclude       = 0x118;
		public const uint cmdGotoOOH           = 0x119;
		public const uint cmdGotoOCPP          = 0x120;

		public const uint cmdVisualize		   = 0x121;

		//public const uint cmdArgumentLabel     = 0x122;

		public const uint cmdStop			   = 0x123;


		public const uint cmdArgumentStart		= 0x300;
		public const uint cmdModifiersStart		= 0x400;

		public const uint opCppArgumentMenu    = 0x1003;
    };
}