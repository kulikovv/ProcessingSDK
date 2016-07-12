#ifndef DEPTHTRACKER_H
#define DEPTHTRACKER_H

#include <QObject>
#include <process/processnode.h>
#include <opencv2/core/core.hpp>
#include <QMap>
#include <types/data.h>
#include <types/general_tracking.h>

class MapFilter : public process::ProcessNode
{
    Q_OBJECT
    Q_PROPERTY( QString filter READ filter WRITE setFilter  )
public:
    explicit MapFilter(QObject *parent = 0);
    QString filter(){return _filter;}
public slots:
    void setFilter(QString value);
protected:
    virtual void receiveProcess(esp data);
    QString _filter;
};
class DepthTracker : public process::ProcessNode
{
    Q_OBJECT
    Q_PROPERTY( int minSize READ minSize WRITE setMinSize  )
    Q_PROPERTY( int maxSize READ maxSize WRITE setMaxSize  )
    Q_PROPERTY( int maxAnimals READ maxAnimals WRITE setMaxAnimals  )
public:
    explicit DepthTracker(QObject *parent = 0);

    int minSize(){return smin;}
    int maxSize(){return smax;}
    int maxAnimals(){return _maxanimals;}
signals:
    
public slots:
    void setMinSize(int value);
    void setMaxSize(int value);
    void setMaxAnimals(int value);
protected:
    /*!
     * \brief getSegmentationMap
     * \return
     */
    virtual cv::Mat getSegmentationMap();
    virtual std::vector<types::Object3d> trackObjects(std::vector<types::Object3d> oldobj,std::vector<types::Object3d> newobj,std::vector<int> &indexes);
    virtual void receiveProcess(esp data);
    std::vector<types::Object3d> oldpts;//Старые координаты
    std::vector<cv::Vec3b> colors;//Цвета для тестовой визуализации
    cv::Mat _depth,_color;
    virtual void reset();
private:
    cv::Vec3b randomColor( cv::RNG& rng );//Создает случайный цвет
    cv::Point getCenterMass(std::vector<cv::Point> &data);//Вычисляет центр масс
    std::vector<types::Object3d> getCentersMass(cv::Mat labelimage, int n, std::vector<int> &sizes, cv::Mat data=cv::Mat(),cv::Mat data2=cv::Mat());//Для n объектов
    void renderWatershade(cv::Mat watershade,cv::Mat render,std::vector<cv::Vec3b> colors,std::vector<int> indexes);
    cv::Mat oldbin;//Бинарное изображении не предыдущем шаге
    int smin;
    int smax;
    int _maxanimals;//Наибольшее число объектов
};

class ColorTracker : public DepthTracker
{
    Q_OBJECT
public:
    explicit ColorTracker(QObject *parent = 0);
protected:
    /*!
     * \brief getSegmentationMap
     * \return
     */
    virtual cv::Mat getSegmentationMap();
    virtual std::vector<types::Object3d> trackObjects(std::vector<types::Object3d> oldobj,std::vector<types::Object3d> newobj,std::vector<int> &indexes);
};

#endif // DEPTHTRACKER_H
