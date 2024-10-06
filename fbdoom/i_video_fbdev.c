// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------

#include <epframebuffer/rm_framebuffer.h>

static const char
rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include "config.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_event.h"
#include "d_main.h"
#include "i_video.h"
#include "z_zone.h"

#include "tables.h"
#include "doomkeys.h"

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define CMAP256

rm_framebuffer fb;

int fb_scaling = 1;
int usemouse = 0;

struct color {
    uint32_t b:8;
    uint32_t g:8;
    uint32_t r:8;
    uint32_t a:8;
};

static struct color colors[256];

// The screen buffer; this is modified to draw things to the screen

byte *I_VideoBuffer = NULL;

/* framebuffer file descriptor */
// int fd_fb = 0;

int	X_width;
int X_height;

// If true, game is running as a screensaver

boolean screensaver_mode = false;

// Flag indicating whether the screen is currently visible:
// when the screen isnt visible, don't render the screen

boolean screenvisible;

// Mouse acceleration
//
// This emulates some of the behavior of DOS mouse drivers by increasing
// the speed when the mouse is moved fast.
//
// The mouse input values are input directly to the game, but when
// the values exceed the value of mouse_threshold, they are multiplied
// by mouse_acceleration to increase the speed.

float mouse_acceleration = 2.0;
int mouse_threshold = 10;

// Gamma correction level to use

int usegamma = 0;

typedef struct
{
	byte r;
	byte g;
	byte b;
} col_t;

// Palette converted to RGB565

static uint16_t rgb565_palette[256];

void cmap_to_rgb565(uint16_t * out, uint8_t * in, int in_pixels)
{
    int i, j;
    struct color c;
    uint16_t r, g, b;

    for (i = 0; i < in_pixels; i++)
    {
        c = colors[*in]; 
        r = ((uint16_t)(c.r >> 3)) << 11;
        g = ((uint16_t)(c.g >> 2)) << 5;
        b = ((uint16_t)(c.b >> 3)) << 0;
        *out = (r | g | b);

        in++;
        for (j = 0; j < fb_scaling; j++) {
            out++;
        }
    }
}

void cmap_to_fb(uint8_t * out, uint8_t * in, int in_pixels)
{

// FFF

    // int i, j, k;
    // struct color c;
    // uint32_t pix;
    // uint16_t r, g, b;

    // for (i = 0; i < in_pixels; i++)
    // {
    //     c = colors[*in];  /* R:8 G:8 B:8 format! */
    //     r = (uint16_t)(c.r >> (8 - fb.red.length));
    //     g = (uint16_t)(c.g >> (8 - fb.green.length));
    //     b = (uint16_t)(c.b >> (8 - fb.blue.length));
    //     pix = r << fb.red.offset;
    //     pix |= g << fb.green.offset;
    //     pix |= b << fb.blue.offset;

    //     for (k = 0; k < fb_scaling; k++) {
    //         for (j = 0; j < fb.bits_per_pixel/8; j++) {
    //             *out = (pix >> (j*8));
    //             out++;
    //         }
    //     }
    //     in++;
    // }
}


void I_InitGraphics (void)
{
    int i;

    /* Open fbdev file descriptor */
    // fd_fb = open("/dev/fb0", O_RDWR);
    // if (fd_fb < 0)
    // {
    //     printf("Could not open /dev/fb0");
    //     exit(-1);
    // }

    /* fetch framebuffer info */
    // ioctl(fd_fb, FBIOGET_VSCREENINFO, &fb);
    /* change params if needed */
    //ioctl(fd_fb, FBIOPUT_VSCREENINFO, &fb);
    // printf("I_InitGraphics: framebuffer: x_res: %d, y_res: %d, x_virtual: %d, y_virtual: %d, bpp: %d, grayscale: %d\n",
    //         fb.xres, fb.yres, fb.xres_virtual, fb.yres_virtual, fb.bits_per_pixel, fb.grayscale);

    // printf("I_InitGraphics: framebuffer: RGBA: %d%d%d%d, red_off: %d, green_off: %d, blue_off: %d, transp_off: %d\n",
    //         fb.red.length, fb.green.length, fb.blue.length, fb.transp.length, fb.red.offset, fb.green.offset, fb.blue.offset, fb.transp.offset);

    printf("I_InitGraphics: DOOM screen size: w x h: %d x %d\n", SCREENWIDTH, SCREENHEIGHT);
    rm_init_framebuffer(&fb);
    printf("I_InitGraphics: epaper screen size: w x h: %d x %d depth: %d bytesPerLine: %d\n", fb.width, fb.height, fb.depth, fb.bytesPerLine);

    // i = M_CheckParmWithArgs("-scaling", 1);
    // if (i > 0) {
    //     i = atoi(myargv[i + 1]);
    //     fb_scaling = i;
    //     printf("I_InitGraphics: Scaling factor: %d\n", fb_scaling);
    // } else {
    //     fb_scaling = fb.xres / SCREENWIDTH;
    //     if (fb.yres / SCREENHEIGHT < fb_scaling)
    //         fb_scaling = fb.yres / SCREENHEIGHT;
    //     printf("I_InitGraphics: Auto-scaling factor: %d\n", fb_scaling);
    // }

    /* Allocate screen to draw to */
	I_VideoBuffer = (byte*)Z_Malloc (SCREENWIDTH * SCREENHEIGHT, PU_STATIC, NULL);  // For DOOM to draw on

	screenvisible = true;

    extern int I_InitInput(void);
    I_InitInput();
}

