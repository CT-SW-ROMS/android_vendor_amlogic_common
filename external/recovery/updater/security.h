/*
* Copyright (c) 2014 Amlogic, Inc. All rights reserved.
* *
This source code is subject to the terms and conditions defined in the
* file 'LICENSE' which is part of this source code package.
* *
Description:
*/

#ifndef _SECURITY_H_
#define _SECURITY_H_

#define DTB_IMG        "dt.img"
#define BOOT_IMG        "boot.img"
#define RECOVERY_IMG    "recovery.img"
#define VENDOR_BOOT_IMG "vendor_boot.img"
#define BOOTLOADER_IMG  "bootloader.img"
#define ARRAY_SIZE(x)   (int)(sizeof(x)/sizeof(x[0]))

#define ANDROID_VERSION_R "30"

#define NORMALBOOT_NAME_SIZE   16
#define NORMALBOOT_ARGS_SIZE   512
#define NORMALBOOT_MAGIC_SIZE  8
#define NORMALBOOT_MAGIC       "ANDROID!"

#define SECUREBOOT_MAGIC       "AMLSECU!"
#define SECUREBOOT_MAGIC_SIZE  16
#define SECUREBOOT_MAGIC_VESRION 0x0801

#define DEFEND_KEY      "/dev/defendkey"
#define DECRYPT_DTB    "/sys/class/defendkey/decrypt_dtb"
#define SECURE_CHECK  "/sys/class/defendkey/secure_check"

#define SECURE_ERROR    (-1)
#define SECURE_FAIL        (0)
#define SECURE_MATCH    (1)
#define SECURE_SKIP        (2)
#define SECURE_OLD_NEW   (3)


#ifndef SECURITY_DEBUG
#define secureDbg(fmt ...)
#else
#define secureDbg(fmt ...) printf(fmt)
#endif

typedef enum SecureCheck {
    FAIL,
    ENCRYPT,
    UNENCRYPT,
    TYPE_MAX,
} T_SecureCheck;

static const char *s_pStatus[TYPE_MAX] = {
    "fail",
    "encrypt",
    "raw",
};

//S905 SECURE BOOT HEAD
#define AML_SECU_BOOT_IMG_HDR_MAGIC_SIZE   (8)


typedef struct __aml_enc_blk{
        unsigned int  nOffset;
        unsigned int  nRawLength;
        unsigned int  nSigLength;
        unsigned int  nAlignment;
        unsigned int  nTotalLength;
        unsigned char szPad[12];
        unsigned char szSHA2IMG[32];
        unsigned char szSHA2KeyID[32];
}t_aml_enc_blk;

typedef struct {

        unsigned char magic[AML_SECU_BOOT_IMG_HDR_MAGIC_SIZE];//magic to identify whether it is a encrypted boot image

        unsigned int  version;                  //ersion for this header struct
        unsigned int  nBlkCnt;

        unsigned char szTimeStamp[16];

        t_aml_enc_blk   amlKernel;
        t_aml_enc_blk   amlRamdisk;
        t_aml_enc_blk   amlDTB;

}AmlEncryptBootImgInfo, *p_AmlEncryptBootImgInfo;

typedef struct _boot_img_hdr_secure_boot
{
    unsigned char           reserve4ImgHdr[1024];

    AmlEncryptBootImgInfo   encrypteImgInfo;

}*AmlSecureBootImgHeader;

int RecoveryPreUpdate(const ZipArchiveHandle zipArchive);

int RecoverySecureCheck(const ZipArchiveHandle zipArchive);

int DtbImgDecrypt(
        const char *imageName,
        const unsigned char *imageBuffer,
        const int imageSize,
        const char *flag,
        unsigned char *encryptedbuf);


//extern RecoveryUI *ui;

#endif  /* _SECURITY_H_ */
