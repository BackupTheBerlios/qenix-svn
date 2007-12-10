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


 /*
  * Platform detection
  */
 
 /* MS Windows flavours
 */
 #ifdef WIN32
 #  ifndef _WIN32_
 #    define _WIN32_
 #  endif
 #  ifndef WINSOCK
 #    define WINSOCK
 #  endif
 #  ifdef _DEBUG
 #    define _DEBUG_
 #  endif
 #endif // _WIN32_
 
 /* Unix flavours
 */
 #ifdef __linux__
 #define _LINUX_
 #endif
 
 #ifdef __FreeBSD__
 #define _FREEBSD_
 #endif
 
 #ifdef __CYGWIN__
 #define _CYGWIN_
 #endif
 
 #ifdef __APPLE__
   // Make Carbon the default choice when compiling on a Mac, even when using gcc
   #if (!defined(_CARBON_) && !defined(_DARWIN_))
     #define _CARBON_
   #endif
 #endif
 
 #ifdef __MWERKS__
   #ifdef macintosh
     #define _CARBON_
     #endif
#endif

#if defined(_LINUX_) || defined(_FREEBSD_) || defined(_CYGWIN_) || defined(_DARWIN_) 
 #define _UNIX_
 #endif
