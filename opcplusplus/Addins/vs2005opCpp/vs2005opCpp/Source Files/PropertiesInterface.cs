///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: PropertiesInterface.cs
/// Date: 09/19/2007
///
/// Description:
///
/// opCpp Visual Studio properties interface.
///****************************************************************

using System;
using Microsoft.Win32;

namespace vs2005opCpp
{
    public interface PropertiesInterface
    {
        // opCpp path property
        string opCppPath
        {
            get;
            set;
        }

        // -verbose property.
        bool Verbose
        {
            get;
            set;
        }

        // -uninline property.
        bool Uninline
        {
            get;
            set;
        }

        // -nodebug property
        bool NoDebug
        {
            get;
            set;
        }

        // -highlighting property
        bool Highlighting
        {
            get;
            set;
        }

        // -ghosts property
        bool Ghosts
        {
            get;
            set;
        }

        // -silent property
        bool Silent
        {
            get;
            set;
        }

        // -force property
        bool Force
        {
            get;
            set;
        }

        // -fixedsys property
        bool FixedSys
        {
            get;
            set;
        }

        // -clean property
        bool Clean
        {
            get;
            set;
        }

        // -globmode property
        bool GlobMode
        {
            get;
            set;
        }

        // -expansion-depth property
        int ExpansionDepth
        {
            get;
            set;
        }

        // -testmode property
        bool TestMode
        {
            get;
            set;
        }

        // -tree property
        bool Tree
        {
            get;
            set;
        }

        // -fulltree property
        bool FullTree
        {
            get;
            set;
        }

        // -notations property
        bool Notations
        {
            get;
            set;
        }
	
        // additional arguments for the opCpp compiler
        string AdditionalArguments
        {
            get;
            set;
        }
    }
}


/*
Options
-------
Note: If a file or directory includes spaces, it must be in quotes!
      Separate multiple strings with commas and NO SPACES.

      Example:  opcpp -oh file1.oh,"file 2.oh" -d dir1,dir2 -category uclass,scriptclass

-oh
	Input files to be compiled.

-d
	Input file directories, separated by a comma.

-ohd
	All .oh files within these directories will be parsed.

-gd
	Generated files output directory.

-doh
	Input dialect files to be read.

-altstruct
	Specify alternative mappings for struct categories (syntax: structprefix=category,...).

-altclass
	Specify alternative mappings for class categories (syntax: classprefix=category,...).
*/