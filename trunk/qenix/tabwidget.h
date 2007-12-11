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

#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui>
#include <QTabWidget>
#include <QList>
#include <QPushButton>
#include "codeedit.h"

class TabWidget: public QTabWidget
{
Q_OBJECT
public:
TabWidget(QWidget *parent = 0);
CodeEdit *newTab();
CodeEdit *newTab(const QString& file);
void deleteTab(int index);
QEditor *currentWidget();
void setFont(QFont font);

private:
void createActions();
void createToolBars();


QToolBar *leftToolBar;
QToolBar *rightToolBar;

QAction *newTabAct;
QAction *deleteTabAct;

QList <QCodeEdit*> list;

QFont lastFont;

public slots:
void newDoc();
void deleteCurrentTab();


};

#endif
