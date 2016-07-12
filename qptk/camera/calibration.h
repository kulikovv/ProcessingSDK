// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.
#ifndef PROCESSINGFRAMEWORK_CALIBRATION_H
#define PROCESSINGFRAMEWORK_CALIBRATION_H

#include <sdk_global.h>
#include <opencv2/core/core.hpp>
#include <vector>
#include <camera/icalibration.h>

/*! \brief Классы для работы с видео камерами
  Протранство имен для работы с камерами включает в
  себя методы оценки их взамного расположения и их внутренних параметров
  */
namespace camera
{

/*! \brief Класс для калибровки одной камеры
  Класс для калибровки одной камеры, если есть несколько камер и надо задать параметры
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT Calibration: public ICalibration
{
public:
    Calibration();
    ~Calibration();
    /*! Настраивает калибровку, если есть несколько камер и надо задать параметры
      * извне
      * \param model точки с координатами углов на модели
      * \param pattern размеры шахматной доски
      */
    void init(std::vector<cv::Point3f> model,cv::Size pattern);
    /*! Добавляет изображение в каллиброочный конвеер
      *\return true, если все углы были найдены false, в противном случае
    */
    bool addImage(const cv::Mat im,cv::Mat &result);
    bool addImage(const cv::Mat im);
    /*! По имеющейся последовательности изображений считаем параметры
      * матриц
      */
    virtual bool callibrate();
    /*! Возвращает ошибку каллибровки
    */
    double computeReprojectionErrors( const std::vector<std::vector<cv::Point3f> >& objectPoints,
                              const std::vector<std::vector<cv::Point2f> >& imagePoints,
                              const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
                              const cv::Mat& cameraMatrix , const cv::Mat& distCoeffs,
                              std::vector<float>& perViewErrors);
    /*! Получить список обнаруженных углов
      */
    std::vector<std::vector<cv::Point2f> > getCorners();
    /*! После каллибровки получить матрицу
    */
    cv::Mat getCamMatrix() const;
    /*!
    */
    cv::Mat getDistCoef() const;
    /*!
     * \brief getMeanReprojectionError возвращает среднюю ошибку репроеции, до каллибровки она равна -1
     * \return
     */
    double getMeanReprojectionError() const;
    /*! Удаляет последнюю запись
      */
    virtual void removeLast();
    /*! Установить матрицу внутрениих параметров камеры
      * \param cam матрица внутренних параметров
      */
    void setCamMatrix(cv::Mat cam);
    /*! Установить параметры коэфициентов дисторсии
      * \param dist матрица коэфициентов дистрсии
      */
    void setDistCoef(cv::Mat dist);
    cv::Point2f undistManual(cv::Point2f coord,cv::Mat distcoef=cv::Mat(),bool useintistics=true);
    void saveDebug(std::string fname);
    cv::Mat loadDebug(std::string fname);
    /*! Вычисляет обратоное преобразование коэфициентов дисторсии
      * \param imsz размер изображения
      * \param l кол-во точек в разбиении
      * \return Mat[l1,l2,b1,b2] - обратное преобразование
      */
    cv::Mat calcInverted(const unsigned int width,const unsigned int height,const unsigned int l);
    /*! Устранение искажения
      * \param x искаженное изображение
      * \return выправленное изображение
      */
    cv::Mat undistort(cv::Mat x);
    cv::Mat distort(cv::Mat x);
    virtual void clear();

    cv::Size getGridSize();
    void setGridSize(cv::Size sz);
    double getSquereSize();
    /*!
     * \brief setSquereSize установить размер ячейки доски
     * \param value
     */
    void setSquereSize(double value);
    /*!
     * \brief setNormalization включить нормализацию яркости перед поиском доски
     * \param value
     */
    void setNormalization(bool value);
    /*!
     * \brief setReduceResolution при поиске доски на первом этапе уменьшать разрешение до 640x480 для ускорение
     * \param value если true, то уменьшаем, false - нет
     */
    void setReduceResolution(bool value);
    bool getReduceResolution() const;
    /*!
     * \brief setPatternType
     * \param pt
     */
    void setPatternType(int pt);
    int getPatternType() const;
protected:
    void generateObject();
    void initUndistort();
    std::vector<std::vector<cv::Point2f> > detected_corners;//!< Список обнаруженных углов
    std::vector<cv::Point3f> model_corners; //!< Модель объекта
    cv::Size pattern_size;//!< Размер шахматной доски
    double squareSize; //!< Размер ячейки калибровочной доски
    cv::Size imsize; //!< Размер изображение вычисляется в addImage
    cv::Mat cameraMatrix; //!< Матрица внутренних параметров
    cv::Mat distCoef; //!< Коэфициенты дисторсии линз
    cv::Mat invDistCoef; //!< Инвертированные коэфициенты дисторсии    
    cv::Mat dx,dy; //! Преобразования для исправления искажений
    cv::Mat idx,idy; //! Преобразования востановления
    bool _normalizeImage;//! Использовать нормализацию по умолчанию true
    double _repError;//!< Сердняя ошибка репроекции
    bool _reduceResolution;//!< Уменьшить разрешение изображения по умолчанию true
    int _ptype;//!< Тип шаблона
};
}

#endif // CALIBRATION_H
