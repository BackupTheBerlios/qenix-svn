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

#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent):QTabWidget(parent)
{
  createActions();
  createToolBars();
  newTab();

}

void TabWidget::createToolBars()
{
  leftToolBar = new QToolBar(this);
  rightToolBar = new QToolBar(this);
  
  setCornerWidget(leftToolBar, Qt::TopLeftCorner);
  setCornerWidget(rightToolBar, Qt::TopRightCorner);
  
  leftToolBar->addAction(newTabAct);
  rightToolBar->addAction(deleteTabAct);

//  foreach ()
// {
// }
}

void TabWidget::createActions()
{
  newTabAct = new QAction(QIcon(":/actions/tab-new.svg"), tr("&New tab"), this);
  newTabAct->setShortcut(tr("Ctrl+N"));
  newTabAct->setStatusTip(tr("Open a new tab"));
  connect(newTabAct, SIGNAL(triggered()), this, SLOT(newDoc()));
  
  deleteTabAct = new QAction(QIcon(":/images/new.png"), tr("&Close tab"), this);
  deleteTabAct->setShortcut(tr("Ctrl+C"));
  deleteTabAct->setStatusTip(tr("Closes current tab"));
  connect(deleteTabAct, SIGNAL(triggered()), this, SLOT(deleteCurrentTab()));
}

void TabWidget::newDoc()
{
	newTab();
}

//Creates a new tab a returns a pointer to new CodeEdit (NEVER call delete on the returned pointer, object is deallocated automatically)
CodeEdit *TabWidget::newTab()
{
   CodeEdit *ce = new CodeEdit(this);
   ce->editor()->document()->setFont(lastFont);
   addTab(ce->editor(), QString("Dokument"));
   setCurrentWidget(ce->editor());
   list << ce;
   return ce;
}

CodeEdit *TabWidget::newTab(const QString& file)
{
	if ( QFile::exists(file) )
	{
		CodeEdit *ce = newTab();
		ce->getLanguages()->setLanguage(ce->editor(), file);
		ce->editor()->load(file);
		return ce;
	}
	return 0;
}



void TabWidget::deleteTab(int index)
{
  /*
  CodeEdit *ceItem;
  bool finish = false;
  foreach (ceItem, list) // Finds CodeEdit from its Editor Widget in the list
  {
    if(finish) break;
    if (ceItem->editor() == widget(index))
    {
      //QMessageBox::about(0, "Debug Window", "deleting tab");
      ceItem->editor()->setText(QString("funguji :)"));
      QMessageBox::about(0, "Debug Window", "breakpoint");
      QTabWidget::removeTab(index);
   
      s
      list.removeAt(list.indexOf(ceItem)); //This AFAIK ineffective
      
      if (ceItem==0) QMessageBox::about(0, "Debug Window", "prusvih");
      delete ceItem; //Frees all the memory. Not working - dont know why.
      finish = true;
    }
  }
  */
  
  if (list.count()== 1) return; //Keep at least one tab
  
  for (int i = 0; i < list.count(); i++)
  {
    if (list[i]->editor() == widget(index))
    {
    removeTab(index);
    delete list.takeAt(index);
    break;
    }
  }
  
  
}


void TabWidget::deleteCurrentTab()
{
qDebug() << currentIndex();
deleteTab(currentIndex());
}

QEditor *TabWidget::currentWidget()
{
  for (int i = 0; i < list.count(); i++) if (list[i]->editor() == QTabWidget::currentWidget()) return list[i]->editor();
  return 0;
}

void TabWidget::setFont(QFont font)
{
  lastFont = font;
  for (int i = 0; i < list.count(); i++)
  {
    list[i]->editor()->document()->setFont(font);
  }
}
