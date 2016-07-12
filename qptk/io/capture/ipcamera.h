#ifndef IPCAMERA_H
#define IPCAMERA_H

#include "opencvcameranode.h"

#include <QFutureWatcher>
#include <opencv2/highgui.hpp>
namespace io
{
namespace capture
{
/*!
 * \brief The IpCamera class класс, подключающий IP камеру в граф средствами OpenCV
 */
class IpCamera : public OpenCVCameraNode{
    Q_OBJECT
    Q_CLASSINFO("ipadress",tr("The url location of the camera including address and port."))
    Q_PROPERTY( QString ipadress READ ipadress WRITE setIpadress USER true)
public:
    explicit IpCamera(QObject *parent);
    ~IpCamera();
    QString ipadress();
public slots:
    /*!
     * \brief setDeviceid не используется вместо нее надо использовать setUrl(QString)
     * \param value не используется
     */
    virtual void setDeviceid (int value);
    /*!
     * \brief setUrl поключиться к удаленной камере по TCP/IP протоколу
     * \param value IP адрес или URL
     */
    virtual void setUrl(QString value);
    void setIpadress(QString address);
protected slots:
    /*!
     * \brief handleConnection слот, срабатывающий после асинхронного подключения
     */
    void handleConnection();
protected:
    /*!
     * \brief concurrentOpen функция асинхронного подключения к IP камере
     * \param pCap указатель в который будет сохранен хэндел
     * \param name адрес см setUrl
     */
    static void concurrentOpen(cv::VideoCapture* pCap,QString name);
    /*!
     * \brief getFrame получить кадр с камеры
     * \param mat куда сохраняется кадр
     * \return
     */
    virtual bool getFrame(cv::Mat &mat);
    /*!
     * \brief _watcher переменная для ассинхронного возаимодействия с камерой
     */
    QFutureWatcher<void> _watcher;
    cv::VideoCapture *_camera;
};
}
}
#endif // IPCAMERA_H


