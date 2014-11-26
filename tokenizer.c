#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// List of constant variables representing the hex equivalents
char *nHex = "[0x0a]";
char *tHex = "[0x09]";
char *vHex = "[0x0b]";
char *bHex = "[0x08]";
char *rHex = "[0x0d]";
char *fHex = "[0x0c]";
char *aHex = "[0x07]";
char *slashHex = "[0x5c]";
char *quoteHex = "[0x22]";

typedef int boolean;
	enum {false, true};

struct TokenizerT_ {
	char *delimiter;
	char *inputString;
	char **tokens;
	long *tokenCount;
	int delLen;
	int inLen;
};

typedef struct TokenizerT_ TokenizerT;

void copy_string(char*, char*);
void format_string(char*);
void format_string_command(char*);
TokenizerT *TKCreate(char*, char*);
void TKDestroy(TokenizerT*);
char *TKGetNext(TokenizerT*);
void resetInputPT(TokenizerT*);
void resetDelPT(TokenizerT*);
void cat_string(char*, char*);




TokenizerT *TKCreate(char *separators, char *ts) {

	// Allocate memory for the TokenizerT object
	TokenizerT *tokenStructP = malloc(sizeof(TokenizerT));	
	tokenStructP->delimiter = malloc(sizeof(char)*(strlen(separators)+1));
	tokenStructP->inputString = malloc(sizeof(char)*(strlen(ts)+1));
	tokenStructP->tokens = (char **)malloc(1+strlen(ts)*sizeof(char));
	tokenStructP->tokenCount = (long*) malloc( sizeof(long) );
	
	// Initialize Variables
	*tokenStructP->tokenCount = 0;
	copy_string(tokenStructP->delimiter, separators);
	copy_string(tokenStructP->inputString, ts);

	// Format escape commands into single byte characters
	format_string_command(tokenStructP->delimiter);
	format_string_command(tokenStructP->inputString);

	// Initialize string length variables
	tokenStructP->inLen = strlen(tokenStructP->inputString);
	tokenStructP->delLen = strlen(tokenStructP->delimiter);
	
	if(&tokenStructP->inputString != &ts) {
		return tokenStructP;	
	}
	else {
  		return NULL;
	}
}

void TKDestroy(TokenizerT *tk) {
	
	// Frees allocated memory of a Tokenizer object
	free(tk);
}

char *TKGetNextToken(TokenizerT *tk) {
	
	char *nextToken = malloc(sizeof(char*)*(tk->inLen+2+tk->delLen)*(*tk->tokenCount+1));
	int counter = 0;

	while(*tk->inputString != '\0') {		
		while(*tk->delimiter != '\0') {		
			if(*tk->inputString == *tk->delimiter) {
				if(counter != 0) {
					counter++;
					tk->inputString++;
					resetDelPT(tk);
					while(counter > 1) {
						nextToken--;
						counter--;
						//printf("\nnext Token: %s\n", nextToken);

					}
					*tk->tokens = nextToken;
					tk->tokens++;
					//*tk->tokenCount++;
					nextToken = realloc(nextToken, sizeof(char)*(counter+10)*(strlen(nextToken)+7)*(tk->inLen + 2));
					format_string(nextToken);

					return nextToken;
				}
				else {
					tk->inputString++;
					resetDelPT(tk);
				}
			}
			else {
				tk->delimiter++;
			}
		}
		if(*tk->inputString != '\0') {
				resetDelPT(tk);
				*nextToken = *tk->inputString;
				nextToken++;
				tk->inputString++;
				counter++;
		}
	}

	*nextToken = '\0';

	if((*tk->inputString) == '\0' && *nextToken == '\0' && counter == 0) {
		return NULL;
	}
	else {
		while(counter > 0) {
			nextToken--;
			counter--;
		}
		*tk->tokens = nextToken;
		tk->tokens++;
		nextToken = realloc(nextToken, sizeof(char)*(counter+10)*(strlen(nextToken)+7)*(tk->inLen + 2));
				//printf("\nstring: %s\n", nextToken);

		format_string(nextToken);
		//printf("\nstring: %s\n", nextToken);
		return nextToken;
	}
}

