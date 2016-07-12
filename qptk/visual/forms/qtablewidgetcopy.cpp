#include "qtablewidgetcopy.h"
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <algorithm>
#include <QAction>
#include <QHeaderView>
#include <QFileDialog>
#include <QTextStream>

using namespace visual::ui;

QTableWidgetCopy::QTableWidgetCopy(QWidget *parent) :
    QTableWidget(parent)
{
    QAction *copy = new QAction(QIcon(":/sdk/visual/forms/Copy"),tr("Copy"),this);
    QAction *copyheaders = new QAction(QIcon(":/sdk/visual/forms/CopyHeaders"),tr("Copy with headers"),this);
    QAction *excel = new QAction(QIcon(":/sdk/visual/forms/excel"),tr("Save as excel"),this);
    copy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    this->addAction(copyheaders);
    this->addAction(copy);
    this->addAction(excel);
    connect(copy,SIGNAL(triggered()),this,SLOT(copy()));
    connect(copyheaders,SIGNAL(triggered()),this,SLOT(copyWithHeaders()));
    connect(excel,SIGNAL(triggered()),this,SLOT(saveExcel()));
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void QTableWidgetCopy::copyWithHeaders()
{
    QApplication::clipboard()->setText(toText(QLatin1Char('\t')));
}

void QTableWidgetCopy::saveExcel()
{
    QFileDialog dlg;
    QString file = dlg.getSaveFileName(0,tr("Save to excel"),QString(),"*.csv");
    if(!file.isEmpty())
    {
        QFile f(file);
        f.open(QFile::WriteOnly);
        QTextStream ts(&f);
        ts << toText(QLatin1Char(';'));
    }
}

void QTableWidgetCopy::copy()
{
  QItemSelectionModel * selection = selectionModel();
  QModelIndexList indexes = selection->selectedIndexes();

  if(indexes.size() < 1)
    return;

  // QModelIndex::operator < sorts first by row, then by column.
  // this is what we need
  std::sort(indexes.begin(), indexes.end());

  // You need a pair of indexes to find the row changes
  QModelIndex previous = indexes.first();
  indexes.removeFirst();
  QString selected_text;
  QModelIndex current;
  Q_FOREACH(current, indexes)
  {
    QVariant data = model()->data(previous);
    QString text = data.toString();
    // At this point `text` contains the text in one cell
    selected_text.append(text);
    // If you are at the start of the row the row number of the previous index
    // isn't the same.  Text is followed by a row separator, which is a newline.
    if (current.row() != previous.row())
    {
      selected_text.append(QLatin1Char('\n'));
    }
    // Otherwise it's the same row, so append a column separator, which is a tab.
    else
    {
      selected_text.append(QLatin1Char('\t'));
    }
    previous = current;
  }

  // add last element
  selected_text.append(model()->data(current).toString());
  selected_text.append(QLatin1Char('\n'));
  QApplication::clipboard()->setText(selected_text);
 // qApp->clipboard()->setText(selected_text);
}

QString QTableWidgetCopy::toText(QLatin1Char sep)
{

    QString selected_text;
    int rows = model()->rowCount(QModelIndex());
    int cols = model()->columnCount(QModelIndex());

    selected_text.append("");
    selected_text.append(QLatin1Char(sep));
    for(int c=0;c<cols;c++)
    {
        selected_text.append(model()->headerData(c,Qt::Horizontal,Qt::DisplayRole).toString().remove('\n'));
        selected_text.append(QLatin1Char(sep));
    }
    selected_text.append(QLatin1Char('\n'));

    for(int r=0;r<rows;r++)
    {
        selected_text.append(model()->headerData(r,Qt::Vertical,Qt::DisplayRole).toString().remove('\n'));
        selected_text.append(QLatin1Char(sep));
        for(int c=0;c<cols;c++)
        {
            QVariant data = model()->data(model()->index(r,c));
            selected_text.append(QString::number(data.toDouble()));
            selected_text.append(QLatin1Char(sep));
        }
        selected_text.append(QLatin1Char('\n'));
    }

    return selected_text.replace(".",",");
}

void QTableWidgetCopy::keyPressEvent(QKeyEvent * event)
{
  if(event->matches(QKeySequence::Copy) )
  {
    copy();
  }else
  {
    QTableWidget::keyPressEvent(event);
  }

}

