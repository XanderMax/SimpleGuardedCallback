#include "SimpleGuardedCallback.h"

#include <iostream>




void someFunc(int token, const int& i)
{
	std::cout << token << " " << (i * i) << std::endl;
}

int main()
{

	Guard<int> guard;

	Guard<int>::Function function = guard.bind(std::bind(someFunc, std::placeholders::_1, std::placeholders::_2));

	function(123);


	return 0;
}