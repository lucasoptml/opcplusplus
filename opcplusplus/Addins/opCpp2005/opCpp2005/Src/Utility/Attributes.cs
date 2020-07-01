///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Attributes.cs
/// Date: 09/24/2007
///
/// Description:
///
/// Contains attribute utility methods.
///****************************************************************

using System;
using System.Reflection;
using System.Collections.Generic;

namespace opGamesLLC.opCpp2005
{
    public class AttributeUtility
    {
        // Returns all attributes from an object as a list.
        public static List<Attribute> GetAllAttributes(FieldInfo field, bool inherit)
        {
            Array a = (Array) field.GetCustomAttributes(inherit);

            List<Attribute> attrs = new List<Attribute>();

            foreach (Attribute attr in a)
                attrs.Add(attr);

            return attrs;
        }

        // Returns an attribute of a specific type from a list, or null if none exist.
        public static Attribute GetAttribute<T>(List<Attribute> attrs)
        {
            Type tType = typeof(T);

            foreach (Attribute a in attrs)
            {
                Type aType = a.GetType();

                if (tType == aType)
                    return a;
            }

            return null;
        }
    };
}