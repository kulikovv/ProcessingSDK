
#ifndef PROCESSINGFRAMEWORK_INTERFACES_H
#define PROCESSINGFRAMEWORK_INTERFACES_H
/*#include "Windows.h"
// near the top of your CPP file
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#include <QFileInfo>*/
#include <sdk_global.h>
#include <base/node.h>
#include <QStringList>

#define REGISTER(classname) REGISTER_DYN_NODE(classname,filename) _nodes.push_back(#classname);
/*! \brief Интерфейс к библиотеке узлов.
  Интерфейс к библиотеке узлов. Унифицированный интерфейс, которые должны реализовывать
  все библиотеки с узлами. Он экспортируется через QT_PLUGIN
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT NodeLibrary
{
    public:


	~NodeLibrary()
	{
       // qDebug() << "Library killed";
		foreach(QString nodename,_nodes)
		{
        //	UNREGISTER_DYN_NODE(nodename)
		}
	}
    /*! Возвращает название библиотеки
      */
    virtual QString getLibraryName()=0;
    /*! В этой функции регистрируются все динамические классы в библиотеке
      */
    virtual void registrate(QString filename)=0;
protected:
	QStringList _nodes;//Доваленные макросом REGISTER узлы
};

Q_DECLARE_INTERFACE(NodeLibrary,"com.ethostudio.EthoStudio.NodeLibrary/1.0")

#endif // INTERFACES2_H
