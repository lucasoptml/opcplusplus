///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
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

namespace vs2005opCpp
{
    public class MyRegistry
    {
        /*=== utility ===*/

        // Get a key from the registry.
        public static string GetRegistryKey(string key, string value, string defaultsetting)
        {
            RegistryKey regkey = Registry.CurrentUser.CreateSubKey(key);
            string      result = (string) regkey.GetValue(value);

            if (result == null)
            {
                regkey.SetValue(value, defaultsetting);

                return defaultsetting;
            }

            return result;
        }

        // Set a key in the registry.
        public static void SetRegistryKey(string key, string value, string setting)
        {
            RegistryKey regkey = Registry.CurrentUser.CreateSubKey(key);

            regkey.SetValue(value, setting);
        }
    }
}