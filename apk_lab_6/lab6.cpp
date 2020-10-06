#include <dos.h>
#include <conio.h>
#include <stdio.h>

void interrupt NewInterrupt(...);
void interrupt(*oldInterrupt)(...);
void UpdateHighlightFlag(unsigned char);
void SetMask(unsigned char mask);
int commandIsExecuted;
int quitFlag;
int needHighlight;
typedef unsigned char byte;

struct VIDEO
{
	byte symb;
	byte attr;
};

const int defaultAttr = 0x70;							

void printRegister(int value)
{
	VIDEO far* screen = (VIDEO far*)MK_FP(0xB800, 0);
	for (int i = 1; i >= 0; i--)
	{
		int currBits = ((value >> (i * 4)) & 0x0F);
		if (currBits < 10)
		{
			screen->symb = currBits + '0';
		}
		else
		{
			screen->symb = currBits - 10 + 'A';
		}
		screen->attr = defaultAttr;
		screen++;
	}
}

void main()
{
	clrscr();
	commandIsExecuted = 0;
	quitFlag = 0;
	needHighlight = 0;
	oldInterrupt = getvect(9);
	setvect(9, NewInterrupt);
	while (!quitFlag)
	{
		if (needHighlight)
		{
			SetMask(0x04);                                      
			delay(100);
			SetMask(0x00);
			delay(100);
			SetMask(0x02);                                      
			delay(100);
			SetMask(0x00);
			delay(100);
			SetMask(0x04);                                     
			delay(100);
			SetMask(0x00);
			delay(100);
			SetMask(0x02);                                      
			delay(100);
			SetMask(0x00);
			delay(100);
			SetMask(0x04);
			delay(1000);
			SetMask(0x00);
			delay(2000);
			SetMask(0x02);
			delay(1000);
			SetMask(0x00);
			delay(2000);
			SetMask(0x04);
			delay(5000);
			SetMask(0x00);
			delay(1000);
			SetMask(0x02);
			delay(5000);
			SetMask(0x00);
			needHighlight = 0;
			clrscr();
		}
	}
	setvect(9, oldInterrupt);
	return;
}

void interrupt NewInterrupt(...)
{
	unsigned char value = 0;
	value = inp(0x60);
	if (value == 0x01)                                    
		quitFlag = 1;
	UpdateHighlightFlag(value);
	commandIsExecuted = commandIsExecuted || (needHighlight == 0) || (value == 0xFA);
	printRegister(inp(0x60));
	oldInterrupt();
}
void UpdateHighlightFlag(unsigned char value)
{
	if (value != 0x1c)                                   
		return;
	if (needHighlight)
		needHighlight = 0;
	else
		needHighlight = 1;                                
}

void SetMask(unsigned char mask)
{
	commandIsExecuted = 0;
	while (!commandIsExecuted)                                                        // Пока нет подтверждения успешного выполнения команды                                           
	{
		while ((inp(0x64) & 0x02) != 0x00);                                           // Ожидаем освобождения входного буфера клавиатуры         
		outp(0x60, 0xED);                                                             // Записываем в порт команду управления индикаторами                                
		outp(0x60, mask);                                                             // Записываем в порт битовую маску для настройки индикаторов
		delay(50);
	}
}