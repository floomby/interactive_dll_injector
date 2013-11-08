#include <stdio.h>

//there is a bad bug here

int main(int argc, char *argv[])
{
	while(argc > 0){
		printf("[%d]: %s\n", argc - 1, argv[argc - 1]);
		argc--;
	}
	return 0;
}
