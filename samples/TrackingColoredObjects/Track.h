#ifndef TRACK_H
#define TRACK_H

#include <vector>
#include <map>
#include <opencv2/core/core.hpp>
using namespace std;

/*! \brief Шаблонный класс трассировщик.
 * Определяет оптимальные параметры сопоставления двух наборов параметров объектов.
 Алгоритм состоит из следующих шагов:
 1.Объединение измерений с разных источников
 2.Упорядочивание по времени

 Для конкретного типа данных надо переопределить методы
 distance и join

 Пример использования:

 Tracker<Point2d> ptrack(5,1);
 vector<Point2d> frame11;
 frame11.push_back(Point2d(1,1.1));
 frame11.push_back(Point2d(3,3));
 vector<Point2d> newobj,removed;
 ptrack.track(newobj,removed);
 */

template <class C> class Tracker
{
public:
    /*! Конструктор создает объект Tracker
      *\param movingthresod порог максимальное смещение объекта в двух последовательных кадрах
      *\param jointreshold порог для склеивания измерений с разных источников
      *\param keephistory время хранения данных после ухода объекта
      */
    Tracker(double movingthresod=2.0,double jointreshold=1.0, double keephistory=10)
    {
        _mtresh = movingthresod;
        _tresh = jointreshold;
        _keephist = keephistory;
    }




    /*! Обнуляет все счетчики
      */
    void reset()
    {
        _samples.clear();
        _oldindex.clear();
        _history.clear();
    }

    /*! Добавляет набор объектов из источника
      если имеется одна камера, то sourceid необязательно указывать
      \param samples набор параметров для сравнивания
      \param sourceid номер источника в случае, если их больше одного
      */
    void addSamples(vector<C> samples,int sourceid=0)
    {
        _samples[sourceid]=samples;
    }

    /*! Переопределяемая пользователем функция сравнивания двух объектов
      \brief растояние между двумя объектами (требуется переодпеделить для конкретного типа данных)
      \param obj1 первый объект
      \param obj2 второй объект
      \return растояние между объектами
      */
    double distance(C obj1,C obj2)
    {
        return 0;
    }
    /*! Функция объединения двух параметров одного и того-же объекта из разных источников
      \brief объединение параметров объектов с двух источников (требуется переодпеделить для конкретного типа данных)
      \param obj1 первый объект
      \param obj2 второй объект
      \return объект сочетание obj1 и obj2
      */
    C join(C obj1,C obj2)
    {
        return obj1;
    }

    /*! \brief Функция трассировки
      Считает растояние
      \param newobjects выходной параметр новые объекты
      \param outobjects выходной параметр ушедшие объекты
      \return соотношение индексов объектов к объектам
      */
    map<int,C> track(vector<C> &newobjects,vector<int> &outobjects,vector<C> &erasedobjects)
    {
        static int id;
        newobjects.clear();
        outobjects.clear();
        erasedobjects.clear();
        map<int,C> out;
        //Если пусто
        if(_samples.empty())
        {
            return out;
        }

        vector<C> joined;

        joinSamples(joined);
        //printDebug(joined);
        _samples.clear();

        //Если нет старых измерений
        if(0==_oldindex.size())
        {
            //Копируем все данные из joined в _oldindex, out и новые объекты
            for(unsigned int i=0;i<joined.size();i++)
            {

                newobjects.push_back(joined[i]);
                _oldindex[id]=out[id]=joined[i];
                id++;
            }
            return out;
        }

        vector<int> found;//Индексы обнаруженых старых объектов
        vector<int> newids;//Индексы новых объектов
     //   qDebug() << "Old size" << _oldindex.size();

        //qDebug() << "newid";
        for(unsigned int i=0;i<joined.size();i++)
        {
            typename std::map<int,C >::const_iterator best = _oldindex.begin();
            double mindist = distance(joined[i],best->second);
            typename std::map<int,C >::const_iterator end = _oldindex.end();
            for (typename std::map<int,C >::const_iterator it = ++_oldindex.begin(); it != end; ++it)
            {
                double dist = distance(joined[i],it->second);
                if(dist<mindist)
                {
                    mindist = dist;
                    best = it;
                }
            }



            if(mindist>_mtresh||(std::find(found.begin(), found.end(), best->first) != found.end()))
            {
                //Если растояние от объекта до ближайшего больше дисперии значит новый объект
                newobjects.push_back(joined[i]);
                out[id]=joined[i];
                newids.push_back(id);
                found.push_back(id);
                id++;
            }else{
                //Считаем объект найденным из старых
                found.push_back(best->first);
                out[best->first]=joined[i];
            }
        }

        //Удаляем ненайденные старые объекты



        if(0!=_oldindex.size())
        {
         //    qDebug() << "_oldindex size"<<_oldindex.size();
      //      for (; it != _oldindex.end(); ++it)
             typename std::map<int,C >::const_iterator it = _oldindex.begin();
             std::vector<int> deleteids;
            for (size_t j=0;j<_oldindex.size() ; j++)
            {


            //    it = _oldindex.begin()+j;
             //   int key = _oldindex.at()
             //   qDebug() << "enter cicle";
                bool finded=false;
                for(unsigned int i=0;i<found.size();i++)
                {
                    if(it->first==found[i])
                    {
                        _oldindex[it->first] = out[it->first];
                        finded = true;
                        break;
                    }
                }
                if(!finded)
                {
                //    qDebug() << "not found";

                    _history[it->first]=_history[it->first]+1;
                    if(_history[it->first]>_keephist)
                    {
                         outobjects.push_back(it->first);

                       // _history.erase(it->first);
                      //  _oldindex.erase(it->first);
                    }
                //     qDebug() << "not found end";
                }
             //   qDebug() << "leave cicle";
                it++;
            }
        }
        for(size_t j=0;j<outobjects.size();j++)
        {
            erasedobjects.push_back(_oldindex[outobjects[j] ]);
            _history.erase(outobjects[j]);
            _oldindex.erase(outobjects[j]);
        }
        // qDebug() << "add new";
        //Добавляем в память новые объекты
        for (size_t i = 0; i < newids.size(); i++)
            _oldindex[newids[i]] = out[newids[i]];
        return out;
    }

protected:
    /*! Обединение набора объектов с разных источников из _samples
      в один вектор для последующей трассировки по времени
      Для объединения вызывается метод join
      \sa join
      \param joined выходной вектор объединения
      */
    void joinSamples(vector<C> &joined)
    {
        joined.clear();
        //Началная склейка - первый вектор
        joined = _samples.begin()->second;

        typename std::map<int,vector<C> >::const_iterator end = _samples.end();
        typename std::map<int,vector<C> >::const_iterator begin = _samples.begin();
        //Для все остальных сравниваем в начальным вектором
        for (typename std::map<int,vector<C> >::const_iterator it = ++begin; it != end; ++it)
        {
            //Поэлементно сравниваем два вектора на самом деле можно ускорить, если ввести на множестве C частичный порядок
            vector<C> vec = it->second;
           // qDebug() << "vec size" << vec.size();
            for(unsigned int j=0;j<vec.size();j++)
            {
                bool found =false;
                unsigned int cursize = joined.size();
                for(unsigned int i=0;i<cursize;i++)
                {

                    //Если растояние меньше порогоа
                   if(distance(joined[i],vec[j])<=_tresh)
                    {
                       qDebug() << "Joint data!";
                        //Делаем усреднение между двумя признаками
                        joined[i] = join(joined[i],vec[j]);
                        found = true;
                    }
                }
                if(!found)
                {
                    joined.push_back(vec[j]);
                }
            }
        }
        //qDebug() << "joined size" << joined.size();

    }

private:
    map<int,vector<C> > _samples;//!< Исходный буфер, заполняемый методом addSamples
    map<int,C> _oldindex;//!< Старые индексы
    map<int,int> _history;//!< Количество кадров в которых отсутсвует ставрый индекс
    double _tresh;//!< Порог на объединение
public:
    double _mtresh;//!< Порог на трассировку
    int _keephist;//!< Кол-во кадров для хранения истории
};
//Резализации растояния для различных популярных типов данных
template<> inline double Tracker<cv::Point3d>::distance(cv::Point3d obj1,cv::Point3d obj2)
{
    return cv::norm(obj1-obj2);
//    double dx = obj1.x-obj2.x;
//    double dy = obj1.y-obj2.y;
//    double d= dx*dx+dy*dy;
 //   qDebug()<<"("<<obj1.x<<","<<obj1.y<<")"<<"("<<obj2.x<<","<<obj2.y<<")="<< d;
//    return d;
}


#endif // TRACK_H
