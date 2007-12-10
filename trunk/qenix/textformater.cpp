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

#include "textformater.h"

textFormater::textFormater()
{
rNum = true; // 0123456789
rRoundBrackets = true; // ()
rSquareBrackets = true; // []
rQuotes = true; // "
rAsterisk = true; // *
rHash = true; // #
rChevrons = true; // <>
rApostrophe = true; // '
toLower = false;
sepNum = false;
}

QString textFormater::format(QString text)
{
if (!rAsterisk) text.replace(QString("*"), QString(" "));
if (!rApostrophe) text.replace(QString("'"), QString(" "));
if (!rHash) text.replace(QString("#"), QString(" "));
if (!rQuotes) text.replace(QString("\""), QString(" "));

if (!rRoundBrackets)
{
    text.replace(QString("("), QString(" "));
    text.replace(QString(")"), QString(" "));
}

if (!rSquareBrackets)
{
    text.replace(QString("["), QString(" "));
    text.replace(QString("]"), QString(" "));
}

if (!rChevrons)
{ 
    text.replace(QString("<"), QString(" "));
	text.replace(QString(">"), QString(" "));
}

if (!rNum)
{
    for (int i = 0; i <= 9; i++) text.replace(QString::number(i, 10), QString(" "));
}

if (sepNum)
{
    for (int i = 0; i <= 9; i++) text.replace(QString::number(i, 10), QString::number(i, 10).append(QString(" ")));
}

if (toLower) text = text.toLower();

return text;
}

