#ifndef PROCESSINGFRAMEWORK_IMAGENODE_H
#define PROCESSINGFRAMEWORK_IMAGENODE_H

#include <io/sourcenode.h>

namespace io{
/*! \brief Классы работы с изображениями
  Содержит базовые узлы для загрузки и сохранения изображений
  \sa types::Rgb
  */
    namespace media{
class PROCESSINGFRAMEWORK_SHARED_EXPORT ImageNode : public SourceNode {
    Q_OBJECT
    Q_ENUMS(Mode)
    Q_PROPERTY( Mode mode READ mode WRITE setMode)
    Q_PROPERTY( bool loadColor READ isColorImage WRITE setColorImage )
    Q_PROPERTY(int signs READ signs WRITE setSigns)
    Q_CLASSINFO("Accept","PMat")
    Q_CLASSINFO("Send","PMat")
public:
    enum Mode {
                Read, //!< Только считывает данные из файловой системы
                Write, //!< Только записывает данные
                ReadWrite//!< Читает и записывает
            };

    /*! Consturct
      */
    explicit ImageNode(QObject *parent);
    /*!Call from synchronizer 
      * general rule frame = (0,length())
      * if frame = -1 it means to take next frame
      */
    virtual esp get(const unsigned long frame);
    Mode mode();
    bool isColorImage();
public slots:
    virtual QIcon getIcon();
    int signs();//Кол-во нулей в номере
    void setSigns(int v);//Установить кол-во нулей
    virtual void setColorImage(bool value);
    void setMode(Mode value);
    virtual void setUrl(QString value);
protected:
    /*! Call from other Nodes
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
private:
    //Current data
    Mode _mode;
    int _iscolor;
    int _signs;//Кол-во нулей
};
}
}
#endif // IMAGENODE_H

