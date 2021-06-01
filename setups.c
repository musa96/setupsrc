#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <dos.h>
#include <process.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <i86.h>

volatile unsigned char readch, oldreadch, extended, keystatus[256];
char inkey, keys[18] = {0xc8, 0xd0, 0xcb, 0xcd, 0x33, 0x34, 0x39, 0x2a,
    			0xc9, 0xd1, 0xc7, 0x1e, 0x2c, 0x1d, 0x1a, 0x1b,
			0x1c, 0xe};
int grafmode, sfxcard, musicsource, inputdevice;
int screenwidth, screenheight;
long setupver;
char *keystring[18] =
{
	"Press a key to move foreward",
	"Press a key to move backward",
	"Press a key to turn left",
	"Press a key to turn right",
	"Press a key to strafe left",
	"Press a key to strafe right",
	"Press a key to open doors",
	"Press a key to run",
	"Press a key to look up",
	"Press a key to look down",
	"Press a key to look centre",
	"Press a key to jump",
	"Press a key to crouch",
	"Press a key to fire weapon",
	"Press a key to previous inventory",
	"Press a key to next inventory",
	"Press a key to use inventory",
	"Press a key to drop inventory"
};
FILE *cfgfil;

void (_interrupt _far *oldkeyhandler)();
void _interrupt _far keyhandler(void);

#define CFGFILE "SPRK7.SET"
#define OLDCFGFILE "SPRK7.OLD"
#define RUNEXE "SPRK7.EXE"

void _interrupt _far keyhandler()
{
	oldreadch = readch;
	_asm
	{
		in al, 60h
		mov readch, al
		in al, 61h
		or al, 80h
		out 61h, al
		and al, 7fh
		out 61h, al
	}
	if ((readch|1) == 0xe1)
		extended = 128;
	else
	{
		if (oldreadch != readch)
		{
			if ((readch&128) == 0)
			{
				if (keystatus[(readch&127)+extended] == 0)
					keystatus[(readch&127)+extended] = 1;
			}
			else
				keystatus[(readch&127)+extended] = 0;
		}
		extended = 0;
	}
	_asm
	{
		mov al, 20h
		out 20h, al
	}
}

int getsymbol(char *str)
{
	char buffer[80], tmp[80], tmp2[80];
	int value;
	
	while (fgets(buffer, 80, cfgfil) != NULL)
	{
		sscanf(buffer, "%s", tmp);
		if (strstr(str, tmp) != 0)
		{
			sscanf(buffer, "%s = %s", tmp, tmp2);
			value = atol(tmp2);
			return(value);
		}
	}
	return(-1);
}

void writesymbol(char *str, int value)
{
	fprintf(cfgfil, "%s = %d\n", str, value);
}

void clrscr(void)
{
    _asm
    {
	mov ax,04h
	int 10h
    }
}

int load()
{
	char tempbuf[80];
	long i;
	
        printf("Loading changes...\n");
	cfgfil = fopen(CFGFILE, "r");
	if (cfgfil == NULL) return -1;
	setupver = getsymbol("setupver");
        if (setupver != 0x0f) {
                setupver = 0x0f;
                printf("This is from an older version of Setup!\n");
                printf("Your settings will not be read.\n");
                printf("But your settings will be backed up.\n");
                getch();
                fclose(cfgfil);
                rename(CFGFILE, OLDCFGFILE);
                return -1;
        }
        else {
	grafmode = getsymbol("grafmode");
	sfxcard = getsymbol("sfx");
	musicsource = getsymbol("music");
	inputdevice = getsymbol("input");
	screenwidth = getsymbol("screenwidth");
	screenheight = getsymbol("screenheight");
	for(i=0;i<18;i++)
	{
		sprintf(tempbuf, "key%i", i);
		keys[i] = getsymbol(tempbuf);
	}
        fclose(cfgfil);
        return 0;
        }
}

int save()
{
	char tempbuf[80];
	long i;
	
        printf("Saving changes...\n");
	cfgfil = fopen(CFGFILE, "w");
	if (cfgfil == NULL) return -1;
	writesymbol("setupver", 0x0f);
	writesymbol("grafmode", grafmode);
	writesymbol("sfx", sfxcard);
	writesymbol("music", musicsource);
	writesymbol("input", inputdevice);
	writesymbol("screenwidth", screenwidth);
	writesymbol("screenheight", screenheight);	
	for(i=0;i<18;i++)
	{
		sprintf(tempbuf, "key%i", i);
		writesymbol(tempbuf, keys[i]);
	}
        fclose(cfgfil);
        return 0;
}

