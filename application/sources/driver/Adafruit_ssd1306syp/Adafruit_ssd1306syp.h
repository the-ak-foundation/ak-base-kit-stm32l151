#ifndef __IIC_SSD1306SYP_H__
#define __IIC_SSD1306SYP_H__

#include <stdlib.h>
#include "Adafruit_GFX.h"

// #define SH1106_DRIVER_EN

#if defined (SH1106_DRIVER_EN)
#define SH1106_CMD

#define SH1106_SETCONTRAST 0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON 0xA5
#define SH1106_NORMALDISPLAY 0xA6
#define SH1106_INVERTDISPLAY 0xA7
#define SH1106_DISPLAYOFF 0xAE
#define SH1106_DISPLAYON 0xAF

#define SH1106_SETDISPLAYOFFSET 0xD3
#define SH1106_SETCOMPINS 0xDA

#define SH1106_SETVCOMDETECT 0xDB

#define SH1106_SETDISPLAYCLOCKDIV 0xD5
#define SH1106_SETPRECHARGE 0xD9

#define SH1106_SETMULTIPLEX 0xA8

#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10

#define SH1106_SETSTARTLINE 0x40

#define SH1106_MEMORYMODE 0x20
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR   0x22

#define SH1106_COMSCANINC 0xC0
#define SH1106_COMSCANDEC 0xC8

#define SH1106_SEGREMAP 0xA0

#define SH1106_CHARGEPUMP 0x8D

#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2
#else
#define SSD1306_CMD

#define SSD1306_CMD_DISPLAY_OFF 0xAE//--turn off the OLED
#define SSD1306_CMD_DISPLAY_ON 0xAF//--turn on oled panel
#endif

#define BLACK 0
#define WHITE 1

//common parameters
#define WIDTH 128
#define HEIGHT 64
#define FBSIZE 1024 //128x8
#define MAXROW 8

class Adafruit_ssd1306syp : public Adafruit_GFX{
public:
    Adafruit_ssd1306syp();
	~Adafruit_ssd1306syp();
	//initialized the ssd1306 in the setup function
	virtual bool initialize();

	//update the framebuffer to the screen.
    virtual void update();
	//totoally 8 rows on this screen in vertical direction.
	virtual void updateRow(int rowIndex);
	virtual void updateRow(int startRow, int endRow);
	
	//draw one pixel on the screen.
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

	//clear the screen
	void clear(bool isUpdateHW=false);

	//on screen
	void display_on();

	//off screen
	void display_off();

protected:
	//write one byte to the screen.
	void writeByte(unsigned char  b);
	void writeCommand(unsigned char  cmd);

	//atomic control function
	void startIIC();//turn on the IIC
	void stopIIC();//turn off the IIC.
	void startDataSequence();

	//
protected:
	int m_sda;
	int m_scl;
	int m_res;
	unsigned char* m_pFramebuffer;//the frame buffer for the adafruit gfx. size=64x8 bytes
};
#endif //__IIC_SSD1306SYP_H__
