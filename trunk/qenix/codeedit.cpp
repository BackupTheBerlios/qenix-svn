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

#include "codeedit.h"


#include <QtGui>

CodeEdit::CodeEdit(QWidget *parent): QObject(parent), QCodeEdit(parent)
{	
	
	static QFormatFactory *ff;
	static QLanguageFactory *lang;

	static bool doit = true;
	if (doit)
	{
	doit = false;
	ff = new QFormatFactory("qxs/formats.qxf"); //Memory leak, ff se nikdy nedealokuje
	QDocument::setFormatFactory(ff);

	QLineMarksInfoCenter::instance()->loadMarkTypes("qxs/marks.qxm");
	lang = new QLanguageFactory(ff); //Memory leak
	lang->addDefinitionPath("qxs");
	}
	l = lang;
	
	addPanel(new QLineMarkPanel, QCodeEdit::West);
	addPanel(new QLineNumberPanel, QCodeEdit::West);
	addPanel(new QFoldPanel, QCodeEdit::West);
	addPanel(new QStatusPanel, QCodeEdit::South);
	addPanel(new QSearchReplacePanel, QCodeEdit::South);
	
	lang->setLanguage(editor(), QString("x.cpp"));
	
	editor()->document()->setFont(QFont("Courier New", 10));
}

QLanguageFactory *CodeEdit::getLanguages()
{
	return l;
}

