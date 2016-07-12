#ifndef HUMANTRACKER_H
#define HUMANTRACKER_H

#include "HumanTracker_global.h"
#include "map"

struct HUMANTRACKERSHARED_EXPORT Point3d
{
    Point3d(){
        x = y = z =0;
    }

    Point3d(float xx,float yy,float zz)
    {
        x =xx;
        y =yy;
        z =zz;
    }

    float x;
    float y;
    float z;
};

typedef HUMANTRACKERSHARED_EXPORT int (*TrackerCallback)(std::map<int,Point3d> coords);

class HUMANTRACKERSHARED_EXPORT HumanTracker
{
public:
    /*!
     * \brief createTracker
     * \param callback recive tracking data
     * \param thresh the theshold in meters how far object from floor
     * \return
     */
    static HumanTracker* createTracker(TrackerCallback callback, double thresh=1);
    /*!
     * \brief setThreshold
     * \param thresh the theshold in meters how far object from floor
     */
    virtual void setThreshold(double thresh)=0;
    /*!
     * \brief start processing
     */
    virtual void start()=0;
    /*!
     * \brief stop processing
     */
    virtual void stop()=0;
    /*!
     * \brief reset tracker settings
     */
    virtual void reset()=0;
};

#endif // HUMANTRACKER_H
