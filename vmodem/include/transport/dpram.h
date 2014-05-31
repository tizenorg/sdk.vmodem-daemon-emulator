/*
 *  telephony-emulator
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 * Sooyoung Ha <yoosah.ha@samsung.com>
 * YeongKyoon Lee <yeongkyoon.lee@samsung.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Contributors:
 * - S-Core Co., Ltd
 *
 */

/* * 	dpram.h 		* */
#ifndef _DPRAM_H_
#define _DPRAM_H_

#include <linux/ioctl.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

#ifndef __cplusplus
#ifndef false
#  define false 0
#endif
#ifndef true
#  define true  !0
#endif
#ifndef bool
#  define bool  char
#endif
#endif /* __cplusplus */

#define IOC_MZ_MAGIC		('h')

/*
   extra IOCTL mapping
   0x00 ~ 0x0f			for TV
   0x10 ~ 0x2f			for Camera
   0x30 ~ 0xff			reserved for future use
 */
#define IOC_MZ2_MAGIC		(0xC1)

/*
 * for touch devices
 */

/*
 * for /dev/misc/apm_bios
 */
#define MZ_LED_ON		0x01
#define MZ_LED_OFF		0x00
#define MZ_LED_BLINK		0x04
#define MZ_LED_BLINK_RATE	0x08	/* variable-rate blink */
#define MZ_LED_READ_ONLY	0x80
#define MZ_LED_COLOR		0x40

typedef struct {
    unsigned int index;		/* LED index to control */
    unsigned int stat;		/* control command or current status */
    unsigned int rate;		/* blinking rate */
    unsigned int color;		/* LED color */
    unsigned int info;		/* capable function */
} LED_RET;

#define MZ_GET_LED_NO		_IOR(IOC_MZ_MAGIC, 0x8b, unsigned int)
#define MZ_GET_LED_STATUS	_IOR(IOC_MZ_MAGIC, 0x8c, LED_RET)
#define MZ_SET_LED_STATUS	_IOW(IOC_MZ_MAGIC, 0x8d, LED_RET)

/*
 * for /dev/ppp device
 */
#define MZ_PPP_DORMANT_ON	_IO (IOC_MZ_MAGIC, 0x92)
#define MZ_PPP_DORMANT_OFF	_IO (IOC_MZ_MAGIC, 0x93)

/*
 * for mz_ctrl_mmap
 */
typedef struct __mz_mmap_t {
    int id;
    unsigned long offset;
    unsigned long size;
} mz_mmap_t;

#define MZ_MMAP_GET		_IOR(IOC_MZ_MAGIC, 0x94, mz_mmap_t)
#define MZ_MMAP_WRITTEN		_IOW(IOC_MZ_MAGIC, 0x95, mz_mmap_t)

/*
 * PDA test
 */
typedef struct _mz_tst_t {
    int id;
    unsigned int buf[3];
    unsigned long jiff;
} mz_tst_t;
#define MZ_TST_ID_KEY		0	/* keyboard, button, ... */
#define MZ_TST_ID_TS		1	/* TouchScreen */

/*******************************************************************/
//here  i8xx.h

/* Audio Path Control */
#define HN_AUDIO_PATH		_IOW(IOC_MZ_MAGIC, 0xc0, unsigned long)

/* select MIC1 */
#define KEYPAD_MIC		(1 << 0)
#define EAR_MIC			(1 << 1)
#define CAM_MIC			(1 << 2)
#define BT_MIC			(1 << 3)

#define REC_MIC1		(1 << 6)
#define REC_PHONE		(1 << 7)

#define PLAY_OUT2		(1 << 8)
#define PLAY_HPOUT		(1 << 9)
#define PLAY_MONOOUT		(1 << 10)
#define PLAY_CRADLE		(1 << 11)
#define PLAY_BT			(1 << 12)
#define PLAY_UART_BT		(1 << 13)
#define PLAY_OUT3		(1 << 14)

#define PHONE_RING		(1 << 16)
#define PHONE_OUT3		(1 << 17)
#define PHONE_HPOUT		(1 << 18)
#define PHONE_OUT2		(1 << 19)
#define PHONE_CRADLE		(1 << 20)
#define PHONE_BT		(1 << 21)

#define MZ_AMP_ON		(1 << 24)
#define ADC_PWR_ON		(1 << 25)
#define MZ_AMP_OFF		(1 << 26)

#define ICP_TX 			(1 << 28)
#define ICP_RX 			(1 << 29)
#define BT_LOOPBACK 		(1 << 30)

