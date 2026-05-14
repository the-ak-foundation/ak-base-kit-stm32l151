#include "Adafruit_ssd1306syp.h"

#if defined (STM32L_PLATFORM)
#include "Arduino.h"
#include "io_cfg.h"
#endif

static unsigned char frame_buffer[FBSIZE];

Adafruit_ssd1306syp::Adafruit_ssd1306syp():
	Adafruit_GFX(WIDTH,HEIGHT)
{
	m_sda = SSD1306_DATA_PIN;
	m_scl = SSD1306_CLK_PIN;
	m_pFramebuffer = 0;
}
Adafruit_ssd1306syp::~Adafruit_ssd1306syp()
{

}
//initialized the ssd1306 in the setup function
bool Adafruit_ssd1306syp::initialize()
{
	//setup the pin mode
	pinMode(m_sda,OUTPUT);
	pinMode(m_scl,OUTPUT);

	//malloc the framebuffer.
	m_pFramebuffer = frame_buffer;
	if(m_pFramebuffer == 0){
		return false;
	}
	memset(m_pFramebuffer,0,FBSIZE);//clear it.

	//write command to the screen registers.
#if defined (SH1106_CMD)
	writeCommand(SH1106_DISPLAYOFF);                    // 0xAE
	writeCommand(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
	writeCommand(0x80);                                  // the suggested ratio 0x80
	writeCommand(SH1106_SETMULTIPLEX);                  // 0xA8
	writeCommand(0x3F);
	writeCommand(SH1106_SETDISPLAYOFFSET);              // 0xD3
	writeCommand(0x00);                                   // no offset

	writeCommand(SH1106_SETSTARTLINE | 0x0);            // line #0 0x40
	writeCommand(SH1106_CHARGEPUMP);                    // 0x8D
	writeCommand(0x14);
	writeCommand(SH1106_MEMORYMODE);                    // 0x20
	writeCommand(0x00);                                  // 0x0 act like ks0108
	writeCommand(SH1106_SEGREMAP | 0x1);
	writeCommand(SH1106_COMSCANDEC);
	writeCommand(SH1106_SETCOMPINS);                    // 0xDA
	writeCommand(0x12);
	writeCommand(SH1106_SETCONTRAST);                   // 0x81
	writeCommand(0xCF);
	writeCommand(SH1106_SETPRECHARGE);                  // 0xd9
	writeCommand(0xF1);
	writeCommand(SH1106_SETVCOMDETECT);                 // 0xDB
	writeCommand(0x40);
	writeCommand(SH1106_DISPLAYALLON_RESUME);           // 0xA4
	writeCommand(SH1106_NORMALDISPLAY);                 // 0xA6
	writeCommand(SH1106_DISPLAYON);//--turn on oled panel
#else
	writeCommand(SSD1306_CMD_DISPLAY_OFF);//display off
	writeCommand(0x00);//Set Memory Addressing Mode
	writeCommand(0x10);//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	writeCommand(0x40);//Set Page Start Address for Page Addressing Mode,0-7
	writeCommand(0xB0);//Set COM Output Scan Direction
	writeCommand(0x81);//---set low column address
	writeCommand(0xCF);//---set high column address
	writeCommand(0xA1);//--set start line address
	writeCommand(0xA6);//--set contrast control register
	writeCommand(0xA8);
	writeCommand(0x3F);//--set segment re-map 0 to 127
	writeCommand(0xC8);//--set normal display
	writeCommand(0xD3);//--set multiplex ratio(1 to 64)
	writeCommand(0x00);//
	writeCommand(0xD5);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	writeCommand(0x80);//-set display offset
	writeCommand(0xD9);//-not offset
	writeCommand(0xF1);//--set display clock divide ratio/oscillator frequency
	writeCommand(0xDA);//--set divide ratio
	writeCommand(0x12);//--set pre-charge period
	writeCommand(0xDB);//
	writeCommand(0x40);//--set com pins hardware configuration
	writeCommand(0x8D);//--set vcomh
	writeCommand(0x14);//0x20,0.77xVcc
	writeCommand(0xAF);//--set DC-DC enable

	writeCommand(SSD1306_CMD_DISPLAY_ON);//--turn on oled panel

	delay(10);//wait for the screen loaded.
#endif

	return true;
}
void Adafruit_ssd1306syp::clear(bool isUpdateHW)
{
	memset(m_pFramebuffer,0,FBSIZE);//clear the back buffer.
	if(isUpdateHW) update();//update the hw immediately
}

void Adafruit_ssd1306syp::display_on()
{
#if defined (SH1106_CMD)
	writeCommand(SH1106_DISPLAYON);
#else
	writeCommand(SSD1306_CMD_DISPLAY_ON);
#endif
}

void Adafruit_ssd1306syp::display_off()
{
#if defined (SH1106_CMD)
	writeCommand(SH1106_DISPLAYOFF);
#else
	writeCommand(SSD1306_CMD_DISPLAY_OFF);
#endif
}

void Adafruit_ssd1306syp::writeCommand(unsigned char cmd)
{
	startIIC();
	writeByte(0x78);  //Slave address,SA0=0
	writeByte(0x00);	//write command
	writeByte(cmd);
	stopIIC();
}
void Adafruit_ssd1306syp::writeByte(unsigned char b)
{
	unsigned char i;
#if defined (SH1106_CMD)
	for(i=0;i<8;i++)
	{
		if((b << i) & 0x80){
			digitalWrite(m_sda, HIGH);
		}else{
			digitalWrite(m_sda, LOW);
		}
		digitalWrite(m_scl, HIGH);
		digitalWrite(m_scl, LOW);
	}
	digitalWrite(m_sda, LOW);
	digitalWrite(m_scl, HIGH);

	digitalWrite(m_scl, LOW);
#else
	for(i=0;i<8;i++)
	{
		if((b << i) & 0x80){
			digitalWrite(m_sda, HIGH);
		}else{
			digitalWrite(m_sda, LOW);
		}
		digitalWrite(m_scl, HIGH);
		digitalWrite(m_scl, LOW);
		//    IIC_Byte<<=1;
	}
	digitalWrite(m_sda, HIGH);
	digitalWrite(m_scl, HIGH);
	
	digitalWrite(m_scl, LOW);
#endif
}
void Adafruit_ssd1306syp::startIIC()
{
	digitalWrite(m_scl, HIGH);
	digitalWrite(m_sda, HIGH);
	digitalWrite(m_sda, LOW);
	digitalWrite(m_scl, LOW);
}
void Adafruit_ssd1306syp::stopIIC()
{
	digitalWrite(m_scl, LOW);
	digitalWrite(m_sda, LOW);
	digitalWrite(m_scl, HIGH);
	digitalWrite(m_sda, HIGH);
}

void Adafruit_ssd1306syp::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	unsigned char row;
	unsigned char offset;
	unsigned char  preData;//previous data.
	unsigned char val;
	int16_t  index;

	if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()) || ( m_pFramebuffer==0))	return;

	//get the previous data;
	row = y/8;
	offset =y%8;
	index = row*width() + x;
	preData = m_pFramebuffer[index];

	//set pixel;
	val = 1<<offset;
	if(color!=0)
	{//white! set bit.
		m_pFramebuffer[index] = preData | val;
	}else
	{//black! clear bit.
		m_pFramebuffer[index] = preData & (~val);
	}

}