int main(int argc, char **argv) {
		
	if(argc==3) {
		TokenizerT *testObject = TKCreate(argv[1], argv[2]);
		printf("\ntokens:");
		while(TKGetNextToken(testObject) != NULL) {
			testObject->tokens--;
			*testObject->tokenCount = *testObject->tokenCount + 1;
			//printf("\ntoken count: %lu", *testObject->tokenCount);
			printf("\n%s", *testObject->tokens);
		}
		printf("\n\n");
				TKDestroy(testObject);

	}

	else if(argc > 3) {
		printf("Error! Too many arguments!\n");
	}
	else {
		printf("Error! Not enough arguments!\n");
	}
	

	  return 0;
}

void resetInputPT(TokenizerT *source) {
	int difference = (source->inLen - strlen(source->inputString));
	int i = 0;
	for(i = 0; i < difference; i++) {
		source->inputString--;
	}
}

void resetDelPT(TokenizerT *source) {
	int difference = (source->delLen - strlen(source->delimiter));
	int i = 0;
	for(i = 0; i < difference; i++) {
		source->delimiter--;
	}
}

void format_string(char *source) {	
	char* output = malloc(sizeof(char*)*strlen(source));
	int count = 0;

	int charCount = strlen(source);
	while(*source) {
			switch (*source){	
				case '\n':
					cat_string(output, nHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\t':
					cat_string(output, tHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\v':
					cat_string(output, vHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\b':
					cat_string(output, bHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\r':
					cat_string(output, rHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\f':
					cat_string(output, fHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\a':
					cat_string(output, aHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\\':
					cat_string(output, slashHex);
					output = output + 6;
					count = count + 6;
				break;
				
				case '\"':
					cat_string(output, quoteHex);
					output = output + 6;
					count = count + 6;
				break;

				default:
					*output = *source;
					output++;
					count = count + 1;
				break;
			}
		source++;
	}
	*output = '\0';
	
	while(count > 0) {
		count--;
		output--;
	}
	while(charCount > 0) {
		charCount--;
		source--;
	}

	copy_string(source, output);
	free(output);
}

void format_string_command(char *source) {	
	char* output = malloc(sizeof(char)*strlen(source)+1);
	int count = 0;
	int charCount = strlen(source);
	int fuckingYolo = strlen(source);
		
	while(*source) {
		if(*source == '\\') {
			*source = '\0';
			source++;
			switch (*source) {	
				case 'n':
					*output = '\n';
				break;
				case 't':
					*output = '\t';
				break;
				case 'v':
					*output = '\v';
				break;
				case 'b':
					*output = '\b';
				break;
				case 'r':
					*output = '\r';
				break;
				case 'f':
					*output = '\f';
				break;
				case 'a':
					*output = '\a';
				break;
				case '\\':
					*output = '\\';
				break;
				case '"':
					*output = '\"';
				break;
				case '\0':
					source--;
					*output = '\0';
					fuckingYolo--;
					break;
				default:
					*output = *source;
				break;
			}
			fuckingYolo--;
		}
		else {
			*output = *source;
		}
			*source = '\0';

		output++;
		source++;
		count++;
	}
	
	*output = '\0';
	
	while(count > 0) {
		count--;
		output--;
		
	}
	
	while(charCount > 0) {
		charCount--;
		source--;
	}


	int i = 0;
	for(i = 0; i <= fuckingYolo; i++) {
		source[i] = output[i];
	}

	printf("\n");
	free(output);
		
}

void copy_string(char *target, char *source) {
	while(*source) {
		*target = '\n';
		*target = *source;
		source++;
		target++;
	}
	*target = '\0';
}

void cat_string(char *target, char *source) {
	copy_string(&target[strlen(target)], source);
}
