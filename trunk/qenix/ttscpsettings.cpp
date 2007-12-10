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
#include <QString>
#include <QByteArray>

class QString;
class QByteArray;

#include "ttscpsettings.h"
#include "iniparser.h"
#include "mainwindow.h"


ttscpSettingsDialog::ttscpSettingsDialog(QWidget *parent) : QDialog(parent)
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
 
 	txtServer->setText(QString(iniparser_getstr(ini, "ttscp:server")));
	txtPort->setText(QString(iniparser_getstr(ini, "ttscp:port")));
	txtStream->setText(QString(iniparser_getstr(ini, "ttscp:stream")));
	txtTimeout->setText(QString(iniparser_getstr(ini, "ttscp:timeout")));
	
  //int timeout = iniparser_getstr(ini, "ttscp:timeout", -1);
  iniparser_freedict(ini);
} 

void ttscpSettingsDialog::accept()
{
  QString qs;
  QByteArray ba;
  char *t;
 
  dictionary * ini = iniparser_load("qenix.ini"); //Must deallocate manualy
  if (ini==NULL)
  {
  iniparser_freedict(ini);
  fprintf(stderr, "cannot parse file [%s]", "qenix.ini");
  return;
  }
	iniparser_dump(ini, stderr);
 
 
  qs = txtServer->text(); 
  ba = qs.toAscii();
  t = ba.data();
  
  
  //msgbox(QString(t));
  
 	if(iniparser_setstr(ini, "ttscp:server", t));
 	if(iniparser_setstr(ini, "ttscp:port", t));
 	if(iniparser_setstr(ini, "ttscp:stream", t));
 	if(iniparser_setstr(ini, "ttscp:timeout", t));
	//iniparser_setstr(ini, "ttscp:port", txtPort->text.toAscii.consData());	
	//iniparser_setstr(ini, "ttscp:stream", txtSteam->text.toAscii.consData());
  
  //int timeout = iniparser_getint(ini, "ttscp:timeout", -1);
	//if (timeout < 0) return -54;
	FILE *fw;
	fw = fopen("qenix.ini", "wb+");
	
	iniparser_dump_ini(ini, fw);
	fclose(fw);
  iniparser_freedict(ini);
  
  initEpos();
  
  done(Accepted);
}
