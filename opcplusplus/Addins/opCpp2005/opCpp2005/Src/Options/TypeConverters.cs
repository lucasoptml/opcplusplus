///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: TypeConverters.cs
/// Date: 09/25/2007
///
/// Description:
///
/// Contains type converters for property grid stuff.
///****************************************************************

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using EnvDTE;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// StringListOptionConverter
    ///==========================================
    
    public class StringListOptionConverter : StringConverter
    {
        /*=== overrides ===*/

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(StringListOption))
                return true;
           
            return base.CanConvertTo(context, destinationType);
        }
        
        public override object ConvertTo(ITypeDescriptorContext context, 
                                         CultureInfo culture, 
                                         object value, 
                                         Type destinationType)
        {
            if (destinationType == typeof(string)
            &&  value is StringListOption)
            {
                StringListOption option = (StringListOption) value;
                List<string>     list   = (List<string>) option.StringList;

                if (list != null)
                {
                    string output = "";
                    int    size   = list.Count;

                    if (size > 0)
                    {
                        output = list[0];

                        for (int i = 1; i < size; i++)
                            output += ";" + list[i];
                    }
                    
                    return output;
                }
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value is string)
            {
                string           val        = (string) value;
                StringListOption option     = new StringListOption();
                List<string>     strings    = option.StringList; 
                char[]           splitchars = {';'};
                string[]         substrings = val.Split(splitchars);

                foreach (string s in substrings)
                {
                    string x = s.Trim();

                    if (x != "")
                        strings.Add(x);
                }
                
                return option;
            }

            return base.ConvertFrom(context, culture, value);
        }
    };

    ///==========================================
    /// StringOptionConverter
    ///==========================================

    public class StringOptionConverter : StringConverter
    {
        /*=== overrides ===*/

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(StringOption))
                return true;

            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context,
                                         CultureInfo culture,
                                         object value,
                                         Type destinationType)
        {
            if (destinationType == typeof(string)
            &&  value is StringOption)
            {
                StringOption option = (StringOption) value;

                return option.Value;
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value is string)
            {
                string       val    = (string) value;
                StringOption option = new StringOption();

                option.Value = val;

                return option;
            }

            return base.ConvertFrom(context, culture, value);
        }
    };

    ///==========================================
    /// CommandConverter
    ///==========================================

    public class CommandConverter : StringConverter
    {
        /*=== data ===*/

        private static StandardValuesCollection comboValues = null;

        public static StandardValuesCollection ComboValues
        {
            get
            {
                if (comboValues == null)
                {
                    List<string> strings = new List<string>();

                    foreach (Command c in opCpp2005.App().Commands)
                    {
                        if (c.Name != "")
                            strings.Add(c.Name);
                    }

                    strings.Sort();
                    strings.Insert(0, "");

                    List<StringOption> stringOptions = new List<StringOption>();

                    foreach (string s in strings)
                        stringOptions.Add(new StringOption(s));

                    comboValues = new StandardValuesCollection(stringOptions);
                }

                return comboValues;
            }
        }

        /*=== overrides ===*/

        // Tells the property grid that we support a list of strings.
        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }

        // Says that we can't edit the combo box.
        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            return true;
        }

        // Return the initial list of values that should be in the combo box.
        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            return ComboValues;
        }

        // This method tells the property grid that we can convert to a BoolOption from a string.
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(StringOption))
                return true;

            return base.CanConvertTo(context, destinationType);
        }

        // This method actually converts from a StringOption to a string.
        public override object ConvertTo(ITypeDescriptorContext context,
                                         CultureInfo culture,
                                         object value,
                                         Type destinationType)
        {
            if (destinationType == typeof(string)
            && value is StringOption)
            {
                StringOption option = (StringOption) value;

                return option.Value;
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        // This method tells the property grid that we can convert from a string to a BoolOption.
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        // This method actually converts from a string to a BoolOption.
        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value is string)
            {
                string       val    = (string) value;
                StringOption option = new StringOption();

                option.Value = val;

                return option;
            }

            return base.ConvertFrom(context, culture, value);
        }
    };

    ///==========================================
    /// BoolOptionConverter
    ///==========================================
    
    public abstract class BoolOptionConverter : StringConverter
    {
        /*=== overrides ===*/

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            return true;
        }

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(BoolOption))
                return true;

            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context,
                                         CultureInfo culture,
                                         object value,
                                         Type destinationType)
        {
            if (destinationType == typeof(string)
            &&  value is BoolOption)
            {
                BoolOption option = (BoolOption) value;

                if (option.UseDefault)
                    return "Default";

                return option.Value.ToString();
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value is string)
            {
                string     val    = (string) value;
                BoolOption option = new BoolOption();

                if (val == "True")
                    option.Value = true;
                else if (val == "False")
                    option.Value = false;
                else if (val == "Default")
                {
                    option.Value      = false;
                    option.UseDefault = true;
                }

                return option;
            }

            return base.ConvertFrom(context, culture, value);
        }
    };

    ///==========================================
    /// GlobalBoolOptionConverter
    ///==========================================

    public class GlobalBoolOptionConverter : BoolOptionConverter
    {
        /*=== data ===*/

        private StandardValuesCollection comboValues = null;

        public StandardValuesCollection ComboValues
        {
            get
            {
                if (comboValues == null)
                {
                    comboValues = new StandardValuesCollection(new BoolOption[] {new BoolOption(true), 
                                                                                 new BoolOption(false)});
                }

                return comboValues;
            }
        }

        /*=== overrides ===*/

        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            return ComboValues;
        }
    };

    ///==========================================
    /// ProjectBoolOptionConverter
    ///==========================================

    public class ProjectBoolOptionConverter : BoolOptionConverter
    {
        /*=== data ===*/

        private StandardValuesCollection comboValues = null;

        public StandardValuesCollection ComboValues
        {
            get
            {
                if (comboValues == null)
                {
                    BoolOption defaultOption = new BoolOption();
                    defaultOption.UseDefault = true;

                    comboValues = new StandardValuesCollection(new BoolOption[] {new BoolOption(true),
                                                                                 new BoolOption(false),
                                                                                 defaultOption});
                }

                return comboValues;
            }
        }

        /*=== overrides ===*/

        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            return ComboValues;
        }
    };

    ///==========================================
    /// GlobalIntOptionConverter
    ///==========================================
    
    public class GlobalIntOptionConverter : StringConverter
    {
        /*=== overrides ===*/

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(IntOption))
                return true;
           
            return base.CanConvertTo(context, destinationType);
        }
        
        public override object ConvertTo(ITypeDescriptorContext context, 
                                         CultureInfo culture, 
                                         object value, 
                                         Type destinationType)
        {
            if (destinationType == typeof(string)
            &&  value is IntOption)
            {
                IntOption option = (IntOption) value;

                return option.Value.ToString();
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value is string)
            {
                string    val    = (string) value;
                IntOption option = new IntOption();

                try 
                {
                    option.Value = Convert.ToInt32(val);

                    return option;
                }
                catch (Exception) {}
            }

            return base.ConvertFrom(context, culture, value);
        }
    };

    ///==========================================
    /// ProjectIntOptionConverter
    ///==========================================

    public class ProjectIntOptionConverter : StringConverter
    {
        /*=== data ===*/

        private List<int> ints = new List<int>();

        public StandardValuesCollection ComboValues
        {
            get
            {
                ints.Sort();

                List<IntOption> intOptions = new List<IntOption>();

                foreach (int i in ints)
                    intOptions.Add(new IntOption(i));

                IntOption defaultOption  = new IntOption();
                defaultOption.UseDefault = true;

                intOptions.Add(defaultOption);

                return new StandardValuesCollection(intOptions);
            }
        }

        /*=== overrides ===*/

        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }

        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            return ComboValues;
        }

        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(IntOption))
                return true;

            return base.CanConvertTo(context, destinationType);
        }

        public override object ConvertTo(ITypeDescriptorContext context,
                                         CultureInfo culture,
                                         object value,
                                         Type destinationType)
        {
            if (destinationType == typeof(string)
            &&  value is IntOption)
            {
                IntOption option = (IntOption) value;

                if (option.UseDefault)
                    return "Default";
                else if (!ints.Exists(delegate(int x) { return x == option.Value; }))
                    ints.Add(option.Value);

                return option.Value.ToString();
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        // This method tells the property grid that we can convert from a string to a StringOption.
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        // This method actually converts from a string to a list of strings.
        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value is string)
            {
                string    val    = (string) value;
                IntOption option = new IntOption();

                if (val == "Default")
                {
                    option.Value      = 0;
                    option.UseDefault = true;

                    return option;
                }

                try 
                {
                    option.Value = Convert.ToInt32(val);

                    if (!ints.Exists(delegate(int x) { return x == option.Value; }))
                        ints.Add(option.Value);

                    return option;
                }
                catch (Exception) {}
            }

            return base.ConvertFrom(context, culture, value);
        }
    };
}