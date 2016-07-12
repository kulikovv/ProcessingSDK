#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <visual/visualnode.h>
#include <types/data.h>
class QTableWidget;

namespace visual{


/*! \brief Визуализация текстовых данных данных в виде двумерной матрицы. Столбцы обычно соответсвуют параметрам, строки - кадрам.
    Классы для визуализации текстовых данных.
  */
namespace text{
class TableView : public VisualNode
{
Q_OBJECT
//Q_PROPERTY( QStringList header READ header WRITE setHeader )
public:
    explicit TableView(QObject *parent=0);

    virtual void reset();
signals:

public slots:
    virtual QIcon getIcon();
    /*!
     * \brief getColumns получить список списков значений из нескольких столбцов
     * \param id0 номер первого столбца
     * \param id1 номер второго столбца
     * \param id2 номер третьего столбца
     * \param id3 номер четвертого столбца
     * \return список списков значений из нескольких столбцов. внешний список соответсвует полонка, внутренний - данным в колонках
     */
    QList<QList<double> > getColumns(int id0,int id1=-1,int id2=-1,int id3=-1);
    /*!
     * \brief getColumn выставить заголовки столбцам
     * \param value список заголовков, разделенных точкой с запятой
     */
    void setHeader(QStringList value);
    /*!
     * \brief getColumn получить столбец по номеру
     * \param id номер столбца
     * \return все значения в столбце в виде списка
     */
    QList<double> getColumn(int id);
    /*!
     * \brief getColumn получить столбец по его заголовку
     * \param name заголовок столбца
     * \return все значения в столбце в виде списка
     */
    QList<double> getColumn(QString name);
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
    /*!
     * \brief getColumnHeader получить заголовок столбца по номеру
     * \param id номер столбца
     * \return  заголовок столбца
     */
    QString getColumnHeader(int id);
    /*!
     * \brief coloringRows покрасить в сктроки в определенный цвет (смотреть также Statistics)
     * \param x список значений класса т.е. для каждой строки в таблице есль значение которое соответсвует
     *к кокому классу активности принадлежит строка x[i]<5
     * \param start начальное значение с которого красим
     * \param end конечное значение до которого красим, если -1 то значить работаем над всеми кадрами
     */
    void coloringRows(QList<double> x,int start=-1,int end=-1);
protected:
    void addRow(types::Data &data,int fid);
    QTableWidget* getTableWidget();
    virtual void receiveProcess(esp data);
    QStringList _header;
    unsigned long _realcount;

};
}
}
#endif // TABLEVIEW_H
