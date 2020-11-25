/*
 * @Author: gongluck 
 * @Date: 2020-11-24 18:44:16 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-11-24 18:53:20
 */

// gcc test.c -lssl -lcrypto

#include <stdio.h>
#include <string.h>
#include <openssl/lhash.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>

#define NAME_LENGTH 32

typedef struct _Person
{
    char name[NAME_LENGTH];
    int high;
    char otherInfo[NAME_LENGTH];
} Person;

// 自定义比较函数
static int person_cmp(const void *a, const void *b)
{
    char *namea = ((Person *)a)->name;
    char *nameb = ((Person *)b)->name;
    return strcmp(namea, nameb);
}

void print_value(void *a)
{
    Person *p = (Person *)a;
    printf("name: %s\n", p->name);
    printf("high: %d\n", p->high);
    printf("other info : %s\n", p->otherInfo);
}

int main()
{
    // hash
    OPENSSL_LHASH *h = lh_new(NULL, person_cmp);
    if (h == NULL)
    {
        printf("err.\n");
        return -1;
    }

    Person p1 = {"gongluck", 170, "xxxx"};
    Person p2 = {"ben", 175, "xxxx"};
    Person p3 = {"ken", 170, "xxxx"};
    Person p4 = {"dio", 170, "xxxx"};

    lh_insert(h, &p1);
    lh_insert(h, &p2);
    lh_insert(h, &p3);
    lh_insert(h, &p4);

    lh_doall(h, print_value);

    void *data = lh_retrieve(h, (const char *)"dio"); //person_cmp return 0
    if (data == NULL)
    {
        return -1;
    }

    print_value(data);
    lh_free(h);

    printf("\n------------------------------\n");

    //bio
    BIO *b = BIO_new(BIO_s_mem());
    int len = BIO_write(b, "OpenSSL", 4);
    len = BIO_printf(b, "%s", "gongluck");
    printf("len: %d\n", len);
    char *out = OPENSSL_malloc(len);
    len = BIO_read(b, out, len);
    printf("%s, len: %d\n", out, len);
    OPENSSL_free(out);
    BIO_free(b);

    printf("\nsocket bio\n");
    int sock = BIO_get_accept_socket("8899", 0);
    BIO *bsock = BIO_new_socket(sock, BIO_NOCLOSE);
    char *addr = NULL;
    int ret = BIO_accept(sock, &addr);
    BIO_set_fd(bsock, ret, BIO_NOCLOSE);
    while (1)
    {
        char out[128] = {0};
        BIO_read(bsock, out, 128);
        if (out[0] = 'q')
            break;
        printf("%s\n", out);
    }
    BIO_free(bsock);

    printf("\n------------------------------\n");

    //base64
    unsigned char in[30], base64[40], decode[30];
    EVP_ENCODE_CTX *ectx = EVP_ENCODE_CTX_new();
    EVP_EncodeInit(ectx);
    for (int i = 0; i < 30; i++)
    {
        in[i] = i;
    }
    int outl, inl = 30;
    EVP_EncodeUpdate(ectx, base64, &outl, in, inl);
    EVP_EncodeFinal(ectx, base64 + outl, &outl);
    EVP_ENCODE_CTX_free(ectx);
    printf("%40s\n", base64);

    printf("\n------------------------------\n");

    //rsa
    unsigned char inbuf[] = "https://github.com/gongluck/CVIP";
    unsigned char outbuf[128] = {0};
    int n = strlen(in);
    MD4(inbuf, n, outbuf);
    printf("MD4 result: \n"); // 16 byte
    for (int i = 0; i < 16; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    MD5(inbuf, n, outbuf);
    printf("MD5 result: \n"); // 16 byte
    for (int i = 0; i < 16; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    // SHA(inbuf, n, outbuf);
    // printf("SHA result: \n"); // 20 byte
    // for (int i = 0; i < 20; i++)
    // {
    //     printf("%x", outbuf[i]);
    // }
    // printf("\n");
    SHA1(inbuf, n, outbuf);
    printf("SHA1 result: \n"); // 20 byte
    for (int i = 0; i < 20; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    SHA256(inbuf, n, outbuf);
    printf("SHA256 result: \n"); // 32 byte
    for (int i = 0; i < 32; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");
    SHA512(inbuf, n, outbuf);
    printf("SHA512 result: \n"); // 64 byte
    for (int i = 0; i < 64; i++)
    {
        printf("%x", outbuf[i]);
    }
    printf("\n");

    return 0;
}