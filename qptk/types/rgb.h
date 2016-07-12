#ifndef PROCESSINGFRAMEWORK_RGB_H
#define PROCESSINGFRAMEWORK_RGB_H

#include <sdk_global.h>
#include <opencv2/core/core.hpp>

class QImage;
namespace types
{
/*! Класс для обертки cv::Mat из opencv
  позволяет экспортировать данные в QImage
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT Rgb
{
public:
    Rgb();
    Rgb(const cv::Mat &);
    Rgb(const Rgb &);
    Rgb(const QImage &);
    Rgb &operator=(const cv::Mat &);
    Rgb &operator=(const Rgb &);
    operator cv::Mat() const;
    cv::Mat getMat() const;
    /*! Получить серое изображение 8 бит один канал
      */
    cv::Mat toGray() const;
    /*! Получить цветное изображение 8 бит три канала
      */
    cv::Mat toColor() const;
    /*! Превращает матрицу из серой в цветную
      \param серое изображение
      \return трехканальное изображение
      */
    static cv::Mat createColor(const cv::Mat &);
    /*! Экспорт в QImage
      если qim пустая, то создается новое изображение, иначе ворзращается указатель на измененный qim
      \param qim возможный уже аллоцированный буфер для изображения
      \return возвращает указатель на объект QImage
      */
    QImage* createQImage(QImage* qim=0);
private:
    cv::Mat _mat;
};
typedef Rgb Gray;//!< Тип сдклан, чтобы не вводить в заблуждение
}

#endif // RGB_H
