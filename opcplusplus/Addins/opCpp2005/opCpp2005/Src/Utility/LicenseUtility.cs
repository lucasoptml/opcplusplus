///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Lockdown.cs
/// Date: 09/27/2007
///
/// Description:
///
/// Contains addin lockdown/trial code.
///****************************************************************

using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.Win32;

namespace opGamesLLC.opCpp2005
{
    public partial class opLicenseUtility
    {
        /*=== construction ===*/

        public opLicenseUtility()
        {

        }

        /*=== data ===*/

        // Name of the license file.

        private static readonly string LicenseFileName = "opC++.oplicense";

        // String appended to the end of the hash string.

        private static readonly string ExtraHashString = "1337haxor? .. :P";

        // String used in generating trial hashes.

        private static readonly string TrialRunString = "opCppTrialRun#";

        // Number of times the trial version can be run.

        private static readonly int maxTrialRuns = 50;

        public static int MaxTrialRuns
        {
            get { return maxTrialRuns; }
        }

        // The number of times this addin has been run in trial mode so far.

        private static int numTrialRuns;

        public static int NumTrialRuns
        {
            get { return numTrialRuns; }
        }

        // The full path to the license file.

        public static string FullLicenseFileName
        {
            get { return Paths.GetGlobalOptionsPath() + LicenseFileName; }
        }
        
        // The blacklist.

        private static bool havePopulatedBlacklist = false;

        private static Dictionary<string, List<String> > blacklist = null;

        public static Dictionary<string, List<string> > Blacklist
        {
            get 
            {
                if (!havePopulatedBlacklist)
                {
                    PopulateBlacklist();

                    havePopulatedBlacklist = true;
                }

                return blacklist;
            }
        }

        /*=== utility ===*/

        // Returns true if the license file contents have been blacklisted.
        public static bool IsBlacklisted(string key, string hash)
        {
            Dictionary<string, List<string>> bl = Blacklist;

            if (bl.ContainsKey(hash))
            {
                List<string> keys = bl[hash];

                foreach (string k in keys)
                {
                    if (k == key)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        // Returns true if the license file contents are valid.
        public static bool IsValidLicenseCombination(string name, string licenseId, string serial)
        {
            name      = name.Replace(" ", "").Trim();
            licenseId = licenseId.Replace(" ", "").Trim();
            serial    = serial.Replace(" ", "").Trim();

            string key  = name + licenseId + ExtraHashString;
            string hash = serial;

            return EncryptionUtility.md5(key) == hash 
                && !IsBlacklisted(key, hash);
        }

        // Generates a license file.
        public static bool GenerateLicenseFile(string name, string licenseId, string serial)
        {
            StreamWriter r;

            try
            {
                r = new StreamWriter(FullLicenseFileName);
            }
            catch (Exception)
            {
                return false;
            }

            r.Write(name.Trim() + "\r\n" + licenseId.Trim() + "\r\n" + serial.Trim());
            r.Flush();
            r.Close();

            return true;
        }

        // This method tries to validate the license file.
        public static bool LicenseIsValid()
        {
            StreamReader r;
            
            // Try to load the file.
            try
            {
                r = new StreamReader(FullLicenseFileName);
            }
            catch (Exception)
            {
                return false;
            }

            // Read the contents of the file.
            string file = r.ReadToEnd();
            
            r.Close();

            // Make sure the file has at least 3 lines.
            char[]   splitchars = {'\n'};
            string[] strings    = file.Split(splitchars);

            if (strings.Length < 3)
                return false;

            // Build the key and grab the hash string.
            string name      = ((string) strings.GetValue(0)).Replace("\r", "");
            string licenseId = ((string) strings.GetValue(1)).Replace("\r", "");
            string serial    = ((string) strings.GetValue(2)).Replace("\r", "");

            // If the key and hash don't match, the license is invalid.
            if (!IsValidLicenseCombination(name, licenseId, serial))
                return false;

            return true;
        }

        // Returns true if we're in trial mode.
        public static bool InTrialMode()
        {
            List<string> hashes = new List<string>();

            // First generate the list of hashes.
            for (int i = 1; i <= maxTrialRuns; i++)
                hashes.Add(EncryptionUtility.md5(TrialRunString + i));

            // Build the path to where we store the number of trial runs.
            string key = @"SOFTWARE\Classes\CLSID\{" + GuidList.TrialVersionGuid + "}";

            // If the registry key exists, see if we're in trial mode.
            if (RegistryUtility.KeyExists(Registry.LocalMachine, key))
            {
                // Reset this key so it looks ligit.
                RegistryUtility.SetKey(Registry.LocalMachine, key + "\\InprocServer32", "", "wsock32.dll");

                // Now read the key that has the number of trial runs hash.
                string hash = RegistryUtility.GetKey(Registry.LocalMachine, key + "\\ProgID", "", "");

                // See if this hash is in our list of hashes.
                for (int i = 1; i < maxTrialRuns; i++)
                {
                    // We found a match, so increment the number of runs in the registry.
                    if (hash == hashes[i - 1])
                    {
                        RegistryUtility.SetKey(Registry.LocalMachine, key + "\\ProgID", "", hashes[i]);

                        numTrialRuns = i + 1;
                        
                        return true;
                    }
                }

                RegistryUtility.SetKey(Registry.LocalMachine, key + "\\ProgID", "", "");
            }
            // The registry key doesn't exist, so create it and start trial mode at 1.
            else
            {
                RegistryUtility.SetKey(Registry.LocalMachine, key + "\\InprocServer32", "", "wsock32.dll");
                RegistryUtility.SetKey(Registry.LocalMachine, key + "\\ProgID",         "", hashes[0]);

                numTrialRuns = 1;

                return true;
            }

            return false;
        }
    };
}