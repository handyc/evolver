#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define VERSION "3.0"
#define YEAR "2000"
#define AUTHOR "Christopher Handy"

#define SIZE 29
#define MUTCHANCE 7 // default mutation chance (usually invisible)
#define MAXCHILD 2000 // default children per generation

#define PROMPT_TARGET_1 "\nTarget strings may contain English characters and the underscore."
#define PROMPT_TARGET_2 "\nI recommend a mutation percentage between 5 and 10."
#define PROMPT_TARGET_3 "\n\nEnter target string (%d characters max): ", SIZE-1

#define PROMPT_MUTATION "\nMutation percentage (0-100): "

#define PROMPT_CHILD "\nNumber of children per generation (maximum at %d): ", MAXCHILD

#define PROMPT_DELAY "\nEnormous delay (milliseconds):"

#define POT 27

#define XOR ^=

#define BUF_SIZE 40 // this is the original size of the buffer (bytes)
#define NEW_ALLOC 20 // this is added on to the buffer with each realloc


int action(char array[], char target[], int mrbill);
int mutate(void);
int sort(int scores[], char maxchild);
void wipe(int array[], char maxchild);
char* failSafeRead(FILE* infile); // read a line (any length) from a file

char mutchance=MUTCHANCE; // 0
char maxchild=MAXCHILD;
int mrbill;
int sleepiness;


main()
{
char *target;
char *winner;
int *c;
char **ch;
int *winchild;
int x;
unsigned long gen=0;

setbuf(stdout, (char *)NULL);
setbuf(stdin,  (char *)NULL);

target=NULL;
winner=NULL;
c=NULL;
ch=NULL;
winchild=NULL;

fprintf(stdout, PROMPT_TARGET_1); // print a prompt to user screen
fprintf(stdout, PROMPT_TARGET_2); // print a prompt to user screen
fprintf(stdout, PROMPT_TARGET_3); // print a prompt to user screen
target = (char *)failSafeRead(stdin); // read user input
mrbill=strlen(target);
*(target+mrbill-1)=NULL;

fprintf(stdout, PROMPT_MUTATION); // print a prompt to user screen
//mutchance = (char *)failSafeRead(stdin); // read user input
scanf("%d", &mutchance);

fprintf(stdout, PROMPT_CHILD); // print a prompt to user screen
//maxchild = (char *)failSafeRead(stdin); // read user input
scanf("%d", &maxchild);

fprintf(stdout, PROMPT_DELAY); // print a prompt to user screen
//sleepiness = (char *)failSafeRead(stdin); // read user input
scanf("%d", &sleepiness);

randomize();

if ((winner = (char *) malloc(mrbill)) == NULL) // request memory
{
	printf("Not enough memory to allocate buffer\n");
	exit(1);  /* terminate program if out of memory */
}
*(winner+mrbill)=NULL;

for(x=0;target[x]!=NULL;x++)
   {
   *(target+x)=toupper(target[x]);
   *(winner+x)=(random(POT)+65);
   if(*(winner+x)==POT+64) *(winner+x)='_';
   //if(initstring[x]==(26+65)) initstring[x]='R';
   }

while(strcmp(winner, target))
{

wipe(c, maxchild);


for(x=0;x<maxchild;x++)
{
strcpy(ch[x], winner);
memcpy(ch[x], winner, strlen(winner));



c[x]=action(ch[x], target, mrbill);
}

winchild=sort(c, maxchild);
strcpy(winner, ch[winchild]);

printf("\nWinner for %s is %s", target, winner);
delay(sleepiness);
gen++;
}

printf("\nSolved for %s in %lu generations", target, gen);
printf("\n\nEvolver v");
printf(VERSION);
printf(" (c) ");
printf(AUTHOR);
printf(" ");
printf(YEAR);

return 0;
}

int action(char array[], char target[], int mrbill)
{
int score=0, x;

 //do{
 for(x=0; x<mrbill; x++)
 {
 if (mutate()) // if we mutate
 array[x]=random(POT)+65; // set this character to a random uppercase
			  // character from A to Z

 if (array[x]==POT+64) array[x]='_'; // if the character is


 }

for(x=0; x<mrbill; x++)

if(array[x]==target[x]) score--;

return score;

}

int mutate(void)
{
if(random(100) < mutchance) return 1;
			    return 0;
}

int sort(int scores[], char maxchild)
{
int temp=0, x;

for(x=1;x<maxchild;x++)
if(scores[x]<scores[temp]) temp=x;

return temp;
}

void wipe(int array[], char maxchild)
{
int x;
for(x=0;x<maxchild;x++)
array[x]=0;
}


char* failSafeRead(FILE* inFile)
{
char *buf; // buffer for input from file
int bufcounter=0; // counts number of chars read, resets on realloc
int realcounter=0; // counts number of chars read, doesn't reset
int buf_size=BUF_SIZE; // initial buffer size is set to BUF_SIZE
char ch; // used for character read

ch=fgetc(inFile); // grab a char from the file
if(ch==EOF) return NULL; // Is this EOF?  If so, stop!

 ////
  if ((buf = (char *) malloc(buf_size)) == NULL) // request memory
{
	printf("Not enough memory to allocate buffer\n");
	exit(1);  /* terminate program if out of memory */
}

buf[0]=ch; // first character read (from EOF test) placed into buffer

	do
	{
		if(bufcounter==(BUF_SIZE-2)) // are we overflowing?
		{
		buf_size+=NEW_ALLOC; // increase the buffer
		buf = (char *) realloc(buf, buf_size); // and reallocate
		bufcounter=(BUF_SIZE-NEW_ALLOC); // reset the counter to
		}                         // remember when to reallocate

		ch = fgetc(inFile); // grab a char

		bufcounter++;
		realcounter++;

		buf[realcounter]=ch; // drop it in the buffer



	}while (ch != EOF && ch!='\n'); // are we done with the file
					// and/or the line?

buf[realcounter+1]=NULL;
return buf; // return the string
}