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

#include "BeeOled.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
    #define OLEDWRITE(a) _oled.write(a);
#else
    #include "WProgram.h"
    #define OLEDWRITE(a) _oled.print(a,BYTE); // For Support Arduino-23 or less
#endif

#ifdef __SOFTWARE_SERIAL__
    #if defined(ARDUINO) && ARDUINO >= 100
        #include <SoftwareSerial.h>
    #else
        #include <NewSoftSerial.h>
    #endif
#endif

/*******************************************************
	Constructor
 ********************************************************/

#ifdef __SOFTWARE_SERIAL__

#ifdef __W4__
    BeeOled::BeeOled() : _oled(7,8) {
#else __W3__
    BeeOled::BeeOled() : _oled(12,11) {
#endif
    
	// Constructor    
    _oled.begin(9600); // 9600 more stability in SoftwareSerial
}

#else if __HARDWARE_SERIAL__

#define _oled Serial
BeeOled::BeeOled() {
	// Constructor    
}
#endif

/*******************************************************
	Function: 	
		Init
	Description:	
		Initialise display. Prior to this function
		you must start serial by _oled.begin()
		For example: _oled.begin(57600);
	Params:	None	
	Return:	0
 ********************************************************/
uint8_t BeeOled::Init()
{
	pinMode(OLED_RESETPIN, OUTPUT);
	Reset();  
	delay(OLED_INITDELAYMS);
	OLEDWRITE(OLED_DETECT_BAUDRATE); 
	delay(10);

	//GetDeviceInfo();

	return 0;
}

/*
void BeeOled::GetDeviceInfo()
{
	OLEDWRITE(OLED_GETDEVICEINFO);
	OLEDWRITE(OLED_GETDEVICEINFO_SERIAL);

	byte incomingByte;
	char counter=0;
	while (!_oled.available()) { delayMicroseconds(150); }
	
	do 
	{
		incomingByte = _oled.read();

		switch (counter)
		{
			case 0: { device_type 		= incomingByte; }; break;
			case 1: { device_hardware_rev 	= incomingByte; }; break;
			case 2: { device_firmware_rev 	= incomingByte; }; break;
			case 3: { device_width 		= incomingByte; }; break;
			case 4: { device_height 	= incomingByte; }; break;
		}

		counter++;
		

	} while (_oled.available());		
	
}

char *BeeOled::GetDeviceType()
{
	switch (device_type)
	{
		case 0x00: { return "micro-OLED"; }; break;
		case 0x01: { return "micro-LCD"; }; break;
		case 0x02: { return "micro-VGA"; }; break;
	}
}

uint8_t BeeOled::GetDeviceWidth()
{
	return device_width;
}

uint8_t BeeOled::GetDeviceHeight()
{
	return device_height;
}
*/
/*******************************************************
	Function:	
		Reset 
	Description:
		Phisicaly reset the display. D7 jumper must be conected
		Also is a good thing to have pinMode(7, OUTPUT) set in Setup function
	Params:	None
	Return: No return
 ********************************************************/
uint8_t BeeOled::Reset() {

	digitalWrite(OLED_RESETPIN, LOW);
	delay(10);                  
	digitalWrite(OLED_RESETPIN, HIGH);
	delay(10); 
}

/*******************************************************
	Function: 
		Clear
	Description:
		Clear entire display
	Params:	None
	Return: Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::Clear() {
	OLEDWRITE(OLED_CLEAR);
	return GetReply();
}

/*******************************************************
	Function: 
		GetReply
	Description:
		Used to get command response
	Params:	None
	Return: Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::GetReply() {
	byte incomingByte = OLED_ACK;
#ifdef __SOFTWARE_SERIAL__
   long timeNow=millis();
    boolean debug=false;
    while(millis() < (timeNow + __SOFTWARE_SERIAL_TIMEOUT__) ){
        incomingByte = _oled.read();
        if(incomingByte == OLED_ACK || incomingByte == OLED_NAK )
            break;
    }
    
#else if __HARDWARE_SERIAL__
	while (!_oled.available()) { delayMicroseconds(150); }
	incomingByte = _oled.read();
#endif
    return incomingByte;

}

/*******************************************************
	Function: 
		RGB
	Description:
		Returns correct int format for color parm
	Params:	red, green, blue - From 0 to 254
	Return:	None
 ********************************************************/
unsigned int BeeOled::RGB(uint8_t red, uint8_t green, uint8_t blue) 
{

	char text[255];

	int outR = ((red * 31) / 255);
	int outG = ((green * 63) / 255);
	int outB = ((blue * 31) / 255);

	return (outR << 11) | (outG << 5) | outB;

}

/*******************************************************
	Function: 
		SetPenSize
	Description:
		This  command  determines  if  certain graphics  objects  are drawn in solid or  wire frame fashion.
	Params:	0 for solid or 1 for wireframe
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::SetPenSize(char val)
{
	OLEDWRITE(OLED_SETPENSIZE);
	OLEDWRITE(val);

	return GetReply();
}

/*******************************************************
	Function: 
		SetBackground
	Description:
		changes the current background colour. Once this command is sent, only the background colour will change.

	Params:	color
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::SetBackground(unsigned int color)
{
	OLEDWRITE(OLED_SETBACKGROUND);

	// Color
	OLEDWRITE(color >> 8);				// MSB
	OLEDWRITE(color & 0xFF);				// LSB

	return GetReply();
}

/*******************************************************
	Function: 
		SetContrast
	Description:
		Set current contrast

	Params:	cval - 0 to 15
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::SetContrast(char val)
{
	OLEDWRITE(OLED_COMMAND_CONTROL);

	// Set contrast
	OLEDWRITE(val); // 0-15

	return GetReply();
}

/*******************************************************
	Function: 
		SetState
	Description:
		This command changes some of the display settings

	Params:	state - Can be COMMAND_DISPLAY_ON, COMMAND_DISPLAY_OFF, OLED_COMMAND_SHUTDOWN, OLED_COMMAND_POWEROFF (for low power save state)
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::SetState(char state)
{
	OLEDWRITE(OLED_COMMAND_CONTROL);

	// Set contrast
	OLEDWRITE(state);

	return GetReply();
}

/*******************************************************
	Function: 
		SetState
	Description:
		Puts GOLDELOX-SGC chip in to low power mode and optionally waits for certain conditions to wake it up

	Params:	wake_cond - Can be OLED_COMMAND_STOP_SD, OLED_COMMAND_WAKEONKOYSTICK or OLED_COMMAND_WAKEONSERIAL
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::Sleep(char wake_cond)
{
	OLEDWRITE(OLED_COMMAND_SLEEP);

	// Set wakeup condition
	OLEDWRITE(wake_cond);

	return GetReply();
}

/*******************************************************
	Function: 
		ScreenCopy
	Description:
		This command copies a specified area of the screen as a bitmap block to another screen position

	Params:	source_x, source_y, dest_x, dest_y, width and height
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::ScreenCopy(uint8_t source_x, uint8_t source_y, uint8_t dest_x, uint8_t dest_y, uint8_t width, uint8_t height)
{

	OLEDWRITE(OLED_SCREENCOPY);

	OLEDWRITE(source_x);
	OLEDWRITE(source_y);
	OLEDWRITE(dest_x);
	OLEDWRITE(dest_y);
	OLEDWRITE(width);
	OLEDWRITE(height);

	return GetReply();

}

/*******************************************************
	Function: 
		putpixel
	Description:
		Basic function to draw one pixel
	Params:
		x, y and color. Please check your screen size in order to use this function!
	Return: Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::putpixel(uint8_t x, uint8_t y, unsigned int color)
{
	OLEDWRITE(OLED_PUTPIXEL);
	OLEDWRITE(x);
	OLEDWRITE(y);

	// Color
	OLEDWRITE(color >> 8);				// MSB			
	OLEDWRITE(color & 0xFF);				// LSB

    	return GetReply();
}

/*******************************************************
	Function: 
		readpixel
	Description:
		Basic function to get the color of one pixel
	Params:
	Return: Returns 16bits color info (MSB/LSB)
 ********************************************************/
uint16_t BeeOled::readpixel(uint8_t x, uint8_t y)
{
	OLEDWRITE(OLED_READPIXEL);
	OLEDWRITE(x);
	OLEDWRITE(y);

	while (!_oled.available()) { delayMicroseconds(150); } // Let's display take the time...

    	return _oled.read() & _oled.read();
}

/*******************************************************
	Function: 
		line
	Description:
	Params:
		x1, y1, x2, y2 and color
		As allways, take care of your screen size!
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, unsigned int color)
{

	OLEDWRITE(OLED_LINE);

	OLEDWRITE(x1);						// X1
	OLEDWRITE(y1);						// Y1
	OLEDWRITE(x2);						// X2
	OLEDWRITE(y2);						// Y2

	// Color
	OLEDWRITE(color >> 8);					// MSB			
	OLEDWRITE(color & 0xFF);					// LSB

	return GetReply();

}

/*******************************************************
	Function: 
		rectangle
	Description:
	Params:
		x, y, width, height, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		As allways, take care of your screen size!
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, char filled, unsigned int color)
{

	SetPenSize(filled);

	OLEDWRITE(OLED_RECTANGLE);

	OLEDWRITE(x);						// X1
	OLEDWRITE(y);						// Y1
	OLEDWRITE(x+width);						// X2
	OLEDWRITE(y+height);						// Y2

	// Color
	OLEDWRITE(color >> 8);					// MSB			
	OLEDWRITE(color & 0xFF);					// LSB

	return GetReply();

}

/*******************************************************
	Function: 
		circle
	Description:
	Params:
		x, y, radius, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		As allways, take care of your screen size!
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t filled, unsigned int color)
{
	SetPenSize(filled);

    OLEDWRITE(OLED_CIRCLE); 

	OLEDWRITE(x);	// x
	OLEDWRITE(y);	// y
	OLEDWRITE(radius);	// radius

	// Color
	OLEDWRITE(color >> 8);		// MSB			
	OLEDWRITE(color & 0xFF);		// LSB

        return GetReply();

}

/*******************************************************
	Function: 
		triangle
	Description:
	Params:
		x and y of each vertex, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		The vertices must be specified in an anticlock wise
	Return:	Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t filled, unsigned int color)
{

	SetPenSize(filled);

    OLEDWRITE(OLED_TRIANGLE); 

	OLEDWRITE(x1);	// x
	OLEDWRITE(y1);	// y
	OLEDWRITE(x2);	// x
	OLEDWRITE(y2);	// y
	OLEDWRITE(x3);	// x
	OLEDWRITE(y3);	// y

	// Color
	OLEDWRITE(color >> 8);		// MSB			
	OLEDWRITE(color & 0xFF);		// LSB

        return GetReply();

}

/*******************************************************
	Function: 
		setfont
	Description:
		This  command selects  one of  the available internal  fonts.
	Params:
		font_type: OLED_FONT5x7, OLED_FONT8x8 or OLED_FONT8x12
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::setfont(uint8_t font_type)
{
    OLEDWRITE(OLED_SETFONT); 
	OLEDWRITE(font_type);

        return GetReply();
}

/*******************************************************
	Function: 
		setfontmode
	Description:
		This command will  change the attribute of the text so that an object behind the text can either be blocked or transparent.
	Params:
		font_mode: OLED_FONT_TRANSPARENT or OLED_FONT_OPAQUE
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::setfontmode(uint8_t font_mode)
{
    OLEDWRITE(OLED_SETFONTMODE); 
	OLEDWRITE(font_mode);

        return GetReply();
}

/*******************************************************
	Function: 
		drawstringblock
	Description:
		This command will  change the attribute of the text so that an object behind the text can either be blocked or transparent.
	Params:
		font_mode: OLED_FONT_TRANSPARENT or OLED_FONT_OPAQUE
	Return:	
		Return OLED_ACK is done or OLED_NAK if not
 ********************************************************/
uint8_t BeeOled::drawstringblock(uint8_t x, uint8_t y, uint8_t font, unsigned int color, uint8_t width, uint8_t height, char *text)
{

	OLEDWRITE(OLED_STRING_BLOCK);

	OLEDWRITE(x);
	OLEDWRITE(y);
	OLEDWRITE(font);

	// Color
	OLEDWRITE(color >> 8);		// MSB			
	OLEDWRITE(color & 0xFF);		// LSB

	OLEDWRITE(width);
	OLEDWRITE(height);

	for (int i=0 ; i<strlen(text) ; i++)
	{
		OLEDWRITE(text[i]);
	}

	//OLEDWRITE(OLED_STRINGTERMINATOR); // String terminator

	return GetReply();
}