void I_ShutdownGraphics (void)
{
	Z_Free (I_VideoBuffer);
    rm_free_framebuffer(&fb);
}

void I_StartFrame (void)
{

}

__attribute__ ((weak)) void I_GetEvent (void)
{
//	event_t event;
//	bool button_state;
//
//	button_state = button_read ();
//
//	if (last_button_state != button_state)
//	{
//		last_button_state = button_state;
//
//		event.type = last_button_state ? ev_keydown : ev_keyup;
//		event.data1 = KEY_FIRE;
//		event.data2 = -1;
//		event.data3 = -1;
//
//		D_PostEvent (&event);
//	}
//
//	touch_main ();
//
//	if ((touch_state.x != last_touch_state.x) || (touch_state.y != last_touch_state.y) || (touch_state.status != last_touch_state.status))
//	{
//		last_touch_state = touch_state;
//
//		event.type = (touch_state.status == TOUCH_PRESSED) ? ev_keydown : ev_keyup;
//		event.data1 = -1;
//		event.data2 = -1;
//		event.data3 = -1;
//
//		if ((touch_state.x > 49)
//		 && (touch_state.x < 72)
//		 && (touch_state.y > 104)
//		 && (touch_state.y < 143))
//		{
//			// select weapon
//			if (touch_state.x < 60)
//			{
//				// lower row (5-7)
//				if (touch_state.y < 119)
//				{
//					event.data1 = '5';
//				}
//				else if (touch_state.y < 131)
//				{
//					event.data1 = '6';
//				}
//				else
//				{
//					event.data1 = '1';
//				}
//			}
//			else
//			{
//				// upper row (2-4)
//				if (touch_state.y < 119)
//				{
//					event.data1 = '2';
//				}
//				else if (touch_state.y < 131)
//				{
//					event.data1 = '3';
//				}
//				else
//				{
//					event.data1 = '4';
//				}
//			}
//		}
//		else if (touch_state.x < 40)
//		{
//			// button bar at bottom screen
//			if (touch_state.y < 40)
//			{
//				// enter
//				event.data1 = KEY_ENTER;
//			}
//			else if (touch_state.y < 80)
//			{
//				// escape
//				event.data1 = KEY_ESCAPE;
//			}
//			else if (touch_state.y < 120)
//			{
//				// use
//				event.data1 = KEY_USE;
//			}
//			else if (touch_state.y < 160)
//			{
//				// map
//				event.data1 = KEY_TAB;
//			}
//			else if (touch_state.y < 200)
//			{
//				// pause
//				event.data1 = KEY_PAUSE;
//			}
//			else if (touch_state.y < 240)
//			{
//				// toggle run
//				if (touch_state.status == TOUCH_PRESSED)
//				{
//					run = !run;
//
//					event.data1 = KEY_RSHIFT;
//
//					if (run)
//					{
//						event.type = ev_keydown;
//					}
//					else
//					{
//						event.type = ev_keyup;
//					}
//				}
//				else
//				{
//					return;
//				}
//			}
//			else if (touch_state.y < 280)
//			{
//				// save
//				event.data1 = KEY_F2;
//			}
//			else if (touch_state.y < 320)
//			{
//				// load
//				event.data1 = KEY_F3;
//			}
//		}
//		else
//		{
//			// movement/menu navigation
//			if (touch_state.x < 100)
//			{
//				if (touch_state.y < 100)
//				{
//					event.data1 = KEY_STRAFE_L;
//				}
//				else if (touch_state.y < 220)
//				{
//					event.data1 = KEY_DOWNARROW;
//				}
//				else
//				{
//					event.data1 = KEY_STRAFE_R;
//				}
//			}
//			else if (touch_state.x < 180)
//			{
//				if (touch_state.y < 160)
//				{
//					event.data1 = KEY_LEFTARROW;
//				}
//				else
//				{
//					event.data1 = KEY_RIGHTARROW;
//				}
//			}
//			else
//			{
//				event.data1 = KEY_UPARROW;
//			}
//		}
//
//		D_PostEvent (&event);
//	}
}

__attribute__ ((weak)) void I_StartTic (void)
{
	I_GetEvent();
}

void I_UpdateNoBlit (void)
{
}

//
// I_FinishUpdate
//


#define GFX_RGB565(r, g, b)			((((r & 0xF8) >> 3) << 11) | (((g & 0xFC) >> 2) << 5) | ((b & 0xF8) >> 3))
#define GFX_RGB565_R(color)			((0xF800 & color) >> 11)
#define GFX_RGB565_G(color)			((0x07E0 & color) >> 5)
#define GFX_RGB565_B(color)			(0x001F & color)


