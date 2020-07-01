// Guids.cs
// MUST match guids.h
using System;

namespace opGamesLLC.opCpp2005
{
	//NOTE: generate GUIDs using uuidgen.exe from the visual studio commandline.

    static class GuidList
    {
        public const string guidopCpp2005PkgString    = "42231b8e-2a34-43c0-8504-96b74d38af4a";
        public const string guidopCpp2005CmdSetString = "55b4a236-8c21-464c-bdf4-e6817502f011";

        public static readonly Guid guidopCpp2005Pkg    = new Guid(guidopCpp2005PkgString);
        public static readonly Guid guidopCpp2005CmdSet = new Guid(guidopCpp2005CmdSetString);

		public const string guidVisualStudioCommands = "5efc7975-14bc-11cf-9b2b-00aa00573819";

        // Guid for storing trial information in the registry.
        public static readonly string TrialVersionGuid = "8CC631DB-C479-45e4-8E7A-EDD28A836DE4";
    };
}