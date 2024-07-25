/*******************************************************
 *  CSI-ROTS Test Case --  aes driver test
 *******************************************************/

#include "rsa_test.h"
#include <string.h>
#include <stdio.h>

char private_exponent[] = {
     0x3c, 0x9e, 0x53, 0x35, 0x32, 0xa9, 0x2b, 0xf2,
     0x64, 0x1e, 0xda, 0x77, 0x44, 0x4e, 0xe6, 0x2d,
     0xff, 0x4b, 0x56, 0x3a, 0xaf, 0xf9, 0x1f, 0x31,
     0x73, 0x82, 0x96, 0xa7, 0x23, 0x5d, 0x53, 0x6c,
     0x5a, 0xad, 0xb1, 0x12, 0xbb, 0xc9, 0xaf, 0x49,
     0x59, 0xfe, 0xae, 0x44, 0x41, 0xc2, 0xf9, 0xbf,
     0xe1, 0xb2, 0x48, 0xf0, 0x1c, 0x87, 0x07, 0xb6,
     0xc4, 0x56, 0x1e, 0xfb, 0xb7, 0x2d, 0xde, 0x5f,
     0xf3, 0x1b, 0x5a, 0xbe, 0x36, 0xbd, 0x9b, 0x75,
     0xa2, 0xca, 0xce, 0x0d, 0xf7, 0xfd, 0x86, 0xff,
     0x59, 0x4b, 0x10, 0xec, 0x5b, 0xa2, 0xce, 0xad,
     0x0e, 0x8c, 0x33, 0xad, 0xf1, 0xe4, 0x84, 0xd9,
     0xe9, 0xf7, 0x16, 0x9d, 0x06, 0x5a, 0x47, 0xa0,
     0x96, 0x21, 0xa6, 0xf2, 0x79, 0xce, 0x5e, 0xc3,
     0x49, 0x3c, 0x20, 0xb3, 0x9c, 0xe9, 0x13, 0x68,
     0xde, 0x70, 0x89, 0x88, 0xd4, 0x8f, 0x0d, 0xb5};

char public_exponent[]  = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};

char modulus[] = {
0xab, 0x83, 0x49, 0x3c, 0xaf, 0x88, 0x9f, 0x30, 0x93, 0x23, 0xb1, 0x99, 0x00, 0xbe, 0x09, 0x0c,
0xef, 0xab, 0x2e, 0x4c, 0x2c, 0x62, 0x16, 0xdc, 0xb2, 0x3f, 0xb4, 0xc6, 0xbd, 0x3d, 0x45, 0x42,
0x91, 0x66, 0xec, 0xf0, 0x97, 0x81, 0xc6, 0x51, 0xc7, 0x14, 0xfd, 0xa8, 0x00, 0xe0, 0x2c, 0xf2,
0x85, 0x77, 0x94, 0x6c, 0x53, 0xad, 0x55, 0x38, 0x77, 0x3d, 0x6e, 0x60, 0x27, 0xb7, 0x78, 0x9d,
0xb5, 0xe9, 0x02, 0xbe, 0xe9, 0xf2, 0xcf, 0x8c, 0x4b, 0xc1, 0x96, 0x13, 0x4d, 0x68, 0xee, 0x7d,
0x06, 0xbf, 0x49, 0x03, 0x6b, 0x1f, 0xb0, 0xa4, 0xd4, 0xd7, 0x63, 0x9e, 0x79, 0x42, 0xbe, 0x6a,
0xbf, 0xa3, 0x84, 0xcd, 0x91, 0xa0, 0x6b, 0xba, 0x40, 0x62, 0x2e, 0x57, 0xf7, 0x23, 0x15, 0x83,
0x9d, 0xf1, 0x31, 0xc3, 0x83, 0x4d, 0x58, 0x2a, 0xb2, 0x03, 0x63, 0x4c, 0xbb, 0x46, 0xff, 0x0d};

char pass[128] = "1234567890abcdef";

void rsa_cb()
{
    //printf("RSA event entered\n");
}

