#ifndef GRAPHPROXYEDITOR_H
#define GRAPHPROXYEDITOR_H

#include <QObject>
#include <utils/graphdom.h>
#include "editorscene.h"
#include "node.h"

class GraphProxyEditor : public QObject
{
    Q_OBJECT
public:
    explicit GraphProxyEditor(QObject *parent = 0);
    ~GraphProxyEditor();

    QString saveGraph();
    bool loadGraph(QString planexml);

    void addBaseGraph(QString name,synch::Synch* graph,QMap<QString,utils::NodeDecoration> decor);
    synch::Synch* getGraph();
    ui::EditorScene* getScene();
    void clear();

signals:
    /*!
     * \brief runзапустить граф
     */
    void run();

    /*!
     * \brief stop
     */
    void stop();
    /*!
     * \brief pause поставить граф на паузу
     */
    void pause();

    void reset();
    void selectedObject(QObject* obj);
    void widgetAdded(QWidget* wid);
public slots:
    /*!
     * \brief step
     */
    void step();
    ui::NodeItem *addItem(QString classname);
    void removeSelectedNode();
    void unlinkSelectedNode();
private slots:
    /*!
     * \brief nodeNameChanged имя узла изменилось
     * \param name
     * \return
     */
    bool nodeNameChanged(QString name);
    /*!
     * \brief onSelection нажатие на элемент из списка
     */
    void onSelection();
private:
    /*!
     * \brief GraphProxyEditor::getDecoration получить декорации
     * \return соотношение координат и названий
     */
    QMap<QString,utils::NodeDecoration> getDecoration();
    /*!
     * \brief assignNodes на карте
     * \param decor координаты узлов
     */
    void assignNodes(base::Graph *graph,QMap<QString,utils::NodeDecoration> decor);
    /*!
     * \brief initConnections
     */
    void initConnections();
    ui::EditorScene _scene;//Сцена графа
    synch::Synch* _synch;//Граф
};

#endif // GRAPHPROXYEDITOR_H