/* Audio EQ Control */
#define HN_AUDIO_EQ		_IOW(IOC_MZ_MAGIC, 0xde, unsigned long)

#define EQ_FLAT			(1 << 0)
#define EQ_POP			(1 << 1)
#define EQ_ROCK			(1 << 2)
#define EQ_JAZZ			(1 << 3)
#define EQ_CLASSIC		(1 << 4)
#define EQ_DANCE		(1 << 5)
#define EQ_HEAVY		(1 << 6)
#define EQ_DISCO		(1 << 7)
#define EQ_SOFT			(1 << 8)
#define EQ_HALL			(1 << 9)
#define EQ_LIVE			(1 << 10)

/* Audio Gain */
struct audio_gain {
    unsigned short apply;
    unsigned int pcm;
    unsigned int altpcm;
    unsigned int phone;
};

#define HN_AUDIO_GAIN		_IOW(IOC_MZ_MAGIC, 0xdf, struct audio_gain)

#define HN_AMP_SET_STATUS	_IOW(IOC_MZ_MAGIC, 0xcf, unsigned int)
#define HN_AMP_OFF_STATUS	0x0
#define HN_AMP_ON_STATUS	0x1

/* rescan external perpheral device */
#define HN_RESCAN_ACCESSARY	_IO (IOC_MZ_MAGIC, 0xc1)

/* rescan & get battery type */
#define HN_RESCAN_BATTERY_TYPE	_IOR(IOC_MZ_MAGIC, 0xc2, unsigned int)
#define HN_BATTERY_TYPE_STD	0x0
#define HN_BATTERY_TYPE_EXT	0x1
#define HN_BATTERY_TYPE_STB	0x2

#define HN_BATTERY_TYPE_STB_RAW	14
#define HN_RESCAN_BATTERY_TYPE2	_IOR(IOC_MZ_MAGIC, 0xc5, unsigned int)

/* UART & USB port switching */
#define HN_UART_TO_PHONE	_IO (IOC_MZ_MAGIC, 0xc8)
#define HN_UART_TO_PDA		_IO (IOC_MZ_MAGIC, 0xc9)
#define HN_USB_TO_PHONE		_IO (IOC_MZ_MAGIC, 0xca)
#define HN_USB_TO_PDA		_IO (IOC_MZ_MAGIC, 0xcb)
#define HN_USB_UART_STATE	_IO (IOC_MZ_MAGIC, 0xce)

#define HN_UART_PATH_PDA	0x0001
#define HN_UART_PATH_PHONE	0x0002
#define HN_USB_PATH_PDA		0x0010
#define HN_USB_PATH_PHONE	0x0020

/* Vibrator Control */
#define HN_MOTOR_ON		_IO (IOC_MZ_MAGIC, 0xcc)
#define HN_MOTOR_OFF		_IO (IOC_MZ_MAGIC, 0xcd)

/* Bluetooth Power Control */
#define HN_BT_ON		_IO (IOC_MZ_MAGIC, 0xdc)
#define HN_BT_OFF		_IO (IOC_MZ_MAGIC, 0xdd)

/* Keypad */
#define HN_KEYPAD_ENABLE	_IO (IOC_MZ_MAGIC, 0xea)
#define HN_KEYPAD_DISABLE	_IO (IOC_MZ_MAGIC, 0xeb)
#define HN_LOCK_STATUS		_IOR(IOC_MZ_MAGIC, 0xec, unsigned int)

/* DPRAM Control for communication between PDA and Phone */

/* DPRAM ioctls for DPRAM tty devices */
#define HN_DPRAM_PHONE_ON		_IO (IOC_MZ_MAGIC, 0xd0)
#define HN_DPRAM_PHONE_OFF		_IO (IOC_MZ_MAGIC, 0xd1)
#define HN_DPRAM_PHONE_GETSTATUS	_IOR(IOC_MZ_MAGIC, 0xd2, unsigned int)
#define HN_DPRAM_PHONE_DOWNLOAD		_IO (IOC_MZ_MAGIC, 0xd5)
#define HN_DPRAM_PHONE_REBUILD		_IO (IOC_MZ_MAGIC, 0xd9)
#define HN_DPRAM_PHONE_MAGICCODE	_IO (IOC_MZ_MAGIC, 0xda)

/* return codes for HN_DPRAM_PHONE_GETSTATUS */
#define HN_DPRAM_PHONE_STATUS_OFF	0x00
#define HN_DPRAM_PHONE_STATUS_ON	0x01

