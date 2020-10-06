#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define timer 105
#define timer_1 90
#define Count 10
#define Count_1 73

#define Gd4 415
#define B4 494
#define Fd4 370
#define Ad4 466
#define E4 330

#define Ad5 932
#define Gd5 831
#define Fd5 740
#define Dd5 622
#define F5 699
#define Cd6 1109

#define MAX_VAL 65536

void RunAway();
void JerkItOut();
void DivisionCoefficientComputation();
void StatusWord();
void RandomNumber();

void main()
{
	clrscr();
	char choice;
	do
	{
		printf("1. Generate sound(JerkItOut).\n2. Generate sound(RunAway).\n3. Status word.\n4. Division coefficient.\n5. Random number.\n0. Exit.\n");
		rewind(stdin);
		printf("\nYour choice: ");
		scanf("%c", &choice);

		switch (choice)
		{
			case '1': JerkItOut(); break;
			case '2': RunAway(); break;
			case '3': StatusWord(); break;
			case '4': DivisionCoefficientComputation(); break;
			case '5': RandomNumber(); break;
		}
	} while (choice != '0');
}

void RunAway() {

int frequency[Count_1] = { Ad5, Ad5, Ad5, Ad5, Gd5, Ad5, Ad5, Ad5, Ad5, Ad5, Gd5, Ad5, Ad5, Cd6, Ad5, Gd5, Fd5, Dd5, Dd5, 
F5, Fd5, Dd5, Ad5, Ad5, Ad5, Ad5, Gd5, Ad5, Ad5, Ad5, Ad5, Ad5, Gd5, Ad5, Ad5, Cd6, Ad5,Gd5, Fd5, Dd5, Dd5, F5, Fd5, Dd5, 
Gd5, Fd5, Dd5, Dd5, F5, Fd5, Dd5, Ad5, Ad5, Ad5, Ad5, Gd5, Ad5, Ad5, Ad5, Ad5, Ad5, Gd5, Ad5, Ad5, Cd6, Ad5, Gd5, Fd5, Dd5,
Dd5, F5, Fd5, Dd5};

int durability[Count_1] = { 3 * timer_1, 2 * timer_1, timer_1,timer_1,timer_1, 3 * timer_1, 2 * timer_1, 2 * timer_1, 
timer_1, timer_1,timer_1, 3 * timer_1,2 * timer_1, 4*timer_1, 2*timer_1, 4*timer_1, timer_1, 2 * timer_1, 2*timer_1, 
2*timer_1, 2*timer_1, 2*timer_1, 3*timer_1, 2*timer_1, timer_1, timer_1, timer_1, 3*timer_1, 2*timer_1, 2*timer_1,
timer_1, timer_1, timer_1, timer_1, timer_1, 4*timer_1, 2*timer_1, timer_1, timer_1, 2*timer_1, 2*timer_1, 2*timer_1, 
2*timer_1, 2*timer_1, 4*timer_1, timer_1, 2*timer_1, 2*timer_1, 2*timer_1, 2*timer_1, 2*timer_1, 3*timer_1, 2*timer_1, 
timer_1, timer_1, timer_1, 3*timer_1, 2*timer_1, 2*timer_1, timer_1, timer_1, timer_1, 3*timer_1, 2*timer_1, 4*timer_1, 
2*timer_1, 4*timer_1, timer_1, 2*timer_1, 2*timer_1, 2*timer_1, 2*timer_1, 2*timer_1};

int delayCounter[Count_1] = {5*timer_1,0,0,0,0,0,6*timer_1,0,0,0,0,0,4*timer_1,0,2*timer_1,0,3*timer_1,0,0,0,0,0,5*timer_1,
0,0,0,0,0,6*timer_1,0,0,0,0,2*timer_1,5*timer_1,timer_1,2*timer_1,2*timer_1,3*timer_1,0,0,0,0,timer_1,0,3*timer_1,
0,0,0,0,0,5*timer_1,0,0,0,0,0,6*timer_1,0,0,0,0,0,4*timer_1,0,2*timer_1,0,3*timer_1,0,0,0,0};
long unsigned base = 1193180;
int frequencyCounter;
int divisionCoefficient;

for (frequencyCounter = 0; frequencyCounter < Count_1; frequencyCounter++)
{
outp(0x43, 0xB6);
divisionCoefficient = base / frequency[frequencyCounter];
outp(0x42, divisionCoefficient % 256); //Low
divisionCoefficient /= 256; //Pause
outp(0x42, divisionCoefficient); //High
outp(0x61, inp(0x61) | 3); //Turn on
delay(durability[frequencyCounter]); //Wait
outp(0x61, inp(0x61) & 0xFC); //Turn off
delay(delayCounter[frequencyCounter]); //Delay
}
}

