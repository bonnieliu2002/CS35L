randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -shared randlibhw.c -o randlibhw.so

randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -fPIC -shared randlibsw.c -o randlibsw.so

randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randmain.o randcpuid.o -o randmain
