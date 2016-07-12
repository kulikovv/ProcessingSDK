#ifndef GRAPHSETTINGS_H
#define GRAPHSETTINGS_H

#include "graphxml.h"
class QSettings;

namespace utils
{
/*!
 * \brief The GraphSettings class
 *
 */
class PROCESSINGFRAMEWORK_SHARED_EXPORT GraphSettings //: public GraphXml
{
public:
    static void saveSettings(base::Graph* graph,QString setting="default");
    static void loadSettings(base::Graph* graph,QString setting="default");
    static void saveSettingsFile(base::Graph* graph,QString filename);
    static void loadSettingsFile(base::Graph* graph,QString filename);
    static QStringList getStoredConfigs(base::Graph* graph);
private:
    static void saveGraph(base::Graph* graph, QSettings* settings);
    static void loadGraph(base::Graph* graph, QSettings* settings);

  /*  GraphSettings(QString graphname,QString setting="default",QString application="qptk");
    ~GraphSettings();

    virtual void storeGraph(QString xmlname,base::Graph* graph,QStringList libs=QStringList(),QList<QPair<int,QPointF> > decor=QList<QPair<int,QPointF> >());
	void setIgnoreUrl(bool value);
protected:
    virtual void readNode(base::Node* node);
    virtual void writeNode(int id,base::Node* node);
    virtual void onConnection(int first,int second);
private:
    QSettings* _settings;//Ссылка на настройки
    QString _settingName;//Название настроек
    bool _ignoreUrls;//Не сохранять поле url
    QStringList _ignorePpties;//Список игнорируемыех свойств*/
};

}

#endif // GRAPHSETTINGS_H
