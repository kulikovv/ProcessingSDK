#ifndef PROCESSINGFRAMEWORK_CALIBRATIONDIALOG_H
#define PROCESSINGFRAMEWORK_CALIBRATIONDIALOG_H

#include <QDialog>

#include <opencv2/core/core.hpp>
class QImage;
class QVBoxLayout;
class QHBoxLayout;
class QLCDNumber;
namespace visual{
/*!
  */
namespace ui{
    class SimpleImageView;

/*! \brief Диалог каллибровки
  * Диалог калибровки используется для каллибровки камер видео устройств
  * он получает изображение и представляет результат на оценку пользователю
  */
class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(QWidget *parent = 0);
    ~CalibrationDialog();
signals:
    //! Request calibration data from source
    void request();
    //! Singal on bad grid viewed by user
    void badgrid();
    //! Callibrate
    void callibrate();
public slots:
    void reciveFrame(cv::Mat mat);
    void reciveSmallFeed(cv::Mat feed);
private slots:
    //! Если на форме нажали запрос на кадр
    void on_getFrame_clicked();
    //! Если пользователь сказал, что структура сетки плохо выглядит
    void on_bad_clicked();
    //! Запустить алгоритм каллибровки
    void on_Calibrate_clicked();

private:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    visual::ui::SimpleImageView *view;
    visual::ui::SimpleImageView *smallview;
    QHBoxLayout *horizontalLayout;
    QLCDNumber *counter;
    QPushButton *getFrame;
    QPushButton *bad;
    QPushButton *Calibrate;
    int count; //!< Кол-во снятых кадров
    QImage* _image; //!< Текущее изображение
    QImage* _fit;
};
}
}
#endif // CALIBRATIONDIALOG_H
