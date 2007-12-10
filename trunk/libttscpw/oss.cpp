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

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/soundcard.h>
#include <string.h>


#include <pthread.h>
#include "oss.h"
#include "ttscpw.h"


pthread_mutex_t mutexOSS = PTHREAD_MUTEX_INITIALIZER;

//This is a Pthread!!!
void *streamWavOSS(void *ptr)
{
//Pthreadmutexunlock by se sem hodil, kvuli cancelu v jinym threadu
pthread_mutex_unlock(&mutexOSS);
    int n;
    unsigned int lenw;
    char buf[2048];

    OSSDATA *od;
    od = (OSSDATA*) ptr;

//Mutex lock
pthread_mutex_lock(&mutexOSS);
    // open device file 
    if ((od->audiofd = open("/dev/dsp", O_WRONLY,0)) == -1) {
        perror("/dev/dsp");
        pthread_exit( NULL );
    }
pthread_mutex_unlock(&mutexOSS); //Kvuli audiofd


    if(recvall(od->sockfd, buf, 44, 200000) <= 0) printf("Cannot read header\n"); //Reads the header
    memcpy(&lenw, buf + 4, 4);
    lenw -= 38; // 44 - 8 = 36 (44 - length of all headers) (8 - first 2 sections no tincluded in ChunkSize) ale nevim proc 38 a ne 36
    printf("lenw:%d\n", lenw);
pthread_mutex_lock(&mutexOSS);
    od->wavelen = lenw;
pthread_mutex_unlock(&mutexOSS);

    /*  Setting Sampling Parameters
        1. sample format
        2. number of channels
        3. sampling rate
    */
    
    int format = AFMT_S16_NE;
    if (ioctl(od->audiofd,SNDCTL_DSP_SETFMT, &format) == -1)
  {
        perror("SNDCTL_DSP_SETFMT");
        pthread_exit( NULL );
    }

    int channels = 1;
    if (ioctl(od->audiofd, SNDCTL_DSP_CHANNELS, & channels) == -1)
  {
        perror("SNDCTL_DSP_CHANNELS");
        pthread_exit( NULL );
    }

        
    int speed = 32000;
    if (ioctl(od->audiofd, SNDCTL_DSP_SPEED, &speed) == -1)
  {
        perror("SNDCTL_DSP_SPEED");
        pthread_exit( NULL );
    } else 
    printf("Actual Speed : %d \n",speed);

    /*  it wont be actually 44.1 KHz , codec devices usually generate the
        sampling clock by dividing the frequency of crystal the oscillator
        inside soundcard.
    */

    while (1)
    {
        pthread_mutex_lock(&mutexOSS);
        n = recvtimeout(od->sockfd, buf, sizeof(buf)-20, 1000000);
        od->wavelen -= n;
        if (od->wavelen == 0) break;
        pthread_mutex_unlock(&mutexOSS);
        if (n < 0)
        {
            printf("Chyba v oss recv!\n");
            break;
        }
        write(od->audiofd, buf, n);
    }

    //Mutex lock
    pthread_mutex_lock(&mutexOSS);
    close(od->audiofd); //jen aby to neseklo cely program, ale snad ne kdyz je prehravani uz u konce
    od->audiofd = 0;

    printf("od->wavelen=%d\n", od->wavelen);
    pthread_mutex_unlock(&mutexOSS);
    //Mutex unlock
    pthread_exit( NULL );
}

int haltOSS(int fd)
{
return ioctl(fd, SNDCTL_DSP_RESET, NULL);
}

