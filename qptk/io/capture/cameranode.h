#ifndef PROCESSINGFRAMEWORK_CAMERANODE_H
#define PROCESSINGFRAMEWORK_CAMERANODE_H

#include <io/sourcenode.h>
#include <opencv2/core/core.hpp>
#include <QLine>

namespace camera
{
    class ICalibration;
}

namespace io
{
/*! \brief Классы захвата данных с камер
  Содержит базовые классы для работы с камерами и стереокамерами с поддержкой
  каллибровки и т.д.
  \sa camera
  */
namespace capture
{
class PROCESSINGFRAMEWORK_SHARED_EXPORT CameraNode : public SourceNode
{
    Q_OBJECT

    Q_PROPERTY( double fps READ fps WRITE setFps)
    Q_PROPERTY( int width READ width WRITE setWidth)
    Q_PROPERTY( int height READ height WRITE setHeight)
public:
    explicit CameraNode(QObject *parent = 0);
    ~CameraNode();
    /*!
     * \brief width количество столбцов в кадре, получаемого с камеры
     * \return
     */
    int width() const;
    /*!
     * \brief height количество строк в кадре
     * \return
     */
    int height() const;
    /*!
     * \brief fps количество кадров в секунду возвращаемых камерой
     * \return
     */
    double fps() const;

public slots:
    virtual QIcon getIcon();
    /*!
     * \brief setWidth принуди
     * \param value
     */
    virtual void setWidth(int value);
    /*!
     * \brief setFps
     * \param value
     */
    virtual void setFps(double value);
    /*!
     * \brief setHeight
     * \param value
     */
    virtual void setHeight(int value);

protected:
    /*!
     * \brief receiveProcess при вызове функции вызывается переопределяемая вирутуальная функция get
     * \param data данные с временным штампом и т.д.
     */
    virtual void receiveProcess(esp data);
    int _width;//!< Size of frame
    int _height;//!< Size of frame

};
}
}

#endif // CAMERANODE_H
