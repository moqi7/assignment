#include <stdio.h>
#include "RSE.h"
#include <string.h>

#define SAFE_K 5



int get_GCD(int a,int b) {      //Get the maximum common factor
    int i = 1;
    while(i != 0) {
        i = a % b;
        a = b;
        b = i;
    }
    return a;
}


int sushu(int p) {      //Fermat primality test algorithm
    int m,gcd,r = 1;
    if(p % 2 == 0) return 0;
    for(int i = 0; i < SAFE_K; i++) {
        while(1) {
            m = rand() % p;
            if(m >= 2 && m <= (p - 1)) break;
        }
       
        if((gcd = get_GCD(m, p)) != 1) return 0;
       
        for(int j = 0; j < p - 1; j++) {
            r = m % p * r % p;
        }
        
        if(r != 1) return 0;
    }
    
    return 1;
}

gcd_s ext_Euclid(int a,int b) {     //Extended Euclid
    gcd_s aa,bb;
    if(b == 0) {
        aa.d = a;
        aa.x = 1;
        aa.y = 0;
        return aa;
    }
    else {
        bb = ext_Euclid(b,a % b);
        aa.d = bb.d;
        aa.x = bb.y;
        aa.y = bb.x - bb.y * (a / b);
    }
    
    return aa;
}

int mod_inverse(int a, int m) {    //Modular inverse operation
    gcd_s aa;
    aa = ext_Euclid(a, m);
    return aa.x;
}

void get_key(int *key) {        //Get Key
    int fn;
    key[0] = rand() % 128 + 1;
    key[1] = rand() % 128 + 1;
    if(sushu(key[0]) != 1 ||sushu(key[1]) != 1) get_key(key);
    key[2] = (key[0]) * (key[1]);
    fn = (key[0] - 1) *  (key[1] - 1);
    while(1) {
        key[3] = rand() % 128 + 1;
        if(get_GCD(key[3], fn) == 1 && (key[4] = mod_inverse(key[3], fn)) > 0) break;
    }
}

int mod_mul(int text, int key, int n) {
    int result = 1;
    for(int i = 0; i < key; i++) {
        result  = ((result % n) * (text % n)) % n;
    }
    return result;
}

void rsa_Encode(unsigned char *input, unsigned char *output, int key, int n) {
    int m,len;
    unsigned char result;
    len = strlen(input);
    for(int i = 0; i < len; i++) {
        m = input[i];
        m = mod_mul(m, key, n);
        output[strlen(output)] = m;
    }
}

void rsa_Decode(unsigned char *input, unsigned char *output, int key, int n) {
    int m,len;
    unsigned char result;
    len = strlen(input);
    for(int i = 0; i < len; i++) {
        m = input[i];
        m = mod_mul(m, key, n);
        output[strlen(output)] = m;
    }
}
