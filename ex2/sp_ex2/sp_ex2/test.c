#include <stdio.h>
#include <string.h>




//int main()
//{
//	test1();
//}

int test1()

{
	char * source = "p8 = -5x^2  -6.0x^8  +9.99x^200   + x^3";
	char dest [200];
	char part1[200];
	char part2[200];
	char * pch;
	char * term;
	char * p;
	int i=0;

	strcpy(dest, source);
	RemoveSpaces(dest);


	sscanf(dest, "%[^\n=]=%[^\n]", part1, part2);

	printf("part 1 : %s\n", part1);
	printf("part 2 : %s\n", part2);



	//split poly into terms

	//using tokenizer
	//printf("the terms are:\n");
	//pch = strtok (part2,"+-");
	//while (pch != NULL)
	//{
	//	printf ("%s,",pch);
	//	pch = strtok (NULL, "+-");
	//}


	//manual
	term = malloc(20); //part 2 size
	p = part2;
	i=0;
	while( *p!= 0)
	{
		printf ("read char: %c\n", *p);

		//reached end of term
		if (*p=='-' || *p=='+')
		{
			term[i] = 0;
			//process full term
			printf("term: %s\n", term);

			//clear for new term
			printf ("start new term\n");
			free(term);
			term = malloc(20);
			//get sign for new term
			term[0] = *p;
			i = 1;
		}
		else
		{
			//continue reading term
			term[i++] = *p;
		}

		p++;
	}

	//process last term.
	term[i] = 0;
	printf("term: %s\n", term);


	return 0;
}