///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Options.cs
/// Date: 09/20/2007
///
/// Description:
///
/// This file contains all opCpp options.
///****************************************************************

using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using System.Reflection;
using System.ComponentModel;
using System.Windows.Forms.Design;

namespace opGamesLLC.opCpp2005
{
    /*=== This is the actual options class.  It contains all opCpp command line options. ===*/

    public class Options
    {
        ///==========================================
        /// Construction.
        ///==========================================
        
        public Options()
        {

        }
        
        // copy constructor
        public Options(Options copy)
        {
            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            // Get the public data members in the 'Options' class.
            oFields = oType.GetFields(BindingFlags.Public|BindingFlags.Instance);
      
            // This loops through all the public data members in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // Handle string list options.
                if (oFields[i].FieldType == typeof(StringListOption))
                {
                    StringListOption theiroption = (StringListOption) oFields[i].GetValue(copy);
                    StringListOption myoption    = (StringListOption) oFields[i].GetValue(this);

                    myoption.StringList = new List<string>(theiroption.StringList);
                }
                // Handle string options.
                else if (oFields[i].FieldType == typeof(StringOption))
                {
                    StringOption theiroption = (StringOption) oFields[i].GetValue(copy);
                    StringOption myoption    = (StringOption) oFields[i].GetValue(this);

                    myoption.Value = theiroption.Value;
                }
                // Handle bool options.
                else if (oFields[i].FieldType == typeof(BoolOption))
                {
                    BoolOption theiroption = (BoolOption) oFields[i].GetValue(copy);
                    BoolOption myoption    = (BoolOption) oFields[i].GetValue(this);

                    myoption.Value      = theiroption.Value;
                    myoption.UseDefault = theiroption.UseDefault;
                }
                // Handle int options.
                else if (oFields[i].FieldType == typeof(IntOption))
                {
                    IntOption theiroption = (IntOption) oFields[i].GetValue(copy);
                    IntOption myoption    = (IntOption) oFields[i].GetValue(this);

                    myoption.Value      = theiroption.Value;
                    myoption.UseDefault = theiroption.UseDefault;
                }
            } 
        }

        ///==========================================
        /// Reflection stuff.
        ///==========================================

        // Tells every method that it should use the default method from globals.
        public void SetDefaults()
        {
            FieldInfo[] oFields;
            Type        oType = typeof(Options);

            // Get the public data members in the 'Options' class.
            oFields = oType.GetFields(BindingFlags.Public|BindingFlags.Instance);
      
            // This loops through all the public data members in the 'Options' class.
            for (int i = 0; i < oFields.Length; i++)
            {
                // Handle string list options.
                if (oFields[i].FieldType == typeof(StringListOption))
                {
                    StringListOption option = (StringListOption) oFields[i].GetValue(this);

                    option.StringList = new List<string>();
                }
                // Handle string options.
                else if (oFields[i].FieldType == typeof(StringOption))
                {
                    StringOption option = (StringOption) oFields[i].GetValue(this);

                    option.Value = "";
                }
                // Handle bool options.
                else if (oFields[i].FieldType == typeof(BoolOption))
                {
                    BoolOption option = (BoolOption) oFields[i].GetValue(this);

                    option.Value      = false;
                    option.UseDefault = true;
                }
                // Handle int options.
                else if (oFields[i].FieldType == typeof(IntOption))
                {
                    IntOption option = (IntOption) oFields[i].GetValue(this);

                    option.Value      = 0;
                    option.UseDefault = true;
                }
            } 
        }

        ///==========================================
        /// Debug options
        ///==========================================

#if DEBUG

        /*=== -tree command line option ===*/

