#ifndef TABLEVIEW1_H
#define TABLEVIEW1_H

#include <visual/visualnode.h>
#include <types/data.h>

#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QTableView>
#include <QHeaderView>

namespace visual{


/*! \brief Визуализация текстовых данных данных в виде двумерной матрицы. Столбцы обычно соответсвуют параметрам, строки - кадрам.
    Классы для визуализации текстовых данных.
  */
namespace text{

class HierarchicalHeaderView : public QHeaderView
{
Q_OBJECT

    class private_data;
    private_data* _pd;

    QStyleOptionHeader styleOptionForCell(int logicalIndex) const;

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const;
    QSize sectionSizeFromContents(int logicalIndex) const;
public:

    enum HeaderDataModelRoles {
        HorizontalHeaderDataRole = Qt::UserRole,
        VerticalHeaderDataRole = Qt::UserRole+1
    };

    HierarchicalHeaderView(Qt::Orientation orientation, QWidget* parent = 0);
    ~HierarchicalHeaderView();

    void setModel(QAbstractItemModel* model);
private slots:
    void on_sectionResized(int logicalIndex);
};

class TableModel : public QAbstractTableModel {

private:
    int mnRows;
    int mnColumns;

    int countArguments;
    int countObjects;

    float emptyValue;
    int constMatSize;

    QStringList _objects;//Список названия объектов
    QStringList _args;//Список свойств каждого из объектов


    QHash <QModelIndex, QVariant> mhash;

    QStandardItemModel _horizontalHeaderModel;

public:
    TableModel(QObject* parent=0): QAbstractTableModel(parent) {
        init();
        mnRows = constMatSize;
    }

    void init() {
        _objects << "A" << "B" << "C";
        _args << "X" << "Y" << "Z";
        countArguments = _args.count();
        countObjects = _objects.count();
        mnColumns = getSize();

        emptyValue = -1;
        constMatSize = 50; //50 //5000
        //matStepSize = 50; //50 //1000

        fillHeaderModel(_horizontalHeaderModel);
    }

    QVariant data(const QModelIndex &index, int nRole) const {
        if(nRole == HierarchicalHeaderView::HorizontalHeaderDataRole) {
            QVariant v;
            v.setValue((QObject*)&_horizontalHeaderModel);
            return v;
        }
        if (!index.isValid()) {
            return QVariant();
        }
        //QString str = QString("%1,%2").arg(index.row() + 1).arg(index.column() + 1);
        QString str = QString::number(emptyValue);
        return (nRole == Qt::DisplayRole || nRole == Qt::EditRole) ?
                    mhash.value(index, QVariant(str)) : QVariant();        
    }

    bool setData(const QModelIndex &index, const QVariant &value, int nRole) {
        if (index.isValid() && (nRole == Qt::DisplayRole || nRole == Qt::EditRole)) {
            mhash[index] = value;
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }

    int rowCount(const QModelIndex &) const {
        return mnRows;
    }

    int columnCount(const QModelIndex &) const {
        return mnColumns;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const {
        Qt::ItemFlags flags = QAbstractItemModel::flags(index);
        return index.isValid() ? (flags | Qt::ItemIsEditable) : flags;
    }

    void setRowCount(int nRows) {
       mnRows = nRows;
    }

    void setColumnCount(int nColumns) {
       mnColumns = nColumns;
    }

    void fillHeaderModel(QStandardItemModel& headerModel);

    void insertRow(types::Data data, int frameId);
    void clear();

    void setCountObjects(int count) {
        if (count >= 0) {
            countObjects = count;
            //mnColumns = getSize();
            fillHeaderModel(_horizontalHeaderModel);
        } else {
            //exception
        }
    }

    void setObjects(QString objs)
    {
        _objects = objs.split(";");
        setCountObjects(_objects.count());
    }

    void setArgs(QString args)
    {
        _args = args.split(";");
        setCountArguments(_args.count());
    }

    void setCountArguments(int count) {
        if (count >= 0) {
            countArguments = count;
            //mnColumns = getSize();
            fillHeaderModel(_horizontalHeaderModel);
        } else {
            //exception
        }
    }

    int getCountObjects() {
        return countObjects;
    }

    int getCountArguments() {
        return countArguments;
    }

    QStringList getObjects()
    {
        return _objects;
    }

    QStringList getArgs()
    {
        return _args;
    }

    void setDataSet(int countObj, int countArg) {
        if ((countObj >= 0) && (countArg >= 0)) {
            countObjects = countObj;
            countArguments = countArg;
            //mnColumns = getSize();
            fillHeaderModel(_horizontalHeaderModel);
        } else {
            //exception
        }
    }

    int getSize() {
        return countObjects * countArguments;
    }

    static const int matStepSize = 50;
    types::Data pDataToData(types::pData pdata);
};

class MultiObjectTableView : public VisualNode
{
Q_OBJECT
//Q_PROPERTY(int countArguments READ getCountArguments WRITE setCountArguments)
//Q_PROPERTY(int countObjects READ getCountObjects WRITE setCountObjects)
Q_PROPERTY(QString objects READ objects WRITE setObjects)
Q_PROPERTY(QString agrs READ args WRITE setArgs)
public:
    explicit MultiObjectTableView(QObject *parent=0);
    virtual void reset(); //что должно здесь происходить?
    int getCountArguments() {
        return tableModel->getCountArguments();
    }
    int getCountObjects() {
        return tableModel->getCountObjects();
    }
    /*!
     * \brief objects список всех объектов, разделенный точкой с запятой
     * \return
     */
    QString objects()
    {
        return tableModel->getObjects().join(";");
    }
    /*!
     * \brief args все аргументы в виде одного списка, разбитого точкой с запятой
     * \return
     */
    QString args()
    {
        return tableModel->getArgs().join(";");
    }

signals:

public slots:
    /*!
     * \brief setObjects раставить заголовки список объектов
     * \param objs
     */
    void setObjects(QString objs)
    {
        tableModel->setObjects(objs);
    }

    /*!
     * \brief setArgs раставить чсписки
     * \param args
     */
    void setArgs(QString args)
    {
        tableModel->setArgs(args);
    }

    //TODO
    //QList<double> getValues(QString obj,QString arg)
    //int getObjectsCount();
    //QString getObjectName(int id);
    //int getArgsCount();
    //QString getArgName(int id);
    //
    /*!
     * \brief getColumn получить столбец по номеру
     * \param id номер столбца
     * \return все значения в столбце в виде списка
     */
    QList<double> getColumn(int id);

    /*!
     * \brief getRow получить строку по номеру
     * \param id
     * \return
     */
    QList<double> getRow(int id);

    /*!
     * \brief getRowCount количество строк
     * \return возвращает количество строк
     */
    unsigned long getRowCount();

    /*!
     * \brief getColumnCount количество столбцов
     * \return возвращает количество столбцов
     */    
    unsigned long getColumnCount();

    void setCountArguments(int value) {
        tableModel->setCountArguments(value);
    }
    void setCountObjects(int value) {
        tableModel->setCountObjects(value);
    }

protected:
    QTableView* getTableView();
    TableModel* tableModel;
    HierarchicalHeaderView* hv;
    virtual void receiveProcess(esp data);


};
}
}
#endif // TABLEVIEW1_H
