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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTabWidget>
#include "ttscpw.h"
#include "textformater.h"
#include "ttscpvoice.h"
#include "find.h"
#include "codeedit.h"
#include "tabwidget.h"
#include "ui_mainwindow.h"


class QAction;
class QMenu;
class QWorkspace;
class QSignalMapper;
class QTabWidget;
class QCodeEdit;

void msgbox(QString t);
int initEpos();

class MainWindow : public QMainWindow, private Ui::MainWindow
{
Q_OBJECT

public:
    MainWindow();
    void say(QString text);
    void sayf(QString text); //formatted
    void stop();
    
    //QTE
    int qte();
    
    
    QCodeEdit *qte2();
    
protected:
    void closeEvent(QCloseEvent *event);
    
private slots:
    void updateMenus();
    
    //File
    void newFile();
    void open();
    void save();
    void saveAs();
    void switchLayoutDirection();
    
    //Edit
    void cut();
    void copy();
    void paste();
    void about();
    void undo();
    void redo();
    //void find();
    //void findNext();
    void quickReplace();
    
    //Format
    void font();
    
    //TTSCP
    void ttscpSettings();
    void ttscpVoice();
    
    //Read
    void readAll();
    void readSelected();
    void stopReading();

    void readNextWord();
    void readPreviousWord();

    void rHash();
    void rAsterisk();
    void rChevrons();
    void rRoundBrackets();
    void rSquareBrackets();
    void rQuotes();
    void rApostrophe();
    void rNumbers();
    void rSeparateNumbers(); //Separate numbers - exapmple: "4236" => "4 2 3 6"
    void rLowerCase();
  
public slots:
    
    
private:
    void createActions();
    void createMenus();
    void createStatusBar();
    
    int loadSettings();
    int writeSettings();
    
    TabWidget *Tabw;

    QString lastFound;
    //QSignalMapper *windowMapper;
    QFont currentFont;

// read/dont read actions
    QAction *rRoundBracketsAct;    // ()
    QAction *rSquareBracketsAct; // []
    QAction *rChevronsAct; // <>
    QAction *rQuotesAct; // "
    QAction *rHashAct; // #
    QAction *rAsteriskAct; // *
    QAction *rNumbersAct; //Dont read numbers
    QAction *rApostropheAct; // '
    QAction *rNumAct; //Arabic numbers 0-9
    QAction *sepNumAct; // "2314" -> "2 3 1 4 "
    QAction *toLowerCAct;
    QAction *ttscpSettingsAct;
    QAction *ttscpVoiceAct;

    textFormater tf;

};

#endif
