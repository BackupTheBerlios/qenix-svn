/*
Qenix - Graphical TTSCP Client for speech synthesis
Copyright (C) 2007  Pavel Slechta (slechta@email.cz)

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

#include <QtGui>
#include <QFontDialog>
#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QTextEdit>
#include <QTextStream>
#include <QApplication>

#include "codeedit.h"

//QTE stuff
#include "qeditor.h"
#include "qcodeedit.h"

#include "qstatuspanel.h"
#include "qsearchreplacepanel.h"

#include "qfoldpanel.h"
#include "qlinemarkpanel.h"
#include "qlinenumberpanel.h"

#include "qdocument.h"
#include "qdocumentline.h"
#include "qdocumentcursor.h"

#include "qformatfactory.h"
#include "qlanguagefactory.h"

#include "qlinemarksinfocenter.h"

//#include "qpluginmanager.h"
//End of QTE stuff

#include "mainwindow.h"
#include "ttscpsettings.h"
#include "iniparser.h"
#include "platform.h"

TTSCPW tts;

//Just for debugging
void msgbox(QString t)
{
QMessageBox::about(0, "?", t);
}

//Constructor
MainWindow::MainWindow()
{
    setupUi(this);

    Tabw = new TabWidget(this);
    setCentralWidget(Tabw);
  
    initEpos();
    loadSettings();

    createActions();
    //createMenus();
    createToolBars();
    //createStatusBar();
    //updateMenus();
}

int initEpos()
{
  int e;
  
 
  dictionary * ini = iniparser_load("qenix.ini"); //Must deallocate manualy
  if (ini==NULL)
  {
		fprintf(stderr, "cannot parse file [%s]", "qenix.ini");
    iniparser_freedict(ini);
    return -50;
	}
	iniparser_dump(ini, stderr);

 	char *server = iniparser_getstr(ini, "ttscp:server");
	if (!server) return -51;
	
	int port = iniparser_getint(ini, "ttscp:port", -1);
	if (port < 0 && port > 65536) return -52;
	
	char *stream = iniparser_getstr(ini, "ttscp:stream");
	if (!stream) return -53;

  int timeout = iniparser_getint(ini, "ttscp:timeout", -1);
	if (timeout < 0) return -54;
  
  e = tts.init(server, port, stream, timeout);
  if (e) return e;
  
  
  tts.setCharset("8859-2");
  
  tts.setLanguage(iniparser_getstr(ini ,"ttscp:language"));
  tts.setVoice(iniparser_getstr(ini ,"ttscp:voice"));
  
  tts.setInit_f(iniparser_getint(ini ,"ttscp:init_f", 195));
  tts.setInit_i(iniparser_getint(ini ,"ttscp:init_i", 100));
  tts.setInit_t(iniparser_getint(ini ,"ttscp:init_t", 120));
  
  //tts.setVoice("violka");
  tts.say("Sokety propojeny\n");
  
  return 0;
}

void MainWindow::createToolBars()
{
toolBar->addAction(DockFiles->toggleViewAction());
}

void MainWindow::closeEvent(QCloseEvent *event)
{

}

void MainWindow::newFile()
{
    Tabw->newTab();
}

void MainWindow::open()
{
    QString fn = QFileDialog::getOpenFileName(this, "Open file...", QDir::currentPath(), 0, 0, 0);
    Tabw->newTab(fn);
}

void MainWindow::save()
{
  Tabw->currentWidget()->save();
}

void MainWindow::saveAs()
{

}

void MainWindow::cut()
{
  Tabw->currentWidget()->cut();
}

void MainWindow::copy()
{
    Tabw->currentWidget()->copy();
}

void MainWindow::paste()
{
    Tabw->currentWidget()->paste();
}

void MainWindow::undo()
{
    Tabw->currentWidget()->undo();
}

void MainWindow::redo()
{
    Tabw->currentWidget()->redo();
}


void MainWindow::about()
{
   QMessageBox::about(this, tr("QENIX"), "QENIX <BR>Multiplatform Epos/TTSCP client written by Pavel Slechta <BR> slechta@email.cz");
}

void MainWindow::updateMenus()
{
/*
    bool hasMdiChild = (activeMdiChild() != 0);
    saveAct->setEnabled(hasMdiChild);
    saveAsAct->setEnabled(hasMdiChild);
    pasteAct->setEnabled(hasMdiChild);
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    separatorAct->setVisible(hasMdiChild);
    sayAllTextAct->setEnabled(hasMdiChild);    
    saySelectedTextAct->setEnabled(hasMdiChild);
    findDlgAct->setEnabled(hasMdiChild);
    findNextAct->setEnabled(hasMdiChild);
    fontDlgAct->setEnabled(hasMdiChild);
    quickReplaceAct->setEnabled(hasMdiChild);
    
    bool hasSelection = (activeMdiChild() && activeMdiChild()->textCursor().hasSelection());
    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);
    
    if(hasMdiChild) highlightCurrentAct->setChecked(activeMdiChild()->hasHighlighter());
 */  
}

