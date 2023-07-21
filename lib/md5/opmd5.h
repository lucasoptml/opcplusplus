///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: opmd5.h
/// Date: 09/17/2007
///
/// Description:
///
/// Wrapper for md5 stuff.
///****************************************************************

#ifndef _OPMD5_
#define _OPMD5_

#include "md5.h"

// Converts a string key to an md5 hash string.
inline string md5Encode(const string& key)
{
	unsigned char uc_hash[16];
	string        hash;
	char          temp[3];

	md5((unsigned char*) key.c_str(), (int)key.size(), &uc_hash[0]);

	for (int i = 0; i < 16; i++)
	{
		sprintf(&temp[0], "%x", uc_hash[i]);

		if (uc_hash[i] < 16)
		{
			hash += '0';
			hash += temp[0];
		}
		else
		{
			hash += temp[0];
			hash += temp[1];
		}
	}

	return hash;
}

#endif