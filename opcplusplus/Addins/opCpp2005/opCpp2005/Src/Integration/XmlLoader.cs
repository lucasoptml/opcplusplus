///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: XmlLoader.cs
/// Date: 10/16/2007
///
/// Description:
///
/// Xml querying tool for generating intellisense.
///****************************************************************

using System;
using System.Xml;
using System.Collections.Generic;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// XmlNodeBase
    ///==========================================
    
    public abstract class XmlNodeBase 
    {
        /*=== data ===*/

        public string Name = "";

        /*=== methods ===*/

        // Utility method to try and populate a string.
        public void SetString(XmlElement element, string tag, ref string s)
        {
            List<XmlElement> elements = new List<XmlElement>();
            
            XmlUtility.GetElementsByTag(element, tag, 1, elements);

            if (elements.Count > 0)
                s = elements[0].InnerText.Trim();
        }

        // Utility method to try and populate a list of strings.
        public void SetStrings(XmlElement element, string tag, ref List<string> container)
        {
            List<XmlElement> elements = new List<XmlElement>();

            XmlUtility.GetElementsByTag(element, tag, 1, elements);

            foreach (XmlElement e in elements)
                container.Add(e.InnerText.Trim());
        }

        // Utility method to try and populate a list of nodes.
        public void SetNodes<T>(XmlElement element, string tag, ref List<T> container) where T : XmlNodeBase, new() 
        {
            List<XmlElement> elements = new List<XmlElement>();

            XmlUtility.GetElementsByTag(element, tag, 1, elements);

            foreach (XmlElement e in elements)
            {
                string name = "";

                SetString(e, "Name", ref name);

                T existing = container.Find(delegate(T x) { return x.Name == name; });

                if (existing != null)
                    existing.Load(e);
                else
                {
                    T newNode = new T();

                    newNode.Load(e);
                    
                    container.Add(newNode);
                }                    
            }
        }

        /*=== virtuals ===*/

        // Override this to load a node from an xml node.
        public virtual void Load(XmlElement element)
        {

        }
    };

    ///==========================================
    /// XmlModifierNode
    ///==========================================
    
    public class XmlModifierNode : XmlNodeBase
    {
        /*=== data ===*/

        public string Description = "";

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the description.
            SetString(element, "Description", ref Description);
        }
    };

    ///==========================================
    /// XmlArgumentNode
    ///==========================================
    
    public class XmlArgumentNode : XmlNodeBase
    {
        /*=== data ===*/

        public string Description = "";

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the description.
            SetString(element, "Description", ref Description);
        }
    };

    ///==========================================
    /// XmlNoteNode
    ///==========================================
    
    public class XmlNoteNode : XmlNodeBase
    {
        /*=== data ===*/

        public List<XmlModifierNode> Modifiers = new List<XmlModifierNode>();
        public List<XmlArgumentNode> Arguments = new List<XmlArgumentNode>();        

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the modifiers.
            SetNodes<XmlModifierNode>(element, "Modifier", ref Modifiers);

            // Grab the arguments.
            SetNodes<XmlArgumentNode>(element, "Argument", ref Arguments);
        }
    };

    ///==========================================
    /// XmlMapNode
    ///==========================================
    
    public class XmlMapNode : XmlNodeBase
    {
        /*=== data ===*/

        public string                Type      = "";
        public List<XmlNoteNode>     Notes     = new List<XmlNoteNode>();
        public List<XmlModifierNode> Modifiers = new List<XmlModifierNode>();

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the type.
            SetString(element, "Type", ref Type);

            // Grab the notes.
            SetNodes<XmlNoteNode>(element, "Note", ref Notes);

            // Grab the modifiers.
            SetNodes<XmlModifierNode>(element, "Modifier", ref Modifiers);
        }
    };

    ///==========================================
    /// XmlLocationNode
    ///==========================================
    
    public class XmlLocationNode : XmlNodeBase
    {
        /*=== data ===*/

        public List<XmlNoteNode> Notes = new List<XmlNoteNode>();
        public List<XmlMapNode>  Maps  = new List<XmlMapNode>();

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the notes.
            SetNodes<XmlNoteNode>(element, "Note", ref Notes);

            // Grab the maps.
            SetNodes<XmlMapNode>(element, "Map", ref Maps);
        }
    };

    ///==========================================
    /// XmlDisallowNode
    ///==========================================
    
    public class XmlDisallowNode : XmlNodeBase
    {
        /*=== data ===*/

        public List<XmlModifierNode> Modifiers = new List<XmlModifierNode>();

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the modifiers.
            SetNodes<XmlModifierNode>(element, "Modifier", ref Modifiers);
        }
    };

    ///==========================================
    /// XmlCategoryModifierNode
    ///==========================================
    
    public class XmlCategoryModifierNode : XmlNodeBase
    {
        /*=== data ===*/

        public string Type  = "";
        public string Value = "";

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the type.
            SetString(element, "Type", ref Type);

            // Grab the value.
            SetString(element, "Value", ref Value);
        }
    };

    ///==========================================
    /// XmlCategoryNode
    ///==========================================
    
    public class XmlCategoryNode : XmlNodeBase
    {
        /*=== data ===*/

        public List<XmlModifierNode> Modifiers = new List<XmlModifierNode>();
        public List<XmlDisallowNode> Disallows = new List<XmlDisallowNode>();
        public List<XmlLocationNode> Locations = new List<XmlLocationNode>();

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the modifiers.
            SetNodes<XmlModifierNode>(element, "Modifier", ref Modifiers);

            // Grab the disallows.
            SetNodes<XmlDisallowNode>(element, "Disallow", ref Disallows);

            // Grab the locations.
            SetNodes<XmlLocationNode>(element, "Location", ref Locations);
        }
    };

    ///==========================================
    /// XmlEnumerationNode
    ///==========================================
    
    public class XmlEnumerationNode : XmlNodeBase
    {
        /*=== data ===*/

        public List<XmlDisallowNode> Disallows = new List<XmlDisallowNode>();
        public List<XmlLocationNode> Locations = new List<XmlLocationNode>();

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the name.
            SetString(element, "Name", ref Name);

            // Grab the disallows.
            SetNodes<XmlDisallowNode>(element, "Disallow", ref Disallows);

            // Grab the locations.
            SetNodes<XmlLocationNode>(element, "Location", ref Locations);
        }
    };

    ///==========================================
    /// XmlRootNode
    ///==========================================
    
    public class XmlRootNode : XmlNodeBase
    {
        /*=== data ===*/

        public string                   Stylesheet   = "";
        public string                   Directory    = "";
        public List<XmlCategoryNode>    Categories   = new List<XmlCategoryNode>();
        public List<XmlEnumerationNode> Enumerations = new List<XmlEnumerationNode>();

        /*=== methods ===*/

        public override void Load(XmlElement element)
        {
            // Grab the stylesheet.
            SetString(element, "Stylesheet", ref Stylesheet);

            // Grab the directory.
            SetString(element, "Directory", ref Directory);
           
            // Grab the categories.
            SetNodes<XmlCategoryNode>(element, "Category", ref Categories);

            // Grab the enumerations.
            SetNodes<XmlEnumerationNode>(element, "Enumeration", ref Enumerations);
        }
    };

    ///==========================================
    /// XmlDialectLoader
    ///==========================================

    public class XmlDialectLoader
    {
        /*=== data ===*/

        private XmlDocument doc  = new XmlDocument();
        public  XmlRootNode Root = new XmlRootNode();

        /*=== methods ===*/

        // Try to reload the xml file.
        public bool Reload(string file)
        {
            /*=== Try to load the file. ===*/

            try
            {
                doc.Load(file);
            }
            catch (Exception)
            {
                return false;
            }

            /*=== Grab the root. ===*/

            XmlNodeList nodelist = doc.GetElementsByTagName("Dialect");

            // Fail if we don't have a root.
            if (nodelist.Count < 1)
                return false;

            Root = new XmlRootNode();

            XmlElement root = nodelist.Item(0) as XmlElement;

            Root.Load(root);

            return true;
        }

        // Get a desired node.
        public XmlNoteNode GetNote(string path)
        {
            List<string> strings = new List<string>(path.Split(new string[] {"::"}, StringSplitOptions.RemoveEmptyEntries));
            int          size    = strings.Count;

            // Make sure the size is either 3 or 4.
            if (size != 3 && size != 4)
                return null;

            // Grab the category/enumeration.
            XmlCategoryNode    category    = Root.Categories.Find(delegate(XmlCategoryNode x) { return x.Name == strings[0]; });
            XmlEnumerationNode enumeration = null;

			if (category == null)
			{
				enumeration = Root.Enumerations.Find(delegate(XmlEnumerationNode x) { return x.Name == strings[0]; });

				if (enumeration == null)
					return null;
			}

            // Grab the location.
            XmlLocationNode location = null;
            
            if (category != null)
                location = category.Locations.Find(delegate(XmlLocationNode x) { return x.Name == strings[1]; });
            else
                location = enumeration.Locations.Find(delegate(XmlLocationNode x) { return x.Name == strings[1]; });

            if (location == null)
                return null;

            // If we have 3 strings, it's a note.  Otherwise it's a map.
            if (strings.Count == 3)
            {
                // Grab the note.
                XmlNoteNode note = location.Notes.Find(delegate(XmlNoteNode x) { return x.Name == strings[2]; });

                return note;
            }
            else
            {
                // Grab the map.
                XmlMapNode map = location.Maps.Find(delegate(XmlMapNode x) { return x.Name == strings[2]; });

                if (map == null)
                    return null;

                // Grab the note.
                XmlNoteNode note = map.Notes.Find(delegate(XmlNoteNode x) { return x.Name == strings[3]; });

                return note;
            }
        }
    };
}