void warpto(int episode)
{
	char tempbuf[10], tempbuf2[10];

        clrscr();
        printf(";-------------------------------------;\n");
        printf(";      Micheal's Setup Program v1.6   ;\n");
        printf(";                                     ;\n");
        printf(";            Warp To?	              ;\n");
        printf(";                                     ;\n");
        printf(";         1. Map %d		      ;\n",episode);
        printf(";         2. Map %d		      ;\n",episode+1);
        printf(";         3. Map %d		      ;\n",episode+2);
        printf(";         4. Map %d		      ;\n",episode+3);
        printf(";         5. Map %d		      ;\n",episode+4);
	if (episode < 27)
	{
        	printf(";         6. Map %d		      ;\n",episode+5);
        	printf(";         7. Map %d		      ;\n",episode+6);
        	printf(";         8. Map %d		      ;\n",episode+7);
        	printf(";         9. Map %d		      ;\n",episode+8);
	}
        printf(";                                     ;\n");
	if (episode > 1)
		printf(";	< to go to next section	      ;\n",episode-1);	
	if (episode < 30)
		printf(";	> to go to previous section   ;\n",episode+1);
        printf(";                                     ;\n");
	if (episode < 27)
 	       printf(";       Press choice (1-9)            ;\n");
	else
 	       printf(";       Press choice (1-5)            ;\n");
	printf(";-------------------------------------;\n");
        inkey = getch();
        switch (inkey)
        {
	    	case 27:
			clrscr();
			setup_main();
			break;
                case '1':
			_asm
			{
				mov ax,0003h
				int 10h
			}
			sprintf(tempbuf, "%d", episode);
                        spawnl(P_WAIT, RUNEXE, RUNEXE,"-m", tempbuf, NULL);
			exit(0);
                        break;
                case '2':
			_asm
			{
				mov ax,0003h
				int 10h
			}
			sprintf(tempbuf, "%d", episode+1);
                        spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
			exit(0);
                        break;
                case '3':
			_asm
			{
				mov ax,0003h
				int 10h
			}
			sprintf(tempbuf, "%d", episode+2);
                        spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
			exit(0);
                        break;
                case '4':
			_asm
			{
				mov ax,0003h
				int 10h
			}
			sprintf(tempbuf, "%d", episode+3);
                        spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
			exit(0);
                        break;
                case '5':
			_asm
			{
				mov ax,0003h
				int 10h
			}
			sprintf(tempbuf, "%d", episode+4);
                        spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
			exit(0);
                        break;
                case '6':
			if (episode < 27)
			{
				_asm
				{
					mov ax,0003h
					int 10h
				}
				sprintf(tempbuf, "%d", episode+5);
                        	spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
				exit(0);
			}
               	 	clrscr(); 
                	warpto(episode);
                        break;
                case '7':
			if (episode < 27)
			{
				_asm
				{
					mov ax,0003h
					int 10h
				}
				sprintf(tempbuf, "%d", episode+6);
                        	spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
				exit(0);
			}
               	 	clrscr(); 
                	warpto(episode);
                        break;
                case '8':
			if (episode < 27)
			{
				_asm
				{
					mov ax,0003h
					int 10h
				}
				sprintf(tempbuf, "%d", episode+7);
                        	spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
				exit(0);
			}
               	 	clrscr(); 
                	warpto(episode);
                        break;
                case '9':
			if (episode < 27)
			{
				_asm
				{
					mov ax,0003h
					int 10h
				}
				sprintf(tempbuf, "%d", episode+8);
                        	spawnl(P_WAIT, RUNEXE, RUNEXE, "-m", tempbuf, NULL);
				exit(0);
			}
               	 	clrscr(); 
                	warpto(episode);
                        break;
		case 46:
		if (episode < 27)
			warpto(episode+9);
		else
			warpto(episode);
		break;
		case 44:
		if (episode > 1)
			warpto(episode-9);
		else
			warpto(episode);
		break;
                default: 
                clrscr(); 
                warpto(episode);
                break;
        }
}

