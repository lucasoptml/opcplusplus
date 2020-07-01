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

using System.Collections.Generic;
using System;
using System.Collections;
using System.Runtime.Serialization;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization.Formatters.Soap;
using System.Xml.Serialization;
using System.Xml;


	[XmlInclude(typeof(object))]
	public class TestClassBase
	{

	};

	[Serializable()]
	public class a_a_a_TestClass : TestClassBase
	{
		public a_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_a_a_TestClass : TestClassBase
	{
		public b_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_a_a_TestClass : TestClassBase
	{
		public c_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		List<string>  stringlist = new List<string>();
	};



	[Serializable()]
	public class d_a_a_TestClass : TestClassBase
	{
		public d_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_a_a_TestClass : TestClassBase
	{
		public e_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_a_a_TestClass : TestClassBase
	{
		public f_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_a_a_TestClass : TestClassBase
	{
		public g_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_a_a_TestClass : TestClassBase
	{
		public h_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_a_a_TestClass : TestClassBase
	{
		public i_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_a_a_TestClass : TestClassBase
	{
		public j_a_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_b_a_TestClass : TestClassBase
	{
		public a_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_b_a_TestClass : TestClassBase
	{
		public b_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_b_a_TestClass : TestClassBase
	{
		public c_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_b_a_TestClass : TestClassBase
	{
		public d_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_b_a_TestClass : TestClassBase
	{
		public e_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_b_a_TestClass : TestClassBase
	{
		public f_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_b_a_TestClass : TestClassBase
	{
		public g_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_b_a_TestClass : TestClassBase
	{
		public h_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_b_a_TestClass : TestClassBase
	{
		public i_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_b_a_TestClass : TestClassBase
	{
		public j_b_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_c_a_TestClass : TestClassBase
	{
		public a_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_c_a_TestClass : TestClassBase
	{
		public b_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_c_a_TestClass : TestClassBase
	{
		public c_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_c_a_TestClass : TestClassBase
	{
		public d_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_c_a_TestClass : TestClassBase
	{
		public e_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_c_a_TestClass : TestClassBase
	{
		public f_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_c_a_TestClass : TestClassBase
	{
		public g_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_c_a_TestClass : TestClassBase
	{
		public h_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_c_a_TestClass : TestClassBase
	{
		public i_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_c_a_TestClass : TestClassBase
	{
		public j_c_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_d_a_TestClass : TestClassBase
	{
		public a_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_d_a_TestClass : TestClassBase
	{
		public b_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_d_a_TestClass : TestClassBase
	{
		public c_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_d_a_TestClass : TestClassBase
	{
		public d_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_d_a_TestClass : TestClassBase
	{
		public e_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_d_a_TestClass : TestClassBase
	{
		public f_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_d_a_TestClass : TestClassBase
	{
		public g_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_d_a_TestClass : TestClassBase
	{
		public h_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_d_a_TestClass : TestClassBase
	{
		public i_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_d_a_TestClass : TestClassBase
	{
		public j_d_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_e_a_TestClass : TestClassBase
	{
		public a_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_e_a_TestClass : TestClassBase
	{
		public b_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_e_a_TestClass : TestClassBase
	{
		public c_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_e_a_TestClass : TestClassBase
	{
		public d_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_e_a_TestClass : TestClassBase
	{
		public e_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_e_a_TestClass : TestClassBase
	{
		public f_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_e_a_TestClass : TestClassBase
	{
		public g_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_e_a_TestClass : TestClassBase
	{
		public h_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_e_a_TestClass : TestClassBase
	{
		public i_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_e_a_TestClass : TestClassBase
	{
		public j_e_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_f_a_TestClass : TestClassBase
	{
		public a_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_f_a_TestClass : TestClassBase
	{
		public b_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_f_a_TestClass : TestClassBase
	{
		public c_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_f_a_TestClass : TestClassBase
	{
		public d_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_f_a_TestClass : TestClassBase
	{
		public e_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_f_a_TestClass : TestClassBase
	{
		public f_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_f_a_TestClass : TestClassBase
	{
		public g_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_f_a_TestClass : TestClassBase
	{
		public h_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_f_a_TestClass : TestClassBase
	{
		public i_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_f_a_TestClass : TestClassBase
	{
		public j_f_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_g_a_TestClass : TestClassBase
	{
		public a_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_g_a_TestClass : TestClassBase
	{
		public b_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_g_a_TestClass : TestClassBase
	{
		public c_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_g_a_TestClass : TestClassBase
	{
		public d_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_g_a_TestClass : TestClassBase
	{
		public e_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_g_a_TestClass : TestClassBase
	{
		public f_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_g_a_TestClass : TestClassBase
	{
		public g_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_g_a_TestClass : TestClassBase
	{
		public h_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_g_a_TestClass : TestClassBase
	{
		public i_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_g_a_TestClass : TestClassBase
	{
		public j_g_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_h_a_TestClass : TestClassBase
	{
		public a_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_h_a_TestClass : TestClassBase
	{
		public b_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_h_a_TestClass : TestClassBase
	{
		public c_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_h_a_TestClass : TestClassBase
	{
		public d_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_h_a_TestClass : TestClassBase
	{
		public e_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_h_a_TestClass : TestClassBase
	{
		public f_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_h_a_TestClass : TestClassBase
	{
		public g_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_h_a_TestClass : TestClassBase
	{
		public h_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_h_a_TestClass : TestClassBase
	{
		public i_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_h_a_TestClass : TestClassBase
	{
		public j_h_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_i_a_TestClass : TestClassBase
	{
		public a_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_i_a_TestClass : TestClassBase
	{
		public b_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_i_a_TestClass : TestClassBase
	{
		public c_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_i_a_TestClass : TestClassBase
	{
		public d_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_i_a_TestClass : TestClassBase
	{
		public e_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_i_a_TestClass : TestClassBase
	{
		public f_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_i_a_TestClass : TestClassBase
	{
		public g_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_i_a_TestClass : TestClassBase
	{
		public h_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_i_a_TestClass : TestClassBase
	{
		public i_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_i_a_TestClass : TestClassBase
	{
		public j_i_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};





	[Serializable()]
	public class a_j_a_TestClass : TestClassBase
	{
		public a_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class b_j_a_TestClass : TestClassBase
	{
		public b_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class c_j_a_TestClass : TestClassBase
	{
		public c_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class d_j_a_TestClass : TestClassBase
	{
		public d_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class e_j_a_TestClass : TestClassBase
	{
		public e_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class f_j_a_TestClass : TestClassBase
	{
		public f_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class g_j_a_TestClass : TestClassBase
	{
		public g_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class h_j_a_TestClass : TestClassBase
	{
		public h_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class i_j_a_TestClass : TestClassBase
	{
		public i_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};



	[Serializable()]
	public class j_j_a_TestClass : TestClassBase
	{
		public j_j_a_TestClass()
		{
			for (int i = 0; i < 100; i++)
				integers[i] = i;

			for (int i = 0; i < 1000; i++)
				intvect[i] = (i * 3134);

			for (int i = 0; i < 1000; i++)
				stringlist.Add("lotsastrings");
		}

		public int[] intvect = new int[1000];
		public int[] integers = new int[100];
		public List<string> stringlist = new List<string>();
	};






[Serializable()]
public class TestClass
{
	public TestClass()
	{
		for (int i = 0; i < 100; i++)
			integers[i] = i;

		for (int i = 0; i < 1000; i++)
			intvect[i] = (i * 3134);

		for (int i = 0; i < 1000; i++)
			stringlist.Add("lotsastrings");
	}

	public int[] intvect = new int[1000];
	public int[] integers = new int[100]; 
	public List<string> stringlist = new List<string>();	
};

[Serializable()]
public class Tester
{
	public Tester()
	{
		int i = 0;


		tests[i++] = new a_a_a_TestClass();



		tests[i++] = new b_a_a_TestClass();



		tests[i++] = new c_a_a_TestClass();



		tests[i++] = new d_a_a_TestClass();



		tests[i++] = new e_a_a_TestClass();



		tests[i++] = new f_a_a_TestClass();



		tests[i++] = new g_a_a_TestClass();



		tests[i++] = new h_a_a_TestClass();



		tests[i++] = new i_a_a_TestClass();



		tests[i++] = new j_a_a_TestClass();





		tests[i++] = new a_b_a_TestClass();



		tests[i++] = new b_b_a_TestClass();



		tests[i++] = new c_b_a_TestClass();



		tests[i++] = new d_b_a_TestClass();



		tests[i++] = new e_b_a_TestClass();



		tests[i++] = new f_b_a_TestClass();



		tests[i++] = new g_b_a_TestClass();



		tests[i++] = new h_b_a_TestClass();



		tests[i++] = new i_b_a_TestClass();



		tests[i++] = new j_b_a_TestClass();





		tests[i++] = new a_c_a_TestClass();



		tests[i++] = new b_c_a_TestClass();



		tests[i++] = new c_c_a_TestClass();



		tests[i++] = new d_c_a_TestClass();



		tests[i++] = new e_c_a_TestClass();



		tests[i++] = new f_c_a_TestClass();



		tests[i++] = new g_c_a_TestClass();



		tests[i++] = new h_c_a_TestClass();



		tests[i++] = new i_c_a_TestClass();



		tests[i++] = new j_c_a_TestClass();





		tests[i++] = new a_d_a_TestClass();



		tests[i++] = new b_d_a_TestClass();



		tests[i++] = new c_d_a_TestClass();



		tests[i++] = new d_d_a_TestClass();



		tests[i++] = new e_d_a_TestClass();



		tests[i++] = new f_d_a_TestClass();



		tests[i++] = new g_d_a_TestClass();



		tests[i++] = new h_d_a_TestClass();



		tests[i++] = new i_d_a_TestClass();



		tests[i++] = new j_d_a_TestClass();





		tests[i++] = new a_e_a_TestClass();



		tests[i++] = new b_e_a_TestClass();



		tests[i++] = new c_e_a_TestClass();



		tests[i++] = new d_e_a_TestClass();



		tests[i++] = new e_e_a_TestClass();



		tests[i++] = new f_e_a_TestClass();



		tests[i++] = new g_e_a_TestClass();



		tests[i++] = new h_e_a_TestClass();



		tests[i++] = new i_e_a_TestClass();



		tests[i++] = new j_e_a_TestClass();





		tests[i++] = new a_f_a_TestClass();



		tests[i++] = new b_f_a_TestClass();



		tests[i++] = new c_f_a_TestClass();



		tests[i++] = new d_f_a_TestClass();



		tests[i++] = new e_f_a_TestClass();



		tests[i++] = new f_f_a_TestClass();



		tests[i++] = new g_f_a_TestClass();



		tests[i++] = new h_f_a_TestClass();



		tests[i++] = new i_f_a_TestClass();



		tests[i++] = new j_f_a_TestClass();





		tests[i++] = new a_g_a_TestClass();



		tests[i++] = new b_g_a_TestClass();



		tests[i++] = new c_g_a_TestClass();



		tests[i++] = new d_g_a_TestClass();



		tests[i++] = new e_g_a_TestClass();



		tests[i++] = new f_g_a_TestClass();



		tests[i++] = new g_g_a_TestClass();



		tests[i++] = new h_g_a_TestClass();



		tests[i++] = new i_g_a_TestClass();



		tests[i++] = new j_g_a_TestClass();





		tests[i++] = new a_h_a_TestClass();



		tests[i++] = new b_h_a_TestClass();



		tests[i++] = new c_h_a_TestClass();



		tests[i++] = new d_h_a_TestClass();



		tests[i++] = new e_h_a_TestClass();



		tests[i++] = new f_h_a_TestClass();



		tests[i++] = new g_h_a_TestClass();



		tests[i++] = new h_h_a_TestClass();



		tests[i++] = new i_h_a_TestClass();



		tests[i++] = new j_h_a_TestClass();





		tests[i++] = new a_i_a_TestClass();



		tests[i++] = new b_i_a_TestClass();



		tests[i++] = new c_i_a_TestClass();



		tests[i++] = new d_i_a_TestClass();



		tests[i++] = new e_i_a_TestClass();



		tests[i++] = new f_i_a_TestClass();



		tests[i++] = new g_i_a_TestClass();



		tests[i++] = new h_i_a_TestClass();



		tests[i++] = new i_i_a_TestClass();



		tests[i++] = new j_i_a_TestClass();





		tests[i++] = new a_j_a_TestClass();



		tests[i++] = new b_j_a_TestClass();



		tests[i++] = new c_j_a_TestClass();



		tests[i++] = new d_j_a_TestClass();



		tests[i++] = new e_j_a_TestClass();



		tests[i++] = new f_j_a_TestClass();



		tests[i++] = new g_j_a_TestClass();



		tests[i++] = new h_j_a_TestClass();



		tests[i++] = new i_j_a_TestClass();



		tests[i++] = new j_j_a_TestClass();	
	
	}

	public TestClassBase[] tests = new TestClassBase[100];
};


namespace Csharp
{
    public class TestObject
    {
		Tester t = new Tester();
        Tester[] Testers = new Tester[10];
		MemoryStream stream = new MemoryStream();
		Type[] Types = new Type[100];

		// Put initializtion code here.
        public TestObject()
        {
            for (int i = 0; i < 10; i++)
                Testers[i] = new Tester();

			int ii = 0;
			foreach (object o in Testers[0].tests)
			{
				Types[ii] = o.GetType();
				ii++;
			}
		}

        // Implement benchmark here.
        public void Test()
		{
			//too slow, post multiply result by 10 :/
			//for (int i = 0; i < 10; i++)
			{
				XmlSerializer xml = new XmlSerializer(typeof(Tester),Types);
				StreamWriter writer = new StreamWriter(stream);

				//for (int j = 0; j < 10; j++)
					xml.Serialize(writer, Testers[0]);
			}
        }

		public void TestLoad()
		{
			stream.Seek(0, SeekOrigin.Begin); 
			
			//too slow, post multiply result by 10 :/
			//for (int i = 0; i < 10; i++)
			{
				XmlSerializer xml = new XmlSerializer(typeof(Tester), Types);
				XmlReader reader = new XmlTextReader(stream);

				//for (int j = 0; j < 10; j++)
				Testers[0] = xml.Deserialize(reader) as Tester;
			}
		}
	};
}