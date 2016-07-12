#include "humantracker.h"
#include "humantrackerimpl.h"

HumanTracker* HumanTracker::createTracker(TrackerCallback callback, double thresh)
{
    try{

        return new HumanTrackerImpl(callback,thresh);
    }catch(...)
    {
        return 0;
    }
}
