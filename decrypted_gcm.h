#define BUFFER_SIZE 4096
#define BlockSize 32
#define DECRYPT_KEY "31323334353637383930313233343536"
#define DECRYPT_IV  "313233343536373839303132"

int decryptFile(const char *inputFile, const char *outputFile);
void from_hex(char *s, int l, char *d);
