#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <dos.h>

#define VERSION "1.0"
#define YEAR "2002"
#define AUTHOR "Christopher Handy"
#define TITLE "Seed Reflector"


#define SIZE 29
#define MUTCHANCE 7 // default mutation chance (usually invisible)
#define MAXCHILD 2000 // change back to 2000 when bug fixed

#define COLORS 64 // colors=pot
#define POT 64

#define XOR ^=

int action(char array[], char target[]);
int mutate(void);
int sort(int scores[]);
void wipe(int array[]);

char mutchance=MUTCHANCE; // 0
int mrbill;
int sleepiness;

main()
{
char target[SIZE], winner[SIZE];
int c[MAXCHILD];
char ch[MAXCHILD][SIZE];
int winchild;
int x;
unsigned long gen=0;

wipe(c);

//for(x=0;x<SIZE-1;x++)
//{
//target[x]='_';
//winner[x]='_';
//}

printf("\nTarget strings may contain English characters and the underscore.");
printf("\nI recommend a mutation percentage between 5 and 10.");
printf("\n\nEnter target string (%d characters max): ", SIZE-1);
scanf("%s", &target);

printf("\nMutation percentage (0-100): ");
scanf("%d", &mutchance);

printf("\nEnormous delay (milliseconds):");
scanf("%d", &sleepiness);

randomize();

//target[SIZE-1]=NULL;

//winner[strlen(target)]=NULL;
mrbill=strlen(target);

winner[mrbill]=NULL;

for(x=0;target[x]!=NULL;x++)
   {
   target[x]=toupper(target[x]);
   winner[x]=(random(POT)+65);
   if(winner[x]==POT+64) winner[x]='_';
   //if(initstring[x]==(26+65)) initstring[x]='R';
   }

while(strcmp(winner, target))
{

wipe(c);


for(x=0;x<MAXCHILD;x++)
{
strcpy(ch[x], winner);
c[x]=action(ch[x], target);
}

winchild=sort(c);
strcpy(winner, ch[winchild]);

//printf("\nWinner for %s is %s", target, winner);

  display_gene(target, 50, 50);
  display_gene(winner, 120, 50);


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


void setmode(unsigned char mode)
     {
	  /* Sets the video display mode */

	  union REGS inreg, outreg;

	  inreg.h.ah = 0;
	  inreg.h.al = mode;
	  int86(0x10,&inreg,&outreg);
     }

     void gopixel(unsigned int x, unsigned char color)
     {
	  /* Direct plot in mode 19 ONLY */
	  char far *video;

	  video = MK_FP(0xA000,0);
	  video[x] = color;
}

display_gene(char *array)
{


    for(codon=0;array[codon]!=STOP_CODON;codon++)
    {
    for (y=0;y<200;y++)

    gopixel(x+(y*320),color);
    }


}

int action(char array[], char target[])
{
int score=0, x;

 //do{
 for(x=0; x<mrbill; x++)
 {
 if (mutate()) // if we mutate
 array[x]=random(POT)+65; // set this character to a random uppercase
			  // character from A to Z

 if (array[x]==POT+64) array[x]='_'; // if the character is

 //if(mutate()) array[x] XOR 1;
 //if(mutate()) array[x] XOR 2;
 //if(mutate()) array[x] XOR 4;
 //if(mutate()) array[x] XOR 8;
 //if(mutate()) array[x] XOR 16;
 }  //}while( (array[x]<'A' || array[x]>'Z') && array[x]!='_');

for(x=0; x<mrbill; x++)
//score+= abs(array[x]-target[x]);
if(array[x]==target[x]) score--;

return score;

}

int mutate(void)
{
if(random(100) < mutchance) return 1;
			    return 0;
}

int sort(int scores[])
{
int temp=0, x;

for(x=1;x<MAXCHILD;x++)
if(scores[x]<scores[temp]) temp=x;

return temp;
}

void wipe(int array[])
{
int x;
for(x=0;x<MAXCHILD;x++)
array[x]=0;
}






void setmode(unsigned char mode);
void gopixel(unsigned int x, unsigned char color);
void read_DNA(char DNA_string);

unsigned int x;
unsigned char y;
unsigned char DNA_string[DNA_size];
unsigned char user_input;

main()
{

setmode(19); //320x200x256

for(x=1;x<COLORS;x++)
{
outportb(0x3c8,x);  //DAC
outportb(0x3c9,x);  //RED
outportb(0x3c9,x);  //GREEN
outportb(0x3c9,x);  //BLUE
}

//user_input=getch();
// switch (user_input) {
//  case '1': copy_to_parent(array  break;
//  case '2':   break;
//  case '3':   break;
//  case '4':   break;
//  case '5':   break;
//  case '6':   break;
//  case '7':   break;
//  case '8':   break;
//  case '9':   break;
//  default:         printf("Bug!\n");

//  exit(1);
//}


setmode(3);
return 0;
 }

