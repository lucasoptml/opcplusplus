///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: OptionTypes.cs
/// Date: 09/20/2007
///
/// Description:
///
/// Declares classes for option types.
///****************************************************************

using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using System;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// OptionBase
    ///==========================================
    
    public class OptionBase
    {
        /*=== virtuals ===*/

        public virtual Type GetOptionType()
        {
            return null;
        }
    };

    ///==========================================
    /// StringListOption
    ///==========================================

    public class StringListOption : OptionBase
    {
        /*=== construction ===*/

        public StringListOption()
        {
            stringlist = new List<string>();
        }

        public StringListOption(List<string> _stringlist)
        {
            stringlist = _stringlist;
        }

        /*=== overrides ===*/

        public override Type GetOptionType()
        {
            return typeof(StringListOption);
        }

        /*=== utility ===*/

        public void Add(string s)
        {
            stringlist.Add(s);
        }

        public void Remove(string s)
        {
            stringlist.Remove(s);
        }

        /*=== data ===*/

        protected List<string> stringlist;

        [XmlArray]
        [XmlArrayItem("String")]
        public List<string> StringList
        {
            get { return stringlist; }
            set { stringlist = value; }
        }
    };

    ///==========================================
    /// StringOption
    ///==========================================

    public class StringOption : OptionBase
    {
        /*=== construction ===*/

        public StringOption()
        {

        }

        public StringOption(string _val)
        {
            val = _val;
        }

        /*=== overrides ===*/

        public override Type GetOptionType()
        {
            return typeof(StringOption);
        }

        /*=== data ===*/

        protected string val = "";

        public string Value
        {
            get { return val; }
            set { val = value; }
        }
    };

    ///==========================================
    /// DefaultOptionBase
    ///==========================================
    
    public class DefaultOptionBase : OptionBase
    {
        /*=== data ===*/

        protected bool usedefault = false;

        public bool UseDefault 
        {
            get { return usedefault; }
            set { usedefault = value; }
        }
    };

    ///==========================================
    /// BoolOption
    ///==========================================

    public class BoolOption : DefaultOptionBase
    {
        /*=== construction ===*/

        public BoolOption()
        {

        }

        public BoolOption(bool _val)
        {
            val = _val;
        }

        /*=== overrides ===*/

        public override Type GetOptionType()
        {
            return typeof(BoolOption);
        }

        /*=== data ===*/

        protected bool val = false;

        public bool Value
        {
            get { return val; }
            set { val = value; }
        }
    };

    ///==========================================
    /// IntOption
    ///==========================================
    
    public class IntOption : DefaultOptionBase
    {
        /*=== construction ===*/

        public IntOption()
        {

        }

        public IntOption(int _val)
        {
            val = _val;
        }

        /*=== overrides ===*/

        public override Type GetOptionType()
        {
            return typeof(IntOption);
        }

        /*=== data ===*/

        protected int val = 0;

        public int Value
        {
            get { return val; }
            set { val = value; }
        }
    };
}