// https://github.com/lawinse/AESnRC4nRSA/blob/master/

#include "rc4.h"

void RC4::set_for_cipher(int kl=8,uint8_t *_key=NULL) {
	keylen = kl;
	if (_key == NULL) {
		srand(time(0));
		K.push_back(rand()%MAX_VALUE);
	} else {
		for (int i=0; i<kl; ++i) K.push_back(_key[i]);
	}
}

void RC4::cipher(uint8_t *in, size_t len, uint8_t *out, uint8_t *ks) {
	int i,j,t;
	for(i=0;i<256;++i){
		S[i]=i;
		T[i]=K[i%keylen];
	}

	j=0;
	for (i=0; i<256; ++i) {
		j=(j+S[i]+T[i])%256;
		std::swap(S[i],S[j]);
	}

	for (size_t k=0; k<len; ++k) {
		i=(i+1)%256;
		j=(j+S[i])%256;
		std::swap(S[i],S[j]);
		t=(S[i]+S[j])%256;
		ks[k] = S[t]; 
		out[k] = ks[k]^in[k];
	}
}

void RC4::decipher(uint8_t *in, size_t len, uint8_t *out, uint8_t *ks) {
	for (size_t k=0; k<len; ++k) {
		out[k] = ks[k]^in[k];
	}
}