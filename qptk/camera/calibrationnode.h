#ifndef CALIBRATIONNODE_H
#define CALIBRATIONNODE_H

#include <process/processnode.h>

#include "calibration.h"
using namespace process;
namespace visual{
    namespace ui{
        class CalibrationDialog;
    }
}
namespace camera{
class PROCESSINGFRAMEWORK_SHARED_EXPORT CalibrationNode : public ProcessNode {
    Q_OBJECT

    Q_PROPERTY( bool calibration READ calibration WRITE setCalibration)
    Q_PROPERTY(QString intristics READ intristics WRITE setIntristics)
    Q_PROPERTY(QString distCoef READ distCoef WRITE setDistCoef)
    Q_PROPERTY(bool calibrated READ calibrated)
    Q_PROPERTY(pattern_type pattern READ pattern WRITE setPattern)
    Q_PROPERTY(double squereSize READ squereSize WRITE setSquereSize USER true)
    Q_PROPERTY(int gridx READ gridx WRITE setGridx USER true)
    Q_PROPERTY(int gridy READ gridy WRITE setGridy USER true)
    Q_PROPERTY(bool undistortion READ undistorsion)
    Q_PROPERTY(bool useNormalization READ useNormalization WRITE setUseNormalization USER true)
    Q_PROPERTY(bool useResolutionReduction READ useResolutionReduction WRITE setUseResolutionReduction USER true)
    Q_PROPERTY(double meanReprojectionError READ meanReprojectionError)
    Q_PROPERTY(float fovX READ fovX)
    Q_PROPERTY(float fovY READ fovY)
    Q_PROPERTY(bool fromMemory READ fromMemory WRITE setFromMemory USER true)
    Q_ENUMS(pattern_type)
 //   Q_PROPERTY()
    //Q_PROPERTY(bool doUndistortion READ intristics WRITE setIntristics)
public:
    enum pattern_type{CHESSBOARD /*! Шахматная доска*/,
                 CIRCLES/*! Круги*/,
                 CIRCLES_ASYM /*! Ассимитричные круги*/};
    explicit CalibrationNode(QObject *parent);
    ~CalibrationNode();
    bool calibration() const;
    QString intristics();
    QString distCoef();
    bool calibrated() const;
    int gridx();
    int gridy();
    double squereSize();
    /*!
     * \brief undistorsion
     * \return
     */
    bool undistorsion();
    bool fromMemory();

    bool useNormalization();
    /*!
     * \brief meanReprojectionError
     * \return
     * \sa Calibration.getMeanReprojectionError
     */
    double meanReprojectionError() const;
    /*!
     * \brief useResolutionReduction включить выключить уменьшение разрешения на первом приближении поска доски
     * \return
     * \sa Calibration.setReduceResolution
     */
    bool useResolutionReduction() const;
    float fovX() const;
    float fovY() const;
    pattern_type pattern() const;
signals:
    /*!
     * \brief onCalibrated срабатывает, если камера успешно откаллибрована
     */
    void onCalibrated();
    /*!
     * \brief sendFrame изображение с найденной доской
     * \param x изображение
     */
    void sendFrame(cv::Mat x);
    /*! Уменьшенная копия изображения для отрисовки
     */
    void sendFeed(cv::Mat feed);

public slots:
    void setUseNormalization(bool value);
    void setSquereSize(double value);
    void setGridx(int value);
    void setGridy(int value);
    void setFromMemory(bool value);

    void setIntristics(QString intistics);
    void setDistCoef(QString dist);
    /*!
     * \brief setCalibration запустить окно каллибровки
     * \param value если значение true, то запускает диалог каллибровки, по умолчанию всегда false
     */
    void setCalibration(bool value);
    /*!
     * \brief frameRequest запрос на новый кадр от диалога к узлу.
     *Кадр передается калибратору, тот возвращает изображение с
     *найденной шахматной доской, если все получилось. Изображение доски передается в диалог.
     */
    virtual void frameRequest();
    /*!
     * \brief setUseResolutionReduction включить/выключить уменьшение разрешения
     * \param value
     */
    void setUseResolutionReduction(bool value);
    void setPattern(pattern_type pt);
protected:
    /*! Procesing in this fuction
      */
    virtual void receiveProcess(esp data);
    Calibration _calib;//!< Класс калибратор. Реализует всю логику калибровки.
  //  virtual void successCallibration(camera::ICalibration* calib);
    bool _cameraReady;//!< Флаг подключена ли камера. Становится true после первого полученного кадра
    void makeFeed(cv::Mat data);

private slots:
    void badResult();
    /*!
     * \brief calibrate запускает калибровку по накопленному массиву данных
     */
    void calibrate();
protected:

    visual::ui::CalibrationDialog* _dlg;
    bool _normalization;
    cv::Mat _data;
    bool _doUndistortion;
    bool _disAdded;
    bool _camAdded;//!< внутренний параметр, отражающий

 //   esp _data;
};
}

#endif // CALIBRATIONNODE_H


