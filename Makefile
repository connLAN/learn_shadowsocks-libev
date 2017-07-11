CC=gcc
CFLAGS=-I.
CFLAGS+= -lmbedcrypto -lsodium -lm 


VERSION = 3.0.7



test:   aead.o base64.o bloom.o crypto.o murmurhash2.o  ppbloom.o stream.o test.o utils.o
	$(CC) $(CFLAGS)  aead.o base64.o bloom.o crypto.o  murmurhash2.o  ppbloom.o stream.o test.o utils.o  -o test -I.