void vesamode(void)
{
        clrscr();
        printf(";-------------------------------------;\n");
        printf(";      Micheal's Setup Program v1.6   ;\n");
        printf(";                                     ;\n");
        printf(";            Graphics mode            ;\n");
        printf(";                                     ;\n");
        printf(";         1. 320 x 200                ;\n");
        printf(";         2. 320 x 400                ;\n");
        printf(";         3. 640 x 400                ;\n");
        printf(";         4. 640 x 480                ;\n");
        printf(";         5. 800 x 600                ;\n");
        printf(";         6. 1024 x 768               ;\n");
        printf(";                                     ;\n");
        printf(";       Press choice (1-6)            ;\n");
        printf(";-------------------------------------;\n");
        inkey = getch();
        switch (inkey)
        {
	    	case 27:
			clrscr();
			setup_main();
			break;
                case '1':
			screenwidth = 320;
			screenheight = 200;
                        clrscr();
                        setup_main();
                        break;
                case '2':
			screenwidth = 320;
			screenheight = 400;
                        clrscr();
                        setup_main();
                        break;
                case '3':
			screenwidth = 640;
			screenheight = 400;
                        clrscr();
                        setup_main();
                        break;
                case '4':
			screenwidth = 640;
			screenheight = 480;
                        clrscr();
                        setup_main();
                        break;
                case '5':
			screenwidth = 800;
			screenheight = 600;
                        clrscr();
                        setup_main();
                        break;
                case '6':
			screenwidth = 1024;
			screenheight = 768;
                        clrscr();
                        setup_main();
                        break;
                default: 
                clrscr(); 
                vesamode();
                break;
        }
}

void graf(void)
{
        clrscr();
        printf(";-------------------------------------;\n");
        printf(";      Micheal's Setup Program v1.6   ;\n");
        printf(";                                     ;\n");
        printf(";            Graphics mode            ;\n");
        printf(";                                     ;\n");
        printf(";         1. VGA 320 x 200 mode       ;\n");
        printf(";         2. VESA mode                ;\n");
        printf(";                                     ;\n");
        printf(";       Press choice (1-2)            ;\n");
        printf(";-------------------------------------;\n");
        inkey = getch();
        switch (inkey)
        {
	    	case 27:
			clrscr();
			setup_main();
			break;
                case '1':
                        grafmode = 2;
                        clrscr();
                        setup_main();
                        break;
                case '2':
                        grafmode = 1;
                        clrscr();
                        vesamode();
                        break;
                default: 
                clrscr(); 
                graf(); 
                break;
        }
}

void sfx(void)
{
        clrscr();
        printf(";-------------------------------------;\n");
        printf(";       Micheal's Setup Program v1.6  ;\n");
        printf(";                                     ;\n");
        printf(";            Sound FX                 ;\n");
        printf(";                                     ;\n");
        printf(";         1. No sound                 ;\n");
        printf(";         2. Sound Blaster	      ;\n");
	printf(";	  3. PC Speaker		      ;\n");
        printf(";                                     ;\n");
        printf(";       Press choice (1-3)            ;\n");
        printf(";-------------------------------------;\n");
        inkey = getch();
        switch (inkey)
        {
	    	case 27:
			clrscr();
			setup_main();
			break;
                case '1':
                        sfxcard = 0;
                        clrscr();
                        setup_main();
                        break;
                case '2':
                        sfxcard = 1;
                        clrscr();
                        setup_main();
                        break;
                case '3':
                        sfxcard = 2;
                        clrscr();
                        setup_main();
                        break;
                default: 
                clrscr(); 
                sfx(); 
                break;
        }
}

void music(void)
{
        clrscr();
        printf(";-------------------------------------;\n");
        printf(";        Micheal's Setup Program v1.6 ;\n");
        printf(";                                     ;\n");
        printf(";            Music Selection          ;\n");
        printf(";                                     ;\n");
        printf(";         1. No background music      ;\n");
        printf(";         2. Adlib music	      ;\n");
	printf(";	  3. MT-32		      ;\n");
	printf(";	  4. General MIDI	      ;\n");
        printf(";                                     ;\n");
        printf(";       Press choice (1-4)            ;\n");
        printf(";-------------------------------------;\n");
        inkey = getch();
        switch (inkey)
        {
	    	case 27:
			clrscr();
			setup_main();
			break;
                case '1':
                        musicsource = 0;
                        clrscr();
                        setup_main();
                        break;
                case '2':
                        musicsource = 1;
                        clrscr();
                        setup_main();
                        break;
                case '3':
                        musicsource = 2;
                        clrscr();
                        setup_main();
                        break;
                case '4':
                        musicsource = 3;
                        clrscr();
                        setup_main();
                        break;
                default:
                clrscr(); 
                music(); 
                break;
        }
}

