///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: Encryption.cs
/// Date: 09/27/2007
///
/// Description:
///
/// Contains encryption methods.
///****************************************************************

using System.Security.Cryptography;
using System.Text;

namespace opGamesLLC.opCpp2005
{
    ///==========================================
    /// EncryptionUtility
    ///==========================================
    
    // Contains some utility methods for encryption.
    public class EncryptionUtility
    {
                // This method returns the md5 hash string, given a key.
        public static string md5(string key)
        {
            MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider();
            byte[]                   bs  = Encoding.UTF8.GetBytes(key);

            bs = md5.ComputeHash(bs);

            StringBuilder s = new StringBuilder();

            foreach (byte b in bs)
                s.Append(b.ToString("x2").ToLower());

            return s.ToString();
        }
    };
}