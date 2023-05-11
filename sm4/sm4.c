#include "sm4.h"

void four_uCh2uLong(u8 *in, u32 *out)
{
	int i = 0;
	*out = 0;
	for (i = 0; i < 4; i++)
		*out = ((u32)in[i] << (24 - i * 8)) ^ *out;
}

void uLong2four_uCh(u32 in, u8 *out)
{ 
	int i = 0;
	for (i = 0; i < 4; i++)
		*(out + i) = (u32)(in >> (24 - i * 8));
}

u32 move(u32 data, int length)
{
	u32 result = 0;
	result = (data << length) ^ (data >> (32 - length));

	return result;
}

u32 func_key(u32 input)
{
	int i = 0;
	u32 ulTmp = 0;
	u8 ucIndexList[4] = { 0 };
	u8 ucSboxValueList[4] = { 0 };
	uLong2four_uCh(input, ucIndexList);
	for (i = 0; i < 4; i++)
	{
		ucSboxValueList[i] = TBL_SBOX[ucIndexList[i]];
	}
	four_uCh2uLong(ucSboxValueList, &ulTmp);
	ulTmp = ulTmp ^ move(ulTmp, 13) ^ move(ulTmp, 23);

	return ulTmp;
}

u32 func_data(u32 input)
{
	int i = 0;
	u32 ulTmp = 0;
	u8 ucIndexList[4] = { 0 };
	u8 ucSboxValueList[4] = { 0 };
	uLong2four_uCh(input, ucIndexList);
	for (i = 0; i < 4; i++)
	{
		ucSboxValueList[i] = TBL_SBOX[ucIndexList[i]];
	}
	four_uCh2uLong(ucSboxValueList, &ulTmp);
	ulTmp = ulTmp ^ move(ulTmp, 2) ^ move(ulTmp, 10) ^ move(ulTmp, 18) ^ move(ulTmp, 24);

	return ulTmp;
}

void encode_fun(u8 len,u8 *key, u8 *input, u8 *output)
{
	int i = 0,j=0; 
	u8 *p = (u8 *)malloc(50);      
	u32 ulKeyTmpList[4] = { 0 };   
	u32 ulKeyList[36] = { 0 };     
	u32 ulDataList[36] = { 0 };    

	four_uCh2uLong(key, &(ulKeyTmpList[0]));
	four_uCh2uLong(key + 4, &(ulKeyTmpList[1]));
	four_uCh2uLong(key + 8, &(ulKeyTmpList[2]));
	four_uCh2uLong(key + 12, &(ulKeyTmpList[3]));

	ulKeyList[0] = ulKeyTmpList[0] ^ TBL_SYS_PARAMS[0];
	ulKeyList[1] = ulKeyTmpList[1] ^ TBL_SYS_PARAMS[1];
	ulKeyList[2] = ulKeyTmpList[2] ^ TBL_SYS_PARAMS[2];
	ulKeyList[3] = ulKeyTmpList[3] ^ TBL_SYS_PARAMS[3];

	for (i = 0; i < 32; i++)            
	{
		//5-36为32个子秘钥
		ulKeyList[i + 4] = ulKeyList[i] ^ func_key(ulKeyList[i + 1] ^ ulKeyList[i + 2] ^ ulKeyList[i + 3] ^ TBL_FIX_PARAMS[i]);
	}

	for (i = 0; i < len; i++)        
		*(p + i) = *(input + i);
	for (i = 0; i < 16-len % 16; i++)
		*(p + len + i) = 0;

	for (j = 0; j < len / 16 + ((len % 16) ? 1:0); j++)  
	{
		four_uCh2uLong(p + 16 * j, &(ulDataList[0]));
		four_uCh2uLong(p + 16 * j + 4, &(ulDataList[1]));
		four_uCh2uLong(p + 16 * j + 8, &(ulDataList[2]));
		four_uCh2uLong(p + 16 * j + 12, &(ulDataList[3]));
		
		for (i = 0; i < 32; i++)
		{
			ulDataList[i + 4] = ulDataList[i] ^ func_data(ulDataList[i + 1] ^ ulDataList[i + 2] ^ ulDataList[i + 3] ^ ulKeyList[i + 4]);
		}
		
		uLong2four_uCh(ulDataList[35], output + 16 * j);
		uLong2four_uCh(ulDataList[34], output + 16 * j + 4);
		uLong2four_uCh(ulDataList[33], output + 16 * j + 8);
		uLong2four_uCh(ulDataList[32], output + 16 * j + 12);
	}
	free(p);
}


