#include <stdio.h>
#include <string.h>
#include "md5.h"


int main(int argc, char** argv)
{
	char r[33];

	md5_encode(argv[1], strlen(argv[1]), r);

	printf("%s\n", r);

	return 0;
}
