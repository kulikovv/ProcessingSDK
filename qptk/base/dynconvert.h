#ifndef PROCESSINGFRAMEWORK_DYNCOVERT_H
#define PROCESSINGFRAMEWORK_DYNCOVERT_H

#include <typeinfo>
#include <sdk_global.h>
#include <string>

using namespace std;
namespace base
{
/*! Динамический конвертор типов по имени
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT ConvertFactory
{
    public:
        //! Деструктор
      virtual ~ConvertFactory() {}
      virtual std::string name() = 0;
        //! Создает динамически класс наследник от QObject
      virtual void * convert(void *data) = 0;
};
/*! \brief Фабрика классов для возможности конвертировать типы из одних в другие
  * Шаблонная фабрика классов
  * реализующая интерфейс ConvertFactory
  */
template <class C,class T>
class PROCESSINGFRAMEWORK_SHARED_EXPORT Convert  : ConvertFactory
{
    public:
      string name()
      {
          return string(typeid(C).name())+string(typeid(T).name());
      }
      void * convert(void* data)
      {
            return 0;
      }
};
}
#endif // DYNCOVERT_H
