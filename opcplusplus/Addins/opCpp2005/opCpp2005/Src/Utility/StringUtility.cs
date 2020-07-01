///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: StringUtility.cs
/// Date: 09/22/2007
///
/// Description:
///
/// String utility stuff.
///****************************************************************

namespace opGamesLLC
{
	class StringUtility
	{
        // Returns everything left of the substring 'leftof'.
		public static string RLeft(string s, string leftof)
		{
			int i = s.LastIndexOf(leftof);

			if (i == -1)
				return "";
			return s.Substring(0, i);
		}

		// Returns everything right of the substring 'leftof'.
		public static string RRight(string s, string rightof)
		{
			int i = s.LastIndexOf(rightof);

			if (i == -1)
				return "";
			return s.Substring(i + 1, s.Length - i - 1);
		}

		public static string Right(string s, int value)
		{
			return s.Substring(value + 1, s.Length - value - 1);
		}

        public static string Rep(string rep, int numtimes)
        {
            string s = "";

            for (int i = 0; i < numtimes; i++)
                s += rep;

            return s;
        }
    };
}