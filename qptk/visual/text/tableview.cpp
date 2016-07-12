#include "tableview.h"

#include <QVector>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDebug>
#include <types/data.h>
#include <utils/logs.h>
#include <visual/forms/qtablewidgetcopy.h>
using namespace visual;
using namespace visual::text;

TableView::TableView(QObject * parent):VisualNode(parent)
{
    _realcount=0;
    this->_widget = new visual::ui::QTableWidgetCopy(_invisibleParent);
    QTableWidget* table =  getTableWidget();
    if(0!=table){
        table->setEditTriggers(QTableWidget::NoEditTriggers);
        table->setFocusPolicy(Qt::NoFocus);
    }

    //setHeader("Col1;Col2;Col3");
}

QIcon TableView::getIcon()
{
    return QIcon(":/sdk/visual/text/TableView");
}

QTableWidget* TableView::getTableWidget()
{
    return qobject_cast<QTableWidget*>(_widget);
}

QString  TableView::getColumnHeader(int id)
{
  //  QStringList headers = _header.split(";", QString::SkipEmptyParts);
    if(id>=0&&id<_header.count())
    {
        return _header[id];
    }
    return "";
}

void TableView::coloringRows(QList<double> x,int start,int end)
{
    QList<QColor> colors;
    colors << QColor(255,255,255) << QColor(255,0,0) << QColor(0,255,0) << QColor(0,0,255) << QColor(255,255,0);
    QTableWidget* table =  getTableWidget();
    if(start<0||start>table->columnCount())
    {
        start = 0;
    }
    if(end<0||end>table->columnCount())
    {
        end = table->columnCount();
    }
    if(0!=table){
        for(int i=0;i<x.length();i++)
        {
            if(table->item(i,0))
            {
                for(int j=start;j<end;j++)
                {
                    int idx = ((int)x[i])%colors.length();
                    table->item(i,j)->setBackgroundColor( colors[idx]);

                }
            }
        }
    }
}

QList<double> TableView::getColumn(int id)
{
    QList<double> res;
    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
        if(id>getColumnCount()&&id<0)
        {
            return res;
        }
        bool isOk = false;
        for(int i=0;i<getRowCount();i++)
        {

            QTableWidgetItem* wi = table->item(i,id);
            if(!wi)
                continue;
            double dat = wi->data(Qt::DisplayRole).toDouble(&isOk);
            if(isOk){
                res.push_back(dat);
            }
        }

        return res;
    }
    return res;
}

QList<double> TableView::getColumn(QString name)
{

    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
       // QStringList headers = _header.split(";", QString::SkipEmptyParts);
        int id = _header.indexOf(name);
        if(-1!=id)
        {
            return getColumn(id);
        }
    }
    return QList<double>();
}

QList<double> TableView::getRow(int id)
{

    QList<double> res;
    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
        if(id>getRowCount()&&id<0)
        {
            return res;
        }
        bool isOk = false;
        for(int i=0;i<getColumnCount()-1;i++)
        {

            QTableWidgetItem* wi = table->item(id,i);
            if(!wi)
                continue;
            double dat = wi->data(Qt::DisplayRole).toDouble(&isOk);
            if(isOk){
                res.push_back(dat);
            }
        }

        return res;
    }

    return res;
}

unsigned long TableView::getRowCount()
{
    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
        return _realcount;
    }
    return 0;
}

unsigned long TableView::getColumnCount()
{
    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
        return table->columnCount();
    }
    return 0;
}

void TableView::receiveProcess(esp data)
{


    QSharedPointer<types::pData> full = data.getReadOnly<types::pData>();
    if(!full.isNull())
    {
		//INFO("Recieve full")
        setHeader(full->header());
        QTableWidget* table =  getTableWidget();
        if(0!=table)
        {
            table->setRowCount(0);
        }
        for(uint i=0;i<full->size();i++)
        {
            types::Data d = (*full)[i];
            if(!d.isempty()){
                addRow(d,d.frameId());
            }
        }
    }
    QSharedPointer<types::Data> item = data.getReadOnly<types::Data>();
    if(!item.isNull())
    {
        if(item->header()!=_header)
        {
            setHeader(item->header());
        }
        addRow(*item,data.getFrameId());
    }
}
void TableView::reset()
{
	INFO("Reset")
    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
        table->clear();
      //  QStringList headers = _header.split(";", QString::SkipEmptyParts);
        table->setColumnCount(_header.count());
        table->setHorizontalHeaderLabels(_header);
    }
}
void TableView::addRow(types::Data &data, int fid)
{
    QTableWidget* table =  getTableWidget();

    if(0!=table)
    {
        if(fid>=table->rowCount())
        {
            table->setRowCount(fid+50);
        }
        unsigned int size = data.size();
        if(table->columnCount()<(int)size)
        {
            size = table->columnCount();
        }
        for(unsigned int i=0;i<size;i++)
        {
            table->setItem(fid,i,new QTableWidgetItem(QString("%1").arg(data[i])));
        }
        table->selectRow(fid);
        if(_realcount<fid)
        {
            _realcount = fid;
        }
   }
}


QList<QList<double> > TableView::getColumns(int id0,int id1,int id2,int id3)
{
    QList<QList<double> > list;

    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
        for(int i=0;i<getRowCount();i++)
        {

            bool isOk = false;
            QList<double> small;
            for(int j=0;j<getColumnCount();j++)
            {
                if(j==id0||j==id1||j==id2||j==id3)
                {
                    QTableWidgetItem* wi = table->item(i,j);
                    if(!wi)
                        continue;
                    double dat = wi->data(Qt::DisplayRole).toDouble(&isOk);
                    if(isOk){
                        small.push_back(dat);
                    }
                }
            }
            list.push_back(small);
        }

    }

    return list;
}

void TableView::setHeader(QStringList value)
{
    //_header = value;
    QTableWidget* table =  getTableWidget();
    if(0!=table)
    {
       // QStringList headers = _header.split(";", QString::SkipEmptyParts);
        table->setColumnCount(value.count());
        table->setHorizontalHeaderLabels(value);
        _header = value;
      //  table->setVerticalHeaderItem();
      //  table->setVerticalHeaderLabels(_header.split(";", QString::SkipEmptyParts));
    }
  //  emit send(esp::init(new QString(value),0,""));
}