void MainWindow::createActions()
{
    //File
    connect(actNewDoc, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actSave, SIGNAL(triggered()), this, SLOT(save()));
    
    //Edit
    connect(actCut, SIGNAL(triggered()), this, SLOT(cut()));
    connect(actCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(actPaste, SIGNAL(triggered()), this, SLOT(paste()));
    connect(actUndo, SIGNAL(triggered()), this, SLOT(undo()));
    connect(actRedo, SIGNAL(triggered()), this, SLOT(redo()));
    connect(actQuickReplace, SIGNAL(triggered()), this, SLOT(quickReplace()));
    //connect(actUnident, SIGNAL(triggered()), this, SLOT(unident()));
    //connect(actIdene, SIGNAL(triggered()), this, SLOT(ident()));
    
    //Format
    connect(actFont, SIGNAL(triggered()), this, SLOT(font()));
    
    //Read
    connect(actReadAll, SIGNAL(triggered()), this, SLOT(readAll()));
    connect(actReadSelected, SIGNAL(triggered()), this, SLOT(readSelected()));
    connect(actStopReading, SIGNAL(triggered()), this, SLOT(stopReading()));
    
    //Help menu
    connect(actAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(actAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));

    rHashAct = new QAction(tr("Don't read #"), this);
    rHashAct->setStatusTip(tr("Say will not read hashes"));
    rHashAct->setCheckable(true);
    connect(rHashAct, SIGNAL(triggered()), this, SLOT(rHash()));

    rRoundBracketsAct = new QAction(tr("Don't read ()"), this);
    rRoundBracketsAct->setStatusTip(tr("Say will not read round brackets"));
    rRoundBracketsAct->setCheckable(true);
    connect(rRoundBracketsAct, SIGNAL(triggered()), this, SLOT(rRoundBrackets()));

    rSquareBracketsAct = new QAction(tr("Don't read []"), this);
    rSquareBracketsAct->setStatusTip(tr("Say will not read square brackets"));
    rSquareBracketsAct->setCheckable(true);
    connect(rSquareBracketsAct, SIGNAL(triggered()), this, SLOT(rSquareBrackets()));

    rChevronsAct = new QAction(tr("Don't read <>"), this);
    rChevronsAct->setStatusTip(tr("Say will not read chevrons"));
    rChevronsAct->setCheckable(true);
    connect(rChevronsAct, SIGNAL(triggered()), this, SLOT(rChevrons()));

    rQuotesAct = new QAction(tr("Don't read \""), this);
    rQuotesAct->setStatusTip(tr("Say will not read double qutoes"));
    rQuotesAct->setCheckable(true);
    connect(rQuotesAct, SIGNAL(triggered()), this, SLOT(rQuotes()));

    rAsteriskAct = new QAction(tr("Don't read *"), this);
    rAsteriskAct->setStatusTip(tr("Say will not read asterisks"));
    rAsteriskAct->setCheckable(true);
    connect(rAsteriskAct, SIGNAL(triggered()), this, SLOT(rAsterisk()));

    rApostropheAct = new QAction(tr("Don't read '"), this);
    rApostropheAct->setStatusTip(tr("Say will not read apostorphes"));
    rApostropheAct->setCheckable(true);
    connect(rApostropheAct, SIGNAL(triggered()), this, SLOT(rApostrophe()));

    rNumAct = new QAction(tr("Don't read arabic numers 0-9"), this);
    rNumAct->setStatusTip(tr("Say will not read arabic numers 0-9"));
    rNumAct->setCheckable(true);
    connect(rNumAct, SIGNAL(triggered()), this, SLOT(rNum()));

    sepNumAct = new QAction(tr("Read numbers separately"), this);
    sepNumAct->setStatusTip(tr("Causes to say 1 2 3 4 5 instead of 12345"));
    sepNumAct->setCheckable(true);
    connect(sepNumAct, SIGNAL(triggered()), this, SLOT(sepNum()));

    toLowerCAct = new QAction(tr("Everything toLoweCase()"), this);
    toLowerCAct->setStatusTip(tr("Sends everything in lowercase to server"));
    toLowerCAct->setCheckable(true);
    connect(toLowerCAct, SIGNAL(triggered()), this, SLOT(toLowerC()));
    
    DockFiles = new QDockWidget(tr("File browser"), this);
    DockFiles->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    TreeFiles = new QTreeView(DockFiles); //DockFiles nebo this
    DirModel = new QDirModel(this); //Radsi this
    TreeFiles->setModel(DirModel);
    DockFiles->setWidget(TreeFiles); 
    addDockWidget(Qt::LeftDockWidgetArea, DockFiles);
    //connect(TreeFiles, SIGNAL(clicked(QModelIndex)), this, SLOT(dockFilesClicked()));
    connect(TreeFiles, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(dockFilesClicked(QModelIndex)));
}

void MainWindow::fileBrowserClicked()
{
  //msgbox(QString(tree->currentItem()->text(0)));
}

void MainWindow::createMenus()
{
/*
    readMenu->addAction(readPreviousWordAct);
    readMenu->addAction(readNextWordAct);   
    readMenu->addSeparator();
    readMenu->addAction(rHashAct);
    readMenu->addAction(rAsteriskAct);
    readMenu->addAction(rQuotesAct);
    readMenu->addAction(rApostropheAct);
    readMenu->addSeparator();
    readMenu->addAction(rRoundBracketsAct);
    readMenu->addAction(rSquareBracketsAct);
    readMenu->addAction(rChevronsAct);
    readMenu->addSeparator();
    readMenu->addAction(rNumAct);
    readMenu->addAction(sepNumAct);
    readMenu->addAction(toLowerCAct);
  */
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

int MainWindow::writeSettings()
{
  QByteArray ba;
 
  dictionary * ini = iniparser_load("qenix.ini"); //Must deallocate manualy
  if (ini==NULL)
  {
		fprintf(stderr, "cannot parse file [%s]", "qenix.ini");
    iniparser_freedict(ini);
    return -50;
	}
	iniparser_dump(ini, stderr);

  //if (highlighting)	iniparser_setstr(ini, "highlighting:highlighting", "1");
  //else iniparser_setstr(ini, "highlighting:highlighting", "0");

  ba = QByteArray::number(pos().x());
 	iniparser_setstr(ini, "window:startx", ba.data());
 	ba = QByteArray::number(pos().y());
 	iniparser_setstr(ini, "window:starty", ba.data());
 	
 	ba = QByteArray::number(size().height());
 	iniparser_setstr(ini, "window:height", ba.data());
 	ba = QByteArray::number(size().width());
 	iniparser_setstr(ini, "window:width", ba.data());
 	
  FILE *fw;
	fw = fopen("qenix.ini", "wb+");
	
	iniparser_dump_ini(ini, fw);
	fclose(fw);
  iniparser_freedict(ini);
 
  return 0;
}




void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void MainWindow::readSelected()
{
  sayf(Tabw->currentWidget()->document()->editCursor()->selectedText());
}

void MainWindow::say(QString text)
{
// On Windows is default cp1250 so setlCharset is needed or repalce iso for cp1250

if (text.length() <= 0) return;

#ifdef _UNIX_
QTextCodec *codec = QTextCodec::codecForName("ISO 8859-2");
#endif

#ifdef WIN32
QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
#endif
QByteArray ba = codec->fromUnicode(text);
tts.say(ba.data());
}

void MainWindow::readAll()
{
sayf(Tabw->currentWidget()->document()->text());
}

void MainWindow::stopReading()
{
tts.stopf();
}

void MainWindow::rHash()
{
tf.rHash = !(rHashAct->isChecked());
if (rHashAct->isChecked()) say("Cteni mrizek vypnuto\n");
else say("Cteni mrizek zapnuto\n");
}

void MainWindow::sayf(QString text)
{
text = tf.format(text);
QTextCodec *codec = QTextCodec::codecForName("ISO 8859-2");
QByteArray ba = codec->fromUnicode(text);
//ba.append("\0"); //Fakt nevim, ale asi uz tam je
tts.say(ba.data());
}

void MainWindow::rQuotes()
{
tf.rQuotes = !(rQuotesAct->isChecked());
if (rQuotesAct->isChecked()) say("Cteni uvozovek vypnuto\n");
else say("Cteni uvozovek zapnuto\n");
}

void MainWindow::rAsterisk()
{
tf.rAsterisk = !(rAsteriskAct->isChecked());
if (rAsteriskAct->isChecked()) say("Cteni hvezdicek vypnuto\n");
else say("Cteni hvezdicek zapnuto\n");
}

void MainWindow::rRoundBrackets()
{
tf.rRoundBrackets = !(rRoundBracketsAct->isChecked());
if (rRoundBracketsAct->isChecked()) say("Cteni kulatych zavorek vypnuto\n");
else say("Cteni kulatych zavorek zapnuto\n");
}

void MainWindow::rSquareBrackets()
{
tf.rSquareBrackets = !(rSquareBracketsAct->isChecked());
if (rSquareBracketsAct->isChecked()) say("Cteni hranatych zavorek vypnuto\n");
else say("Cteni hranatych zavorek zapnuto\n");
}

void MainWindow::rChevrons()
{
tf.rChevrons = !(rChevronsAct->isChecked());
if (rChevronsAct->isChecked()) say("Cteni ostrych zavorek vypnuto\n");
else say("Cteni ostrych zavorek zapnuto\n");
}

void MainWindow::rApostrophe()
{
tf.rApostrophe = !(rApostropheAct->isChecked());
if (rApostropheAct->isChecked()) say("Cteni apostrofu vypnuto\n");
else say("Cteni apostrofu zapnuto\n");
}

void MainWindow::rNumbers()
{
tf.rNum = !(rNumAct->isChecked());
if (rNumAct->isChecked()) say("Cteni cisel vypnuto\n");
else say("Cteni cisel zapnuto\n");
}

void MainWindow::rSeparateNumbers()
{
tf.sepNum = sepNumAct->isChecked();
if (sepNumAct->isChecked()) say("Cteni cisel po cifrach zapnuto\n");
else say("Cteni cisel po cifrach vypnuto\n");
}

void MainWindow::rLowerCase()
{
tf.toLower = toLowerCAct->isChecked();
if (toLowerCAct->isChecked()) say("Vsechno male \n");
else say("Cteni normalne\n");
}

void MainWindow::ttscpSettings()
{
//ttscpSettingsDialog td(this); //you can also then td.getResult() = QDialog::Accepted/Rejected (destory on close defaultly not set)
//td.exec();
}

int MainWindow::loadSettings()
{
  dictionary * ini = iniparser_load("qenix.ini"); //Must deallocate manualy
  if (ini==NULL)
  {
		fprintf(stderr, "cannot parse file [%s]", "qenix.ini");
    iniparser_freedict(ini);
    return -1;
	}
	iniparser_dump(ini, stderr);
	
  move(iniparser_getint(ini, "window:startx", 100),iniparser_getint(ini, "window:starty", 100));
  resize(iniparser_getint(ini, "window:width", 100),iniparser_getint(ini, "window:height", 100));	  	
  
  iniparser_freedict(ini);
	return 0;
}

void MainWindow::ttscpVoice()
{
     ttscpVoiceDialog tvdlg(this);
     tvdlg.exec();
}

void MainWindow::font()
{

  QString qs;
  QByteArray ba;
  
  QFont qf = QFontDialog::getFont(0, currentFont);
  Tabw->setFont(qf);

  QFontInfo fi(qf);
 
  dictionary * ini = iniparser_load("qenix.ini"); //Must deallocate manualy
  if (ini==NULL)
  {
  iniparser_freedict(ini);
  fprintf(stderr, "cannot parse file [%s]", "qenix.ini");
  return;
  }
	iniparser_dump(ini, stderr);
  
  qs = fi.family();
  ba = qs.toAscii();
 	if(iniparser_setstr(ini, "editor:font", ba.data()));
 	
 	
  ba = QByteArray::number(fi.pointSize());
 	if(iniparser_setstr(ini, "editor:pointsize", ba.data()));
 	
  ba = QByteArray::number(fi.pixelSize());
 	if(iniparser_setstr(ini, "editor:pixelsize", ba.data()));
 	
 	ba = QByteArray::number(fi.style());
 	if(iniparser_setstr(ini, "editor:style", ba.data()));
 	
 	ba = QByteArray::number(fi.weight());
 	if(iniparser_setstr(ini, "editor:weight", ba.data()));
 	
	FILE *fw;
	fw = fopen("qenix.ini", "wb+");
	
	iniparser_dump_ini(ini, fw);
	fclose(fw);
  iniparser_freedict(ini);

}

void MainWindow::quickReplace()
{
 Tabw->currentWidget()->setText(QString(" "));
 paste();
}

void MainWindow::readNextWord()
{

}

void MainWindow::readPreviousWord()
{

}


void MainWindow::dockFilesClicked(QModelIndex index)
{
  if (index.isValid())
  {
    QString filePath = DirModel->filePath(index);
    //or QFileInfo fileInfo = dirModel->fileInfo(index);  ...
    if (!DirModel->isDir(index))
    {
      //msgbox(filePath);
      Tabw->newTab(filePath);
    }
  }
}


