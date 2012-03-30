/* fb.c */
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include "frame.h"

#define FB_DEVICE "/dev/fb0"

/*
*  init frame buffer
*/
int init_fb(fb_info *fb_inf)
{
    int fd = -1;

    if ((fd = open(FB_DEVICE, O_RDWR)) < 0)
    {
    	  fprintf(stderr, "Open %s failed:%s\n", FB_DEVICE, strerror(errno));
    	  return -1;
    }
    
    struct fb_var_screeninfo fb_var;
    
    if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_var) < 0)
    {
    	  fprintf(stderr, "fb ioctl failed:%s\n", strerror(errno));
    	  return -1;
    }
    
    fb_inf->w = fb_var.xres;
    fb_inf->h = fb_var.yres;
    fb_inf->bpp = fb_var.bits_per_pixel;
    fb_inf->fbmem = mmap(0, fb_inf->w * fb_inf->h * fb_inf->bpp /8,
    				             PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb_inf->fbmem == MAP_FAILED)
    {
    	  fprintf(stderr, "fb mmap failed:%s\n", strerror(errno));
    	  return -1;
    }
    
    close(fd);
    
    return 0;
}

