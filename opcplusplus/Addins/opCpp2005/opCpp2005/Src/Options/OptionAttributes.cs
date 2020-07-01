///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: OptionAttributes.cs
/// Date: 09/23/2007
///
/// Description:
///
/// This file contains attribute classes for oCpp command line option classes.
///****************************************************************

using System;

namespace opGamesLLC.opCpp2005
{
    /*=== This is an attribute for specifying the command line tag for an option. ===*/

    public class CommandLineAttribute : Attribute
    {
        /*=== construction ===*/

        public CommandLineAttribute(string _commandLine)
        {
            commandLine = _commandLine;
        }

        /*=== data ===*/

        private string commandLine;

        public string CommandLine
        {
            get
            {
                return commandLine;
            }
        }
    };

    /*=== This attribute specifies the option is visible in the global property grid. ===*/

    public class IsGlobalOptionAttribute : Attribute
    {
        /*=== construction ===*/

        public IsGlobalOptionAttribute()
        {

        }
    };

    /*=== This attribute specifies the option is visible in the project property grid. ===*/

    public class IsProjectOptionAttribute : Attribute
    {
        /*=== construction ===*/

        public IsProjectOptionAttribute()
        {

        }
    };

    /*=== Attribute specifying the option name in the property grid. ===*/

    public class OptionNameAttribute : Attribute
    {
        /*=== construction ===*/

        public OptionNameAttribute(string _optionName)
        {
            optionName = _optionName;
        }

        /*=== data ===*/

        private string optionName;

        public string OptionName
        {
            get
            {
                return optionName;
            }
        }
    };

    /*=== Attribute specifying the option category in the property grid. ===*/

    public class OptionCategoryAttribute : Attribute
    {
        /*=== construction ===*/

        public OptionCategoryAttribute(string _optionCategory)
        {
            optionCategory = _optionCategory;
        }

        /*=== data ===*/

        private string optionCategory;

        public string OptionCategory
        {
            get
            {
                return optionCategory;
            }
        }
    };

    /*=== Attribute specifying the option description in the property grid. ===*/

    public class OptionDescriptionAttribute : Attribute
    {
        /*=== construction ===*/

        public OptionDescriptionAttribute(string _optionDescription)
        {
            optionDescription = _optionDescription;
        }

        /*=== data ===*/

        private string optionDescription;

        public string OptionDescription
        {
            get
            {
                return optionDescription;
            }
        }
    };

    /*=== Attribute specifying the option editor type in the property grid. ===*/

    public class OptionEditorTypeAttribute : Attribute
    {
        /*=== construction ===*/

        public OptionEditorTypeAttribute(Type _optionEditorType)
        {
            optionEditorType = _optionEditorType;
        }

        /*=== data ===*/

        //private string optionEditorType;
        private Type optionEditorType;

        //public string OptionEditorType
        public Type OptionEditorType
        {
            get
            {
                return optionEditorType;
            }
        }
    };

    /*=== Attribute specifying the option type converter for the global property grid. ===*/

    public class GlobalOptionTypeConverterAttribute : Attribute
    {
        /*=== construction ===*/

        public GlobalOptionTypeConverterAttribute(Type _optionTypeConverter)
        {
            optionTypeConverter = _optionTypeConverter;
        }

        /*=== data ===*/

        private Type optionTypeConverter;

        public Type OptionTypeConverter
        {
            get { return optionTypeConverter; }
        }
    };

    /*=== Attribute specifying the option type converter for the project property grid. ===*/

    public class ProjectOptionTypeConverterAttribute : Attribute
    {
        /*=== construction ===*/

        public ProjectOptionTypeConverterAttribute(Type _optionTypeConverter)
        {
            optionTypeConverter = _optionTypeConverter;
        }

        /*=== data ===*/

        private Type optionTypeConverter;

        public Type OptionTypeConverter
        {
            get { return optionTypeConverter; }
        }
    };
}

