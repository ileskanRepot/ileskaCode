#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Check if string contains only numbers
char isNumber(char *str)
{
	for (int i = 0;str[i] != '\0';i++){
		if (48 > str[i] ||  57 < str[i])
		{
			return 0;
		}
	}
	return 1;
}

// get strings lenght
int strLen(char* str)
{
	int len = 0;
	while(str[len]){len++;}
	return len;
}

int strToInt(char* str)
{
	int i = 0;
	int num = 0;
	for (i = 0;str[i];i++){
		if (48 > str[i] ||  57 < str[i])
		{
			return 0;
		}
	}

	for (int j = i-1; j >= 0 ;j--)
	{
		int exp = 1;
		for (int k = 0; k < i-j-1;k++){exp *= 10;}
		num += (str[j]-48)*exp;
	}
	return num;
}

// get strings nTh word
char* nThWord(unsigned char *originalWord, int word)
{
	char *parsedWord;
	char isOther = 0;
	int spaceCount = 0;
	int count = 0;
		
	parsedWord = (char *)malloc(100*sizeof(char));
	
	for (int i = 0;originalWord[i] != '\0';i++)
	{
		if (isOther && word == spaceCount){
			while (originalWord[i] != '\0' && originalWord[i] != ' ')
			{
				parsedWord[count] = originalWord[i];
				i++;
				count++;
			}
			return parsedWord;
		}
		if (isOther && originalWord[i] == ' ') spaceCount++;
		if (!isOther && originalWord[i] != '\t' && originalWord[i] != ' '){
			isOther = 1;
			i--;
		}
	}
	return "";
}

// check if strings are equal
char strEq(char *word1,char *word2)
{
	int len1 = 0;
	int len2 = 0;
	while (word1[len1] != '\0'){
		len1++;
	}
	while (word2[len2] != '\0'){
		len2++;
	}
	if (len1 != len2){
		return 0;
	}
	for (int i = 0;i < len1;i++){
		if (word1[i] != word2[i]){
			return 0;
		}
	}
	
	return 1;
}



int main(int argc, char **argv)
{
	unsigned char *data;
	FILE *fileR = fopen(argv[1], "rb");

	if (fileR == NULL)
	{
		printf("Cannot open file \033[31m%s\033[0m\n", argv[1]);
		return 0;
	}
	fseek(fileR, 0, SEEK_END);
	unsigned long length = ftell(fileR);
	fseek(fileR, 0, SEEK_SET);
	data = (unsigned char *)malloc(length*sizeof(unsigned char ));
	fread(data, 1, length, fileR);

	char **intNames;
	intNames = (char **)malloc(sizeof(char *));
	int *intValues;
	intValues = (int *)malloc(sizeof(int));
	int intCount = 0;

	unsigned long num = 0;
	unsigned long lastPos = 0;
	for (unsigned long i = 0; i < length;i++)
	{
		if (data[i] == '\n' && data[i+1] != '\n' && data[i + 1] != '\0')
		{
			num++;
		}
	}
	num++;

	// Put code to easily accessable format
	// TODO FIX MALLOCCING
	unsigned char **lines;
	lines = (unsigned char **)malloc(num*10*sizeof(unsigned char *));
	int pos = 0;
	int lineCount = 0;
	lines[lineCount] = (unsigned char *)malloc(1000*sizeof(unsigned char));
	for (unsigned long i = 0; i < length;i++)
	{
		pos++;
		if (data[i] != '\n')
		{
		  	lines[lineCount][pos-1] = data[i];
		}else{
			// TODO mieti mit?? t???? \0 teki t??s ja tarviiks sit?? oikeesti ku se aiheuttaa segfaulting
		  //lines[lineCount][pos+1] = '\0';
		 	pos = 0;
			if (!(lineCount != 0 && data[i-1] == '\n')){
		 		lineCount++;
			}
			if (lineCount < num){
				lines[lineCount] = (unsigned char *)malloc(1000*sizeof(unsigned char));
			}
		}
	}
	
	int line = 0;

	for (int i = 0;i < lineCount;i++){
		// printf("'%s'\n",nThWord(lines[i],0));
		// Create new variable
		if (strEq(nThWord(lines[i],0),"new")){
			// Create new int
			if (strEq(nThWord(lines[i],1),"int")){
				if (strEq(nThWord(lines[i],2),"\0")){
					fprintf(stderr,"\033[31mError on line %d\nNo variable name\033[0m\n",i+1);
					return 1;
				}
				// Check if int has value. If not set it to zero
				if (strEq(nThWord(lines[i],3),"\0")){
					intNames = (char **)malloc(sizeof(char *));
					intNames[intCount] = (char *)malloc(strLen(nThWord(lines[i],2))*sizeof(char));
					intNames[intCount] = nThWord(lines[i],2);
					intValues = malloc(sizeof(int)*15); // <- EI TOIMI
					intValues[intCount] = 0;

				}
				else
				{
					if (isNumber(nThWord(lines[i],3)))
					{
						intNames = (char **)malloc(sizeof(char *));
						intNames[intCount] = (char *)malloc(strLen(nThWord(lines[i],2))*sizeof(char));
						intNames[intCount] = nThWord(lines[i],2);
						printf("%d\n",strToInt(nThWord(lines[i],3)));
						intValues = (int *)malloc(sizeof(int));
						intValues[intCount] = strToInt(nThWord(lines[i],3));
					}else{
						fprintf(stderr,"\033[31mOn line %d Int \"%s\" must be null or Ingeres, not \"%s\"\033[0m\n",i+1,nThWord(lines[i],2),nThWord(lines[i],3));
						return 1;
					}
				}
				intCount++;
			}
		}

		// Print text
		else if (strEq(nThWord(lines[i],0),"put")){
			printf("PRINT");
		}
	}

	printf("\n");


	return 0;
}
