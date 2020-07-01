using System;
using System.Collections.Generic;
using System.Text;

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

					//couldn't get it to run enough so x100
                    Console.WriteLine("Serialize Xml Save / C# : " + timer.Duration*10 + "ms");
                }
                {
                    Timer timer;
                    timer = new Timer();

                    timer.Start();

                    testObject.TestLoad();

                    timer.Stop();

                    Console.WriteLine("Serialize Xml Load / C# : " + timer.Duration*10 + "ms");
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