/* DPRAM ioctls for DPRAM ctl device */
#define HN_DPRAM_PPP_ENABLE		_IO (IOC_MZ_MAGIC, 0xd3)
#define HN_DPRAM_PPP_DISABLE		_IO (IOC_MZ_MAGIC, 0xd4)
#define HN_DPRAM_PPP_AC_ENABLE		_IO (IOC_MZ_MAGIC, 0xd6)
#define HN_DPRAM_PPP_AC_DISABLE		_IO (IOC_MZ_MAGIC, 0xd7)
#define HN_DPRAM_SIM_GETSTATUS		_IOR(IOC_MZ_MAGIC, 0xd8, unsigned int)

/* return codes for HN_DPRAM_SIM_GETSTATUS */
#define HN_DPRAM_SIM_STATUS_CLOSE	0x00
#define HN_DPRAM_SIM_STATUS_OPEN	0x01

/* DPRAM events through /dev/dpram/ctl */
#define HN_DPRAM_EVENT_PPP_ACCESS	0x0001
#define HN_DPRAM_EVENT_PHONE_DN_DONE	0x0002
#define HN_DPRAM_EVENT_SIM_COVER_OPEN	0x0004
#define HN_DPRAM_EVENT_SIM_COVER_CLOSE	0x0008
#define HN_DPRAM_EVENT_PHONE_REB_START	0x0010
#define HN_DPRAM_EVENT_PHONE_REB_END	0x0020
#define HN_DPRAM_EVENT_PHONE_INACTIVE	0x0040
#define HN_DPRAM_EVENT_PHONE_ACTIVE	0x0080

/* button definition */
#define HN_POWER_BUTTON		MZ_SCANCODE_POWER
#define HN_CAMERA_BUTTON	MZ_SCANCODE_U1
#define HN_CAMERA_AF_BUTTON	MZ_SCANCODE_U3
#define HN_VOICE_BUTTON		MZ_SCANCODE_RECORD

#define HN_SIDE_UP_BUTTON	MZ_SCANCODE_SLIDE_UP
#define HN_SIDE_DOWN_BUTTON	MZ_SCANCODE_SLIDE_DOWN

#define HN_HOME_BUTTON		MZ_SCANCODE_MENU
#define HN_BACK_BUTTON		MZ_SCANCODE_CLR
#define HN_SEND_BUTTON		MZ_SCANCODE_SEND
#define HN_END_BUTTON		MZ_SCANCODE_END
#define HN_END_LONG_BUTTON	MZ_SCANCODE_F11

#define HN_UP_BUTTON		MZ_SCANCODE_UP
#define HN_DOWN_BUTTON		MZ_SCANCODE_DOWN
#define HN_LEFT_BUTTON		MZ_SCANCODE_LEFT
#define HN_RIGHT_BUTTON		MZ_SCANCODE_RIGHT
#define HN_OK_BUTTON		MZ_SCANCODE_ENTER

#define HN_EAR_SEND_BUTTON	MZ_SCANCODE_U2

#define HN_0_BUTTON		MZ_SCANCODE_PAD_0
#define HN_1_BUTTON		MZ_SCANCODE_PAD_1
#define HN_2_BUTTON		MZ_SCANCODE_PAD_2
#define HN_3_BUTTON		MZ_SCANCODE_PAD_3
#define HN_4_BUTTON		MZ_SCANCODE_PAD_4
#define HN_5_BUTTON		MZ_SCANCODE_PAD_5
#define HN_6_BUTTON		MZ_SCANCODE_PAD_6
#define HN_7_BUTTON		MZ_SCANCODE_PAD_7
#define HN_8_BUTTON		MZ_SCANCODE_PAD_8
#define HN_9_BUTTON		MZ_SCANCODE_PAD_9
#define HN_ASTERISK_BUTTON	MZ_SCANCODE_ASTERISK
#define HN_SHARP_BUTTON		MZ_SCANCODE_SHARP

#define HN_MENU_BUTTON		MZ_SCANCODE_F12
#define HN_CANCEL_BUTTON	MZ_SCANCODE_CLR
#define HN_ADDRESS_BUTTON	MZ_SCANCODE_F5
#define HN_MSG_BUTTON		MZ_SCANCODE_F6
#define HN_UMAX_BUTTON		MZ_SCANCODE_F7

#define HN_PLAY_BUTTON		MZ_SCANCODE_F8
#define HN_PREVIOUS_BUTTON	MZ_SCANCODE_F9
#define HN_NEXT_BUTTON		MZ_SCANCODE_F10

/*
 * CAMERA interface from intc3
 */
