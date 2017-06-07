/********************************************************************
file:
	crypto_engine.h
purpose:
    provide all the function entries that a crypto engine should provide

interface:

History
    Date        Who         RefDoc     Memo
	2007-12-27  NL                     CREATE
*******************************************************/

#ifndef CRYPTO_ENGINE_H
#define CRYPTO_ENGINE_H

#ifndef dm_h

typedef int   lint;

typedef short sint;

typedef signed char tint;

typedef unsigned int ulint;   

typedef unsigned short usint;

typedef unsigned char byte;

#ifdef _WIN64
#define dm_int3264 __int64
#define dm_uint3264 unsigned __int64
#else
#define dm_int3264 long
#define dm_uint3264 unsigned long
#endif

#endif //#ifndef dm_h

#ifdef WIN32
#define DllImport __declspec( dllimport )
#define DllExport __declspec( dllexport )
#else
#define DllImport 
#define DllExport 
#endif

#define  MIN_EXTERNAL_CIPHER_ID         5000
#define  CYT_TYPE_UNKNOWN		        0			//类型, 未知算法
#define	 CYT_TYPE_SYM_BLOCK_ENCRYPT	    1			//类型, 分组对称加密算法
#define  CYT_TYPE_SYM_STREAM_ENCRYPT	2			//类型, 流式对称加密算法
#define  CYT_TYPE_ASYM_ENCRYPT		    3			//类型, 非对称加密算法, 保留
#define  CYT_TYPE_HASH				    4			//类型, 散列算法

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define dm_bool long
#else
#define dm_bool unsigned int
#endif

#define DM_FALSE	0
#define DM_TRUE		1

DllExport	
ulint
cipher_get_count(
);

DllExport
dm_bool
cipher_get_info(
    ulint	seqno,
    ulint*	cipher_id,
    byte**	cipher_name,
    byte*	type,
    ulint*	blk_size,
    ulint*	kh_size 
);

DllExport
dm_bool
cipher_encrypt_init(
	ulint	inner_id,
	byte*	key,
	ulint	key_size,
	void**	encrypt_para
);

DllExport
lint
cipher_get_cipher_text_size(
	ulint	inner_id,
	void*	cipher_para,
	lint	plain_text_size
);

DllExport
lint
cipher_encrypt(
	ulint	inner_id,
	void*	encrypt_para,
	byte*	plain_text,
	ulint	plain_text_size,
	byte*	cipher_text,
	ulint	cipher_text_buf_size
);

DllExport
dm_bool
cipher_decrypt_init(
	ulint	inner_id,
	byte*	key,
	ulint	key_size,
	void**	decrypt_para
);

DllExport
lint
cipher_decrypt(
   ulint	inner_id,
   void*	decrypt_para,
   byte*	cipher_text,
   ulint	cipher_text_size,
   byte*	plain_text,
   ulint	plain_text_buf_size
);

DllExport
dm_bool
cipher_hash_init(
	ulint	inner_id,	
	void**	hash_para
);

DllExport
void
cipher_hash_update(
	ulint	inner_id,
	void*	hash_para,
	byte*	msg,
	ulint	msg_size
);

DllExport
lint
cipher_hash_final(
	ulint	inner_id,
	void*	hash_para,
	byte*	digest,
	ulint	digest_buf_size
);

DllExport
void
cipher_cleanup(
	ulint	inner_id,
	void*	hash_para
);

#ifdef __cplusplus
}
#endif

#endif