void JerkItOut() {

int frequency[Count] = { Gd4, B4, Gd4, B4,B4,Gd4,Fd4,Ad4,Fd4,E4 };

int durability[Count] = { 4 * timer - 20, 2 * timer, 2 * timer, 2 * timer - 10, 2 * timer, 4*timer, 4*timer, 2*timer-10,2*timer-10,2*timer-10 };

int delayCounter[Count] = { 20, 0,0,10,0,0,0,20,20,0 };
long unsigned base = 1193180;
int frequencyCounter;
int divisionCoefficient;

for (frequencyCounter = 0; frequencyCounter < Count; frequencyCounter++)
{
outp(0x43, 0xB6);
divisionCoefficient = base / frequency[frequencyCounter];
outp(0x42, divisionCoefficient % 256); //Low
divisionCoefficient /= 256; //Pause
outp(0x42, divisionCoefficient); //High
outp(0x61, inp(0x61) | 3); //Turn on
delay(durability[frequencyCounter]); //Wait
outp(0x61, inp(0x61) & 0xFC); //Turn off
delay(delayCounter[frequencyCounter]); //Delay
}
}

void DivisionCoefficientComputation()
{
	unsigned long j;
	int Channel;
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWords[] = { 0, 64, 128 };
	unsigned divisionCoefficientLow, divisionCoefficientHigh, divisionCoefficient, max;

	printf("\n\nCoefficient of division: \n");

	for (Channel = 0; Channel < 3; Channel++)
	{
		divisionCoefficient = 0;
		max = 0;

		if (Channel == 2)
			outp(0x61, inp(0x61) | 3);		//turn ON

		for (j = 0; j < MAX_VAL; j++)
		{
			outp(0x43, controlWords[Channel]);
			divisionCoefficientLow = inp(ports[Channel]);
			divisionCoefficientHigh = inp(ports[Channel]);
			divisionCoefficient = divisionCoefficientHigh * 256 + divisionCoefficientLow;

			if (divisionCoefficient > max)
			{
				max = divisionCoefficient;
			}
		}
		if (Channel == 2)
			outp(0x61, inp(0x61) & 0xFC);	//turn OFF
		printf("\nChannel %d: %4X\n", Channel, max);
	}
}

void StatusWord()
{
	printf("\n\nStatus word: \n");
	char stateWord[8];
	int Channel, state;
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWords[] = { 226, 228, 232 };		//command words,
												//acording to the rules

	for (Channel = 0; Channel < 3; Channel++)
	{
		outp(0x43, controlWords[Channel]);
		state = inp(ports[Channel]);            //read state

		for (int i = 7; i >= 0; i--)
		{
			stateWord[i] = state % 2 + '0';
			state /= 2;
		}
		printf("\nChannel %d: ", Channel);
		for (i = 0; i < 8; i++)
		{
			printf("%c", stateWord[i]);
		}
		printf("\n");
	}
}

void RandomNumber()
{
	char choice;
	unsigned int limit = MAX_VAL - 1, numberLow, numberHigh, number;

	do
	{
		printf("\n\n1. Set higher bound.\n2. Get random number.\n0. Exit\n");
		fflush(stdin);
		printf("\nChoise: ");
		scanf("%s", &choice);

		switch (choice)
		{
		case '1':
		{
			do
			{
				printf("\nEnter a limit in range [+1 ; +65635].\n");
				fflush(stdin);
			} while (!scanf("%d", &limit) || limit < 1);

			outp(0x43, 0xB6);
			outp(0x42, limit % 256);
			limit /= 256;
			outp(0x42, limit);
			outp(0x61, inp(0x61) | 1);
			break;
		}
		case '2':
		{
			outp(0x43, 128);
			numberLow = inp(0x42);
			numberHigh = inp(0x42);
			number = numberHigh * 256 + numberLow;

			printf("\nRandom number: %u\n", number);

			break;
		}
		}
	} while (choice != '0');
	outp(0x61, inp(0x61) & 0xFC);
}