        [CommandLine("-tree")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Tree")]
        [OptionCategory("Debug Compiler Options")]
        [OptionDescription("Prints the abstract syntax tree to the standard out.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption Tree = new BoolOption(false);

        /*=== -fulltree command line option ===*/

        [CommandLine("-fulltree")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Full Tree")]
        [OptionCategory("Debug Compiler Options")]
        [OptionDescription("Prints the abstract syntax tree with values to the standard out.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption FullTree = new BoolOption(false);

#endif

        ///==========================================
        /// Locations options
        ///==========================================

        /*=== path to opCpp.exe ===*/

        [IsGlobalOption]
        [IsProjectOption]
        [OptionName("Compiler Location")]
        [OptionCategory("Locations")]
        [OptionDescription("Specify this if you want to use a different version of the opC++ compiler.")]
        [OptionEditorType(typeof(FindExecutableEditor))]
        [GlobalOptionTypeConverter(typeof(StringOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringOptionConverter))]
        public StringOption ExecutablePath = new StringOption();

        /*=== license location ===*/

        [IsGlobalOption]
        [IsProjectOption]
        [OptionName("License Location")]
        [OptionCategory("Locations")]
        [OptionDescription("Specify this if you want to use a different version of the opC++ compiler.")]
        [OptionEditorType(typeof(FindLicenseEditor))]
        [GlobalOptionTypeConverter(typeof(StringOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringOptionConverter))]
        public StringOption LicensePath = new StringOption();

        /*=== -oh command line option ===*/

		[CommandLine("-oh")]
        [IsProjectOption]
        [OptionName("Header Files")]
        [OptionCategory("Locations")]
        [OptionDescription("Additional code (.oh) files to be compiled that are not in your project.")]
        [OptionEditorType(typeof(OhFilesEditor))]
        [GlobalOptionTypeConverter(typeof(StringListOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringListOptionConverter))]
        public StringListOption OhFiles = new StringListOption();

        /*=== -d command line option ===*/

		[CommandLine("-d")]
        [IsProjectOption]
        [IsGlobalOption]
		[OptionName("Include Directories")]
        [OptionCategory("Locations")]
		[OptionDescription("Additional include directories for both code (.oh) and dialect (.doh) files.")]
        [OptionEditorType(typeof(IncludeDirectoriesEditor))]
        [GlobalOptionTypeConverter(typeof(StringListOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringListOptionConverter))]
        public StringListOption Directories = new StringListOption();

        /*=== -ohd command line option ===*/
		
		[CommandLine("-ohd")]
        [IsProjectOption]
		[OptionName("Header Compile Directories")]
        [OptionCategory("Locations")]
		[OptionDescription("All .oh files within these directories will be compiled.")]
        [OptionEditorType(typeof(OhCompileDirectories))]
        [GlobalOptionTypeConverter(typeof(StringListOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringListOptionConverter))]
		public StringListOption OhDirectories = new StringListOption();

        /*=== -gd command line option ===*/

		[CommandLine("-gd")]
        [IsProjectOption]
        [IsGlobalOption]
		[OptionName("Generated Directory")]
        [OptionCategory("Locations")]
		[OptionDescription("All code is output to this directory.")]
        [OptionEditorType(typeof(FolderNameEditor))]
        [GlobalOptionTypeConverter(typeof(StringOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringOptionConverter))]
		public StringOption GeneratedDirectory = new StringOption("Generated/$(ProjectName)");

        /*=== -doh command line option ===*/

		[CommandLine("-doh")]
        [IsProjectOption]
        [IsGlobalOption]
		[OptionName("Dialect Files")]
        [OptionCategory("Locations")]
		[OptionDescription("Additional dialect (.doh) files to be read that are not in your project.")]
        [OptionEditorType(typeof(DohFilesEditor))]
        [GlobalOptionTypeConverter(typeof(StringListOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringListOptionConverter))]
		public StringListOption DialectFiles = new StringListOption();

        /*=== additional dependencies ===*/

        [CommandLine("-dependencies")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Dependencies")]
        [OptionCategory("Locations")]
        [OptionDescription("List of additional dependencies that will be tracked by opC++.  Detected changes will cause a full rebuild.")]
        [OptionEditorType(typeof(DependencyFilesEditor))]
        [GlobalOptionTypeConverter(typeof(StringListOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringListOptionConverter))]
        public StringListOption DependencyFiles = new StringListOption();

        /*=== additional feature directories ===*/

        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Feature Directories")]
        [OptionCategory("Locations")]
        [OptionDescription("Additional directories where features are installed.  The compiler will also look in the 'features/' directory where opCpp.exe is installed.")]
        [OptionEditorType(typeof(FeatureDirectoriesEditor))]
        [GlobalOptionTypeConverter(typeof(StringListOptionConverter))]
        [ProjectOptionTypeConverter(typeof(StringListOptionConverter))]
        public StringListOption FeatureDirectories = new StringListOption();

        ///==========================================
        /// 'Compiler Options' options
        ///==========================================
        
        /*=== -verbose command line option ===*/

        [CommandLine("-verbose")]
        [IsProjectOption]
        [IsGlobalOption]
		[OptionName("Verbose")]
        [OptionCategory("Compiler Options")]
		[OptionDescription("Tells the compiler to print verbose output.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption Verbose = new BoolOption(false);

        /*=== -diagnostics command line option ===*/

        [CommandLine("-diagnostics")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Diagnostics")]
        [OptionCategory("Compiler Options")]
        [OptionDescription("Tells the compiler to print diagnostic output.  This includes dialect validation messages.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption Diagnostics = new BoolOption(false);

        /*=== -silent command line option ===*/

        [CommandLine("-silent")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Silent")]
        [OptionCategory("Compiler Options")]
        [OptionDescription("The compiler will only print out compiler errors.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption Silent = new BoolOption(false);

        /*=== -force command line option ===*/

        [CommandLine("-force")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Force Recompile")]
        [OptionCategory("Compiler Options")]
        [OptionDescription("Forces the compiler to recompile everything.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption Force = new BoolOption(false);

        /*=== -nostandardincludes command line option ===*/

        [CommandLine("-nostandardincludes")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("No Standard Includes")]
        [OptionCategory("Compiler Options")]
        [OptionDescription("Tells the compiler to not search its standard include path.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption NoStandardIncludes = new BoolOption(false);

        ///==========================================
        /// 'Code Generation' options
        ///==========================================

        /*=== -nouninline command line option ===*/

		[CommandLine("-inlineall")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Inline All")]
        [OptionCategory("Code Generation")]
		[OptionDescription("Tells the compiler to inline all functions in .oh files.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption InlineAll = new BoolOption(false);

        /*=== -nodebug command line option ===*/
		
        [CommandLine("-nodebug")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("No Debugging")]
        [OptionCategory("Code Generation")]
		[OptionDescription("Suppress #line directives (suppress debugging redirection support).")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption NoDebug = new BoolOption(false);

        /*=== -highlighting command line option ===*/

		[CommandLine("-highlighting")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Highlighting")]
        [OptionCategory("Code Generation")]
		[OptionDescription(
        "Tells the compiler to generate highlighting #define's based on your dialect(s)." +
        "Note: This only works if you have a good syntax highlighting engine installed (e.g., Visual Assist)."
        )]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption Highlighting = new BoolOption(true);

        /*=== -ghosts command line option ===*/

		[CommandLine("-ghosts")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Ghosts")]
        [OptionCategory("Code Generation")]
		[OptionDescription("Tells the compiler to output ghost namespaces to improve IDE auto-completion.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption Ghosts = new BoolOption(true);

        /*=== -globmode command line option ===*/

		[CommandLine("-globmode")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Glob Mode")]
        [OptionCategory("Code Generation")]
		[OptionDescription(
        "Enables glob indexing mode.  In glob mode, the compiler creates index source and header files" +
        " (Generated.oohindex, Generated.ocppindex) that include all generated code files.  In this way," +
        " you can include all your generated code in a project by including 'Generated.oohindex'."
		)]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption GlobMode = new BoolOption(true);

        /*=== -expansion-depth command line option ===*/

		[CommandLine("-expansion-depth")]
        [IsProjectOption]
        [IsGlobalOption]
		[OptionName("Expansion Depth")]
        [OptionCategory("Code Generation")]
        [OptionDescription("Specifies the maximum opmacro expansion depth.")]
        [GlobalOptionTypeConverter(typeof(GlobalIntOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectIntOptionConverter))]
		public IntOption ExpansionDepth = new IntOption(100);

        /*=== -notations command line option ===*/

		[CommandLine("-notations")]
        [IsProjectOption]
        [IsGlobalOption]
		[OptionName("Notations")]
        [OptionCategory("Code Generation")]
        [OptionDescription("The compiler will place note expansion comments in the generated code.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption Notations = new BoolOption(false);

        /*=== -compact command line option ===*/

        [CommandLine("-compact")]
        [IsProjectOption]
        [IsGlobalOption]
        [OptionName("Compact")]
        [OptionCategory("Code Generation")]
        [OptionDescription("Almost all comments and empty lines are removed from the generated code.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption Compact = new BoolOption(true);

        /*=== -printxml command line option ===*/

        [CommandLine("-printxml")]
        [IsProjectOption]
        [IsGlobalOption]
		[OptionName("Print Xml")]
        [OptionCategory("Code Generation")]
        [OptionDescription("The compiler will generate an xml representation of your opC++ code.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
		public BoolOption PrintXml = new BoolOption(true);

        ///==========================================
        /// 'Addin' options
        ///==========================================
        
        /*=== whether or not to change the build commands' behavior ===*/

        [IsGlobalOption]
        [OptionName("Extend Commands")]
        [OptionCategory("Addin")]
        [OptionDescription("Extend the common IDE build, clean and debug commands so that opC++ is run first.")]
        [GlobalOptionTypeConverter(typeof(GlobalBoolOptionConverter))]
        [ProjectOptionTypeConverter(typeof(ProjectBoolOptionConverter))]
        public BoolOption ExtendCommands = new BoolOption(true);

        /*=== what command is called after opCpp builds solution on our toolbar ===*/

        [IsGlobalOption]
        [OptionName("Post Build Solution")]
        [OptionCategory("Addin")]
        [OptionDescription("The command that is executed after the opC++ build solution button successfully runs.")]
        [GlobalOptionTypeConverter(typeof(CommandConverter))]
        public StringOption PostBuildSolution = new StringOption("Build.BuildSolution");

        /*=== what command is called after opCpp builds project on our toolbar ===*/

        [IsGlobalOption]
        [OptionName("Post Build Project")]
        [OptionCategory("Addin")]
        [OptionDescription("The command that is executed after the opC++ build project button successfully runs.")]
        [GlobalOptionTypeConverter(typeof(CommandConverter))]
        public StringOption PostBuildProject = new StringOption("Build.BuildSelection");

        /*=== what command is called after opCpp cleans on our toolbar ===*/

        [IsGlobalOption]
        [OptionName("Post Clean Solution")]
        [OptionCategory("Addin")]
        [OptionDescription("The command that is executed after the opC++ clean solution button successfully runs.")]
        [GlobalOptionTypeConverter(typeof(CommandConverter))]
        public StringOption PostCleanSolution = new StringOption("Build.CleanSolution");

        /*=== whether or not to print the command line output ===*/

        [IsGlobalOption]
        [OptionName( "Output Commandline" )]
        [OptionCategory( "Addin" )]
        [OptionDescription( "Prints the compiler location, command line parameters, and current working directory." )]
        [GlobalOptionTypeConverter( typeof( GlobalBoolOptionConverter ) )]
         public BoolOption OutputCommandline = new BoolOption(false);
    };

    ///==========================================
    /// OptionInfo
    ///==========================================
    
    // This class can be used to parse all of an option object's attributes.
    public class OptionInfo
    {
        /*=== construction ===*/

        public OptionInfo(List<Attribute> attrs)
        {
            /*=== name ===*/

            OptionNameAttribute name = (OptionNameAttribute) AttributeUtility.GetAttribute<OptionNameAttribute>(attrs);

            if (name != null)
                Name = name.OptionName;

            /*=== is global ===*/

            IsGlobalOptionAttribute isGlobal = (IsGlobalOptionAttribute) AttributeUtility.GetAttribute<IsGlobalOptionAttribute>(attrs);

            if (isGlobal != null)
                IsGlobal = true;

            /*=== is project ===*/

            IsProjectOptionAttribute isProject = (IsProjectOptionAttribute) AttributeUtility.GetAttribute<IsProjectOptionAttribute>(attrs);

            if (isProject != null)
                IsProject = true;

            /*=== category ===*/

            OptionCategoryAttribute category = (OptionCategoryAttribute) AttributeUtility.GetAttribute<OptionCategoryAttribute>(attrs);

            if (category != null)
                Category = category.OptionCategory;

            /*=== description ===*/

            OptionDescriptionAttribute description = (OptionDescriptionAttribute) AttributeUtility.GetAttribute<OptionDescriptionAttribute>(attrs);

            if (description != null)
                Description = description.OptionDescription;

            /*=== editor type ===*/

            OptionEditorTypeAttribute editorType = (OptionEditorTypeAttribute) AttributeUtility.GetAttribute<OptionEditorTypeAttribute>(attrs);

            if (editorType != null)
                EditorType = editorType.OptionEditorType.AssemblyQualifiedName;

            /*=== global type converter ===*/

            GlobalOptionTypeConverterAttribute globalTypeConverter = (GlobalOptionTypeConverterAttribute) AttributeUtility.GetAttribute<GlobalOptionTypeConverterAttribute>(attrs);

            if (globalTypeConverter != null)
                GlobalTypeConverter = globalTypeConverter.OptionTypeConverter.AssemblyQualifiedName;

            /*=== project type converter ===*/

            ProjectOptionTypeConverterAttribute projectTypeConverter = (ProjectOptionTypeConverterAttribute) AttributeUtility.GetAttribute<ProjectOptionTypeConverterAttribute>(attrs);

            if (projectTypeConverter != null)
                ProjectTypeConverter = projectTypeConverter.OptionTypeConverter.AssemblyQualifiedName;

            /*=== command line converter ===*/

            CommandLineAttribute commandLine = (CommandLineAttribute) AttributeUtility.GetAttribute<CommandLineAttribute>(attrs);

            if (commandLine != null)
                CommandLine = commandLine.CommandLine;
        }

        /*=== data ===*/

        public string Name                 = "";
        public bool   IsGlobal             = false;
        public bool   IsProject            = false;
        public string Category             = "";
        public string Description          = "";
        public string EditorType           = "";
        public string GlobalTypeConverter  = "";
        public string ProjectTypeConverter = "";
        public string CommandLine          = "";
    };
}
