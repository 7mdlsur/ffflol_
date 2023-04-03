#pragma once
#include <windows.h>

#define nombre "LoL"

#include <stdio.h>
#include <string.h>



/*prototipos*/

#define ROL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define A5_STEP(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

void a5_1_decrypt(unsigned char* key, int key_len, unsigned char* cipher, int cipher_len, unsigned char* out);

void a5_1_decrypt(unsigned char*, int , unsigned char* , int, unsigned char* );



#define nombre "LoL"