void rsa_if_test() {

    //get_capabilities
    rsa_capabilities_t cap = csi_rsa_get_capabilities(2);
    ASSERT(cap.bits_256 == 0);
    ASSERT(cap.bits_512 == 0);

    //initialize
    rsa_handle_t rsa = csi_rsa_initialize(2, rsa_cb);
    ASSERT(rsa == NULL);

    rsa = csi_rsa_initialize(0, NULL);
    ASSERT(rsa != NULL);

    int32_t ret;
    //uninitialize
    ret = csi_rsa_uninitialize(NULL);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE|DRV_ERROR_PARAMETER));

    //poower control
    ret = csi_rsa_power_control(rsa,DRV_POWER_FULL);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_UNSUPPORTED));
    ret = csi_rsa_power_control(NULL,DRV_POWER_FULL);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_UNSUPPORTED));

    //config
    ret = csi_rsa_config(NULL, RSA_DATA_BITS_1024,RSA_ENDIAN_MODE_LITTLE,NULL);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE|DRV_ERROR_PARAMETER));

    ret = csi_rsa_config(rsa,6,RSA_ENDIAN_MODE_LITTLE,NULL);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |RSA_ERROR_DATA_BITS));

    ret = csi_rsa_config(rsa,RSA_DATA_BITS_1024,2,NULL);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |RSA_ERROR_ENDIAN));

    ret = csi_rsa_config(rsa, RSA_DATA_BITS_1024,RSA_ENDIAN_MODE_LITTLE,NULL);

    char encrypt[128];
    char decrypt[128];
    char sign[128];
    uint32_t out_size;

    rsa_padding_t padding;
    //encrypt
    padding.padding_type = RSA_PADDING_MODE_PKCS1;
    padding.hash_type = RSA_HASH_TYPE_MD5;
    ret = csi_rsa_encrypt(NULL, modulus, public_exponent, pass, strlen(pass), encrypt, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_encrypt(rsa, NULL, public_exponent, pass, strlen(pass), encrypt, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_encrypt(rsa, modulus, NULL, pass, strlen(pass), encrypt, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_encrypt(rsa, modulus, public_exponent, NULL, strlen(pass), encrypt, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_encrypt(rsa, modulus, public_exponent, pass, 0, encrypt, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_encrypt(rsa, modulus, public_exponent, pass, strlen(pass), NULL, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.padding_type = 7;
    ret = csi_rsa_encrypt(rsa, modulus, public_exponent, pass, strlen(pass), encrypt, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.padding_type = RSA_PADDING_MODE_PKCS1;
    padding.hash_type = 6;
    ret = csi_rsa_encrypt(rsa, modulus, public_exponent, pass, strlen(pass), encrypt, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.hash_type = RSA_HASH_TYPE_MD5;

    //decrypt
    ret = csi_rsa_decrypt(NULL, modulus, private_exponent, encrypt, out_size, decrypt, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_decrypt(rsa, NULL, private_exponent, encrypt, out_size, decrypt, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_decrypt(rsa, modulus, NULL, encrypt, out_size, decrypt, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_decrypt(rsa, modulus, private_exponent, NULL, out_size, decrypt, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_decrypt(rsa, modulus, private_exponent, encrypt, 0, decrypt, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_decrypt(rsa, modulus, private_exponent, encrypt, out_size, NULL, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_decrypt(rsa, modulus, private_exponent, encrypt, out_size, decrypt, NULL ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));
    
    padding.padding_type = 7;
    ret = csi_rsa_decrypt(rsa, modulus, private_exponent, encrypt, out_size, decrypt, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.padding_type = RSA_PADDING_MODE_PKCS1;
    padding.hash_type = 6;
    ret = csi_rsa_decrypt(rsa, modulus, private_exponent, encrypt, out_size, decrypt, &out_size ,padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.hash_type = RSA_HASH_TYPE_MD5;

    //sign
    ret = csi_rsa_sign(NULL, modulus, public_exponent, pass, strlen(pass), sign, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_sign(rsa, NULL, public_exponent, pass, strlen(pass), sign, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_sign(rsa, modulus, NULL, pass, strlen(pass), sign, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_sign(rsa, modulus, public_exponent, NULL, strlen(pass), sign, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_sign(rsa, modulus, public_exponent, pass, 0, sign, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_sign(rsa, modulus, public_exponent, pass, strlen(pass), NULL, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_sign(rsa, modulus, public_exponent, pass, strlen(pass), sign, NULL, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.padding_type = 7;
    ret = csi_rsa_sign(rsa, modulus, public_exponent, pass, strlen(pass), sign, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.padding_type = RSA_PADDING_MODE_PKCS1;
    padding.hash_type = 6;
    ret = csi_rsa_sign(rsa, modulus, public_exponent, pass, strlen(pass), sign, &out_size, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.hash_type = RSA_HASH_TYPE_MD5;

    //verify
    char verify[128];
    ret = csi_rsa_verify(NULL, modulus, private_exponent, pass, strlen(pass), sign, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_verify(rsa, NULL, private_exponent, pass, strlen(pass), sign, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_verify(rsa, modulus, NULL, pass, strlen(pass), sign, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_verify(rsa, modulus, private_exponent, NULL, strlen(pass), sign, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_verify(rsa, modulus, private_exponent, pass, 0, sign, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_verify(rsa, modulus, private_exponent, pass, strlen(pass), NULL, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_verify(rsa, modulus, private_exponent, pass, strlen(pass), sign, 0, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    ret = csi_rsa_verify(rsa, modulus, private_exponent, pass, strlen(pass), sign, out_size, NULL, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.padding_type = 7;
    ret = csi_rsa_verify(rsa, modulus, private_exponent, pass, strlen(pass), sign, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

    padding.padding_type = RSA_PADDING_MODE_PKCS1;
    padding.hash_type = 6;
    ret = csi_rsa_verify(rsa, modulus, private_exponent, pass, strlen(pass), sign, out_size, &verify, padding);
    ASSERT(ret == (CSI_DRV_ERRNO_RSA_BASE |DRV_ERROR_PARAMETER));

	//get status
	rsa_status_t status;
	status = csi_rsa_get_status(NULL);
	ASSERT(status.busy == 0);

    csi_rsa_uninitialize(rsa);
}

void rsa_func_test()
{
    char *encrypt, *decrypt, *sign;
    int8_t verify;

    /*********** Function Test ***********/
    rsa_test_t check_list[] = {
    /**********  PKCS1 - MD5 *********/
    {RSA_HASH_TYPE_MD5, RSA_PADDING_MODE_PKCS1},
    /**********  NOPAD - MD5 *********/
    {RSA_HASH_TYPE_MD5, RSA_PADDING_MODE_NO},
    /**********  PKCS1 - SHA1 *********/
    //{RSA_HASH_TYPE_SHA1, RSA_PADDING_MODE_PKCS1},
    /**********  NOPAD - SHA1 *********/
    {RSA_HASH_TYPE_SHA1, RSA_PADDING_MODE_NO},
    };

    int i=0;
    uint32_t ret;
    int out_size;
    rsa_handle_t rsa;

    /*******  Test get instance  *********/
    
    rsa = csi_rsa_initialize(0,rsa_cb);
    ASSERT(rsa != NULL);
    rsa_padding_t padding;
    
    ret = csi_rsa_config(rsa,RSA_DATA_BITS_1024, RSA_ENDIAN_MODE_LITTLE,NULL);
    ASSERT(ret == 0);

    /**********  Function Test   *********************/
    for(i=0;i<sizeof(check_list)/sizeof(rsa_test_t);i++)
    {
        /*************  Encrypt Test  ***************************/
        padding.padding_type = check_list[i].padding;
        padding.hash_type = check_list[i].hash;
       
        encrypt = (char*) malloc(128);

        ret = csi_rsa_encrypt(rsa, modulus, public_exponent, pass, strlen(pass), encrypt, &out_size, padding);
        
        ASSERT(ret == 0);

        /*************  Decrypt Test  ***************************/
        decrypt = (char *) malloc(strlen(pass));

        ret = csi_rsa_decrypt(rsa, modulus, private_exponent, encrypt, out_size, decrypt, &out_size ,padding);
        ASSERT(ret == 0);
        ASSERT_STRING(decrypt, pass, strlen(pass));
        
        free(encrypt);
        free(decrypt);
        /*************  Sign Test  *********************************/
        sign = (char*) malloc(128);
        ret = csi_rsa_sign(rsa, modulus, public_exponent, pass, strlen(pass), sign, &out_size, padding);
        ASSERT(ret == 0);

        /************  Verify Test  ********************************/
        ret = csi_rsa_verify(rsa, modulus, private_exponent, pass, strlen(pass), sign, out_size, &verify, padding);
        ASSERT(verify == 1);
        free(sign);
    }

    csi_rsa_uninitialize(rsa);

}

void User_Task() {
//    rsa_if_test();

    rsa_func_test();
	printf("\n\ntest done.\n");
}
