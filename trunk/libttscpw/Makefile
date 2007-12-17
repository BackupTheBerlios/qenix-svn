linuxbin: main.o ttscpw.o oss.o
	g++ -lpthread -o ttscp main.o ttscpw.o oss.o
linuxstaticlib: ttscpw.o oss.o
	ar rcs libttscpw.a ttscpw.o oss.o
linuxshared: ttscpw.o.fpic oss.o.fpic
	g++ -shared -Wl,-soname,libttscpw.so.1 -o libttscpw.so.1.0.1 ttscpw.o oss.o -lpthread
	
	
	
winbin: main.o ttscpw.o rawaudio.o
	g++ -o ttscp main.o ttscpw.o rawaudio.o -lws2_32 -lwinmm
dll: ttscpw.o ttscpw.o rawaudio.o
	g++ -shared -enable-realtime-pseudo-reloc --no-undefined -Wl,--out-implib,libttscpw.a -o ttscpw.dll ttscpw.o rawaudio.o -lws2_32 -lwinmm
vbdll: vbdllexport.o ttscpw.o rawaudio.o
	g++ -shared -Wl,--add-stdcall-alias -Wl,--output-def,ttscpw.def,--out-implib,libttscpw.a -o ttscpw.dll vbdllexport.o ttscpw.o rawaudio.o -lws2_32 -lwinmm
	lib /machine:i386 /def:ttscpw.def


main.o: main.cpp
	g++ -c main.cpp
oss.o: oss.cpp
	g++ -c oss.cpp
vbdllexport.o: vbdllexport.cpp
	g++ -c vbdllexport.cpp
ttscpw.o: ttscpw.cpp
	g++ -c ttscpw.cpp		
rawaudio.o: rawaudio.cpp
	g++ -c rawaudio.cpp
	
	
ttscpw.o.fpic: ttscpw.cpp
	g++ -fPIC -c ttscpw.cpp		
oss.o.fpic: oss.cpp
	g++ -fPIC -c oss.cpp
