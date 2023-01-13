#ifndef RSE_h
#define RSE_h
typedef struct gcdstruct{
    int d;
    int x;
    int y;
} gcd_s;

void get_key(int *key);     //Get key pair
void rsa_Decode(unsigned char *input, unsigned char *output, int key, int n);       //RSA decryption
int sushu(int p);       //Prime algorithm
int get_GCD(int a,int b);       //Get the maximum common factor
int mod_inverse(int a, int m);      //Modular inverse operation
gcd_s ext_Euclid(int a,int b);      //extended euclidean algorithm
void rsa_Encode(unsigned char *input, unsigned char *output, int key, int n);       //RSA decryption
int mod_mul(int text, int key, int n);      //Modular multiplication
#endif
