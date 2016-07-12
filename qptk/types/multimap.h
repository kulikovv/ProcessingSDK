#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <sdk_global.h>
#include <opencv2/core/core.hpp>
#include <Qmap>
#include <QStringList>

class QImage;
namespace types
{
/*!
 * \brief The MultiMap класс для хранения множества изображений. Работает как Map, с типом данных cv::Mat
 */
typedef QMap<QString,cv::Mat> MultiMap;

}
#endif // MULTIMAP_H
