#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <i86.h>
#include "config.h"

char inkey;
int music[8] = {4500, 2100, 2572, 1246, 1472, 5721, 2127, 1222};
int music2[12] = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600};

void intests(void)
{
    long i;
    
     	printf("Hi!\n");
	printf("This is a test program.\n");
	if (musicsource == 1) {
		for (i=0;i<=12;i++)
		{
	    		sound(music2[i]);

	    		delay(200);
		}
		nosound();
	}
	getch();
}

test()
{
        long i;

        if (sfxcard == 1) {
                	printf("now lets make a PC Speaker sound...\n");
                	getch();
                	sound(1000);
			delay(500);
			nosound();
        	}
        if (musicsource == 1) {
                printf("now lets play a short song using PC Speaker...\n");
                getch();
                for (i=0;i<=8;i++)
                {
                        sound(music[i]);
                        delay(200);
                }
                nosound();
        }
        if (grafmode == 0)
        {
                printf("now lets enter mode 13h....\n");
                getch();
                _asm {
		mov ax,0013h
                int 10h
		}
                intests();
                _asm {
		mov ax,0003h
		int 10h
		}
        }
        if (grafmode == 1)
        {
                printf("now lets enter CGA 320 x 200 mode....\n");
                getch();
                _asm
		{
		mov ax,0004h
                int 10h
		}
		intests();
                _asm {
		mov ax,0003h
		int 10h
		}
        }
        printf("That's all folks!\n");
        exit(0);
}

main()
{
        /* Remember to check for errors! */
        if (loadconfig() != 0)
        {
                printf("Error loading configuration.\n");
                exit(1);
        }
        printf("TEST program for my SETUP program.\n");
        printf("You must set PC Speaker as for sound and music.\n");
        printf("Press Q to quit, C to continue.\n");
	READKEY:
	inkey = getch();
        switch (inkey)
        {
                case 'q':
                exit(0);
                break;
                case 'c':
                test();
                break;
		default:
		goto READKEY;
		break;
        }        
}
