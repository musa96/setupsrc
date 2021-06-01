# Uncomment this if it is to be compiled on 16-bit
# environment
#!define 286

test.exe : test.obj config.obj
        wlink f test.obj f config.obj
setup.exe : setup.obj
        wlink f setup.obj
setup2.exe : setup2.obj
	wlink f setup2.obj
setups.exe : setups.obj
	wlink f setups.obj


test.obj : test.c config.h
!ifdef 286
	wcc /s test.c
!else
	wcc386 /s /mf test.c
!endif
config.obj : config.c config.h
!ifdef 286
        wcc /s config.c
!else
	wcc386 /s /mf config.c
!endif
setup.obj : setup.c
!ifdef 286
        wcc /s setup.c
!else
	wcc386 /s /mf setup.c
!endif

setup2.obj : setup2.c
!ifdef 286
        wcc /s setup2.c
!else
	wcc386 /s /mf setup2.c
!endif

setups.obj : setups.c
!ifdef 286
        wcc /s setups.c
!else
	wcc386 /s /mf setups.c
!endif
