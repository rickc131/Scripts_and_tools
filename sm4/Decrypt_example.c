#include "sm4.c"

int main()
{
	while(1)
	{
	u8 Decode[16] ={0};
    	printf("Input ciphertext:");
    	for(int i=0;i<16;i++)
    		scanf("%x",&Decode[i]);
    	Dncrypt_sm4(Decode);
	}
	return 0;
}