#define WCAM_VIDIOCSCAMREG  	_IOW(IOC_MZ2_MAGIC, 0x10, int)
#define WCAM_VIDIOCGCAMREG   	_IOR(IOC_MZ2_MAGIC, 0x11, int)
#define WCAM_VIDIOCSCIREG    	_IOW(IOC_MZ2_MAGIC, 0x12, int)
#define WCAM_VIDIOCGCIREG    	_IOR(IOC_MZ2_MAGIC, 0x13, int)
#define WCAM_VIDIOCSINFOR	_IOW(IOC_MZ2_MAGIC, 0x14, int)
#define WCAM_VIDIOCGINFOR	_IOR(IOC_MZ2_MAGIC, 0x15, int)

typedef struct _mz_cif_reg_t {
    int val1, val2;
} mz_cif_reg_t;

/*
 * camera DSP control
 */
struct i819_cam_window {
    int width;
    int height;
    int zoom;
    int color_space;
    int ret_mode;
};
#define HN_CAM_S_WB		_IOW(IOC_MZ2_MAGIC, 0x16, unsigned int)
#define HN_CAM_S_BRIGHTNESS	_IOW(IOC_MZ2_MAGIC, 0x17, unsigned int)
#define HN_CAM_S_EFFECT		_IOW(IOC_MZ2_MAGIC, 0x18, unsigned int)
#define HN_CAM_PREPARE_STILL	_IO (IOC_MZ2_MAGIC, 0x19)
#define HN_CAM_S_FOCUS_AUTO	_IOW(IOC_MZ2_MAGIC, 0x1a, int)
#define HN_CAM_S_FOCUS_MAN	_IOW(IOC_MZ2_MAGIC, 0x1b, int)
#define HN_CAM_CHANGETO_C	_IO (IOC_MZ2_MAGIC, 0x1c)
#define HN_CAM_RETURNTO_B	_IO (IOC_MZ2_MAGIC, 0x1d)
#define HN_CAM_G_IRIS		_IOR(IOC_MZ2_MAGIC, 0x1e, int)
#define HN_CAM_G_LENS		_IOR(IOC_MZ2_MAGIC, 0x1f, unsigned int)
#define HN_CAM_G_WBR		_IOR(IOC_MZ2_MAGIC, 0x20, unsigned int)
#define HN_CAM_G_WBB		_IOR(IOC_MZ2_MAGIC, 0x21, unsigned int)
#define HN_CAM_TEST_EMP		_IO(IOC_MZ2_MAGIC, 0x22)
/* removed : #define HN_CAM_S_FLASH		_IO(IOC_MZ2_MAGIC, 0x23) */
#define HN_CAM_AF_SET		_IOW(IOC_MZ2_MAGIC, 0x23, int)
#define HN_CAM_CHECK_WB_1		_IO(IOC_MZ2_MAGIC, 0x24)
#define HN_CAM_CHECK_WB_2		_IO(IOC_MZ2_MAGIC, 0x25)
#define HN_CAM_PREPARE_STILL_2		_IO (IOC_MZ2_MAGIC, 0x26)
#define HN_CAM_S_FOCUS_SEMI_AUTO	_IOW(IOC_MZ2_MAGIC, 0x27, unsigned int)
#define HN_CAM_MORE_SHARPEN		_IO (IOC_MZ2_MAGIC, 0x28)
#define HN_CAM_NORMAL_SHARPEN		_IO (IOC_MZ2_MAGIC, 0x29)
#define HN_CAM_S_WINDOW		_IOWR(IOC_MZ2_MAGIC, 0x2A, struct i819_cam_window)
#define HN_CAM_S_LENS		_IO (IOC_MZ2_MAGIC, 0x2B)
#define HN_CAM_S_WB_SP		_IO (IOC_MZ2_MAGIC, 0x2C)
#define HN_CAM_FREEZE_AWB	_IO (IOC_MZ2_MAGIC, 0x2D)


#define FOCUS_MODE_NORMAL	0
#define FOCUS_MODE_MACRO	1
#define FOCUS_MODE_POTRAIT	2
#define FOCUS_MODE_LANDSCAPE 	3

/*
 * Image format definition
 */
#define CAMERA_IMAGE_FORMAT_RAW8                0
#define CAMERA_IMAGE_FORMAT_RAW9                1
#define CAMERA_IMAGE_FORMAT_RAW10               2
#define CAMERA_IMAGE_FORMAT_RGB444              3
#define CAMERA_IMAGE_FORMAT_RGB555              4
#define CAMERA_IMAGE_FORMAT_RGB565              5
#define CAMERA_IMAGE_FORMAT_RGB666_PACKED       6
#define CAMERA_IMAGE_FORMAT_RGB666_PLANAR       7
#define CAMERA_IMAGE_FORMAT_RGB888_PACKED       8

