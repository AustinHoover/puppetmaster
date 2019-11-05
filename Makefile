
all: launch puppetmasterlib puppetmaster

launch:
	gcc launch.c -o launch

puppetmasterlib:
	gcc -c -DBUILDING_PUPPETMASTERLIB_DLL puppetmasterlib.c
	gcc -shared -o puppetmasterlib.dll puppetmasterlib.o
	copy puppetmasterlib.dll puppetmaster.abc
	type puppetmasterlib.dll > puppetmaster.exe:lib
	copy puppetmasterlib.dll puppetmasterlib.backup
	del puppetmasterlib.dll

puppetmaster: puppetmaster.c
	gcc -o puppetmaster puppetmaster.c
	echo 0 > puppetmaster.exe:vars

cmd:
	del cmd.exe
	del cmd.exe.backup
	copy cmd.backup cmd.exe


cmd2:
	del cmd2.exe
	del cmd2.exe.backup
	copy cmd.backup cmd2.exe



calc:
	del calc.exe
	del calc.exe.backup
	copy calc.backup calc.exe
