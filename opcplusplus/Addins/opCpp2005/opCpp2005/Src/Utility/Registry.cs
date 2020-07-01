///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Registry.cs
/// Date: 09/18/2007
///
/// Description:
///
/// Code for manipulating the registry.
///****************************************************************

using System;
using Microsoft.Win32;

namespace opGamesLLC.opCpp2005
{
    public class RegistryUtility
    {
        /*=== utility ===*/

        // Returns true if the specified registry key exists.
        public static bool KeyExists(RegistryKey baseKey, string key)
        {
            RegistryKey regKey = baseKey.OpenSubKey(key);

            return regKey != null;
        }

        // Get a key from the registry.
        public static string GetKey(RegistryKey baseKey, string key, string value, string defaultsetting)
        {
            RegistryKey regkey = baseKey.CreateSubKey(key);
            string      result = (string) regkey.GetValue(value);

            if (result == null)
            {
                regkey.SetValue(value, defaultsetting);

                return defaultsetting;
            }

            return result;
        }

        // Set a key in the registry.
        public static void SetKey(RegistryKey baseKey, string key, string value, string setting)
        {
            RegistryKey regkey = baseKey.CreateSubKey(key);

            regkey.SetValue(value, setting);
        }
    }
}