/*
HW #6 피보나치 수열의 계산: Memoization
Fibonachi Algorithm을 (1) recursive call, (2) recursive call with memoization을 이용하여 이 두가지를 각각 프로그램하라. (3) 이 두가지 프로그램에 대하여 각각, n을 1부터 큰 수로 키워가면서, 매번 프로그램의 수행시간을 측정하는 방법을 찾아서 프로그램을 수정한다. 그리고 그 결과를 이용하여 x축은 n의 값들로 y축은 프로그램의 수행시간의 값들로 그린 그래프를 하나의 그래프에 그려라. (4) 그 그래프의 결과를 관찰하여 관찰의견을 기술하라.

참조: 프로그램의 수행시간을 측정하는 방법은 timer.h / clock()을 구글링하여 참조한다.

기한: 6월 4일 (화) 수업시간 전까지 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n = 51;

int fib(int n)
{
	if(n<=1)
		return n;
	return fib(n-1) + fib(n-2);
}

int data[100] = {};
int fib_memoization(int n)
{
	if(n <= 1)
		return n;
	else if(data[n] != 0)
		return data[n];
	else {
		data[n] = fib_memoization(n-1) + fib_memoization(n-2);
		return data[n];
	}
}

int main(void)
{
	int i;
/*
	// Fibonacci algorithm with recursive call
	printf("Fibonacci algorithm with recursive call\n");
	for(i=0; i<=n; i++){
		clock_t start = clock();
		fib(i);
		printf("%d: %0.10f\n", i, (float)(clock() - start)/CLOCKS_PER_SEC);
	}
*/
	// Fibonacci algorithm with ressive call with memoization
	printf("Fibonacci algorithm with ressive call with memoization\n");
	for(i=0; i<=n; i++){
		clock_t start = clock();
		fib_memoization(i);
		printf("%d: %0.10f\n", i, (float)(clock() - start)/CLOCKS_PER_SEC);
	}

	return 0;
}
