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

                Console.WriteLine("Reflection / C# : " + timer.Duration + "ms");

				TestObjectCacheType testObjectCache = new TestObjectCacheType();
               
				timer.Start();
				
                for (int i = 0; i < 1000000; i++)
					testObjectCache.Test();

                timer.Stop();

                Console.WriteLine("Reflection / C# Cached Fields : " + timer.Duration + "ms");

				timer.Start();

				TestObjectAccessors testObjectAccessors = new TestObjectAccessors();

				for (int i = 0; i < 1000000; i++)
					testObjectAccessors.Test();

				timer.Stop();

				Console.WriteLine("Reflection / C# Reflection Emit Cache : " + timer.Duration + "ms");

            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
