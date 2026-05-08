#include<lpc21xx.h>

#define LCD_D 0xFF<<0
#define RS 1<<8
#define E 1<<9

#define INIT LCD_INIT
#define CMD  LCD_COMMAND
#define DATA LCD_DATA

void STR(unsigned char*,unsigned char);
void delay(unsigned int);
void LCD_INIT(void);
void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);

int main()
{
	unsigned char pos=0;
	INIT();
	while(1)
	{
		CMD(0xc0 + pos++);
		STR("HELLO",pos);
		delay(300);
		CMD(0x01);
		if(pos==16)
			pos=0;
	}
}
void STR(unsigned char*s,unsigned char pos)
{
	int i=0;
	while(s[i])
	{
	DATA(s[i++]);
	if(pos>=16)
	CMD(0xc0);
	pos++;
}
}
void delay( unsigned int ms)
{
	T0PR =15000-1;
	T0TCR =0X01;
	while(T0TC<ms);
	T0TCR =0X03;
	T0TCR =0X00;
}
void LCD_INIT(void)
{
	IODIR0 |= LCD_D|RS|E;//P0.0 to P0.9 are configured as OUTPUT pins
	LCD_COMMAND(0X01);//To clear the display
  LCD_COMMAND(0X02);//set cursor to home position
	LCD_COMMAND(0X0C);//Display ON cursor OFF
	LCD_COMMAND(0X38);//8bit interface mode with both lines 
}
void LCD_COMMAND(unsigned char cmd)
{
	IOCLR0 = LCD_D;//clear the data pins 
	IOSET0 = cmd;//assign the command byte to the data pins
	IOCLR0 = RS;//select command register
	IOSET0 = E;//copy the data from datapins to the LCD
	delay(2);
	IOCLR0 = E;//To re-latch the next byte
}
void LCD_DATA(unsigned char d)
{
	IOCLR0 = LCD_D;//clear the data pins 
	IOSET0 = d;//assign the data byte to the data pins
	IOSET0 = RS;//select data register
	IOSET0 = E;//copy the data from datapins to the LCD
	delay(2);
	IOCLR0 = E;//To re-latch the next byte
}
