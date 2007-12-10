/*
Qenix - Graphical TTSCP Client for speech synthesis
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
* TTSCP Dialog Settings - server, port, stream
*/
#include <stdio.h>
#include <QByteArray>

class QString;
class QByteArray;

#include "ttscpvoice.h"
#include "iniparser.h"
#include "mainwindow.h"

extern TTSCPW tts;

ttscpVoiceDialog::ttscpVoiceDialog(QWidget *parent) : QDialog(parent)
{
  setupUi(this);
  
  dictionary * ini = iniparser_load("qenix.ini"); //Must deallocate manualy
  if (ini==NULL)
  {
  iniparser_freedict(ini);
  fprintf(stderr, "cannot parse file [%s]", "qenix.ini");
  return;
  }
	iniparser_dump(ini, stderr);
 
 	spinF->setValue(iniparser_getint(ini, "ttscp:init_f", 20));
	spinI->setValue(iniparser_getint(ini, "ttscp:init_i", 20));
	spinT->setValue(iniparser_getint(ini, "ttscp:init_t", 20));
  //int timeout = iniparser_getstr(ini, "ttscp:timeout", -1);
  iniparser_freedict(ini);

}

void ttscpVoiceDialog::accept()
{
  QByteArray ba;
  
   tts.stopf();
  //tts.stop();
 
  dictionary * ini = iniparser_load("qenix.ini"); //Must deallocate manualy
  if (ini==NULL)
  {
  iniparser_freedict(ini);
  fprintf(stderr, "cannot parse file [%s]", "qenix.ini");
  return;
  }
	iniparser_dump(ini, stderr);
 
  tts.setInit_f(spinF->value());
  ba = QByteArray::number(spinF->value());
  iniparser_setstr(ini, "ttscp:init_f", ba.data());
  
  tts.setInit_i(spinI->value());
  ba = QByteArray::number(spinI->value());
  iniparser_setstr(ini, "ttscp:init_i", ba.data());
  
  tts.setInit_t(spinT->value());
  ba = QByteArray::number(spinT->value());
  iniparser_setstr(ini, "ttscp:init_t", ba.data());
  
  //msgbox(QString(t));
  
	FILE *fw;
	fw = fopen("qenix.ini", "wb+");
	
	iniparser_dump_ini(ini, fw);
	fclose(fw);
  iniparser_freedict(ini);
    
  done(Accepted);
}

