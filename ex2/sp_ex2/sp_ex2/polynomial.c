#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COMMAND_PRINT 0
#define COMMAND_DEF 1
#define COMMAND_SUM 2
//....
#define COMMAND_STORE_SUM	6
#define COMMAND_STORE_SUB	7
#define COMMAND_STORE_MULT	8
#define COMMAND_STORE_DER	9
#define COMMAND_QUIT 10
#define COMMAND_UNKNOWN 100
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

//remove spaces from given string
//note: changes the given string!
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

//for sort. 
//compares between 2 terms
int compare_terms (const void * p1, const void * p2)
{
	const term * t1 = p1;
	const term * t2 = p2;
	return ((t1->exp) - (t2->exp));
}


//try to parse given char pointer as a "store" request
// i.e. p3 = p1 + p2 is a "store" request
//		p3 = x + 7 is a polynomial , not store request definition
// the area checked above by "^" 
int parse_req_store(char * command, 
	char ** part1, int * part1_len,
	char ** part2, int * part2_len,
	char ** part3, int * part3_len)
{
	char * p1;
	char * p2;

	//find "="
	p1 = strchr(command, '=');
	if (p1) //found "="
	{
		//remove spaces and update pointers.
		RemoveSpaces(command);
		p1 = strchr(command, '=');
		p_end = strchr(command, 0);

		//get 1st part (before "=")
		*part1_len = (p1 - command);
		*part1 = command;

		//get 2nd part (after "=")
		p1++;
		*part2_len = (p_end - p1);
		*part2 = p1;

		//now decide about 2nd part.
		//on following chars:  -, number - it is not store command
		if (**part2 == '-' || isdigit(**part2))
		{
			return COMMAND_DEF;
		}
		
		//try to parse as store sum: p1 + p3
		if ((p2 = strchr(p1, '+'))!=NULL)
		{
			//update 2nd part length.
			*part2_len = (p2-p1);
			*part2 = p1;
			//get 3rd part, after '+'
			p2++;
			*part3_len = (p_end - p2);
			*part3 = p2;
			return COMMAND_STORE_SUM;
		}

		//any other case , assume it is DEF command
		return COMMAND_DEF;
	}
}


//will get user string and return type of command
// i.e. if sees "eval ..." will know it is of type COMMAND_EVAL
int parse_req(char * command, 
								char ** part1, int * part1_len,
								char ** part2, int * part2_len,
								char ** part3, int * part3_len)
{
	
	char * p_end;
	char *p1;
	int command_type ;

	//quit command
	if (strcmp(command, "quit")==0) 
	{
		return COMMAND_QUIT;
	}

	//handle "="
	p1 = strchr(command, '=');
	if (p1) //found "="
	{

		//try to parse 2nd part as 'store' command.
		command_type = parse_req_store(command, part1, part1_len, 
												part2, part2_len,
												part3, part3_len);
		if (command_type!=COMMAND_DEF)
		{
			//request was already parsed as a "store" command.
			return command_type;
		}
		else
		{

			//not store command , command is normal definition.
			//remove spaces and update pointers.
			RemoveSpaces(command);
			p1 = strchr(command, '=');
			p_end = strchr(command, 0);

			//get 1st part (before "=")
			*part1_len = (p1 - command);
			*part1 = command;

			//get 2nd part (after "=")
			p1++;
			*part2_len = (p_end - p1);
			*part2 = p1;

			return COMMAND_DEF;

		}



	}

	//assume print command  -- "  p1  "
	RemoveSpaces(command);
	p_end = strchr(command, 0);
	*part1_len = (p_end - command);
	*part1 = command;
	return COMMAND_PRINT;


	return COMMAND_UNKNOWN;
}

