#ifndef GRAPHDOM_H
#define GRAPHDOM_H

#include "graphparser.h"
#include <synch/synch.h>
#include <utils/graphxml.h>

namespace utils
{
    /*!
     * \brief The GraphDom class новый класс для сериализации графов в xml
     * позволяеют использовать подграфы
     * как локальные, так и удаленные
     */
    class PROCESSINGFRAMEWORK_SHARED_EXPORT GraphDom
    {
        public:
        /*!
         * \brief loadGraph загрузить граф, когда элементы декора не важны
         * \param xmlname путь к xml файлу с графом
         * \return
         */
        static synch::Synch* loadGraph(QString xmlname);
        /*!
         * \brief loadGraph загрузить граф
         * \param xmlname путь к xml файлу с графом
         * \param decor настройки декора (положения элементов для рисования графа)
         * \return
         */
        static synch::Synch* loadGraph(QString xmlname,QMap<QString,NodeDecoration> &decor);
        /*!
         * \brief loadGraphMemory читает граф из строки, загруженной в оперативку
         * \param xml загруженная схема в память
         * \return  указатель на граф, если что-то не так то возвращается 0
         */
        static synch::Synch* loadGraphMemory(QString xml);
        /*!
         * \brief loadGraphMemory читает граф из строки, загруженной в оперативку
         * \param xml загруженная схема в память
         * \param decor настройки декора (положения элементов для рисования графа)
         * \return  указатель на граф, если что-то не так то возвращается 0
         */
        static synch::Synch* loadGraphMemory(QString xml,QMap<QString,NodeDecoration> &decor);
        /*!
         * \brief saveGraph сохранить граф в виде xml файла
         * \param xmlname путь к xml файлу с графом
         * \param graph граф, который сохраняем
         * \param decor настроки визуализации
         */
        static void saveGraph(QString xmlname, synch::Synch* graph, QMap<QString,NodeDecoration> &decor, bool expand=false);
        /*!
         * \brief saveGraphMemory сохранить граф в виде строки
         * \param graph граф, который сохраняем
         * \param decor настроки визуализации
         * \return
         */
        static QString saveGraphMemory(synch::Synch* graph,QMap<QString,NodeDecoration> &decor);
        static QString saveGraphMemoryExpanded(synch::Synch* graph,QMap<QString,NodeDecoration> &decor);
    };
}

#endif // GRAPHDOM_H
