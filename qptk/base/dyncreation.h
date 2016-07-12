#ifndef PROCESSINGFRAMEWORK_DYNCREATION_H
#define PROCESSINGFRAMEWORK_DYNCREATION_H

#include <QString>
class QObject;

namespace base
{
/*!  \breaf Динамическое создание объекта по имени
  * Базовый класс для динамического создания объектов
  * часть механизма создания класса по имени
  */
class BaseFactory
{
public:
      BaseFactory(QString libraryname="")
      {
          _libraryname = libraryname;
      }

      //! Заполняет все параметры об узле
      void initInfo(QObject* obj);
      //! Деструктор
      virtual ~BaseFactory() {}
      //! Создает динамически класс наследник от QObject
      virtual QObject * create(QObject* parent) = 0;
protected:
      QString _libraryname;
};
/*! Шаблонная фабрика классов
  * реализующая интерфейс BaseFactory
  */
template <class C>
class Factory  : public BaseFactory
{
public:
    Factory(QString libraryname):BaseFactory(libraryname){}
    //! Создает динамически класс наследник от QObject
      QObject * create(QObject* parent);
};

template <class C>
QObject * Factory<C>::create(QObject* parent)
{
      C* data = new C(parent);
      data->setLibrary(_libraryname);
      return data;
}
}
#endif // DYNCREATION_H
