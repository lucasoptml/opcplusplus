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

namespace Csharp
{
    public class baseclass
    {
	    public virtual void blah()
	    {

        }
    };

    //1
    public class A : baseclass
    {
    	
    };

    //2
    public class AA : A
    {

    };

    public class AB : A
    {

    };

    //4
    public class AAA : AA
    {

    };

    public class AAB : AA
    {

    };

    public class ABB : AB
    {

    };

    public class ABA : AB
    {

    };

    //8
    public class AAAA : AAA
    {

    };

    public class AABA : AAB
    {

    };

    public class ABBA : ABB
    {

    };

    public class ABAA : ABA
    {

    };

    public class AAAB : AAA
    {

    };

    public class AABB : AAB
    {

    };

    public class ABBB : ABB
    {

    };

    public class ABAB : ABA
    {

    };

    public class TestObject
    {
        // Put initializtion code here.
        public TestObject()
        {
            Classes = new A[]
                {new A(),
                 new AA(),
	             new AB(),
	             new AAA(),
	             new ABA(),
	             new AAB(),
	             new ABB(),
	             new AAAA(),
	             new ABAA(),
	             new AABA(),
	             new ABBA(),
	             new AAAB(),
	             new ABAB(),
	             new AABB(),
	             new ABBB()
				};
        }

        // Test as-style casts.
        public void AsStyleTest()
        {
            for (int i = 0; i < 15; i++)
            {
                AA aa = Classes[i] as AA;

                if (aa != null)
                    aa.blah();

                AB ab = Classes[i] as AB;

                if (ab != null)
                    ab.blah();
            }
        }

        // Test C-style casts.
        public void CStyleTest()
        {
            for (int i = 0; i < 15; i++)
            {   
                try
                {
                    AA aa = (AA) Classes[i];

                    aa.blah();
                }
                catch (Exception) {}

                try
                {
                    AB ab = (AB) Classes[i];

                    ab.blah();
                }
                catch (Exception) {}
            }
        }

        private A[] Classes = null;
    };
}