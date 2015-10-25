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
	char *token;
};

#define FREE(ptr) 		\
	do{ 				\
		free((ptr));	\
		(ptr)=NULL;		\
	}while(0) 


typedef struct TokenizerT_ TokenizerT;

void copy_string(char*, char*);
void format_output(char**);
void format_input(char*);
TokenizerT *TKCreate(char*, char*);
void TKDestroy(TokenizerT*);
char *TKGetNext(TokenizerT*);
void cat_string(char*, char*);

TokenizerT *TKCreate( char *separators, char *ts ) {

	// Allocate memory for the TokenizerT object
	TokenizerT *tokenStructP = malloc( sizeof( TokenizerT ) );

	tokenStructP->delimiter = malloc( sizeof(char) * (strlen( separators ) + 1) );
	tokenStructP->inputString = malloc( sizeof(char) * (strlen( ts ) + 1 ) );
	
	// Initialize Variables
	copy_string(tokenStructP->delimiter, separators);
	copy_string(tokenStructP->inputString, ts);

	// Format escape commands into single byte characters
	format_input(tokenStructP->delimiter);
	format_input(tokenStructP->inputString);

	if(&tokenStructP->inputString != &ts) {
		return tokenStructP;	
	}
	else {
  		return NULL;
	}
}

void TKDestroy(TokenizerT *tk) {
	
	// Frees allocated memory of a Tokenizer object
	FREE(tk->delimiter);
	FREE(tk->inputString);
	FREE(tk->token);
	FREE(tk);
}

char *TKGetNextToken(TokenizerT *tk) {

	char *delim = tk->delimiter;
	char *delim_itr = tk->delimiter;
	char *output = malloc(strlen(tk->inputString) + 1);


	while(*tk->inputString != '\0') {		
		while(*delim_itr != '\0') {
			if(*tk->inputString == *delim_itr) {
				if( strlen(output) == 0)
				{
					delim_itr = delim;
					tk->inputString++;
					break;
				}
				else
				{
					tk->inputString++;
					format_output(&output);

					return output;
				}
			}
			delim_itr++;
		}
		delim_itr = delim;
		output[strlen(output)] = *tk->inputString;
		output[strlen(output) + 1] = '\0';
		tk->inputString++;
	}	

	if(strlen(output)==0)
	{
		return NULL;
	}
	else
	{
		format_output(&output);
		tk->token = realloc(tk->token, strlen(output)+1);
		copy_string(tk->token, output);
		//FREE(output);
		return output;
	}
}

int main ( int argc, char **argv ) {
		
	if ( argc == 3 ) {
		/* Initialize a struct to contain all token data*/
		TokenizerT *testObject = TKCreate( argv[ 1 ], argv[ 2 ] );
		if ( testObject == NULL ) {
			printf( "\nInitialization failed. Insufficient memory" );
			exit( 1 );
		}

		/* Print all the tokens for the given string */
		printf( "\ntokens:" );

		char *string_start = testObject->inputString;
		char *delim_start = testObject->delimiter;
		char *output;
		int count = 0;

		while( (output = TKGetNextToken( testObject) ) != NULL )
		{
			printf("\n%d: %s", count, output);
			count++;
		}

		printf("\n\n");
		
		/* Clean up allocated memory */
		testObject->inputString = string_start;
		testObject->delimiter = delim_start;
		TKDestroy(testObject);

	}
	else {
		printf("\nUsage:");
		printf("\ntokenizer <delimiters> <string>\n\n");
	}

	return 0;
}

void format_output(char **source) {	
	printf("\n%s", *source);
	char *output = malloc(sizeof(char*)*strlen(*source));
	char *output_ptr = output;

	int charCount = strlen(*source);
	char *src_ptr = *source;
		
	while(*src_ptr) {
		switch (*src_ptr){	
			case '\n':
				cat_string(output, nHex);
			break;
			
			case '\t':
				cat_string(output, tHex);
			break;
			
			case '\v':
				cat_string(output, vHex);
			break;
			
			case '\b':
				cat_string(output, bHex);
			break;
			
			case '\r':
				cat_string(output, rHex);
			break;
			
			case '\f':
				cat_string(output, fHex);
			break;
			
			case '\a':
				cat_string(output, aHex);
			break;
			
			case '\\':
				cat_string(output, slashHex);
			break;
			
			case '\"':
				cat_string(output, quoteHex);
			break;

			default:
				output[strlen(output)] = *src_ptr;
			break;
		}
		src_ptr++;
	}

	output[strlen(output)+1] = '\0';
	printf("\n%s", output);
	//source = realloc(source, strlen(output)+1);
	//copy_string(source, output);

	*source = output;
		printf("\nsource: %s", *source);

	//FREE(output);
}

void format_input(char *source) {

	/* create temporary string that we free at the end of this function */	
	char* output = malloc(sizeof(char)*strlen(source)+1);

	/* pointer to the source, to iterate*/
	char *src_ptr = source;

	while(*src_ptr) {
		if(*src_ptr == '\\') {
			src_ptr++;
			switch (*src_ptr) {	
				case 'n':
					cat_string(output, "\n");
				break;
				case 't':
					cat_string(output, "\t");
				break;
				case 'v':
					cat_string(output, "\v");
				break;
				case 'b':
					cat_string(output, "\b");
				break;
				case 'r':
					cat_string(output, "\r");
				break;
				case 'f':
					cat_string(output, "\f");
				break;
				case 'a':
					cat_string(output, "\a");
				break;
				case '\\':
					cat_string(output, "\\");
				break;
				case '"':
					cat_string(output, "\"");
				break;
				case '\0':
					src_ptr--;
					cat_string(output, "\0");
					break;
				default:
					output[strlen(output)] = *src_ptr;
				break;
			}
		}
		else {
			output[strlen(output)] = *src_ptr;
		}

		src_ptr++;
	}
	output[strlen(output)+1] = '\0';

	source = realloc(source, strlen(output) + 1);
	copy_string(source, output);
	FREE(output);		
}

/* My Implementation of a strcpy function */
void copy_string(char *target, char *source) {
	while(*source) {
		//*target = '\n';
		*target = *source;
		source++;
		target++;
	}
	*target = '\0';
}

/* My Implementation of a strcat function */
void cat_string(char *target, char *source) {
	copy_string(&target[strlen(target)], source);
}
