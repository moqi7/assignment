#include<stdint.h>
#include<math.h>
#include"miracl.h"
#include<stdlib.h>

void getp(big p);
void getx(big p, big x);
void getg(big p, big g);
void encrypt(big p, big g, big y, big m, big y1, big y2);
void decrypt(big x, big y1, big y2, big p, big plaintext);

int main() {
	FILE* fp;
	fp = fopen("secrect4.txt", "r+");
	if (fp == NULL) {
		printf("File open error");
		return 0;
	}

	miracl* mip = mirsys(5000, 10);
	mip->IOBASE = 10;
	big p,g,x,y,m,y1,y2,plaintext;
	g = mirvar(0);
	p = mirvar(0);
	x = mirvar(0);
	y = mirvar(0);
	m = mirvar(0);
	y1 = mirvar(0);
	y2 = mirvar(0);
	plaintext = mirvar(0);
	cinnum(m, fp);
	printf("m:");
	cotnum(m, stdout);
	printf("\n");
	fclose(fp);
	
	//获取公钥(p,g,y),私钥x
	getp(p);
	getg(p, g);
	getx(p, x);
	powmod(g, x, p, y);
	printf("g:");
	cotnum(g, stdout);
	printf("\n");
	printf("p:");
	cotnum(p, stdout);
	printf("\n");
	printf("y:");
	cotnum(y, stdout);
	printf("\n\n");

	//加密
	encrypt(p, g, y, m, y1, y2);
	printf("y1:");
	cotnum(y1, stdout);
	printf("\n");
	printf("y2:");
	cotnum(y2, stdout);
	printf("\n\n");

	//解密
	decrypt(x, y1, y2, p, plaintext);
	printf("plaintext:");
	cotnum(plaintext, stdout);
	printf("\n");
	
	mirkill(p);
	mirkill(g);
	mirkill(y);
	mirkill(m);
	mirkill(y1);
	mirkill(y2);
	mirkill(x);
	mirkill(plaintext);
	return 0;
}

void getp(big p) {
	big q;
	q = mirvar(0);
	bigdig(150, 10, p);		//获取p
	copy(p, q);
	decr(q, 1, q);
	divide(q, mirvar(2), q);		//获取q = （p-1）/2
	if((!isprime(p)) && (!isprime(q))) {		//判断p、q是否为素数
		getp(p);
	}
	mirkill(q);
}

void getg(big p,big g) {		//获得p的本原根
	big tmp;
	tmp = mirvar(0);

	while (compare(tmp,mirvar(1)) != 0) {		//g^(p-1) mod p = 1  不太确定这么算对不对,但最后结果是正确的
		copy(p, tmp);
		decr(tmp, 1, tmp);
		getp(g);
		powmod(g, tmp, p, tmp);
	}
	mirkill(tmp);
}

void getx(big p,big x) {   //产生大于1小于p的随机数x作为私钥
	while (compare(x, mirvar(1)) != 1) {
		bigrand(p, x);
	}
}

void encrypt(big p, big g, big y,big m,big y1,big y2) {
	big k,tmp,a,b;
	k = mirvar(0);
	a = mirvar(1);
	b = mirvar(0);
	tmp = mirvar(0);
	copy(p, tmp);
	decr(tmp, 2, tmp);
	while (compare(k, mirvar(1)) != 1 && tmp != 1) {		//产生1<k<p-2,且（k,p)=1
		bigrand(tmp, k);
		decr(p, mirvar(1), tmp);
		egcd(k, tmp,tmp);
	}
	powmod(y, k, p, b);		//b = y^k mod p
	multiply(m, b, a);
	powmod(g, k, p, y1);	//y1 = g^k mod p
	powmod(a, mirvar(1), p, y2);		//y2 = m*y^k mod p
	mirkill(k);
	mirkill(a);
	mirkill(b);
}

void decrypt(big x, big y1, big y2, big p, big plaintext) {
	big tmp1,tmp2;
	tmp1 = mirvar(0);
	tmp2 = mirvar(0);
	copy(y1, tmp1);
	xgcd(tmp1, p, tmp1, tmp1, tmp1);		//求y1模p的逆	
	powmod(tmp1, x, p, tmp1);		//y1^(-x) mod p
	multiply(tmp1, y2, tmp1);
	powmod(tmp1, mirvar(1), p, plaintext);		//plaintext = y2y1^(-x) mod p
	mirkill(tmp1);
	mirkill(tmp2);
}