void decode_fun(u8 len,u8 *key, u8 *input, u8 *output)
{
	int i = 0,j=0;
	u32 ulKeyTmpList[4] = { 0 };
	u32 ulKeyList[36] = { 0 };  
	u32 ulDataList[36] = { 0 };

	four_uCh2uLong(key, &(ulKeyTmpList[0]));
	four_uCh2uLong(key + 4, &(ulKeyTmpList[1]));
	four_uCh2uLong(key + 8, &(ulKeyTmpList[2]));
	four_uCh2uLong(key + 12, &(ulKeyTmpList[3]));

	ulKeyList[0] = ulKeyTmpList[0] ^ TBL_SYS_PARAMS[0];
	ulKeyList[1] = ulKeyTmpList[1] ^ TBL_SYS_PARAMS[1];
	ulKeyList[2] = ulKeyTmpList[2] ^ TBL_SYS_PARAMS[2];
	ulKeyList[3] = ulKeyTmpList[3] ^ TBL_SYS_PARAMS[3];

	for (i = 0; i < 32; i++)             
	{
		ulKeyList[i + 4] = ulKeyList[i] ^ func_key(ulKeyList[i + 1] ^ ulKeyList[i + 2] ^ ulKeyList[i + 3] ^ TBL_FIX_PARAMS[i]);
	}

	for (j = 0; j < len / 16; j++)  
	{
		four_uCh2uLong(input + 16 * j, &(ulDataList[0]));
		four_uCh2uLong(input + 16 * j + 4, &(ulDataList[1]));
		four_uCh2uLong(input + 16 * j + 8, &(ulDataList[2]));
		four_uCh2uLong(input + 16 * j + 12, &(ulDataList[3]));

		for (i = 0; i < 32; i++)
		{
			ulDataList[i + 4] = ulDataList[i] ^ func_data(ulDataList[i + 1] ^ ulDataList[i + 2] ^ ulDataList[i + 3] ^ ulKeyList[35 - i]);//与加密唯一不同的就是轮密钥的使用顺序
		}
		uLong2four_uCh(ulDataList[35], output + 16 * j);
		uLong2four_uCh(ulDataList[34], output + 16 * j + 4);
		uLong2four_uCh(ulDataList[33], output + 16 * j + 8);
		uLong2four_uCh(ulDataList[32], output + 16 * j + 12);
	}
}


void print_hex(u8 *data, int len)
{
	int i = 0;
	char alTmp[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
	for (i = 0; i < len; i++)
	{
		printf("%c", alTmp[data[i] / 16]);
		printf("%c", alTmp[data[i] % 16]);
        putchar(' ');
	}
	putchar('\n');
}

//加密应用函数
char *Encrypt_sm4(u8 Data_Before[],u8 Key_Before[])
{
    u8 i, len;
    u8 encode_Result[50] = {0}; 
    u8 key[16] = {0}; 
    u8 Data_plain[16] = {0};

    for (int i = 0; i < 16; i++)
        {
            key[i] = Key_Before[i];
        }
    for (int i = 0; i < 16; i++)
        {
            Data_plain[i] = Data_Before[i];
        }
    len = 16 * (sizeof(Data_plain) / 16) + 16 * ((sizeof(Data_plain) % 16) ? 1 : 0); 
	//注意：此处解密函数的输入数据长度应为扩展后的数据长度，即必为16的倍数
    encode_fun(sizeof(Data_plain), key, Data_plain, encode_Result); 

    printf("Ciphertext:\n");
    for (int i = 0; i < len; i++)
        printf("%x ", *(encode_Result + i));
    printf("\n");
    return 0;   
}
//解密应用函数
char *Dncrypt_sm4(u8 encode_Result[])
{
    u8 i, len;
    u8 decode_Result[50] = {0}; //定义解密输出缓存区
    u8 Key_Before[16]={0};  //输入数据
    u8 key[16] = {0};     //实际加密数据
    u8 Data_plain[16] = {0};

Key_input:
    printf("Input key:");
    scanf("%s", Key_Before);
    if (strlen(Key_Before) > 16) //限制大小
        goto Key_input;

    for (int i = 0; i < 16; i++)
    {
        key[i] = Key_Before[i];
        
    }
    for (int i = 0; i < 16; i++)
        {
            Data_plain[i] = encode_Result[i];
        }
    len = 16 * (sizeof(Data_plain) / 16) + 16 * ((sizeof(Data_plain) % 16) ? 1 : 0); //得到扩充后的字节数（解密函数会用到）

    //注意：此处解密函数的输入数据长度应为扩展后的数据长度，即必为16的倍数
    decode_fun(len, key, Data_plain, decode_Result); //数据解密
    int lenth=16;
    printf("Plaintext:");
    for (int j = 0; j < lenth; j++)
            printf("%c", *(decode_Result + j));
    
    printf("\n \n");   
      
}

//参考：https://blog.csdn.net/weixin_42700740/article/details/102667012
