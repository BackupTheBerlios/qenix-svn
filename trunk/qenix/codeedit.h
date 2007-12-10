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

#ifndef CODEEDIT_DERIVED_H
#define CODEEDIT_DERIVED_H

//QCodeEdit headers
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

class CodeEdit: QObject, public QCodeEdit
{ 
Q_OBJECT
public:
CodeEdit(QWidget *parent = 0);
QLanguageFactory *getLanguages();

private:
QLanguageFactory *l;

};

#endif
