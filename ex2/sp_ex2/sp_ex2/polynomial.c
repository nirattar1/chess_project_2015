#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_PRINT 0
#define COMMAND_DEF 1
#define COMMAND_SUM 2
//..



//term  - struct that stores 1 term of form ax^b.
typedef struct {
	int exp;
	float coeff;
} term;

//poly - struct that stores 1 polynomial.
typedef struct{
	char * name;
	int numTerms;
	term * terms;
} poly;


//globals
poly * _poly_arr = NULL;
int _num_polys = 0;

//helper function
//not to be used on constant strings !
void RemoveSpaces(char* source)
{
	char* i = source;
	char* j = source;
	while(*j != 0)
	{
		*i = *j++;
		if(*i != ' ')
			i++;
	}
	*i = 0;
}

//will get user string and return type of command
// i.e. if sees "eval ..." will know it is of type COMMAND_EVAL
int parse_req(char * command, char * part1, char * part2, char * part3)
{
	sscanf ();
	return COMMAND_DEF;
}

//will store the term that is in string, into term object.
//assumes no spaces. 
//caller is responsible allocating memory.
void ParseTerm(char * str, term * t1)
{
	if (!t1) {return;};

	//sign will be handled by 'float'

	sscanf (str, "%fx^%d", t1->coeff, t1->exp);

}

//will parse the polynomial in string, into poly object.
//caller is responsible allocating memory.
//assumes no spaces.
void ParsePoly(char * poly_str, poly * poly1)
{

	char * term_str = (char * )malloc (strlen(poly_str)); //whole size
	char * p = poly_str;
	int term_len=0;

	while( *p != 0)
	{
		printf ("read char: %c\n", *p);

		//reached end of term
		if (*p=='-' || *p=='+' || *(p+1)==0)
		{
			//special case - end of string, get last character.
			if (*(p+1)==0)
			{
				term_str[term_len++] = *p;
			}
			
			//null-ternimate existing buffer.
			term_str[term_len] = 0;
			//process full term
			printf("term: %s\n", term_str);

			//create space for 1 new term.
			int newsize = (poly1->numTerms + 1) * sizeof(term *);
			poly1->terms = (term *) realloc ( poly1->terms, newsize);
			if (!poly1->terms) 
			{
				return; 
			};
			ParseTerm(term_str, &(poly1->terms[poly1->numTerms]));
			poly1->numTerms++;

			//clear for new term
			printf ("start new term\n");
			//get sign for new term
			term_str[0] = *p;
			term_len = 1;
		}
		else
		{
			//continue reading term
			term_str[term_len++] = *p;
		}

		p++;
	}


}

void define (char * name, char * poly_str)
{
	
	//check validaty of name
	//...

	//create space for 1 new poly.
	int newsize = (_num_polys + 1) * sizeof(poly);
	_poly_arr = (poly *) realloc ( _poly_arr, newsize);
	if (!_poly_arr) 
	{
		return; 
	};

	//update new poly's name
	_poly_arr[_num_polys].name = (char *) malloc(strlen(name));
	strcpy( _poly_arr[_num_polys].name, name);

	//update new poly's terms.
	ParsePoly(poly_str, &(_poly_arr[_num_polys]));
	
	_num_polys++;
}

int main()

{
	//get input from user
	char command[2];
	char c;
	int char_cnt = 0;
	int command_type;
	while ((c = getchar()) != '\n')
	{
		char_cnt++;
		if ((command = (char *)realloc(command, char_cnt*sizeof(char))) == NULL)
		{
			printf("%s", "Error: could not allocate.\n");
			return 0;
		}

		command[char_cnt-1] = c;
	}

	command[char_cnt] = '\0';

	//do appropriate command
	command_type = parse_req(command);
	switch (command_type){
	case COMMAND_DEF:
		define(command);
		break;
	}

	return 0;
}