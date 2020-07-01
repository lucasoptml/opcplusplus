///****************************************************************
/// Copyright (C) 2007 OP Games LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Test.cs
/// Date: 10/12/2007
///
/// Description:
///
/// Put your benchmark here.
///****************************************************************

using System;
using System.Reflection;
using FastDynamicPropertyAccessor;
using System.Collections.Generic;

namespace Csharp
{
    public class MyObject
    {
        /*=== data ===*/

        public bool   MyBool   = false;
        public int    MyInt    = 0; 
        public float  MyFloat  = 0;
        public double MyDouble = 0;
        public string MyString = "string";

        /*=== virtuals ===*/

        public virtual void Blah(string s)
        {
            
        }

		public bool MyBoolProp
		{
			get
			{
				return MyBool;
			}
			set
			{
				MyBool = value;
			}
		}

		public int MyIntProp
		{
			get
			{
				return MyInt;
			}
			set
			{
				MyInt = value;
			}
		}

		public float MyFloatProp
		{
			get
			{
				return MyFloat;
			}
			set
			{
				MyFloat = value;
			}
		}

		public double MyDoubleProp
		{
			get
			{
				return MyDouble;
			}
			set
			{
				MyDouble = value;
			}
		}

		public string MyStringProp
		{
			get
			{
				return MyString;
			}
			set
			{
				MyString = value;
			}
		}
    };

	//no caching...
    public class TestObject
    {
        // Put initializtion code here.
        public TestObject()
        {
            myobject = new MyObject();
        }

        // Implement your benchmark here.
        public void Test()
        {
			Type oType = typeof(MyObject);
			FieldInfo[] oFields;
            oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);

            for (int i = 0; i < oFields.Length; i++)
            {
                if (oFields[i].FieldType == typeof(string))
                {
                    string s = oFields[i].GetValue(myobject) as string;

                    myobject.Blah(s);
                }
            }
        }

        private MyObject myobject;
    };

	//cache a full field map, and just iterate it manually
	public class TestObjectCacheType
	{
		// Put initializtion code here.
		public TestObjectCacheType()
		{
			myobject = new MyObject();
			oFields = oType.GetFields(BindingFlags.Public | BindingFlags.Instance);
		}
		
		Type oType = typeof(MyObject);
		FieldInfo[] oFields;

		// Implement your benchmark here.
		public void Test()
		{
			for (int i = 0; i < oFields.Length; i++)
			{
				if (oFields[i].FieldType == typeof(string))
				{
					string s = oFields[i].GetValue(myobject) as string;

					myobject.Blah(s);
				}
			}
		}

		private MyObject myobject;
	};


		//cache a full field map, and just iterate it manually
	public class TestObjectAccessors
	{
		// Put initializtion code here.
		public TestObjectAccessors()
		{
			myobject = new MyObject();
			
			PropertyInfo[] oProps; 
			Type oType = typeof(MyObject);

			oProps = oType.GetProperties();


			for (int i = 0; i < oProps.Length; i++)
			{
				Accessors.Add(new PropertyAccessor(oType, oProps[i].Name));
			}
		}
		
		List<PropertyAccessor> Accessors = new List<PropertyAccessor>();
		
		// Implement your benchmark here.
		public void Test()
		{
			for (int i = 0; i < Accessors.Count; i++)
			{
				if (Accessors[i].PropertyType == typeof(string))
				{
					string s = Accessors[i].Get(myobject) as string;

					myobject.Blah(s);
				}
			}
		}

		private MyObject myobject;
	};

}