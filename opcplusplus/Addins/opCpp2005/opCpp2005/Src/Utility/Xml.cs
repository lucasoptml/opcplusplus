///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Xml.cs
/// Date: 09/21/2007
///
/// Description:
///
/// Xml utility methods.
///****************************************************************

using System;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Collections.Generic;

namespace opGamesLLC.opCpp2005
{
    public class XmlUtility
    {
        // Saves this set of options as xml to the specified file.
        public static bool SaveXml<T>(string file, ref T t)
        {
            XmlSerializer s;
            TextWriter    w;

            // Try to create the serializer.
            try
            {
                s = new XmlSerializer(typeof(T));
            }
            catch (Exception)
            {
                return false;
            }

            // Try to open the file.
            try
            {
                w = new StreamWriter(file);
            }
            catch (Exception)
            {
                return false;
            }

            // Try to serialize the file.
            try
            {
                s.Serialize(w, t);
            }
            catch (Exception)
            {
                return false;
            }

            w.Close();

            return true;
        }

        // Loads a set of options from the specified xml file to this object.
        public static bool LoadXml<T>(string file, ref T t)
        {
            XmlSerializer s;
            TextReader    r;

            // Try to create the serializer.
            try
            {
                s = new XmlSerializer(typeof(T));
            }
            catch (Exception)
            {
                return false;
            }

            // Try to open the file.
            try
            {
                r = new StreamReader(file);
            }
            catch (Exception)
            {
                return false;
            }       

            // Try to deserialize the file.
            try
            {
                t = (T) s.Deserialize(r);
            }
            catch (Exception)
            {
                return false;
            }

            r.Close();

            return true;
        }

        // Returns a list of xml elements matching a certain tag name to a depth of n.  
        // If a matching tag is found, it will not look below that tag for more elements.
        public static void GetElementsByTag(XmlElement element, string tag, int depth, List<XmlElement> nodes)
        {
            if (depth <= 0)
                return;

            foreach (object o in element.ChildNodes)
            {
                XmlElement e = o as XmlElement;

                if (e == null)
                    continue;

                if (e.Name == tag)
                    nodes.Add(e);
                else
                    GetElementsByTag(e, tag, depth - 1, nodes);
            }
        }
    }
}