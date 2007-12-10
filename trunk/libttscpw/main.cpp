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

#include <stdio.h>
#include <stdlib.h>

#include "ttscpw.h"


//DLL libraries dont work with main, they will not link in the program where you use them
int main (int argc, char *argv[])
{
char buf[1000];

TTSCPW tts;
if(tts.init("localhost", 8778, ":raw:rules:diphs:synth:", 1000000))
{
printf("Init FAILED\n");
return -1;
}
if (tts.setVoice("violka")) printf("Voice setl error\n"); else printf("Voice succesfully set\n"); // Violka ve verzi Eposu 2.5.37 dela nejaky haluze, machac ne (try test it on say-epos)
//x = tts.say("Ahoj, jak se mas. Cau. Nazdar.\n");

/*
int e = tts.getl("languages", buf, sizeof(buf));
printf("RESPONSE:%d:\n%s\n******\n", e,buf);
*/
int f, i, t;
int y1 = tts.getInit_f(&f);
int y2 = tts.getInit_i(&i);
int y3 = tts.getInit_t(&t);
printf("init_f:%d >> %d\n", f, y1);
printf("init_i:%d >> %d\n", i, y2);
printf("init_t:%d >> %d\n", t, y3);

tts.say("Prave cte violka moc rychle a proto musim napsat delsi text, abych vedel kdy prestane. Dnes uz to hodlam s tim zastavovanim cteni u OSS vyresit, resp s tim s flushem.\n");
tts.say("Dnes je ale krasny den.\n");
tts.say("To jsem na to zvedavy.\n");
//tts.stop();
//tts.stopf();
//tts.say("123 456 789 Ahoj, jak se mas. Cau. Nazdar.\n");
//tts.say("Ahoj, jak se mas. Cau. Nazdar.\n");

getchar();
return 0;
}