void Adafruit_ssd1306syp::startDataSequence()
{
	startIIC();
	writeByte(0x78);
	writeByte(0x40);	//write data
}

void Adafruit_ssd1306syp::update()
{
#if defined (SH1106_CMD)
	writeCommand(SH1106_SETLOWCOLUMN | 0x0);  // low col = 0
	writeCommand(SH1106_SETHIGHCOLUMN | 0x0);  // hi col = 0
	writeCommand(SH1106_SETSTARTLINE | 0x0); // line #0

	byte height=64;
	byte width=132;
	byte m_row = 0;
	byte m_col = 2;

	height >>= 3;
	width >>= 3;

	int p = 0;
	byte i, j, k =0;

	for ( i = 0; i < height; i++) {
		// send a bunch of data in one xmission
		writeCommand(0xB0 + i + m_row);//set page address
		writeCommand(m_col & 0xf);//set lower column address
		writeCommand(0x10 | (m_col >> 4));//set higher column address

		for( j = 0; j < 8; j++){
			startDataSequence();
			for (k = 0; k < width; k++, p++) {
				writeByte(m_pFramebuffer[p]);
			}
			stopIIC ();
		}
	}
#else
#if 1
	unsigned int  i=0;
	unsigned int m,n;
	for(m=0;m<8;m++)
	{
		writeCommand(0xb0+m);	//page0-page1
		writeCommand(0x00);		//low column start address
		writeCommand(0x10);		//high column start address

		startDataSequence();
		for(n=0;n<128;n++)
		{
			writeByte(m_pFramebuffer[i++]);
		}
		stopIIC();
	}
#else
	updateRow(0,SSD1306_MAXROW);
#endif

#endif
}

void Adafruit_ssd1306syp::updateRow(int rowID)
{
	unsigned char x = 0;
	unsigned int  index = 0;
	if(rowID>=0 && rowID<MAXROW && m_pFramebuffer)
	{//this part is faster than else.
		//set the position
		startIIC();
		writeByte(0x78);  //Slave address,SA0=0
		writeByte(0x00);	//write command

		writeByte(0xb0+rowID);
		writeByte(((x&0xf0)>>4)|0x10);//|0x10
		writeByte((x&0x0f)|0x01);//|0x01

		stopIIC();

		//start painting the buffer.
		startDataSequence();
		for(x=0;x<WIDTH;x++)
		{
			index = rowID*WIDTH+x;
			writeByte(m_pFramebuffer[index]);
		}
		stopIIC();
	}
}
void Adafruit_ssd1306syp::updateRow(int startID, int endID)
{
	unsigned char y =0;
	for(y=startID; y<endID; y++)
	{
		updateRow(y);
	}
}
