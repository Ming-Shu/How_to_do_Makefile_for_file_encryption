OBJ = decrypted_gcm.c

all: target clean

target: decrypted_gcm.o
	gcc -o decrypted_gcm decrypted_gcm.o -lcrypto
decrypted_gcm.o:$(OBJ) encrypted
	gcc -c $(OBJ)
.PHONY: encrypted
encrypted:
	openssl aes-128-gcm -nosalt -e -p -K "31323334353637383930313233343536" -iv "313233343536373839303132" -in input.txt  -out input.encrypt
	openssl aes-128-gcm -nosalt -e -p -K "31323334353637383930313233343536" -iv "313233343536373839303132" -in input2.txt  -out input2.encrypt
	openssl aes-128-gcm -nosalt -e -p -K "31323334353637383930313233343536" -iv "313233343536373839303132" -in input3.txt  -out input3.encrypt
.PHONY: clean
clean:
	rm -f decrypted_gcm.o
