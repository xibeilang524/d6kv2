#ifndef _SYMMETRICAL_H_
#define _SYMMETRICAL_H_

#ifdef __cplusplus
extern "C"
{
#endif
void encrypt_epri(unsigned char* in, unsigned int in_len, unsigned char* out, unsigned int* out_len, unsigned int key, int type);
#ifdef __cplusplus
}
#endif

#endif