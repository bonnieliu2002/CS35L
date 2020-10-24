#include <stdlib.h>
//#include <stdio.h>
#include <string.h>

void printChar(const char* from, const char* to, int size, char c)
{
	int index = 0;
	char outputMe = c;
	while (index != size)
	{
		if (c == from[index])
		{
			outputMe = to[index];
			break;
		}
		index++;
	}
	write(1, &outputMe, 1);
}

int main(int argc, char *argv[])
{
	if (argc > 3)
	{
		char error[] = "Error: extra operand\n";
		write(2,&error,strlen(error));
		exit(1);
	}
	else if (argc < 3)
	{
		char error[] = "Error: missing operand\n";
		write(2,&error,strlen(error));
		exit(1);
	}
	const char* from = argv[1];
	const char* to = argv[2];
	int fromSize = strlen(from);
	int toSize = strlen(to);
	if (fromSize != toSize)
	{
		char error[] = "Error: arguments must have the same length";
		write(2,&error,strlen(error));
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
	char error_duplicate[] = "Error: from has duplicate bytes";
	for (i = 0; i < fromSize - 1; i++)
	{
		if (fromCopy[i] == fromCopy[i+1])
		{
			write(2,&error_duplicate,strlen(error_duplicate));
			exit(1);
		}
	}
	char* c_pointer = (char*) malloc(1);
	ssize_t result;
	char c;
	char error[] = "error or signal interrupt";
	while (1)
	{
		result = read(0, c_pointer, 1);
		// check for error
		if (result == -1)
		{
			write(2,&error,strlen(error));
			free(c_pointer);
			exit(1);
		}
		// check if end of file
		if (result == 0)
		{
			free(c_pointer);
			break;
		}
		c = *c_pointer;
		printChar(from, to, fromSize, c);
		free(c_pointer);
		c_pointer = (char*) malloc(1);
	}
}