//will store the term that is in string, into term object.
//assumes no spaces. 
//caller is responsible allocating memory.
//sign of coefficient will be handled by 'float'
void ParseTerm(char * str, term * t1)
{
	if (!t1) {return;};

	//term doesn't contain x  (example '5')
	if (strchr(str,'x')==NULL)
	{
		sscanf(str, "%f", &(t1->coeff));
		t1->exp = 0;  //0 exponent (constant number)
		return;
	}

	//handle more cases

	//term doesn't contain ^  (example '7x')
	if (strchr(str,'x')==NULL)
	{
		sscanf(str, "%f", &(t1->coeff));
		t1->exp = 0;  //0 exponent (constant number)
	}


	
	sscanf (str, "%fx^%d", &(t1->coeff), &(t1->exp));

}

//will parse the polynomial in string, into poly object.
//caller is responsible allocating memory.
//assumes no spaces.
void ParsePoly(char * poly_str, poly * poly1)
{

	char * term_str = (char * )malloc (strlen(poly_str)); //whole size
	char * p = poly_str;
	int term_len=0;
	int newsize=0;

	while( *p != 0)
	{
		printf ("read char: %c\n", *p);

		//reached end of term (and not at start)
		if ((p!=poly_str)&&(*p=='-' || *p=='+' || *(p+1)==0))
		{
			//special case - end of string, get last character.
			if (*(p+1)==0)
			{
				term_str[term_len++] = *p;
			}
		
			//null-ternimate existing buffer.
			term_str[term_len] = 0;


			//process previous term

			//create space for 1 new term.
			newsize = (poly1->numTerms + 1) * sizeof(term);

			poly1->terms = (term *) realloc ( poly1->terms, newsize);
			if (!poly1->terms) 
			{
				return; 
			};
			ParseTerm(term_str, &(poly1->terms[poly1->numTerms]));
			poly1->numTerms++;

			printf("term: %s\n", term_str);


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

//will return poly with given name.
poly * FindPoly(char * name, int name_len)
{
	int i;
	for (i=0; i<_num_polys; i++)
	{
		if (strncmp(_poly_arr[i].name,name, name_len)==0)
		{
			return &(_poly_arr[i]);
		}
	}
	return NULL;
}



//prints the poly pointed by p1.
void PrintPoly (poly * p1)
{
	int i;
	float coeff;
	int exp;

	//iterate poly's terms and print them.
	for (i=0; i<p1->numTerms; i++)
	{
		coeff = p1->terms[i].coeff;
		exp = p1->terms[i].exp;

		//print coefficient
		if (coeff==1.0)
		{
			if (exp==0) {printf("1");}; //print coefficient 1 if only number.
		}
		else if (coeff == -1.0)
		{
			if (exp==0) 
			{
				printf("-1");
			}
			else
			{
				printf("-");
			}
		}
		else
		{
			//print + sign from second term onward
			if (i>0)
			{
				printf("%+.2f", coeff); //uses printf '+' flag
			}
			else
			{
				printf("%.2f", coeff);
			}
		}

		//print x and its power
		if (exp==0)
		{
			printf("");
		}
		else if (exp==1)
		{
			printf("x");
		}
		else	//exponent > 1
		{
			printf("x^%d", exp);
		}

		//print space delimiter (always)
		printf (" ");
	}
	printf("\n");

}

void print (char * name, int name_len)
{
	poly * found;
	if ((found = FindPoly(name,name_len))==NULL)
	{
		printf("unknown polynomial %s", name);
		return;
	}

	//if found, call print function
	PrintPoly(found);
}


void define (char * name, int name_len, char * poly_str, int poly_str_len)
{
	
	//check validity of name
	//...

	poly * found = NULL ;
	poly * newPoly = 0;

	//check if name exists.
	if ((found = FindPoly(name,name_len))==NULL)
	{
		//poly doesn't exist, create space for it.
		//create 1 new poly.
		//allocate space
		int newsize = (_num_polys + 1) * sizeof(poly);
		_poly_arr = (poly *) realloc ( _poly_arr, newsize);
		if (!_poly_arr) 
		{
			return; 
		};

		newPoly = &(_poly_arr[_num_polys]);

		//increment number of polys in system
		_num_polys++;

	}
	else 
	{	//poly exists, update its values.
		newPoly = found;
		//clear previous terms.
		free(newPoly->terms);
		newPoly->terms = NULL;
	}

	//update num of terms
	newPoly->numTerms = 0;

	//update new poly's name
	newPoly->name = (char *) malloc(name_len+1);
	strncpy( newPoly->name, name, name_len);
	newPoly->name[name_len] = 0; //null terminate

	//update new poly's terms- starts as null
	newPoly->terms = NULL;

	//parse poly's terms.
	ParsePoly(poly_str, newPoly);

	//sort poly's terms array after insertion
	qsort(newPoly->terms, newPoly->numTerms, sizeof(term), compare_terms);

}

//define polynomial stored in poly1.
void define2(char * name, int name_len, poly * poly1)
{
	(dest_name, dest_name_len, result)
}

//will get names of 2 vars, will add them and store them in variable called dest_name.
void store_sum(char * dest_name, int dest_name_len, 
	char * var1, int var1_len, 
	char * var2, int var2_len)
{

	//call the sum function
	poly * result = sum (var1, var1_len, var2, var2_len) ;

	// return false if failed
	if (result == NULL)
	{
		//
	}
	else 
	{
		//define poly under name
		define2 (dest_name, dest_name_len, result);
	}
	
}

//will sum the 2 variables. caller will print.
poly * sum_op (	char * var1, int var1_len, 
				char * var2, int var2_len)
{
	//create new poly struct
	poly * result = (poly *) malloc (sizeof(poly));

	//find 2 polys by name, if failed return NULL

	//do the summation...
	
	return result;
}


//poly actions algorithms
/////////////////////////
//will recieve 2 polys, p1 and p2, and sum them. stores the result into p3.
//caller is responsible for allocate p3.
//if is_subtract is 1 - will substract, on 0 - will sum.
//assumes all pointers are not null.
void sum (poly * p1, poly * p2, poly * p3, int is_subtract)
{
	
}

//will recieve 2 polys, p1 and p2, and multiply them. stores the result into p3.
//caller is responsible for allocate p3.
//assumes all pointers are not null.
void mult (poly * p1, poly * p2, poly * p3)
{
	
}


//will recieve 1 poly, p1, and compute its derivative. stores the result into p2.
//caller is responsible for allocate p2.
//assumes all pointers are not null.
void derive (poly * p1, poly * p2)
{
	
}


int quit ()
{
	//free all memory.
}

int main()

{
	//get input from user
	char * command = NULL;
	char c;
	int char_cnt = 0;
	int command_type = COMMAND_UNKNOWN;
	char * part1, * part2, * part3;
	int part1_len, part2_len, part3_len;

	while (1)	//continue reading user inputs until end.
	{
		//free buffers related to command
		part1 = part2 = part3 = NULL;
		part1_len = part2_len = part3_len = 0;
		free(command);
		command = (char *) malloc(2);
		char_cnt = 0;

		//print user prompt
		printf("\n>");

		while ((c = getchar()) != '\n')
		{
			char_cnt++;
			if ((command = (char *)realloc(command, (char_cnt*sizeof(char) + 1)))==NULL)
			{
				printf("%s", "Error: could not allocate.\n");
				return 0;
			}

			command[char_cnt-1] = c;
		}

		command[char_cnt] = '\0';

		//do appropriate command
		command_type = parse_req(command, &part1, &part1_len, &part2, &part2_len, &part3, &part3_len);
		switch (command_type)
		{
		case COMMAND_DEF:
			define(part1, part1_len, part2, part2_len);
			break;
		case COMMAND_PRINT:
			print(part1, part1_len);
			break;
		case COMMAND_STORE_SUM:
			store_sum(part1, part1_len, part2, part2_len, part3, part3_len);
		case COMMAND_QUIT:
			quit();
			return 0;
			break;
		case COMMAND_UNKNOWN:
			return 0;
			break;
		}


	}

	
	return 0;
}