/* Camera Flash Modes */
#define HN_FLASH_MODE_UNKNOWN		0
#define HN_FLASH_MODE_STILLIMAGE	1
#define HN_FLASH_MODE_MOVIE		2

/* Light Sensor control */
#define HN_GET_ILLUMINANCE	_IOR(IOC_MZ_MAGIC, 0xfa, unsigned int)
#define HN_LIGHT_SENSOR_ON	_IO (IOC_MZ_MAGIC, 0xfb)
#define HN_LIGHT_SENSOR_OFF	_IO (IOC_MZ_MAGIC, 0xfc)

/* TVOUT */
#define HN_TVO2_S_BASE		_IOW(IOC_MZ2_MAGIC, 0x00, int)
#define HN_TVO2_G_BASE_DESC	_IOR(IOC_MZ2_MAGIC, 0x01, unsigned long)
#define HN_TVO2_POWER_ON	_IO (IOC_MZ2_MAGIC, 0x02)
#define HN_TVO2_POWER_OFF	_IO (IOC_MZ2_MAGIC, 0x03)
#define HN_TVO2_START		_IO (IOC_MZ2_MAGIC, 0x04)
#define HN_TVO2_STOP		_IO (IOC_MZ2_MAGIC, 0x05)
#define HN_TVO2_FRAMERATE	_IOW(IOC_MZ2_MAGIC, 0x06, int)
#define HN_TVO2_S_CMDADR	_IOW(IOC_MZ2_MAGIC, 0x07, unsigned long)
#define HN_TVO2_S_DATADR	_IOW(IOC_MZ2_MAGIC, 0x08, unsigned long)

/*
 * Framebuffer control
 */
#define HN_FB_SET_DISP_ORDER	_IOW (IOC_MZ_MAGIC, 0xf6, int)
#define FB_DISP_ORDER_BASE_BOTTOM	0
#define FB_DISP_ORDER_BASE_TOP		1
#define HN_FB_SET_K1_CONST	_IOW (IOC_MZ_MAGIC, 0xf7, int)
#define HN_FB_SET_K2_CONST	_IOW (IOC_MZ_MAGIC, 0xf8, int)
#define HN_FB_SET_K3_CONST	_IOW (IOC_MZ_MAGIC, 0xf9, int)

/*
 * Backup Battery Cut-On/Off
 */
#define HN_BAK_BATTERY_ON	_IO (IOC_MZ_MAGIC, 0xfe)
#define HN_BAK_BATTERY_OFF	_IO (IOC_MZ_MAGIC, 0xff)

/*
 * mz_mmap mapping
 */
#define MMAP_SIZE		4096	/* = kernel's PAGE_SIZE */

#define HN_MMAP_BATTERY		0x01
typedef struct _hn_mmap_bat_t {
    unsigned int voltage, raw;
    unsigned int level, battery, ac, blite;

    unsigned int cpu, load;
    unsigned int master, altpcm, pcm, flash;
    unsigned long stat;

    /* used by iom */
    unsigned int phone_fsm_bak;
    unsigned int stat_bak;
} hn_mmap_bat_t;

#if defined(__KERNEL__)
struct hn_b_mmap_t {
    mz_kmap_t k;
    hn_mmap_bat_t *v;
};
extern struct hn_b_mmap_t hn_b_mmap;
#endif

/*
 * mz_ops_t.bstat
 */
#define HN_BATTERY_AMP			(1 << 0)
#define HN_BATTERY_LCD_OFF		(1 << 1)
#define HN_BATTERY_BT			(1 << 2)
#define HN_BATTERY_CAM_ON		(1 << 3)
#define HN_BATTERY_CAM_RUN		(1 << 4)
#define HN_BATTERY_CAM_FLASH		(1 << 5)
#define HN_BATTERY_TV			(1 << 6)
#define HN_BATTERY_MMC			(1 << 7)
#define HN_BATTERY_MOTOR		(1 << 8)
#define HN_BATTERY_LED0			(1 << 9)
#define HN_BATTERY_LED1			(1 << 10)
#define HN_BATTERY_LED2			(1 << 11)


/*******************************************************************/
typedef struct {
    int         hPhone;
    bool        Power;
    struct termios  old_settings;
} GSM_Device_DPRAMData;


#endif /* _DPRAM_H_ */


