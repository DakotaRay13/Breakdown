e:\GB_Dev\gbdk-2.95-3-win32\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o main.o main.c
e:\GB_Dev\gbdk-2.95-3-win32\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o main.gb main.o

@REM e:\GB_Dev\gbdk-2.95-3-win32\gbdk\bin\lcc -Wa-l -Wf-ba0 -c -o saveState.o saveState.c
@REM e:\GB_Dev\gbdk-2.95-3-win32\gbdk\bin\lcc -Wa-l -c -o main.o main.c
@REM e:\GB_Dev\gbdk-2.95-3-win32\gbdk\bin\lcc -Wl-yt3 -Wl-ya4 -o main.gb main.o saveState.o