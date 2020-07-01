///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Blacklist.cs
/// Date: 09/28/2007
///
/// Description:
///
/// Put blacklisted serials in here.
///****************************************************************

using System.Collections.Generic;

namespace opGamesLLC.opCpp2005
{
    public partial class opLicenseUtility
    {
        // This method populates the blacklist.
        private static void PopulateBlacklist()
        {
            blacklist = new Dictionary<string,List<string>>();

            // "Kevin Depue", "1", "6514b252542f74e943c83480f22c237a"
            //List<string> keys = new List<string>();

            //keys.Add("KevinDepue1" + ExtraHashString);

            //blacklist.Add("6514b252542f74e943c83480f22c237a", keys);
        }
    };
}