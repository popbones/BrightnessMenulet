/*
 *  ddc.h
 *  ddc
 *
 *  Created by Jonathan Taylor on 07/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif 

#include <IOKit/IOKitLib.h>
#include <ApplicationServices/ApplicationServices.h>
#include <IOKit/i2c/IOI2CInterface.h>

#define RESET 0x04
#define RESET_BRIGHTNESS_AND_CONTRAST 0x05
#define RESET_GEOMETRY 0x06
#define RESET_COLOR 0x08	
#define BRIGHTNESS 0x10  //OK
#define CONTRAST 0x12 //OK
#define RED_GAIN 0x16
#define GREEN_GAIN 0x18
#define BLUE_GAIN 0x1A
#define AUTO_SIZE_CENTER 0x1E	
#define WIDTH 0x22
#define HEIGHT 0x32
#define VERTICAL_POS	0x30
#define HORIZONTAL_POS 0x20
#define PINCUSHION_AMP 0x24
#define PINCUSHION_PHASE 0x42
#define KEYSTONE_BALANCE 0x40
#define PINCUSHION_BALANCE 0x26
#define TOP_PINCUSHION_AMP 0x46
#define TOP_PINCUSHION_BALANCE 0x48
#define BOTTOM_PINCUSHION_AMP 0x4A
#define BOTTOM_PINCUSHION_BALANCE 0x4C
#define VERTICAL_LINEARITY 0x3A
#define VERTICAL_LINEARITY_BALANCE 0x3C
#define HORIZONTAL_STATIC_CONVERGENCE 0x28
#define VERTICAL_STATIC_CONVERGENCE 0x28
#define MOIRE_CANCEL 0x56
#define INPUT_SOURCE 0x60
#define AUDIO_SPEAKER_VOLUME 0x62
#define RED_BLACK_LEVEL 0x6C
#define GREEN_BLACK_LEVEL 0x6E
#define BLUE_BLACK_LEVEL 0x70

#define SETTINGS 0xB0                  //unsure on this one	
#define ON_SCREEN_DISPLAY 0xCA
#define OSD_LANGUAGE 0xCC
#define DPMS 0xD6
#define MAGIC_BRIGHT 0xDC //unsure
#define VCP_VERSION 0xDF
#define COLOR_PRESET 0xE0
#define POWER_CONTROL 0xE1	
	
#define TOP_LEFT_SCREEN_PURITY 0xE8
#define TOP_RIGHT_SCREEN_PURITY 0xE9
#define BOTTOM_LEFT_SCREEN_PURITY 0xE8
#define BOTTOM_RIGHT_SCREEN_PURITY 0xEB	
	
struct DDCWriteCommand {
    UInt8 control_id;
    UInt8 new_value;
};
    
struct DDCReadResponse {
    bool valid;
    
    UInt8 max_value;
    UInt8 current_value;
};

struct DDCReadCommand {
    UInt8 control_id;
    size_t reply_bytes;
    unsigned char*  reply_buffer;
    struct DDCReadResponse response;
};
    
// EDID struct credits to https://github.com/kfix/ddcctl
struct EDID {
    UInt64 header : 64;
    UInt8 : 1;
    UInt16 eisaid :15;
    UInt16 productcode : 16;
    UInt32 serial : 32;
    UInt8 week : 8;
    UInt8 year : 8;
    UInt8 versionmajor : 8;
    UInt8 versionminor : 8;
    UInt8 digitalinput : 1;
    union inputbitmap {
        struct digitalinput {
            UInt8 : 6;
            UInt8 dfp : 1;
        } digital;
        struct analoginput {
            UInt8 synclevels : 2;
            UInt8 pedestal : 1;
            UInt8 separate : 1;
            UInt8 composite : 1;
            UInt8 green : 1;
            UInt8 serrated : 1;
        } analog;
    };
    UInt8 maxh : 8;
    UInt8 maxv : 8;
    UInt8 gamma : 8;
    UInt8 standby : 1;
    UInt8 suspend : 1;
    UInt8 activeoff : 1;
    UInt8 displaytype: 2;
    UInt8 srgb : 1;
    UInt8 preferredtiming : 1;
    UInt8 gtf : 1;
    UInt8 redxlsb : 2;
    UInt8 redylsb : 2;
    UInt8 greenxlsb : 2;
    UInt8 greenylsb : 2;
    UInt8 bluexlsb : 2;
    UInt8 blueylsb : 2;
    UInt8 whitexlsb : 2;
    UInt8 whiteylsb : 2;
    UInt8 redxmsb : 8;
    UInt8 redymsb : 8;
    UInt8 greenxmsb : 8;
    UInt8 greenymsb : 8;
    UInt8 bluexmsb : 8;
    UInt8 blueymsb : 8;
    UInt8 whitexmsb : 8;
    UInt8 whiteymsb : 8;
    UInt8 t720x400a70 : 1;
    UInt8 t720x400a88 : 1;
    UInt8 t640x480a60 : 1;
    UInt8 t640x480a67 : 1;
    UInt8 t640x480a72 : 1;
    UInt8 t640x480a75 : 1;
    UInt8 t800x600a56 : 1;
    UInt8 t800x600a60 : 1;
    UInt8 t800x600a72 : 1;
    UInt8 t800x600a75 : 1;
    UInt8 t832x624a75 : 1;
    UInt8 t1024x768a87 : 1;
    UInt8 t1024x768a60 : 1;
    UInt8 t1024x768a72 : 1;
    UInt8 t1024x768a75 : 1;
    UInt8 t1280x1024a75 : 1;
    UInt8 t1152x870a75 : 1;
    UInt8 othermodes : 7;
    struct timing {
        UInt8 xresolution : 8;
        UInt8 ratio : 2;
        UInt8 vertical : 6;
    } timing1;
    struct timing timing2;
    struct timing timing3;
    struct timing timing4;
    struct timing timing5;
    struct timing timing6;
    struct timing timing7;
    struct timing timing8;
    union descriptor {
        struct timingdetail {
            UInt16 clock : 16;
            UInt8 hactivelsb : 8;
            UInt8 hblankinglsb : 8;
            UInt8 hactivemsb : 4;
            UInt8 hblankingmsb : 4;
            UInt8 vactivelsb : 8;
            UInt8 vblankinglsb : 8;
            UInt8 vactivemsb : 4;
            UInt8 vblankingmsb : 4;
            UInt8 hsyncoffsetlsb : 8;
            UInt8 hsyncpulselsb : 8;
            UInt8 vsyncoffsetlsb : 4;
            UInt8 vsyncpulselsb : 4;
            UInt8 hsyncoffsetmsb : 2;
            UInt8 hsyncpulsemsb : 2;
            UInt8 vsyncoffsetmsb : 2;
            UInt8 vsyncpulsemsb : 2;
            UInt8 hsizelsb : 8;
            UInt8 vsizelsb : 8;
            UInt8 hsizemsb : 4;
            UInt8 vsizemsb : 4;
            UInt8 hborder : 8;
            UInt8 vborder : 8;
            UInt8 interlaced : 1;
            UInt8 stereo : 2;
            UInt8 synctype : 2;
            union sync {
                struct analogsync {
                    UInt8 serrated : 1;
                    UInt8 syncall : 1;
                } analog;
                struct digitalsync {
                    UInt8 vsync : 1;
                    UInt8 hsync : 1;
                } digital;
            };
            UInt8 twowaystereo : 1;
        } timing;
        struct text {
            UInt32 : 24;
            UInt8 type : 8;
            UInt8 : 8;
            char data[13];
        } text;
        struct __attribute__ ((packed)) rangelimits {
            UInt64 header : 40;
            UInt8 minvfield : 8;
            UInt8 minhfield : 8;
            UInt8 minhline : 8;
            UInt8 minvline : 8;
            UInt8 maxclock : 8;
            UInt8 extended : 8;
            UInt8 : 8;
            UInt8 startfreq : 8;
            UInt8 cvalue : 8;
            UInt16 mvalue : 16;
            UInt8 kvalue : 8;
            UInt8 jvalue : 8;
        } range;
        struct __attribute__ ((packed)) whitepoint {
            UInt64 header : 40;
            UInt8 index : 8;
            UInt8 : 4;
            UInt8 whitexlsb : 2;
            UInt8 whiteylsb : 2;
            UInt8 whitexmsb : 8;
            UInt8 whiteymsb : 8;
            UInt8 gamma : 8;
            UInt8 index2 : 8;
            UInt8 : 4;
            UInt8 whitexlsb2 : 2;
            UInt8 whiteylsb2 : 2;
            UInt8 whitexmsb2 : 8;
            UInt8 whiteymsb2 : 8;
            UInt8 gamma2 : 8;
            UInt32 : 24;
        } whitepoint;
    } descriptor1;
    union descriptor descriptor2;
    union descriptor descriptor3;
    union descriptor descriptor4;
    UInt8 extensions : 8;
    UInt8 checksum : 8;
};

int ddc_write(CGDirectDisplayID display_id, struct DDCWriteCommand * p_write);
int ddc_read(CGDirectDisplayID display_id, struct DDCReadCommand * p_read);
void EDIDRead(CGDirectDisplayID display_id, struct EDID *edid);

#ifdef __cplusplus
}
#endif 