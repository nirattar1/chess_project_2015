#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 

#define COMMAND_PRINT 0
#define COMMAND_DEF 1
#define COMMAND_SUM 2
#define COMMAND_SUBTRACT 3
#define COMMAND_MULT	4
#define COMMAND_DER		5
#define COMMAND_EVAL	6
#define COMMAND_STORE_SUM	7
#define COMMAND_STORE_SUBTRACT	8
#define COMMAND_STORE_MULT	9
#define COMMAND_STORE_DER	10
#define COMMAND_QUIT 11
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
//poly's terms will be sorted by EXP in DESCENDING order
int compare_terms (const void * p1, const void * p2)
{
	const term * t1 = p1;
	const term * t2 = p2;
	return ((t2->exp) - (t1->exp));
}


//try to parse given char pointer as a "store" request
// i.e. p3 = p1 + p2 is a "store" request
//		p3 = x + 7 is a polynomial , not store request definition
// the area checked above by "^" 
int ParseCommandStore(char * command, 
	char ** part1, int * part1_len,
	char ** part2, int * part2_len,
	char ** part3, int * part3_len)
{
	char * p1;
	char * p2;
	char * p_end;

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
		if (**part2 == '-' || isdigit(**part2) || !isalpha(**part2) || **part2 == 'x')
		{
			return COMMAND_DEF;
		}

		//else, try to parse as store command.

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

		//- 
		//try to parse as store sum: p1 - p3
		if ((p2 = strchr(p1, '-'))!=NULL)
		{
			//update 2nd part length.
			*part2_len = (p2-p1);
			*part2 = p1;
			//get 3rd part, after '-'
			p2++;
			*part3_len = (p_end - p2);
			*part3 = p2;
			return COMMAND_STORE_SUBTRACT;
		}

		// *
		//try to parse as store command: p1 * p3
		if ((p2 = strchr(p1, '*'))!=NULL)
		{
			//update 2nd part length.
			*part2_len = (p2-p1);
			*part2 = p1;
			//get 3rd part, after '-'
			p2++;
			*part3_len = (p_end - p2);
			*part3 = p2;
			return COMMAND_STORE_MULT;
		}

		//der - format of command is "der    p1"
		if ((p2 = strstr(command, "der"))!=NULL)
		{
			p2 = p2 + 3;
			*part2_len = (p_end - p2);
			*part2 = p2;
			return COMMAND_STORE_DER;
		}


		//any other case , assume it is DEF command
		return COMMAND_DEF;
	}

	return COMMAND_UNKNOWN;
}


