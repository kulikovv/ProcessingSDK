#ifndef PROCESSINGFRAMEWORK_VISUALNODE_H
#define PROCESSINGFRAMEWORK_VISUALNODE_H

#include <base/node.h>
/*! \brief Простарнство имен для узлов визуализации.
  Простарнство имен для узлов визуализации. Содежит базовые классы, такие как VisualNode,
  а также примитивы для визуализации изобржений.
  Подпространства имен содержат методы визуализации различных типов данных.
  Подпространство имен visual::ui содержит виджеты
  \sa visual::ui visual::kinect visual::text
  */
namespace visual
{
/*! Визуализация
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT VisualNode:public base::Node
{
    Q_OBJECT
    Q_PROPERTY( bool visible READ visible WRITE setVisible )
    Q_PROPERTY(int tabOrder READ tabOrder WRITE setTabOrder)
    public:
    /*! Создание узла
      */
    explicit VisualNode(QObject *parent = 0);
    /*! Удаление созданного виджета
      */
    virtual ~VisualNode();
    /*! Получить изображение
     *  \return представление
     */
    virtual QWidget* getWidget();
    virtual bool visible();
    int threadId() const;
    /*! Обнулить представление если надо
      по умолчанию ничего не делает.
      */
    virtual void reset();
	int tabOrder();
public slots:
    virtual void setVisible(bool vis);
	void setTabOrder(int order);
protected:
    /*! Переопределяемая функция обработки данных
      * т.к. скорость вызова виртуальной функции >> чем вызов виртуального СЛОТа то
      * мы ее выделили
      */
    virtual void receiveProcess(esp data)=0;
    QWidget* _widget;
    static QWidget* _invisibleParent;//!< Для невидимых
	int _taborder;

};

}


#endif // VISUALNODE_H
