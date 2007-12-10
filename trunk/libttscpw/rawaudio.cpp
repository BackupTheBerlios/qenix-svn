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


#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include "rawaudio.h"
#include "ttscpw.h"


//some good values for block size and count
#define BLOCK_SIZE  8192
#define BLOCK_COUNT 20

//module level variables
static CRITICAL_SECTION waveCriticalSection;
static WAVEHDR*         waveBlocks;
static volatile int     waveFreeBlockCount;
static int              waveCurrentBlock;

int streamWav(int sock, int usec)
{
    int r;
    HWAVEOUT hWaveOut; /* device handle */
    WAVEFORMATEX wfx;  /* look this up in your documentation */
    
    char buffer[1024]; /* intermediate buffer for reading */
    int i;

    if (sock == 0) return -1;

    //initialise the module variables 
    waveBlocks         = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);
    waveFreeBlockCount = BLOCK_COUNT;
    waveCurrentBlock   = 0;
    
    InitializeCriticalSection(&waveCriticalSection);

    r = recvall(sock, buffer, 44, usec);
    if (r != 44) //Cuts the header
    {
      printf("prusvih r:%d\n", r);
    }
    
    //set up the WAVEFORMATEX structure.
    //These 3 lines will 100% not work on big-endian machines!!!
    memcpy(&wfx.nSamplesPerSec, buffer + 24 , 4);
    memcpy(&wfx.wBitsPerSample, buffer + 34 , 2);
    memcpy(&wfx.nChannels, buffer + 22 , 2);
    //************************************************
    //wfx.nSamplesPerSec  = 32000;  /* sample rate */
    //wfx.wBitsPerSample  = 16;     /* sample size */
    //wfx.nChannels       = 1;      /* channels    */
    wfx.cbSize          = 0;      /* size of _extra_ info */
    wfx.wFormatTag      = WAVE_FORMAT_PCM;
    wfx.nBlockAlign     = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
     
     /*
     * try to open the default wave device. WAVE_MAPPER is
     * a constant defined in mmsystem.h, it always points to the
     * default wave device on the system (some people have 2 or
     * more sound cards).
     */     
    if(waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, (DWORD_PTR)&waveFreeBlockCount, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) 
    {
        fprintf(stderr, "unable to open wave mapper device\n");
        DeleteCriticalSection(&waveCriticalSection);
        return -2;
    }

//***************************************************************
// Get the data form socket and write them to wave buffer :):)
    while(1)
    {
    
      //Bylo by dobry zkusit zajistit vetsi bloky, ne miniaturni ?
      //Wav musi byt bez hlavicky, jinak hlavicka praska :) jako zvuk
      //jinyTTTThread jinak disconnect grrrrrrrrrrrrr
      r = recvall(sock, buffer, sizeof(buffer), usec);
      //r = recvall(sock, buffer, sizeof(buffer), usec);
      printf("r:%d\t", r);
      if (r <= 0) break;
      //if (!x)
      writeAudio(hWaveOut, buffer, r);
      //x = false;
    } 
//End of get the data from socket
//***************************************************************
    
    //wait for all blocks to complete
    while(waveFreeBlockCount < BLOCK_COUNT)
        Sleep(10);

    //unprepare any blocks that are still prepared
    for(i = 0; i < waveFreeBlockCount; i++) 
        if(waveBlocks[i].dwFlags & WHDR_PREPARED)
            waveOutUnprepareHeader(hWaveOut, &waveBlocks[i], sizeof(WAVEHDR));
    

    DeleteCriticalSection(&waveCriticalSection);
    freeBlocks(waveBlocks);
    waveOutClose(hWaveOut); // (Should destroy waveOutProc thread)
    
    return r;
}


static void writeAudio(HWAVEOUT hWaveOut, LPSTR data, int size)
{
    WAVEHDR* current;
    int remain;

    current = &waveBlocks[waveCurrentBlock];
    
    while(size > 0) {
        /* 
         * first make sure the header we're going to use is unprepared
         */
        if(current->dwFlags & WHDR_PREPARED) 
            waveOutUnprepareHeader(hWaveOut, current, sizeof(WAVEHDR));

        if(size < (int)(BLOCK_SIZE - current->dwUser)) {
            memcpy(current->lpData + current->dwUser, data, size);
            current->dwUser += size;
            break;
        }

        remain = BLOCK_SIZE - current->dwUser;
        memcpy(current->lpData + current->dwUser, data, remain);
        size -= remain;
        data += remain;
        current->dwBufferLength = BLOCK_SIZE;
       
        waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
        
        EnterCriticalSection(&waveCriticalSection);
        waveFreeBlockCount--;
        LeaveCriticalSection(&waveCriticalSection);
        
        /*
         * wait for a block to become free
         */
        while(!waveFreeBlockCount)
            Sleep(10);

        /*
         * point to the next block
         */
        waveCurrentBlock++;
        waveCurrentBlock %= BLOCK_COUNT;

        current = &waveBlocks[waveCurrentBlock];
        current->dwUser = 0;
    }
}

static WAVEHDR* allocateBlocks(int size, int count)
{
    unsigned char* buffer;
    int i;
    WAVEHDR* blocks;
    DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
    
    /*
     * allocate memory for the entire set in one go
     */
    if((buffer = (unsigned char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, totalBufferSize)) == NULL) 
    {
        fprintf(stderr, "Memory allocation error\n");
        ExitProcess(1);
    }

    /*
     * and set up the pointers to each bit
     */
    blocks = (WAVEHDR*)buffer;
    buffer += sizeof(WAVEHDR) * count;
    for(i = 0; i < count; i++) {
        blocks[i].dwBufferLength = size;
        blocks[i].lpData = (char *)buffer;
        buffer += size;
    }
    
    return blocks;
}

static void freeBlocks(WAVEHDR* blockArray)
{
    //and this is why allocateBlocks works the way it does 
    HeapFree(GetProcessHeap(), 0, blockArray);
}

static void CALLBACK waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    int* freeBlockCounter = (int*)dwInstance;
    /*
     * ignore calls that occur due to openining and closing the
     * device.
     */
    if(uMsg != WOM_DONE)
        return;

    EnterCriticalSection(&waveCriticalSection);
    (*freeBlockCounter)++;
    LeaveCriticalSection(&waveCriticalSection);
}
