#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include "decrypted_gcm.h"
void from_hex(char *s, int l, char *d)/*Converting a hexadecimal HEX string into an array of numerical values. ex: char hex[]="112233AABB" --> char hex[]={0x11,0x22,0xAA,0xBB}*/
{
    int i;
    for (i = l - 1; i >= 0; i--)
    {
        char* p = s+i;
        char* p2 = p-1;
        *(d+i/2) = ( (*p>'9'? *p+9 : *p) & 0x0f ) | ( (*p2>'9'? *p2+9 : *p2) << 4 );
        i--;
    }

}

int decryptFile(const char *inputFile, const char *outputFile) {
    unsigned char key[EVP_MAX_KEY_LENGTH]={0}, iv[EVP_MAX_IV_LENGTH]={0};
	char gcm_key[]= DECRYPT_KEY;
	char gcm_iv[]= DECRYPT_IV;	
    int bytesRead, bytesWritten, finalBytesWritten;    

    unsigned char inBuffer[BUFFER_SIZE]={0};
    unsigned char outBuffer[BUFFER_SIZE+BlockSize]={0};
    
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");
		
    if (inFile == NULL || outFile == NULL) {
        printf("Failed to open file %s\n",inputFile);
        return 1;
    }

 	from_hex(gcm_key,strlen(gcm_key),key);
	from_hex(gcm_iv,strlen(gcm_iv),iv);		

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN,strlen(iv), NULL);
    EVP_DecryptInit_ex(ctx,NULL, NULL,key,iv);

    while ((bytesRead = fread(inBuffer, sizeof(unsigned char), BUFFER_SIZE , inFile)) > 0) {
        EVP_DecryptUpdate(ctx, outBuffer, &bytesWritten, inBuffer, bytesRead);
        fwrite(outBuffer,sizeof(unsigned char), bytesWritten, outFile);
    }

    EVP_DecryptFinal_ex(ctx, outBuffer, &finalBytesWritten);
    fwrite(outBuffer,sizeof(unsigned char), finalBytesWritten, outFile);

    EVP_CIPHER_CTX_free(ctx);
	EVP_cleanup();
    fclose(inFile);
    fclose(outFile);

    return 0;
}

int main(int argc, char *argv[], void *p)
{
	char *country[]={"input","input2","input3"};
	char inputFile[256]={0}, outputFile[256]={0};
	int countryNum;

	for(countryNum=0;countryNum<(sizeof(country)/sizeof(country[0]));countryNum++)
	{
		snprintf(inputFile,sizeof(inputFile),"%s.encrypt",country[countryNum]);
		snprintf(outputFile,sizeof(outputFile),"decrypted_%s.txt",country[countryNum]);
		if(decryptFile(inputFile,outputFile))
		{
			memset(outputFile, 0, sizeof(outputFile));
			snprintf(outputFile,sizeof(outputFile),"rm -rf decrypted_%s.txt",country[countryNum]);
			system(outputFile);
		}		
	}
	system("rm -rf *.encrypt");

	return 0;
}
