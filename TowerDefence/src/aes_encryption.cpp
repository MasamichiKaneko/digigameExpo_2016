#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

void encrypt( const char* ifile,  const unsigned char* password ) {

    // salt key
    unsigned char salt_key[] = "xxxxxxxx";
    // salt string
    unsigned char salt[] = "Salted__xxxxxxxx";

    // saltをランダムに生成
    for (int i = 0; i < PKCS5_SALT_LEN; i++) {
        salt[i + PKCS5_SALT_LEN] = salt_key[i] = (int) ((unsigned char) std::rand());
    }

    struct stat ifile_stat;
    if (0 == stat(ifile, &ifile_stat)) {
        if (ifile_stat.st_size > 0) {
            unsigned char *indata = (unsigned char *) malloc(ifile_stat.st_size);
            unsigned char *outdata = (unsigned char *) malloc(ifile_stat.st_size + 32);
            unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];

            const EVP_CIPHER* ci = EVP_aes_256_cbc();
            EVP_BytesToKey(ci, EVP_md5(), salt_key, password, strlen((const char *) password), 1, key, iv);

            FILE* i_file = fopen(ifile, "rb");
            fread(indata, sizeof(char), ifile_stat.st_size, i_file);

			fclose(i_file);

            int outdata1_len = 0;
            int outdata2_len = 0;

            EVP_CIPHER_CTX ctx;
            EVP_EncryptInit(&ctx, ci, key, iv);
            EVP_EncryptUpdate(&ctx, outdata, &outdata1_len, indata,ifile_stat.st_size);
            EVP_EncryptFinal(&ctx, outdata + outdata1_len, &outdata2_len);
            EVP_CIPHER_CTX_cleanup(&ctx);
            
            FILE* o_file = fopen(ifile, "wb");

            // salt書き込み
            fwrite(salt, sizeof(char), strlen((const char *) salt), o_file);
            // データ書き込み
            fwrite(outdata, sizeof(char), outdata1_len + outdata2_len, o_file);
            
            fclose(o_file);
            
            free(indata);
            free(outdata);
            indata = NULL;
            outdata = NULL;
        }
    }
}

void encrypt( const char* ifile, const char* password ){
	encrypt( ifile, reinterpret_cast<unsigned char*>( const_cast<char*>(password) ) );
}

void decrypt( const char* ifile, const unsigned char* password ) {

    struct stat ifile_stat;
    if (0 == stat(ifile, &ifile_stat)) {
        FILE* i_file = fopen(ifile, "rb");

        unsigned char salt_key[PKCS5_SALT_LEN];
        unsigned char salt[16];

        // salt読み込み
        fread(salt, sizeof(char), 16, i_file);
        for (int i = 0; i < PKCS5_SALT_LEN; i++) {
            salt_key[i] = salt[i + PKCS5_SALT_LEN];
        }

        int outdata1_len = 0;
        int outdata2_len = 0;

        unsigned char *indata = (unsigned char *) malloc(ifile_stat.st_size);
        unsigned char *outdata = (unsigned char *) malloc(ifile_stat.st_size);
        unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];

        // データ読み込み
        fread(indata, sizeof(char), ifile_stat.st_size, i_file);

        const EVP_CIPHER* ci = EVP_aes_256_cbc();
        EVP_BytesToKey(ci, EVP_md5(), salt_key, password,strlen((const char *) password), 1, key, iv);

        EVP_CIPHER_CTX ctx;
        EVP_DecryptInit(&ctx, ci, key, iv);
        EVP_DecryptUpdate(&ctx, outdata, &outdata1_len, indata,ifile_stat.st_size);
        EVP_DecryptFinal(&ctx, outdata + outdata1_len, &outdata2_len);
        EVP_CIPHER_CTX_cleanup(&ctx);
        
		fclose(i_file);

        FILE* o_file = fopen(ifile, "wb");
        // padding部分を削って書き込む。書き込みデータの一番最後のデータから削る分を特定する。
        fwrite(outdata, sizeof(char),outdata1_len - (int) outdata[ifile_stat.st_size - 16 - 1],o_file);
        
        fclose(o_file);
        
        free(indata);
        free(outdata);
        indata = NULL;
        outdata = NULL;
    }
}

void decrypt( const char* ifile, const char* password ){
	decrypt( ifile, reinterpret_cast<unsigned char*>( const_cast<char*>(password) ) );
}