//will get user string and return type of command
// i.e. if sees "eval ..." will know it is of type COMMAND_EVAL
int ParseCommand(char * command, 
								char ** part1, int * part1_len,
								char ** part2, int * part2_len,
								char ** part3, int * part3_len)
{
	
	char * p_end;
	char * p1;
	char * p2;
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
		command_type = ParseCommandStore(command, part1, part1_len, 
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

	//there is not =. the specific command will be determined by unique chars.

	//+  sum
	if ((p1 = strchr(command, '+'))!=NULL)
	{
		RemoveSpaces(command);
		p1 = strchr(command, '+');
		p_end = strchr(command, 0);

		//get 1st part (before "+")
		*part1_len = (p1 - command);
		*part1 = command;

		//get 2nd part (after "+")
		p1++;
		*part2_len = (p_end - p1);
		*part2 = p1;
		return COMMAND_SUM;
	}


	//-
	if ((p1 = strchr(command, '-'))!=NULL)
	{
		RemoveSpaces(command);
		p1 = strchr(command, '-');
		p_end = strchr(command, 0);

		//get 1st part (before "-")
		*part1_len = (p1 - command);
		*part1 = command;

		//get 2nd part (after "-")
		p1++;
		*part2_len = (p_end - p1);
		*part2 = p1;
		return COMMAND_SUBTRACT;
	}


	//*
	if ((p1 = strchr(command, '*'))!=NULL)
	{
		RemoveSpaces(command);
		p1 = strchr(command, '*');
		p_end = strchr(command, 0);

		//get 1st part (before "*")
		*part1_len = (p1 - command);
		*part1 = command;

		//get 2nd part (after "*")
		p1++;
		*part2_len = (p_end - p1);
		*part2 = p1;
		return COMMAND_MULT;
	}


	//der - format is "der    p1"
	if ((p1 = strstr(command, "der"))!=NULL)
	{
		p1 = p1 + 3;
		while (*p1 == ' ')
		{
			p1++;
		}
		p2 = p1;
		while (*p2 != ' ' && *p2 != 0)
		{
			p2++;
		}

		//get 1st part (after der - is the name of the polynomial)
		*part1_len = (p2 - p1);
		*part1 = p1;
		return COMMAND_DER;
	}


	//eval - format is "eval  p1  6.2"
	if ((p1 = strstr(command, "eval"))!=NULL)
	{

		p1 = p1 + 4;
		while (*p1 == ' ')
		{
			p1++;
		}
		p2 = p1;
		while (*p2 != ' ')
		{
			p2++;
		}

		p_end = strchr(command, 0);

		//get 1st part (after eval - is the name of the polynomial)
		*part1_len = (p2 - p1);
		*part1 = p1;

		//get 2nd part (after name of polynomial)
		while (*p2 == ' ')
		{
			p2++;
		}
		*part2_len = (p_end - p2);
		*part2 = p2;
		return COMMAND_EVAL;
	}




	//otherwise, (only name of poly appears)
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
	char * p_hat;

	if (!t1) {return;};


	//term doesn't contain x  (example '5')
	if (strchr(str,'x')==NULL)
	{
		sscanf(str, "%f", &(t1->coeff));
		t1->exp = 0;  //0 exponent (constant number)
		return;
	}

	//handle more cases, term contains x

	//read exponent	
	if ((p_hat = strchr(str,'^'))==NULL)
	{
		t1->exp = 1;
	}
	else
	{
		//read exponent
		sscanf (p_hat, "^%d", &(t1->exp));
	}

	// if x, +x = then parse as 1x
	if ((strncmp(str, "+x", 2)==0)||(strncmp(str, "x", 1)==0))
	{
		t1->coeff = 1.0;
		return;
	}

	// if -x  = then parse as -1x
	if (strncmp(str, "-x", 2)==0)
	{
		t1->coeff = -1.0;
		return;
	}


	//if ax or -ax
	sscanf(str, "%f", &(t1->coeff));
	return;
	

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
		//printf ("read char: %c\n", *p);

		//reached end of term (and not at start)
		if ((*p=='-' && (poly_str!=p) ) || *p=='+' || *(p+1)==0)
		{
			//special case - end of string, get last character.
			if (*(p+1)==0)
			{
				term_str[term_len++] = *p;
			}
		
			//null-ternimate existing buffer.
			term_str[term_len] = 0;


			//process previous term
			if (term_len!=0)
			{
				//create space for 1 new term.
				newsize = (poly1->numTerms + 1) * sizeof(term);

				poly1->terms = (term *) realloc ( poly1->terms, newsize);
				if (!poly1->terms) 
				{
					return; 
				};
				ParseTerm(term_str, &(poly1->terms[poly1->numTerms]));
				poly1->numTerms++;

				//printf("term: %s\n", term_str);

				//clear for new term
				//printf ("start new term\n");
				//get sign for new term
				term_str[0] = *p;
				term_len = 1;

			}

		}
		else
		{
			//continue reading term
			term_str[term_len++] = *p;
		}

		p++;
	}


}


//Init poly
//will delete all memory associated with poly.
void PolyInit (poly *p1)
{
	if (!p1)
	{
		return;
	}

	p1->name = NULL;

	p1->numTerms = 0;


	p1->terms = NULL;

}

//will free the data associated with poly p1.
void PolyFreeData(poly *p1)
{
	if (!p1)
	{
		return;
	}
	//free char pointer if exists.
	if (p1->name)
	{
		free(p1->name);
	}

	//free term pointer if exists.
	if (p1->terms)
	{
		free(p1->terms);
	}
}


//Copy poly
//copy all data in p1 to p2.
void PolyCopySimple (poly *p1, poly *p1_copy)
{
	*p1_copy = *p1;
}

