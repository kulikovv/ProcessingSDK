#ifndef PROCESSINGFRAMEWORK_UIAUTOFITTER_H
#define PROCESSINGFRAMEWORK_UIAUTOFITTER_H

#include <QObject>
#include <sdk_global.h>
#include <QMap>
#include <QList>

namespace base
{
    class Graph;
    class Node;
}
namespace visual
{
    class VisualNode;
}
namespace utils
{
/*! Данный класс позволяет автоматически сопоставлять узлы визуализации
    объектам на форме по именам
    \brief Класс для автоматического расставления окон визуализации на главном окне в соотвествии структуры графа
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT UIAutoFitter : public QObject
{
    Q_OBJECT
public:
    explicit UIAutoFitter(QWidget *main);

signals:
    void widgetAdded(QString name,QWidget *widget,QWidget *parent);
public slots:
    /*!
     * \brief setObjects
     * \param nodes
     */
    void setObjects(QList<base::Node*> nodes);
    /*! Добавляет граф в список на работу
      \param gr граф
    */
    void addGraph(base::Graph* gr);
    /*! Срабатывает при изменении графа
      */
    void graphUpdate(base::Graph* g);
private slots:
    /*! В случае уничтожения объекта удаляет его из памяти
      */
    void onDestroyed(QObject* g);
private:
    /*! Сопоставляет элементы формы узлам визуализации по названиям
      objectName == name
      */
    void fitUI();
    /*! Рекурсивный разбор пользовательского интерфейса и
        индексирование его елементов
      */
    void parseUI(QWidget* root);
    QMap<QString,QWidget*> _uimap;
    QWidget* _mainframe;    //!< Корневой элемент дерева пользовательского интерфейса
    QList<visual::VisualNode*> _names;   //!< Список графов
};
}
#endif // UIAUTOFITTER_H
