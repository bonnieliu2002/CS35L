#include <stdlib.h>
#include <stdio.h>

int frobcmp(char const *a, char const *b)
{
	int i = 0;
	while ( a[i] != ' ' && b[i] != ' ')
	{
		if ((a[i] ^ 42) < (b[i] ^ 42))
			return -1;
		if ((a[i] ^ 42) > (b[i] ^ 42))
			return 1;
		i++;
	}
	if (a[i] == ' ' && b[i] != ' ')
		return -1;
	if (a[i] != ' ' && b[i] == ' ')
		return 1;
	return 0;
}

int compare(void *a, void *b)
{
	return frobcmp(*((char const **) a), *((char const **) b));
}

int main()
{
	char c;
	int letterIndex = 0;
	int wordIndex = 0;
	char* ptrToLetter = (char*)malloc(sizeof(char));
	char** ptrToWord = (char**)malloc(sizeof(char*));
	while (1)
	{
		c = getchar();
		if (c == EOF)
		{
			ptrToLetter = realloc(ptrToLetter, (letterIndex+1)*sizeof(char));
			ptrToLetter[letterIndex] = ' ';
			ptrToWord[wordIndex] = ptrToLetter;
			wordIndex++;
			break;
		}
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
	}
	int i;
	int j = 0;
	qsort(ptrToWord, wordIndex, sizeof(char*), compare);
	for (i = 0; i < wordIndex; i++)
	{
		while (ptrToWord[i][j] != ' ')
		{
			putchar(ptrToWord[i][j]);
			j++;
		}
		printf(" ");
		j = 0;
	}
	for (i = 0; i < wordIndex; i++)
	{
		free(ptrToWord[i]);
		ptrToWord[i] = NULL;
	}
	free(ptrToWord);
	ptrToWord = NULL;
}
