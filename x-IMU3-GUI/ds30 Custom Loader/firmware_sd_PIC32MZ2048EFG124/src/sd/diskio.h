/*-----------------------------------------------------------------------/
/  Low level disk interface module include file   (C)ChaN, 2014          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#include "integer.h"


/* Status of Disk Functions */
typedef BYTE	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR,		/* 1: R/W Error */
	RES_WRPRT,		/* 2: Write Protected */
	RES_NOTRDY,		/* 3: Not Ready */
	RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;

/*typedef struct {
	unsigned csd_structure:2;
	unsigned reserved1:6;
	unsigned taac:8;
	unsigned nsac:8;
	unsigned tran_speed:8;
	unsigned ccc:12;
	unsigned read_bl_len:4;
	unsigned read_bl_partial:1;
	unsigned write_blk_misalign:1;
	unsigned read_blk_misalign:1;
	unsigned dsr_imp:1;
	unsigned reserved2:2;
	unsigned c_size:12;
	unsigned vdd_r_curr_min:3;
	unsigned vdd_r_curr_max:3;
	unsigned vdd_w_curr_min:3;
	unsigned vdd_w_curr_max:3;
	unsigned c_size_mult:3;
	unsigned erase_blk_en:1;
	unsigned sector_size:7;
	unsigned wp_grp_size:7;
	unsigned wp_grp_enable:1;
	unsigned reserved3:2;
	unsigned r2w_factor:3;
	unsigned write_bl_len:4;
	unsigned write_bl_partial:1;
	unsigned reserved4:5;
	unsigned file_format_grp:1;
	unsigned copy:1;
	unsigned perm_write_protect:1;
	unsigned tmp_write_protect:1;
	unsigned file_format:2;
	unsigned reserved5:2;
	unsigned crc:7;
	unsigned reserved6:1;
} sd_csd_v1_t;

typedef struct {
	unsigned csd_structure:2;
    unsigned reserved:6;
    unsigned TAAC:8;
    unsigned NSAC:8;
	unsigned tran_speed:8;
	unsigned ccc:12;
    unsigned READ_BL_LEN:4;
    unsigned READ_BL_PARTIAL:1;
    unsigned WRITE_BLK_MISALIGN:1;
    unsigned READ_BLK_MISALIGN:1;
	unsigned dsr_imp:1;
    unsigned reserved2:6;
	unsigned c_size:22;
    unsigned reserved3:1;
    unsigned ERASE_BLK_EN:1;
    unsigned SECTOR_SIZE:7;
    unsigned WP_GRP_SIZE:7;
    unsigned WP_GRP_ENABLE:1;
    unsigned reserved4:2;
    unsigned R2W_FACTOR:3;
    unsigned WRITE_BL_LEN:4;
    unsigned WRITE_BL_PARTIAL:1;
    unsigned reserved5:5;
    unsigned FILE_FORMAT_GRP:1;
	unsigned copy:1;
	unsigned perm_write_protect:1;
	unsigned tmp_write_protect:1;
    unsigned FILE_FORMAT:2;
    unsigned reserved6:2;
	unsigned crc:7;
    unsigned not_used:1;
} sd_csd_v2_t;*/


/*---------------------------------------*/
/* Prototypes for disk control functions */


DSTATUS disk_initialize (BYTE pdrv);
DSTATUS disk_status (BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);


/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC			0	/* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT	1	/* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at _USE_MKFS == 1) */
#define CTRL_TRIM			4	/* Inform device that the data on the block of sectors is no longer used (needed at _USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */
#define CTRL_FORMAT			8	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */
#define ISDIO_READ			55	/* Read data form SD iSDIO register */
#define ISDIO_WRITE			56	/* Write data to SD iSDIO register */
#define ISDIO_MRITE			57	/* Masked write data to SD iSDIO register */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20	/* Get F/W revision */
#define ATA_GET_MODEL		21	/* Get model name */
#define ATA_GET_SN			22	/* Get serial number */

#ifdef __cplusplus
}
#endif

#endif
