#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printChar(const char* from, const char* to, int size, char c)
{
	int index = 0;
	while (index != size)
	{
		if (c == from[index])
		{
			putchar(to[index]);
			return;
		}
		index++;
	}
	putchar(c);
}

int main(int argc, char *argv[])
{
	if (argc > 3)
	{
		fprintf(stderr,"Error: extra operand");
		exit(1);
	}
	else if (argc < 3)
	{
		fprintf(stderr,"Error: missing operand");
		exit(1);
	}
	const char* from = argv[1];
	const char* to = argv[2];
	int fromSize = strlen(from);
	int toSize = strlen(to);
	if (fromSize != toSize)
	{
		fprintf(stderr, "Error: arguments must have the same length");
		exit(1);
	}
	char fromCopy[fromSize+1];
	int index = 0;
	while (from[index] != '\0')
	{
		fromCopy[index] = from[index];
		index++;
	}
	fromCopy[fromSize] == '\0';
	int i = 0;
	int j;
	char temp;
	for (; i < fromSize - 1; i++)
	{
		for (j = i + 1; j < fromSize; j++)
		{
			if (fromCopy[i] > fromCopy[j])
			{
				temp = fromCopy[i];
				fromCopy[i] = fromCopy[j];
				fromCopy[j] = temp;
			}
		}	
	}
	for (i = 0; i < fromSize - 1; i++)
	{
		if (fromCopy[i] == fromCopy[i+1])
		{
			fprintf(stderr, "Error: from has duplicate bytes");
			exit(1);
		}
	}
	char c;
	while (1)
	{
		c = getchar();
		if (c == EOF)
			break;
		printChar(from, to, fromSize, c);
	}
}
