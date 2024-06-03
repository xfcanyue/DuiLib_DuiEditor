/******************************************************************************
 *  Copyright (C) 2000 by Robert Hubley.                                      *
 *  All rights reserved.                                                      *
 *                                                                            *
 *  This software is provided ``AS IS'' and any express or implied            *
 *  warranties, including, but not limited to, the implied warranties of      *
 *  merchantability and fitness for a particular purpose, are disclaimed.     *
 *  In no event shall the authors be liable for any direct, indirect,         *
 *  incidental, special, exemplary, or consequential damages (including, but  *
 *  not limited to, procurement of substitute goods or services; loss of use, *
 *  data, or profits; or business interruption) however caused and on any     *
 *  theory of liability, whether in contract, strict liability, or tort       *
 *  (including negligence or otherwise) arising in any way out of the use of  *
 *  this software, even if advised of the possibility of such damage.         *
 *                                                                            *
 ******************************************************************************

 MD5.H - header file for MD5C.C
 
   Port to Win32 DLL by Robert Hubley 1/5/2000

   Original Copyright:

		Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
		rights reserved.

		License to copy and use this software is granted provided that it
		is identified as the "RSA Data Security, Inc. MD5 Message-Digest
		Algorithm" in all material mentioning or referencing this software
		or this function.

		License is also granted to make and use derivative works provided
		that such works are identified as "derived from the RSA Data
		Security, Inc. MD5 Message-Digest Algorithm" in all material
		mentioning or referencing the derived work.
		
		RSA Data Security, Inc. makes no representations concerning either
		the merchantability of this software or the suitability of this
		software for any particular purpose. It is provided "as is"
		without express or implied warranty of any kind.

		These notices must be retained in any copies of any part of this
		documentation and/or software.
*/
/******************************************************************************
 *  Copyright (C) 2000 by Robert Hubley.                                      *
 *  All rights reserved.                                                      *
 *                                                                            *
 *  This software is provided ``AS IS'' and any express or implied            *
 *  warranties, including, but not limited to, the implied warranties of      *
 *  merchantability and fitness for a particular purpose, are disclaimed.     *
 *  In no event shall the authors be liable for any direct, indirect,         *
 *  incidental, special, exemplary, or consequential damages (including, but  *
 *  not limited to, procurement of substitute goods or services; loss of use, *
 *  data, or profits; or business interruption) however caused and on any     *
 *  theory of liability, whether in contract, strict liability, or tort       *
 *  (including negligence or otherwise) arising in any way out of the use of  *
 *  this software, even if advised of the possibility of such damage.         *
 *                                                                            *
 ******************************************************************************
*/

/******************************************************************************
 *   2002-4-18 Modified by Liguangyi.									      *
 *   struct MD5_CTX ==> class MD5_CTX.								          *
 *   Take off the Globals Functions                                           *
 ******************************************************************************
*/

#ifndef _DUI_MD5_H
#define _DUI_MD5_H


/* MD5 Class. */
namespace DuiLib
{

class UILIB_API CDuiMd5 
{
public:
	CDuiString MakeMd5(LPCTSTR strSource);
	CDuiMd5();
	~CDuiMd5();
	void MD5Update ( unsigned char *input, unsigned int inputLen);
	void MD5Final (unsigned char digest[16]);

private:
	unsigned long int state[4];					/* state (ABCD) */
	unsigned long int count[2];					/* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];       /* input buffer */
	unsigned char PADDING[64];		/* What? */

private:
	void MD5Init ();
	void MD5Transform (unsigned long int state[4], unsigned char block[64]);
	void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, unsigned long int *input,unsigned int len);
	void Decode (unsigned long int *output, unsigned char *input, unsigned int len);
	void MD5_memset (unsigned char* output,int value,unsigned int len);
};

} //namespace DuiLib

#endif