#ifndef PROCESSINGFRAMEWORK_SYNCHFACTORY_H
#define PROCESSINGFRAMEWORK_SYNCHFACTORY_H

#include <synch/synch.h>
/*! Класс фабрика для различных сихнхронизаторов
  */
namespace synch
{
class PROCESSINGFRAMEWORK_SHARED_EXPORT SynchFactory
{
public:
    /** Методы интерполяции
         */
    enum SyncMethod
    {
        Skip,/**< Привязка к главной нити таким образом чтобы пропускать не нужные */
        MapFps,/**< Синхронизация с помощью кучи таймеров настроенных на разные интервалы в зависимости от длинны*/
        None/**< Оставить все как есть дергать все входные SourceNode по очереди */
    };

    static Synch* createSynch(Synch::SyncPolicy policy = Synch::None,SyncMethod method = None,QObject* parent=0);
};
}
#endif // SYNCHFACTORY_H
