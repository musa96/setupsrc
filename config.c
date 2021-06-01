#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include "config.h"

int grafmode, sfxcard, musicsource, inputdevice;
long setupver;

int loadconfig()
{
        int cfgfil;

    if ((cfgfil = open(CFGFILE, O_BINARY|O_RDONLY,S_IREAD)) == -1) return -1;
        read(cfgfil, &setupver, 1);
        if (setupver != 0x0c) {
                printf("This is from an older version of Setup!\n");
                printf("Your settings will not be read.\n");
        printf("Please re-run SETUP.\n");
        close(cfgfil);
        return -1;
        }
        else {
        read(cfgfil, &grafmode, 1);
        read(cfgfil, &sfxcard, 1);
        read(cfgfil, &musicsource, 1);
        read(cfgfil, &inputdevice, 1);
        close(cfgfil);
        }
        return 0;
}

int saveconfig()
{
        int cfgfil;

    if ((cfgfil = open(CFGFILE, O_BINARY|O_CREAT|O_WRONLY,S_IWRITE)) == -1) return -1;
        setupver = 0x0c;
        write(cfgfil, &setupver, 1);
        write(cfgfil, &grafmode, 1);
        write(cfgfil, &sfxcard, 1);
        write(cfgfil, &musicsource, 1);
        write(cfgfil, &inputdevice, 1);
        close(cfgfil);
        return 0;
}
