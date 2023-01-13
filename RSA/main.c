
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "MD5.h"
#include "RSE.h"
 
int main()
{
    int nRet = -1;
    int nSize = 0;
    unsigned char MD5ID[100]={0,},de_out[100] = {0,},de_md5[100] = {0,},re_md5[100] = {0,},sig_de_out[100] = {0,};
    unsigned char pData[100] ="Hello",output[100] = {0,},sig_out[100] = {0,};
    int key[5];
    get_key(key); //n = key[2], e = key[3], d = key[4]
    nSize = strlen(pData);
    nRet = MD5EncryStr(MD5ID,pData,nSize);      //hash
    int len1 = strlen(MD5ID);
    rsa_Encode(MD5ID, sig_out, key[3], key[2]);      //signature
    rsa_Encode(pData, output, key[3], key[2]);      //Plaintext encryption
    strcat(output, sig_out);        //Signature sent after text
    strcpy(sig_de_out, output + strlen(output) - 16);        //The hash is 128bit, 16 bytes
    output[strlen(output) - 16] = 0;
    rsa_Decode(output, de_out, key[4], key[2]);     //Plaintext decryption
    rsa_Decode(sig_de_out,de_md5,key[4],key[2]);       //The hash value obtained from previous content
    MD5EncryStr(re_md5, de_out, nSize);     //Decrypted plaintext hash
    printf("Decrypted ciphertext：");
    for(int i = 0; i < nSize; i++) {
        printf("%c",de_out[i]);
    }
    printf("\n");
    printf("Hash after decryption：");
    for(int i = 0; i < strlen(de_md5); i++) {
        printf("%x",de_md5[i]);
    }
    printf("\n");
    printf("Hash after getting clear text：");
    for(int i = 0; i < strlen(re_md5); i++) {
        printf("%x",re_md5[i]);
    }
    printf("\n");
    return 0;
}

