#include "synchfactory.h"
#include <synch/leadersynch.h>
using namespace synch;
using namespace base;
Synch* SynchFactory::createSynch(Synch::SyncPolicy policy,SyncMethod method,QObject* parent)
 {
    Q_UNUSED(policy)
     Synch* sync = 0;
     switch(method)
     {
         case  None: sync = new LeaderSynch(parent);  break;
         case  MapFps:  break;
         case  Skip: sync = new LeaderSynch(parent); break;
     }

     return sync;
 }
