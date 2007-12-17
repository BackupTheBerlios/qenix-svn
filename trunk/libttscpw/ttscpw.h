/*
libttscpw - TTSCP Protocol Wrapper
Copyright (C) 20007  Pavel Slechta (slechta@email.cz)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef TTSCPW_H
#define TTSCPW_H

#include "platform.h"

// Standart C headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Unix/linux headers that are also in MinGW
#include <sys/time.h>
#include <sys/types.h>
#include <sys/unistd.h>

#ifdef _UNIX_
  // Unix headers that are NOT in MinGW
  #include <sys/select.h>
  #include <sys/socket.h>

  //DNS resolving
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <netdb.h>

  //Threads
  #include <pthread.h>
#endif


#ifdef WINSOCK
  #include <windows.h>
  #include <winsock2.h>
  #include <mmsystem.h>
#endif

#define TTSCPW_LOCALSOUND 0
#define TTSCPW_WAVE 1


int recvtimeout(int s, char *buf, int len, int timeout);
int recvall(int s, char *buf, int len, int usec); //Get data of len (not less that len) into buffer, 0 = No error

#ifdef _UNIX_
struct OSSDATA
{
int sockfd;
int audiofd;
unsigned int wavelen;
};
#endif

class TTSCPW
{
public:
  //Init & Deinit stuff
  ~TTSCPW();
  int init(char *HOST, int PORT, char *STREAM, int TIMEOUT);
  int deinit();
  int done();
  
  //Say
  int say(char *text);
  int sayw(char * text);
  
  //Stop
  int stop(); //intr hCtrl
  int stopf(); //appl 1
  
  // Setters
  int setCharset(char *charset);
  int setLanguage(char *language);
  int setVoice(char *voice);  
  int setInit_f(int f);
  int setInit_i(int i);
  int setInit_t(int t);
  
  // Getters
  int getInit_f(int *f);
  int getInit_i(int *i);
  int getInit_t(int *t);
  int getl(char *param, char *buffer, int len);
  

private:
  //Constructor things
  char host[200];
  int port;
  char stream[200];
  int utimeout;
  bool bDone;
  
  // Runtime
  int sockCtrl, sockData;
  char hCtrl[50], hData[50];
  
  // Windows BLOB
  #ifdef WINSOCK
  WSADATA wsaData;
  #endif
  
  // Threads & OpenSoundSystem
  #ifdef _UNIX_
  pthread_t threadOSS;
  OSSDATA ossd;
  #endif
  
  // Private methods
  int createConnection();
  int createDataChannel(); // Just sends "data $hCtrl" to sockData
  int createSocket();
  int getHandle(int sockfd, char *handle);
  
  int sendall(int s, char *buf, int *len);
  int sendq(int sock, char *text); // Quick and easy send to socket a \0 terminated string
  int setl(char *param, char *value);
  int setStream(int iMethod);
  int waitForLine(int s, char *line, int len, int timeout);
  int sflush(int sock);
  int getResponse(char *buffer, int len);
  int isOK(int utimeout);
  
  //String methods
  int *ipstrlen(char *buf); // Return length of \0 terminated string in pointer to int not size_t, you must then free() the pointer
  int istrlen(char *buf); // Return length of \0 terminated string in int not size_t
  int isspace(char c);
  char *lTrim(char *p);
  char *rTrim(char *p);
  char *Trim(char *p);

};
#endif

