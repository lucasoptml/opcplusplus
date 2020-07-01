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

                timer.Start();

                for (int i = 0; i < 1000000; i++)
                    testObject.Test();

                timer.Stop();

                Console.WriteLine(timer.Duration + "ms");
                Console.ReadLine();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
