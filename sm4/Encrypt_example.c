#include "sm4.c"



int main()
{
	u8 Key_y[16]={0};
	u8 eXm[16]={0};


Key_input:
    printf("Input key:");
    scanf("%s", Key_y);
    if (strlen(Key_y) > 16) //限定密钥长度
    	goto Key_input;
	
	while(1)
    {
    printf("\n");
eXm_input:
    printf("Input plaintext:");
    scanf("%s", eXm);
    if (strlen(eXm) > 16) 
    	goto eXm_input;
    Encrypt_sm4(eXm,Key_y);
    
    }
    return 0;
}


    