//Copy poly
//will copy all memory associated with poly (deep copy).
//will return pointer to new poly.
//caller is responsible to allocate p1_copy
poly * PolyCopy (poly *p1, poly *p1_copy)
{

	int i=0;
	int size_name ;

	if (!p1)
	{
		return NULL;
	}

	p1_copy = (poly *) malloc(sizeof(poly));
	PolyInit(p1_copy);

	//copy name data if exists.
	if (p1->name)
	{
		size_name = strlen(p1->name);
		p1_copy -> name = (char *) malloc (size_name);
		strcpy(p1_copy->name, p1 -> name );
	}

	//copy terms data, if they exist.
	if (p1->terms)
	{
		p1_copy->terms = (term *) malloc (p1->numTerms * sizeof(term));
		//copy each term.
		for (i=0; i<p1->numTerms; i++)
		{
			//copy the whole term struct
			p1_copy->terms[i] = p1->terms[i];
			p1_copy->numTerms ++;
		}
	}

	return p1_copy;
}




//Free poly.
//

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
	for (i=(p1->numTerms-1); i>=0; i--)
	{
		coeff = p1->terms[i].coeff;
		exp = p1->terms[i].exp;

		//print coefficient
		if (coeff==1.0)
		{
			if (i!=(p1->numTerms-1))
			{
				printf("+");
			}

			if (exp==0) 
			{
				printf("1");
			}
			
			//print coefficient 1 if only number.
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
		else if (coeff == 0)
		{
			//do not print terms with 0
			continue;
		}
		else
		{
			//print + sign from second term onward
			if (i!=(p1->numTerms-1))
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
			//do nothing
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

void command_print (char * name, int name_len)
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


//will get pointer to poly, and its name. 
//will add it to poly's array.
//caller already allocated p1.
void AddUpdatePoly (poly * p1, char * name, int name_len)
{

	//check validity of name
	//...

	poly * found = NULL;
	poly * newPoly = 0;

	//check if poly exists (by name).

	//if not exist
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

	PolyInit(newPoly);


	//copy terms data from p1 to poly
	PolyCopySimple (p1, newPoly);


	//update new poly's name
	newPoly->name = (char *) malloc(name_len+1);
	strncpy( newPoly->name, name, name_len);
	newPoly->name[name_len] = 0; //null terminate


}

void define (char * name, int name_len, char * poly_str, int poly_str_len)
{
	
	//check validity of name
	//...

	poly * found = NULL ;
	poly * newPoly = 0;
	int update;
	int newsize;

	//check if name exists.
	if ((found = FindPoly(name,name_len))==NULL)
	{
		update = 0;
		//poly doesn't exist, create space for it.
		//create 1 new poly.
		//allocate space
		newsize = (_num_polys + 1) * sizeof(poly);
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
		update = 1;
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

	if (update==0)
	{
		printf("created %.*s\n" , name_len, name);
	}
	else
	{
		printf("updated %.*s\n" , name_len, name);
	}

}



//poly actions algorithms
/////////////////////////
//will recieve 2 polys, p1 and p2, and sum them. stores the result into p3.
//caller is responsible for allocate&init p3.
//if is_subtract is 1 - will substract, on 0 - will sum.
//assumes all pointers are not null.
void sum (poly * p1, poly * p2, poly * p3, int is_subtract)
{
	int i=0, j=0;
	int cnt_terms_p3 = 0;
	float sign = (is_subtract) ? -1.0 : 1.0;

	//p3 has already been Init. (0 terms)

	while (i < p1->numTerms || j < p2->numTerms)
	{

		//allocate space for new term in p3
		p3->terms = (term *) realloc (p3->terms, ((cnt_terms_p3 + 1) * sizeof (term)) );
		//TODO write on fail.


		//p1 exp is bigger.
		if (p1->terms[i].exp > p2->terms[j].exp)
		{
			//write new value (copy the term from p1)
			p3->terms[cnt_terms_p3] = p1->terms[i];

			//increment i
			i++;
		}

		//p2 exp is bigger.
		else if (p1->terms[i].exp < p2->terms[j].exp)
		{
			//write new value (copy the term from p2)
			p3->terms[cnt_terms_p3].exp = p2->terms[j].exp;
			p3->terms[cnt_terms_p3].coeff = sign * p2->terms[j].coeff;

			//increment j
			j++;
		}

		//exp are equal
		else
		{
			p3->terms[cnt_terms_p3].exp = p1->terms[i].exp;
			p3->terms[cnt_terms_p3].coeff = p1->terms[i].coeff + sign * p2->terms[j].coeff;

			//increment both
			i++; 
			j++;
		}

		p3->numTerms++;
		cnt_terms_p3++;
	}
}

//will receive 2 polys, p1 and p2, and multiply them. stores the result into p3.
//caller is responsible for allocate p3.
//assumes all pointers are not null.
void mult (poly * p1, poly * p2, poly * p3)
{
	int i=0, j=0;
	poly pTemp; //will hold sum of 1 'row'
	poly * p3_copy;

	for (i=0; i < p1->numTerms; i ++)
	{
		PolyInit(&pTemp);

		//allocate space for new terms on pTemp
		//(there will be exactly as number of p2's terms)
		pTemp.terms = (term *) malloc (p2->numTerms * sizeof (term)) ;

		for (j=0; j < p2->numTerms; j++)
		{
			pTemp.terms[j].coeff = p1->terms[i].coeff * p2->terms[j].coeff;
			pTemp.terms[j].exp = p1->terms[i].exp + p2->terms[j].exp;
			pTemp.numTerms++;
		}

		if (p3->numTerms==0)
		{
			p3 = PolyCopy(&pTemp, p3);
		}
		else
		{
			p3_copy = PolyCopy(p3, p3_copy);
			sum (p3_copy, &pTemp, p3, 0);
		}
	}

}


//will recieve 1 poly, p1, and compute its derivative. stores the result into p2.
//caller is responsible for allocate p2.
//assumes all pointers are not null.
void derive (poly * p1, poly * p2)
{

	int i=0;
	int cnt_terms_p2 = 0; 
	term * curr_term = NULL;

	for (i=0; i<p1->numTerms; i++)
	{
		curr_term = &(p1->terms[i]);
		if (curr_term->exp == 0)
		{
			continue;
		}

		//allocate space for new term in p2
		p2->terms = (term *) realloc (p2->terms, ((cnt_terms_p2 + 1) * sizeof (term)) );

		//fill new term
		p2->terms[cnt_terms_p2].coeff = (curr_term->coeff)*(curr_term->exp);
		p2->terms[cnt_terms_p2].exp = curr_term->exp - 1;

		p2->numTerms++;
		cnt_terms_p2++;
	}
}

//will recieve 2 pointers for poly's. if 1 of them not found, will print error with its name.
int PrintIfNotFound(poly * p1, poly * p2,
					char * var1, int var1_len,
					char * var2, int var2_len)
{
	//one of them not found
	if (!p1)
	{
		printf("unknown polynomial %.*s\n", var1_len, var1);
		return 0;
	}
	if (!p2)
	{
		printf("unknown polynomial %.*s\n", var2_len, var2);
		return 0;
	}
	return 1;
}



//will get names of 2 vars.
//1. will sums/substracts them
//2. will store them in new variable called dest_name.
//will first try to find them in system, if not exist will exit.
//prints result at the end.
//if is_subtract is 1 - will substract, on 0 - will sum.
void command_store_sum(char * dest_name, int dest_name_len, 
	char * var1, int var1_len, 
	char * var2, int var2_len, int is_subtract)
{

	//create new poly on heap
	poly result;

	//find 2 polys by name, if failed return NULL
	poly * p1 = FindPoly(var1, var1_len);
	poly * p2 = FindPoly(var2, var2_len);

	//init result
	PolyInit(&result);

	if (!PrintIfNotFound(p1, p2, var1, var1_len, var2, var2_len))
	{
		return;
	}

	//both of them are in system.
	//do the summation, will be stored in "result".
	sum (p1, p2, &result, is_subtract);

	//add result
	AddUpdatePoly(&result, dest_name, dest_name_len);

}

//recives names of 2 variables, and sums/substracts them.
//will first try to find them in system, if not exist will exit.
//prints result at the end.
//if is_subtract is 1 - will substract, on 0 - will sum.
void command_sum (	char * var1, int var1_len, 
	char * var2, int var2_len, int is_subtract)
{
	//create new poly on stack, init it.
	poly result;

	//find 2 polys by name, if failed return NULL
	poly * p1 = FindPoly(var1, var1_len);
	poly * p2 = FindPoly(var2, var2_len);

	//init result
	PolyInit(&result);

	if (!PrintIfNotFound(p1, p2, var1, var1_len, var2, var2_len))
	{
		return;
	}

	//both of them are in system.
	//do the summation, will be stored in "result".
	sum (p1, p2, &result, is_subtract);

	//print result
	PrintPoly(&result);


}


//eval command
void command_eval (	char * var1, int var1_len, 
					char * var2, int var2_len)
{

	int i;
	float t;
	term * curr_term = NULL;
	//find poly by name, if failed return NULL
	poly * p1 = FindPoly(var1, var1_len);
	float result;

	if (!p1)
	{
		printf("unknown polynomial %.*s\n", var1_len, var1);
		return;
	}

	//if here, poly is in system. 

	//read value of 't' into float
	sscanf(var2, "%f", &t);
	result = 0;

	for (i=0; i<p1->numTerms; i++)
	{
		curr_term = &(p1->terms[i]);
		result += ((float) pow(t, curr_term->exp))*curr_term->coeff;
	}
	//print result
	printf("%.2f\n", result);


}



//derive command
void command_der ( char * var1, int var1_len )
{	
	//create new poly on stack, init it.
	poly result;
	poly * p1;
	
	//init result
	PolyInit(&result);

	//find poly by name, if failed return NULL
	p1 = FindPoly(var1, var1_len);
	if (!p1)
	{
		printf("unknown polynomial %.*s\n", var1_len, var1);
		return;
	}

	//p1 is in the system.
	//do the derivative, will be stored in "result".
	derive(p1, &result);

	//print result
	PrintPoly(&result);
}


//derive store command : p2 = der p1
void command_store_der ( char * dest_name, int dest_name_len,
						char * var1, int var1_len )
{	
	//create new poly on stack, init it.
	poly result;
	poly * p1;

	//init result
	PolyInit(&result);

	//find poly by name, if failed return NULL
	p1 = FindPoly(var1, var1_len);
	if (!p1)
	{
		printf("unknown polynomial %.*s\n", var1_len, var1);
		return;
	}

	//p1 is in the system.
	//do the derivative, will be stored in "result".
	derive(p1, &result);

	//add result
	AddUpdatePoly(&result, dest_name, dest_name_len);
}

//recives names of 2 variables, and multiplies them.
//will first try to find them in system, if not exist will exit.
//prints result at the end.
void command_mult (	char * var1, int var1_len, 
						char * var2, int var2_len)
{
	//create new poly on stack, init it.
	poly result;

	//find 2 polys by name, if failed return NULL
	poly * p1 = FindPoly(var1, var1_len);
	poly * p2 = FindPoly(var2, var2_len);

	//init result
	PolyInit(&result);

	if (!PrintIfNotFound(p1, p2, var1, var1_len, var2, var2_len))
	{
		return;
	}

	//both of them are in system.
	//do the summation, will be stored in "result".
	mult(p1, p2, &result);

	//print result
	PrintPoly(&result);

}



void quit ()
{
	//free all memory.

	int i;
	for (i = 0; i < _num_polys ; i++)
	{
		//clear data asspciated with all polys
		PolyFreeData(&(_poly_arr[i]));
	}

	//clear polys array
	free(_poly_arr);

	_poly_arr = NULL;
	_num_polys = 0;
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

	printf("Welcome to Polynomials!\n");

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
		command_type = ParseCommand(command, &part1, &part1_len, &part2, &part2_len, &part3, &part3_len);
		switch (command_type)
		{
		case COMMAND_DEF:
			define(part1, part1_len, part2, part2_len);
			break;
		case COMMAND_PRINT:
			command_print(part1, part1_len);
			break;
		case COMMAND_SUM:
			command_sum(part1, part1_len, part2, part2_len, 0);
			break;
		case COMMAND_SUBTRACT:
			//just call "command_sum" with subtract flag
			command_sum(part1, part1_len, part2, part2_len, 1);
			break;
		case COMMAND_MULT:
			command_mult(part1, part1_len, part2, part2_len);
			break;
		case COMMAND_STORE_SUM:
			command_store_sum(part1, part1_len, part2, part2_len, part3, part3_len, 0);
			break;
		case COMMAND_STORE_SUBTRACT:
			command_store_sum(part1, part1_len, part2, part2_len, part3, part3_len, 1);
			break;
		case COMMAND_EVAL:
			command_eval(part1, part1_len, part2, part2_len);
			break;
		case COMMAND_DER:
			command_der(part1, part1_len);
			break;
		case COMMAND_STORE_DER:
			command_store_der(part1, part1_len, part2, part2_len);
			break;
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
