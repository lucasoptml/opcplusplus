///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: GlobalProperties.cs
/// Date: 09/18/2007
///
/// Description:
///
/// Global opCpp Visual Studio properties.
///****************************************************************

using System;
using Microsoft.Win32;

namespace vs2005opCpp
{
    public class GlobalProperties : PropertiesInterface
    {
        /*=== data ===*/

        // Location of registry keys.
        private string RegistryLocation = @"Software\OP Games\opCpp";

        /*=== properties ===*/

        // opCpp path property
        public string opCppPath
        {
            get
            {
                return MyRegistry.GetRegistryKey(RegistryLocation, "opCppPath", "");
            }
            set
            {
                MyRegistry.SetRegistryKey(RegistryLocation, "opCppPath", value);
            }
        }

        // -verbose property
        public bool Verbose
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Verbose", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Verbose", setting);
            }
        }

        // -uninline property
        public bool Uninline
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Uninline", "True");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Uninline", setting);
            }
        }

        // -nodebug property
        public bool NoDebug
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "NoDebug", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "NoDebug", setting);
            }
        }

        // -highlighting property
        public bool Highlighting
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Highlighting", "True");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Highlighting", setting);
            }
        }

        // -ghosts property
        public bool Ghosts
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Ghosts", "True");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Ghosts", setting);
            }
        }

        // -silent property
        public bool Silent
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Silent", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Silent", setting);
            }
        }

        // -force property
        public bool Force
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Force", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Force", setting);
            }
        }

        // -fixedsys property
        public bool FixesSys
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "FixesSys", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "FixesSys", setting);
            }
        }

        // -clean property
        public bool Clean
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Clean", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Clean", setting);
            }
        }

        // -globmode property
        public bool GlobMode
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "GlobMode", "True");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "GlobMode", setting);
            }
        }

        // -expansion-depth property
        public int ExpansionDepth
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "ExpansionDepth", "100");

                return Convert.ToInt32(result);
            }                 
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "ExpansionDepth", setting);
            }
        }

        // -testmode property
        public bool TestMode
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "TestMode", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "TestMode", setting);
            }
        }

        // -notations property
        public bool Notations
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Notations", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Notations", setting);
            }
        }

        // -tree property
        public bool Tree
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "Tree", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "Tree", setting);
            }
        }

        // -fulltree property
        public bool FullTree
        {
            get
            {
                string result = MyRegistry.GetRegistryKey(RegistryLocation, "FullTree", "False");

                return Convert.ToBoolean(result);
            }
            set
            {
                string setting = Convert.ToString(value);

                MyRegistry.SetRegistryKey(RegistryLocation, "FullTree", setting);
            }
        }

        // additional arguments for the opCpp compiler
        public string AdditionalArguments
        {
            get
            {
                return MyRegistry.GetRegistryKey(RegistryLocation, "AdditionalArguments", "");
            }
            set
            {
                MyRegistry.SetRegistryKey(RegistryLocation, "AdditionalArguments", value);
            }
        }
    }
}