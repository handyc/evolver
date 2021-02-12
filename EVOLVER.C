#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#define SIZE 29
//#define MUTCHANCE 15
#define MAXCHILD 2000

#define POT 27

#define XOR ^=

int action(char array[], char target[]);
int mutate(void);
int sort(int scores[]);

char mutchance=0;

main()
{
char target[SIZE], winner[SIZE];
int c[MAXCHILD];
char ch[MAXCHILD][SIZE];
int winchild;
int x;
unsigned long gen=0;

for(x=0;x<MAXCHILD;x++)
c[x]=0;

printf("\nTarget strings may contain English characters and the underscore.");
printf("\nI recommend a mutation percentage between 5 and 10.");
printf("\n\nEnter target string (%d characters max): ", SIZE-1);
scanf("%s", &target);
printf("\nMutation percentage (0-100): ");
scanf("%d", &mutchance);
randomize();

target[SIZE-1]=NULL;
winner[strlen(target)]=NULL;

for(x=0;target[x]!=NULL;x++)
   {
   target[x]=toupper(target[x]);
   winner[x]=(random(POT)+65);
   if(winner[x]==POT+64) winner[x]='_';
   //if(initstring[x]==(26+65)) initstring[x]='R';
   }

while(strcmp(winner, target))
{

for(x=0;x<MAXCHILD;x++)
c[x]=0;


for(x=0;x<MAXCHILD;x++)
{
strcpy(ch[x], winner);
c[x]=action(ch[x], target);
}

winchild=sort(c);
strcpy(winner, ch[winchild]);

printf("\nWinner for %s is %s", target, winner);
gen++;
}

printf("\nSolved for %s in %lu generations", target, gen);
printf("\nEvolver v1.0 (c) Christopher Handy 1998");

return 0;
}

int action(char array[], char target[])
{
int score=0, x;

 //do{
 for(x=0; x<SIZE-1; x++)
 {
 if (mutate()) array[x]=random(POT)+65;
 if (array[x]==POT+64) array[x]='_';

 //if(mutate()) array[x] XOR 1;
 //if(mutate()) array[x] XOR 2;
 //if(mutate()) array[x] XOR 4;
 //if(mutate()) array[x] XOR 8;
 //if(mutate()) array[x] XOR 16;
 }  //}while( (array[x]<'A' || array[x]>'Z') && array[x]!='_');

for(x=0; x<SIZE-1; x++)
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