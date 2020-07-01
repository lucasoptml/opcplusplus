using System;
using System.Collections.Generic;
using System.Text;

//NOTE: its unclear what List<> uses internally.
//		this is not exactly equivalent to boost/opC++ which use array + vector + list
namespace Csharp
{
    class Program
    {
        static void Main(string[] args)
        {
            TestObject testObject;
            testObject = new TestObject();

            try
            {
                {
                    Timer timer;
                    timer = new Timer();

                    timer.Start();

                    testObject.Test();

                    timer.Stop();

                    Console.WriteLine("Serialize Binary Save / C# : " + timer.Duration + "ms");
                }
                {
                    Timer timer;
                    timer = new Timer();

                    timer.Start();

                    testObject.TestLoad();

                    timer.Stop();

                    Console.WriteLine("Serialize Binary Load / C# : " + timer.Duration + "ms");
                }
                //Console.ReadLine();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
