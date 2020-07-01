using System;
using System.Collections.Generic;
using System.Text;

namespace Csharp
{
    class Program
    {
        static void Main(string[] args)
        {
            Timer      timer;
            TestObject testObject;

            try
            {
                timer      = new Timer();
                testObject = new TestObject();

                /*=== as-style cast ===*/

                timer.Start();

                for (int i = 0; i < 1000000; i++)
                    testObject.AsStyleTest();

                timer.Stop();

				Console.WriteLine("ClassCast - as-style / C# : " + timer.Duration + "ms");

				//NOTE: about results:
				//		it seems to be constant time, but
				//		if you use less classes it may speed up (due to caching?)

                /*=== c-style cast ===*/

                timer.Start();

				//NOTE: I cut it down and multiply the result
                for (int i = 0; i < 100; i++)
                    testObject.CStyleTest();

                timer.Stop();

				Console.WriteLine("ClassCast - C-style / C# : " + timer.Duration * 1000.0 + "ms");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
