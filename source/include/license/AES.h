#ifndef _AES_H
#define _AES_H

#ifdef WIN32
#ifdef LICENECELIB_DLL
# define LICENSELIB_EXPORT __declspec(dllexport) 
#else
# define LICENSELIB_EXPORT __declspec(dllimport) 
#endif
#else
#define	LICENSELIB_EXPORT
#endif

class LICENSELIB_EXPORT AES
{
public:
	AES(unsigned char* key);
	~AES();

	unsigned char* Cipher(unsigned char* input);
	unsigned char* InvCipher(unsigned char* input);
	void* Cipher(void* input, int length=0);
	void* InvCipher(void* input, int length);

private:
	unsigned char Sbox[256];
	unsigned char InvSbox[256];
	unsigned char w[11][4][4];

	void KeyExpansion(unsigned char* key, unsigned char w[][4][4]);
	unsigned char FFmul(unsigned char a, unsigned char b);

	void SubBytes(unsigned char state[][4]);
	void ShiftRows(unsigned char state[][4]);
	void MixColumns(unsigned char state[][4]);
	void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

	void InvSubBytes(unsigned char state[][4]);
	void InvShiftRows(unsigned char state[][4]);
	void InvMixColumns(unsigned char state[][4]);
};

#endif
