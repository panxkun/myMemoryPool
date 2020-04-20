#include <iostream>
#include <time.h>

#include "allocator.h"
#include "myStack.h"

using namespace std;

const int ELEMS = 4;
const int REPS = 100000000;

int main()
{

	StackAlloc<int, myAllocator<int>> stack;

	StackAlloc<int> stackDefault;

	clock_t start = clock();

	for (int i = 0; i < REPS; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			stack.push(j);
		}
		for (int j = 0; j < 4; j++)
		{
			stack.pop();
		}
	}


	cout << ((double)clock() - start) / CLOCKS_PER_SEC << endl;

	start = clock();

	for (int i = 0; i < REPS; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			stackDefault.push(j);
		}
		for (int j = 0; j < 4; j++)
		{
			stackDefault.pop();
		}
	}


	cout << ((double)clock() - start) / CLOCKS_PER_SEC << endl;


	return 0;
}