// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.
#ifndef PROCESSINGFRAMEWORK_STERIOCALIBRATION_H
#define PROCESSINGFRAMEWORK_STERIOCALIBRATION_H

#include <sdk_global.h>
#include "calibration.h"
#include <camera/icalibration.h>

/*! Протранство имен для работы с камерами включает в
  * себя методы оценки их взамного расположения и их внутренних параметров
  */
namespace camera
{
/*! Класс для каллибровки двух камер левой и правой. Использует OpenCV
  * и класс calibration для калибровки одной камеры.
  * Использование:
  *     создать экземпляр класса
  *     добавить N кадров методом addImage
  *     запустить алгоритмы калибровки камер методом callibrate
  * \sa Calibration
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT SterioCalibration : public ICalibration
{
public:
    //! Перечисление камер
    enum cameras{LEFT /*! Левая*/,
                 RIGHT/*! Правая*/};
    SterioCalibration();
    SterioCalibration(const SterioCalibration &another);
    explicit SterioCalibration(const cv::Mat &leftCam,
                               const cv::Mat &leftDist,
                               const cv::Mat &rightCam,
                               const cv::Mat &rightDist,
                               const cv::Mat &matR,
                               const cv::Mat &matT);
    /*! Добавить стерео пару в список
      * \param left левое изображение
      * \param right правое изображение
      * \return true если ушлы найденны на обоих изображения, false в противном сулчае
      */
    bool addImage(const cv::Mat left,const cv::Mat right);
    bool addImage(const cv::Mat left,const cv::Mat right,cv::Mat &resultLeft,cv::Mat &resultRight);
    /*! По имеющейся последовательность картинок с обнаруженными на них углами решентки
      */
    virtual bool callibrate();
    /*! Получить матрицу трансляции одной камеры к другой
      */
    cv::Mat getT() const;
    /*! Получить матрицу поворота одной камеры к другой
      */
    cv::Mat getR() const;
    /*! Получить объект содержащий внутрении параметры камер
      * \param cam название камеры
      * \return объект со внутренними параметрами камеры
      * \sa cameras, Calibration
      */
    Calibration getCalib(cameras cam) const;
    /*! Сохранить результаты калибровки в файл
      * \param file название файла
      * \sa cv::FileStorage
      */
    void saveSettings(std::string file) const;
    /*! Загрузить результаты калибровки из файла
      * \param file название файла
      * \sa cv::FileStorage
      */
    void loadSettings(std::string file);
    virtual void removeLast();
    virtual void clear();
  //  void setCallibrationWidget(CallibrationResult* cw);
protected:
    Calibration _left;
    Calibration _right;
    std::vector<cv::Point3f> model_corners;
    cv::Size pattern_size;
    cv::Size imsize;
    cv::Mat R,T,E,F;
 //   CallibrationResult* res;
    double squareSize;
};
}

#endif // STERIOCALIBRATION_H