void drawPixelScaled(int scale, int xoffset, int yoffset, int x, int y, uint8_t red, uint8_t green, uint8_t blue, uint8_t transparency)
{    
    for (int y_scale = 0; y_scale < scale; ++y_scale) {
        for (int x_scale = 0; x_scale < scale; ++x_scale) {
            // BGR?
            *(fb.fb + (xoffset + x * scale + x_scale) * (fb.depth / 8) + (yoffset + y * scale + y_scale) * fb.bytesPerLine + 0) = blue;
            *(fb.fb + (xoffset + x * scale + x_scale) * (fb.depth / 8) + (yoffset + y * scale + y_scale) * fb.bytesPerLine + 1) = green;
            *(fb.fb + (xoffset + x * scale + x_scale) * (fb.depth / 8) + (yoffset + y * scale + y_scale) * fb.bytesPerLine + 2) = red;
            // alpha needs no update *(fb.fb + (xoffset + x * scale + x_scale) * (fb.depth / 8) + (yoffset + y * scale + y_scale) * fb.bytesPerLine + 3) = 0;
        }
    }
}


void I_FinishUpdate (void)
{
    unsigned char *line_in;

    int scale = 6;

    // Rotating 90 degrees makes a lot of sense. We rotate so that the type folio could be used.
    // x is rotated, so really height, y is width
    int xoffset = (fb.width - 200 * scale)/2;
    int yoffset = (fb.height - 320 * scale)/2;

    /* the next frame is in the buffer, now we need to draw it to
       the rm_framebuffer */
    line_in  = (unsigned char *) I_VideoBuffer;

    for (int y = 0; y < SCREENHEIGHT; ++y) {
        for (int x = 0; x < SCREENWIDTH; ++x) {
            char color_8bit = line_in[x + (y * SCREENWIDTH)];
            struct color c = colors[color_8bit]; 


            /* Rotation and translation:
                x = - y + 200
                y = x
            */

            int x_rotated = - y + 200;
            int y_rotated = x;

            drawPixelScaled(scale, xoffset, yoffset, x_rotated, y_rotated, c.r,  c.g, c.b, 0);
        }
	}

    const int pen_mode = 0;       // colored, we violate the pre-conditions (no flipping while waveform is running)
    const int mono_mode = 1;      // uhhh, mono, looks horrible
    const int animation_mode = 2; // grayscale, fast, lots of ghosting
    const int ui_mode = 3;        // looks good, way too slow, horrible framerate
    const int content_mode = 4;   // good looking, extra slow

    rm_framebuffer_update(&fb, xoffset, yoffset, fb.width - xoffset, fb.height - yoffset, animation_mode, 0);
}

//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, I_VideoBuffer, SCREENWIDTH * SCREENHEIGHT);
}

//
// I_SetPalette
//
void I_SetPalette (byte* palette)
{
	int i;
	//col_t* c;

	//for (i = 0; i < 256; i++)
	//{
	//	c = (col_t*)palette;

	//	rgb565_palette[i] = GFX_RGB565(gammatable[usegamma][c->r],
	//								   gammatable[usegamma][c->g],
	//								   gammatable[usegamma][c->b]);

	//	palette += 3;
	//}
    

    /* performance boost:
     * map to the right pixel format over here! */

    for (i=0; i<256; ++i ) {
        colors[i].a = 0;
        colors[i].r = gammatable[usegamma][*palette++];
        colors[i].g = gammatable[usegamma][*palette++];
        colors[i].b = gammatable[usegamma][*palette++];
    }

    /* Set new color map in kernel framebuffer driver */
    //XXX FIXME ioctl(fd_fb, IOCTL_FB_PUTCMAP, colors);
}

// Given an RGB value, find the closest matching palette index.

int I_GetPaletteIndex (int r, int g, int b)
{
    int best, best_diff, diff;
    int i;
    col_t color;

    printf("I_GetPaletteIndex\n");

    best = 0;
    best_diff = INT_MAX;

    for (i = 0; i < 256; ++i)
    {
    	color.r = GFX_RGB565_R(rgb565_palette[i]);
    	color.g = GFX_RGB565_G(rgb565_palette[i]);
    	color.b = GFX_RGB565_B(rgb565_palette[i]);

        diff = (r - color.r) * (r - color.r)
             + (g - color.g) * (g - color.g)
             + (b - color.b) * (b - color.b);

        if (diff < best_diff)
        {
            best = i;
            best_diff = diff;
        }

        if (diff == 0)
        {
            break;
        }
    }

    return best;
}

void I_BeginRead (void)
{
}

void I_EndRead (void)
{
}

void I_SetWindowTitle (char *title)
{
}

void I_GraphicsCheckCommandLine (void)
{
}

void I_SetGrabMouseCallback (grabmouse_callback_t func)
{
}

void I_EnableLoadingDisk(void)
{
}

void I_BindVideoVariables (void)
{
}

void I_DisplayFPSDots (boolean dots_on)
{
}

void I_CheckIsScreensaver (void)
{
}
