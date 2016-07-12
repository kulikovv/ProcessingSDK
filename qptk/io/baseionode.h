#ifndef BASEIONODE_H
#define BASEIONODE_H

#include <base/node.h>

namespace io{

class PROCESSINGFRAMEWORK_SHARED_EXPORT BaseIONode : public base::Node
{
    Q_OBJECT
    Q_CLASSINFO("url",tr("Location of stored/loaded file."))
    Q_PROPERTY(QString url READ url WRITE setUrl )
    Q_CLASSINFO("extension",tr("The extension of selected file."))
    Q_PROPERTY(QString extension READ extension )
public:
    BaseIONode(QObject* obj);
    /*! Узнать название файла в который будет происхдить запись
      */
    QString url() const;
    /*! Получить расширение файла
      */
    QString extension() const;
signals:
   /*! После успешной записли вызывается метод
     * \param url адрес записанного файла
     */
   void ended(QString url);
public slots:
    /*! Изменить название в файла в которой будет происходить запись
      */
    virtual void setUrl(QString value);
    /*! Настроить расширение файла
      */
    //void setExtension(QString value);
    /*!
     * \brief reset обнуляется _url по умолчанию
     */
    virtual void reset();
   virtual QIcon getIcon();
protected:
    QString _url; //!< Куда записываем
    QString _extension; //!< Расширение файла
};

}

#endif // BASEIONODE_H
