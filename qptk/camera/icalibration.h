#ifndef PROCESSINGFRAMEWORK_ICALIBRATION_H
#define PROCESSINGFRAMEWORK_ICALIBRATION_H
#include <sdk_global.h>
namespace camera
{
    /*! \breaf Интерфейс, обобщающий calibration и steriocalibration
      Интерфейс, обобщающий calibration и steriocalibration, так как они имеют общую семантику,
      но не наследуют друг от друга
      */
    class PROCESSINGFRAMEWORK_SHARED_EXPORT ICalibration
    {
        public:
        //! Удалить последнии записанные данные
        virtual void removeLast() =0;
        //! Запустить процесс каллибровки. \return true в случае успеха, false в противном случае
        virtual bool callibrate() = 0;
        //! Очистить временные данные
        virtual void clear() =0;
    };
}
#endif // ICALIBRATION_H
