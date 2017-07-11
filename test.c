#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>

#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/un.h>

// #include <libcork/core.h>
#include <udns.h>

#include "crypto.h"


#if defined(HAVE_SYS_IOCTL_H) && defined(HAVE_NET_IF_H) && defined(__linux__)
#include <net/if.h>
#include <sys/ioctl.h>
#define SET_INTERFACE
#endif


#ifndef EAGAIN
#define EAGAIN EWOULDBLOCK
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

#ifndef BUF_SIZE
#define BUF_SIZE 2048
#endif

#ifndef SSMAXCONN
#define SSMAXCONN 1024
#endif

#ifndef MAX_FRAG
#define MAX_FRAG 1
#endif

#ifdef USE_NFCONNTRACK_TOS

#ifndef MARK_MAX_PACKET
#define MARK_MAX_PACKET 10
#endif

#ifndef MARK_MASK_PREFIX
#define MARK_MASK_PREFIX 0xDC00
#endif

#endif

typedef struct mytest_s{
    cipher_ctx_t *e_ctx;
    cipher_ctx_t *d_ctx;
    buffer_t *buf;
    buffer_t *buf_encrypt;
    buffer_t *buf_decrypt;

	
}mytest_t;

int main()
{
	int ret = 0;
    static crypto_t *crypto;
    unsigned char password[] ="password";
    unsigned char key[] ="dLrUwG-pa1uj7QmTEpLcMg==";
    unsigned char method[] ="aes-128-gcm";

	mytest_t * p_mytest;

	p_mytest= calloc(1, sizeof(mytest_t));
	if( NULL == p_mytest ){
		return -1;
	}
    p_mytest->buf = ss_malloc(sizeof(buffer_t));
    p_mytest->buf_encrypt = ss_malloc(sizeof(buffer_t));
    p_mytest->buf_decrypt = ss_malloc(sizeof(buffer_t));
    balloc(p_mytest->buf, BUF_SIZE);
    balloc(p_mytest->buf_encrypt, BUF_SIZE);
    balloc(p_mytest->buf_decrypt, BUF_SIZE);

	
	snprintf(p_mytest->buf->data,BUF_SIZE,"Hello,World.");
	p_mytest->buf->len = strlen("Hello,World.") +1;

    crypto = crypto_init(password, key, method);
    if (crypto == NULL){
        FATAL("failed to initialize ciphers");
    }

    p_mytest->e_ctx = ss_align(sizeof(cipher_ctx_t));
    p_mytest->d_ctx = ss_align(sizeof(cipher_ctx_t));
    crypto->ctx_init(crypto->cipher, p_mytest->e_ctx, 1);
    crypto->ctx_init(crypto->cipher, p_mytest->d_ctx, 0);

	ret = crypto->encrypt(p_mytest->buf, p_mytest->e_ctx, BUF_SIZE);
    printf("%s(%d) ret=%d\n", __FUNCTION__, __LINE__, ret);
    printf("%s(%d) p_mytest->buf->data=%s\n", __FUNCTION__, __LINE__, p_mytest->buf->data);

	ret = crypto->decrypt(p_mytest->buf, p_mytest->d_ctx, BUF_SIZE);
    printf("%s(%d) ret=%d\n", __FUNCTION__, __LINE__, ret);
    printf("%s(%d) p_mytest->buf->data=%s\n", __FUNCTION__, __LINE__, p_mytest->buf->data);

	




    printf("%s(%d) hello, world.\n", __FUNCTION__, __LINE__);

    return 0;
}
