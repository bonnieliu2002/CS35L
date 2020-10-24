#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>

int frobcmp(char const *a, char const *b, int ignoreCase)
{
	int i = 0;
	char a_defrob, b_defrob;
	while (a[i] != ' ' && b[i] != ' ')
	{
		a_defrob = a[i] ^ 42;
		b_defrob = b[i] ^ 42;
		if (ignoreCase)
		{
			if (a_defrob < CHAR_MAX && a_defrob > CHAR_MIN)
				a_defrob = toupper(a_defrob);
			if (b_defrob < CHAR_MAX && b_defrob > CHAR_MIN)
				b_defrob = toupper(b_defrob);
		}
		if (a_defrob < b_defrob)
			return -1;
		if (a_defrob > b_defrob)
			return 1;
		i++;
	}
	if (a_defrob == ' ' && b_defrob != ' ')
		return -1;
	if (a_defrob != ' ' && b_defrob == ' ')
		return 1;
	return 0;
}

int compare(void *a, void *b)
{
	return frobcmp(*((char const **) a), *((char const **) b),0);
}

int compare_ignore(void *a, void *b)
{
	return frobcmp(*((char const **) a), *((char const **) b), 1);
}

int main(int argc, char* argv[])
{
	int flagSet = 0;
	if (argc > 1)
	{
		if (argv[1][0] == '-' && argv[1][1] == 'f' && argv[1][2] == '\0')
			flagSet = 1;
		else
		{
			char error[] = "Error: wrong arguments";
			write(2, &error, strlen(error));
			exit(1);
		}
	}
	struct stat fileData;
	if (fstat(0, &fileData) < 0)
	{
		char error[] = "fstat error";
		write(2, &error, strlen(error));
		exit(1);
	}
	char* buffer;
	int bufferSize;
	if (S_ISREG(fileData.st_mode))
	{
		bufferSize = fileData.st_size;
		buffer = (char*) malloc(fileData.st_size * sizeof(char));
	}
	else
	{
		bufferSize = 1024;
		buffer = (char*) malloc(1024);
	}
	char c;
	int i = 0;
	size_t result = read(0, &c, 1);
	while (result != 0)
	{
		if (i > bufferSize / 2)
		{
			bufferSize *= 2;
			buffer = realloc(buffer, bufferSize);
		}
		if (result == -1)
		{
			write(2, "error reading file", 18);
			exit(1);
		}
		buffer[i] = c;
		result = read(0, &c, 1);
		i++;
	}
	buffer[i] = '\0';
	int letterIndex = 0;
	int wordIndex = 0;
	char* ptrToLetter = (char*)malloc(sizeof(char));
	char** ptrToWord = (char**)malloc(sizeof(char*));
	i = 0;
	while (buffer[i] != '\0')
	{
		c = buffer[i];
		ptrToLetter[letterIndex] = c;
                if (c == ' ')
                {
                        ptrToWord[wordIndex] = ptrToLetter;
                        wordIndex++;
                        ptrToWord = realloc(ptrToWord, (wordIndex+1)*sizeof(char*));
                        ptrToLetter = (char*)malloc(sizeof(char));
                        letterIndex = 0;
                }
                else
                {
                        letterIndex++;
                        ptrToLetter = realloc(ptrToLetter, (letterIndex+1)*sizeof(char));
                }
		i++;
	}
	ptrToLetter = realloc(ptrToLetter, (letterIndex+1)*sizeof(char));
	ptrToLetter[letterIndex] = ' ';
	ptrToWord[wordIndex] = ptrToLetter;
	wordIndex++;
	i = 0;
	int j = 0;
	if (flagSet)
		qsort(ptrToWord, wordIndex, sizeof(char*), compare_ignore);
	else
		qsort(ptrToWord, wordIndex, sizeof(char*), compare);	
	for (i = 0; i < wordIndex; i++)
	{
		while (ptrToWord[i][j] != ' ')
		{
			write(1, &ptrToWord[i][j],1);
			j++;
		}
		write(1, " ", 1);
		j = 0;
	}
	for (i = 0; i < wordIndex; i++)
	{
		free(ptrToWord[i]);
		ptrToWord[i] = NULL;
	}
	free(ptrToWord);
	ptrToWord = NULL;
	free(buffer);
	buffer = NULL;
}
