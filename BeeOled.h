/* 
 
    BEE Board 4Display Libray
 
    Copyright(c) February 2011 Marco Antonini
 
    for more info:
    beeboard.madefree.eu -  www.acmesystems.it
 
    This library is Designed to work with BeeBoard Arduino Compatible Board
    See www.acmesystems.it for more info
 
    Based on Code: Oscar Gonzalez December 2010
 
    Licensed under GNU General Public License v3 
    http://creativecommons.org/licenses/by/3.0/

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
 
    Compatibility Support
 
    * Work on Arduino IDE 23 or less (Hardware Serial or NewSoftwareSerial)
    * Work on Arduino IDE 1.0        (Hardware Serial or SoftwareSerial)
 
*/

/********************* OPTIONS *******************/

/** Select Wings to Plug **/

//#define __W2__  // in Hardware Serial Mode (no HW LCD reset)
//#define __W3__    // in SoftwareSerial Mode (with HW LCD reset)
#define __W4__  // in SoftwareSerial Mode (with HW LCD reset)

/**************************************************/

#ifdef __W2__
    #define __HARDWARE_SERIAL__
#else if defined(__W3__) || defined(__W4__)
    #define __SOFTWARE_SERIAL__
#endif
 
#ifndef _DISPLAYSHIELD4D_H_
#define _DISPLAYSHIELD4D_H_

#ifdef __SOFTWARE_SERIAL__

    #if defined(ARDUINO) && ARDUINO >= 100
        #include <SoftwareSerial.h>
    #else
        #include <NewSoftSerial.h>
    #endif
    #define __SOFTWARE_SERIAL_TIMEOUT__ 120
#endif

	#include <inttypes.h>

    #ifdef __W4__
        #define OLED_RESETPIN			4 
    #else if __W3__
        #define OLED_RESETPIN			9
    #endif

	#define OLED_BAUDRATE			9600 
	#define OLED_INITDELAYMS		1500

	#define	OLED_DETECT_BAUDRATE		0x55
	#define OLED_GETDEVICEINFO		0x56
	#define OLED_STRINGTERMINATOR		0

	#define	OLED_CLEAR			0x45

	#define OLED_SOLID			0
	#define OLED_WIREFRAME			1

	#define	OLED_COMMAND_CONTROL		0x59
	#define	OLED_COMMAND_DISPLAY_OFF	0x00
	#define	OLED_COMMAND_DISPLAY_ON		0x01
	#define	OLED_COMMAND_SHUTDOWN		0x00
	#define	OLED_COMMAND_POWEROFF		0x01
	#define OLED_COMMAND_SLEEP		0x5A
	#define OLED_COMMAND_STOP_SD		0x80
	#define OLED_COMMAND_WAKEONKOYSTICK	0x02
	#define OLED_COMMAND_WAKEONSERIAL	0x01
	#define OLED_SCREENCOPY			0x63

	#define OLED_ACK			0x06
	#define OLED_NAK 			0x15

	// Graphics
	#define	OLED_PUTPIXEL			0x50
	#define	OLED_READPIXEL			0x52
	#define	OLED_LINE			0x4C
	#define	OLED_SETBACKGROUND		0x42
	#define	OLED_SETPENSIZE			0x70
	#define	OLED_RECTANGLE			0x72
	#define	OLED_CIRCLE			0x43
	#define	OLED_TRIANGLE			0x47

	// Text
	#define	OLED_SETFONT			0x46
		#define	OLED_FONT5x7		0x00
		#define	OLED_FONT8x8		0x01
		#define	OLED_FONT8x12		0x02

	#define	OLED_SETFONTMODE		0x4F
		#define	OLED_FONT_TRANSPARENT	0x00
		#define	OLED_FONT_OPAQUE	0x01

	#define OLED_STRING_BLOCK		0x53

	// Class definition
	class BeeOled 
	{
		public:
			BeeOled();

			uint8_t Init();
			uint8_t Reset();
			uint8_t Clear();
			uint8_t GetReply();			
			/*
			char *GetDeviceType();
			uint8_t GetDeviceWidth();
			uint8_t GetDeviceHeight();
			*/
			unsigned int RGB(uint8_t red, uint8_t green, uint8_t blue);
			uint8_t SetPenSize(char val);
			uint8_t SetBackground(unsigned int color);
			uint8_t SetContrast(char val);
			uint8_t SetState(char state);
			uint8_t Sleep(char wake_cond);

			// Utility
			uint8_t ScreenCopy(uint8_t source_x, uint8_t source_y, uint8_t dest_x, uint8_t dest_y, uint8_t width, uint8_t height);

			// Graphics functions
			uint8_t putpixel(uint8_t x, uint8_t y, unsigned int color);
			uint16_t readpixel(uint8_t x, uint8_t y);
			uint8_t line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, unsigned int color);
			uint8_t rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, char filled, unsigned int color);
			uint8_t circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t filled, unsigned int color);
			uint8_t triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t filled, unsigned int color);

			// Text functions
			uint8_t setfont(uint8_t font_type);
			uint8_t setfontmode(uint8_t font_mode);
			uint8_t drawstringblock(uint8_t x, uint8_t y, uint8_t font, unsigned int color, uint8_t width, uint8_t height, char *text);


		private:
			//void GetDeviceInfo();

			uint8_t	device_type;
			uint8_t	device_hardware_rev;
			uint8_t	device_firmware_rev;
			uint8_t	device_width;
			uint8_t	device_height;
#ifdef __SOFTWARE_SERIAL__
    #if defined(ARDUINO) && ARDUINO >= 100
            SoftwareSerial _oled;
    #else
            NewSoftSerial _oled;
    #endif
#endif
        
	};

#endif
