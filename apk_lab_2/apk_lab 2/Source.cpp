#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define n 8
#define iter 100000000

int main()
{
	srand((unsigned)time(NULL));
	do
	{
		float time1, time2, time3;
		int matrix[n][n];
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				matrix[i][j] = -5000 + rand() % 10000;
		system("cls");


		for (int i = 0; i < n; i++)
		{
			printf("\n");
			for (int j = 0; j < n; j++)
				printf("%10d", matrix[i][j]);
		}

		int sum;
		clock_t ttime1 = clock();
		for (int counter = 0; counter < iter; counter++)
		{
			sum = 0;
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					sum += matrix[i][j];
		}

		clock_t ttime2 = clock();
		time1 = (float)(ttime2 - ttime1) / CLOCKS_PER_SEC;
		printf("\n\ntime work on C: %f\n", time1);
		printf("Sum is %d\n", sum);

		int asm_sum;
		clock_t ttime3 = clock();
		for (int counter = 0; counter < iter; counter++)
		{
			asm_sum = 0;
			int cnt = n*n;
			_asm
			{
				pusha
				xor esi, esi
					xor eax, eax
					loop1 :
				add eax, dword ptr matrix[esi]
					add esi, 4
					sub cnt, 1
					jnz loop1
					mov asm_sum, eax
					popa
			}
		}
		clock_t ttime4 = clock();
		time2 = (float)(ttime4 - ttime3) / CLOCKS_PER_SEC;
		printf("\ntime work on Asm: %f\n", time2);
		printf("Asm_Sum is %d", asm_sum);

		int MMX_sum;
		clock_t ttime5 = clock();
		for (int counter = 0; counter < iter; counter++)
		{
			MMX_sum = 0;
			int cnt_1 = n*n;
			_asm
			{
				pusha
				xor esi, esi
				pxor MM7, MM7
			loop2 :
				movq MM0, matrix[esi]
				paddd MM7, MM0
				add esi, 8
				sub cnt_1, 2
				jnz loop2
				movq MM0, MM7
				psrlq MM7, 32
				paddd MM7, MM0
				movd MMX_sum, MM7
				emms
				popa
			}
		}
		clock_t ttime6 = clock();
		time3 = (float)(ttime6 - ttime5) / CLOCKS_PER_SEC;
		printf("\n\ntime work on MMX: %f\n", time3);
		printf("MMX_Sum is %d", MMX_sum);
		rewind(stdin);
	} while (getchar() == 'y');
}