void input(void)
{
        clrscr();
        printf(";-------------------------------------;\n");
        printf(";        Micheal's Setup Program v1.6 ;\n");
        printf(";                                     ;\n");
        printf(";            Input Device             ;\n");
        printf(";                                     ;\n");
        printf(";         1. Keyboard only            ;\n");
        printf(";         2. Keyboard & Mouse         ;\n");
        printf(";         3. Keyboard & Joystick      ;\n");
        printf(";                                     ;\n");
        printf(";       Press choice (1-3)            ;\n");
        printf(";-------------------------------------;\n");
        inkey = getch();
        switch (inkey)
        {
		case 27:
			clrscr();
			setup_main();
			break;
                case '1':
                        inputdevice = 0;
                        clrscr();
                        setup_main();
                        break;
                case '2':
                        inputdevice = 1;
                        clrscr();
                        setup_main();
                        break;
                case '3':
                        inputdevice = 2;
                        clrscr();
                        setup_main();
                        break;
                default: 
                clrscr(); 
                input(); 
                break;
        }
}

getscancode(unsigned char scancode)
{
#if defined(__386__)
	unsigned char *ptr;
#else
	unsigned char far *ptr;
#endif
	int i, j;

	for(j=0;j<25;j++)
		for(i=0;i<57;i++)
		{
#if defined(__386__)
			ptr = (unsigned char *)(0xb8000+(((j*80)+i)<<1)+1);
#else
			ptr = (unsigned char far *)(0xb8000000+(((j*80)+i)<<1)+1);
#endif
			if (*ptr == 75)
				*ptr = 19;
		}

	for(i=0;i<256;i++)
		keystatus[i] = 0;

	oldkeyhandler = _dos_getvect(0x9);
	_disable(); _dos_setvect(0x9, keyhandler); _enable();

	i = 0;
	while (1)
	{
		if (i != 0xaa)
		{
			if (keystatus[i] != 0)
			{
				j = i;
				break;
			}
		}
		i = ((i+1)&255);
	}

	_disable(); _dos_setvect(0x9, oldkeyhandler); _enable();

	if (j > 1)
		return(j);
	else
		return(scancode);
}

void redefinekeys(void)
{
	long i;

	i = 0;
	while (i < 14)
	{
		printf("%s\n", keystring[i]);
		keys[i] = getscancode(keys[i]);
		i++;
	}
	clrscr();
	setup_main();
}

void setup_main(void)
{
        printf(";-------------------------------------;\n");
        printf(";        Micheal's Setup Program v1.6 ;\n");
        printf(";                                     ;\n");
        printf(";            Main Menu                ;\n");
        printf(";                                     ;\n");
        printf(";         1. Graphics mode            ;\n");
        printf(";         2. Sound FX Selection       ;\n");
        printf(";         3. Music Selection          ;\n");
        printf(";         4. Input Device             ;\n");
	printf(";	  5. Redefine Keys	      ;\n");
        printf(";         6. Save & Launch            ;\n");
        printf(";         7. Just Exit                ;\n");
        printf(";                                     ;\n");
        printf(";       Press choice (1-7)            ;\n");
        printf(";-------------------------------------;\n");
        inkey = getch();
        switch (inkey)
        {
	    	case 27:
			if (save() != 0)
			{
			    _asm
			    {
				mov ax,03h
				int 10h
			    }
			    printf("Error saving changes!\n");
			    exit(1);
			}
			else {
				_asm
				{
				    mov ax,03h
				    int 10h
				}
				printf("Type "RUNEXE".\n");
				exit(0);
			}		
                case '1':
                        graf();
                        break;
                case '2':
                        sfx();
                        break;
                case '3':
                        music();
                        break;
                case '4':
                        input();
                        break;
		case '5':
			redefinekeys();
			break;
                case '6':
                        if (save() != 0)
                        {
                                _asm
				{
				    mov ax,03h
				    int 10h
				}
                                printf("Error saving changes\n");
                                exit(-1);
                        }
                        else {
			    	_asm
				{
				    mov ax,03h
				    int 10h
				}
                                spawnl(P_WAIT, RUNEXE, NULL, NULL);
                                exit(0);
                        }
                        break;
                case '7':
                        _asm
			{
			    mov ax,03h
			    int 10h
			}
                        exit(0);
                        break;
		case 9:
		save();
		warpto(1);
		break;
                default: 
                clrscr(); 
                setup_main(); 
                break;
        }
}

main()
{
        setupver = 0x0f;
        if (load() != 0)
        {
                grafmode = 2;
		screenwidth = 320;
		screenheight = 200;
                sfxcard = 0;
                musicsource = 0;
                inputdevice = 1;
        }
        clrscr();
        setup_main();
}
