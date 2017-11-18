#include "kernel.h"
#include <iostream.h>

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) 
{
	int userMain_value;
	Kernel::load();
	userMain_value = userMain(argc, argv);
	Kernel::unload();
	return userMain_value;
}