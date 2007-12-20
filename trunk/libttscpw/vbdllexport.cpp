#include <windows.h>
#include "ttscpw.h"

TTSCPW tts;

extern "C" // only required if using g++
{

//Init & Deinit
__declspec (dllexport) int __stdcall ttscpw_init (LPSTR host, int port, LPSTR stream, int timeoutms)
{
  return tts.init(host, port, stream, timeoutms * 1000);
}

__declspec (dllexport) int __stdcall ttscpw_deinit ()
{
  return tts.deinit();
}

// Say
__declspec (dllexport) int __stdcall ttscpw_say (LPSTR text)
{
  return tts.say(text);
}

__declspec (dllexport) int __stdcall ttscpw_sayw (LPSTR text)
{
  return tts.sayw(text);
}

//Stop
__declspec (dllexport) int __stdcall ttscpw_stop ()
{
  return tts.stop();
}

__declspec (dllexport) int __stdcall ttscpw_stopf ()
{
  return tts.stopf();
}

// Setters
__declspec (dllexport) int __stdcall ttscpw_setCharset (LPSTR charset)
{
  return tts.setCharset(charset);
}

__declspec (dllexport) int __stdcall ttscpw_setLanguage (LPSTR language)
{
  return tts.setLanguage(language);
}

__declspec (dllexport) int __stdcall ttscpw_setVoice (LPSTR voice)
{
  return tts.setVoice(voice);
}

__declspec (dllexport) int __stdcall ttscpw_setInit_f (unsigned long f)
{
  return tts.setInit_f((int)f);
}

__declspec (dllexport) int __stdcall ttscpw_setInit_i (unsigned long i)
{
  return tts.setInit_i((int)i);
}

__declspec (dllexport) int __stdcall ttscpw_setInit_t (unsigned long t)
{
  return tts.setInit_t((int)t);
}
// Getters
__declspec (dllexport) int __stdcall ttscpw_getInit_f (int *f)
{
  return tts.getInit_f(f);
}

__declspec (dllexport) int __stdcall ttscpw_getInit_i (int *i)
{
  return tts.getInit_i(i);
}

__declspec (dllexport) int __stdcall ttscpw_getInit_t (int *t)
{
  return tts.getInit_t(t);
}

__declspec (dllexport) int __stdcall ttscpw_getl (LPSTR param, LPSTR buffer, int len)
{
  return tts.getl(param, buffer, len);
}


//DLL Entry point
__declspec (dllexport) BOOL __stdcall DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
        switch (ul_reason_for_call)
        {
        case DLL_PROCESS_ATTACH:
                break;
        case DLL_THREAD_ATTACH:
                break;
        case DLL_THREAD_DETACH:
                break;
        case DLL_PROCESS_DETACH:
                break;
        }
        return TRUE;
}

} // extern "C"
