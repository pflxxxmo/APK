#include <math.h>
#include <stdio.h> 
#include <ctime>

int main()
{
	float a = 0, b = 0, x = 0, d = 0, f = 0, f1 = 0, result = 0;
	float time1, time2;
	do
	{
		printf("Enter the range of the interval:");
		while (!scanf_s("%f", &a) || a < 2)
		{
			printf("Try again\n");
			rewind(stdin);
		}
		while (!scanf_s("%f", &b) || b <= a)
		{
			printf("Try again\n");
			rewind(stdin);
		}
		printf("Enter the increment step:");
		clock_t ttime1 = clock();
		while(!scanf_s("%f", &d) || a<0)
		{
			printf("Try again\n");
			rewind(stdin);
		}
		for (x = a; x < b; x += d)
			f += x*sqrt(x) / log2(x);
		clock_t ttime2 = clock();
		time1 =(float) (ttime2 - ttime1)/CLOCKS_PER_SEC;
		printf("time work on C: %f\n", time1);
		printf("Result = %f\n", f);
		f = 0;
		clock_t ttime3 = clock();
		_asm finit
		for (x = a; x < b; x += d)
		{
			_asm
			{
				fld x
				fsqrt
				fld1
				fld x
				fyl2x
				fdiv
				fmul x
				fstp f
			}
			_asm fwait
			result += f;
		}
		clock_t ttime4 = clock();
		time2 =(float) (ttime4 - ttime3)/CLOCKS_PER_SEC;
		printf("result = %f\n", result);
		printf("time work on Asm: %f\n", time2);
		printf("Again?(y/n)?");
		rewind(stdin);
	} while (getchar() == 'y');
	return 0;
}