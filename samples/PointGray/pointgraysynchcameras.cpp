#include "pointgraysynchcameras.h"
#include <QtConcurrent/QtConcurrent>
#include <omp.h>

#define CHECK_ERRORX(x)  if (x != PGRERROR_OK)\
{\
    qDebug() << "Error" ;\
    error.PrintErrorTrace();\
    return esp();\
}

bool CheckSoftwareTriggerPresence( Camera* pCam )
{
    const unsigned int k_triggerInq = 0x530;

    Error error;
    unsigned int regVal = 0;

    error = pCam->ReadRegister( k_triggerInq, &regVal );

    if (error != PGRERROR_OK)
    {
        error.PrintErrorTrace();

        return false;
    }

    if( ( regVal & 0x10000 ) != 0x10000 )
    {
        return false;
    }

    return true;
}

PointGraySynchCameras::PointGraySynchCameras(QObject *parent) :
    io::capture::CameraNode(parent)
{
    omp_set_num_threads(6);


    // Initialize all the times
  // qDebug() << "The result of omp_get_num_procs " << omp_get_num_procs();
  //  qDebug() << "The result of omp_get_num_threads " << omp_get_num_threads();

   /* #pragma omp parallel for
    for (int i = 0; i < 100; i++ )
    {
        qDebug() << omp_get_thread_num();
    }*/
    FlyCapture2::Error error;
    BusManager busMgr;
    error = busMgr.GetNumOfCameras(&numCameras);

   // busMgr.
    CHECK_ERROR(error);

    if ( numCameras < 1 )
    {
        printf( "Insufficient number of cameras... press Enter to exit.\n" );
        getchar();
        return;
    }

    ppCameras = new Camera*[numCameras];

    // Connect to all detected cameras and attempt to set them to
    // a common video mode and frame rate
    for ( unsigned int i = 0; i < numCameras; i++)
    {
        ppCameras[i] = new Camera();

        PGRGuid guid;
        error = busMgr.GetCameraFromIndex( i, &guid );
        CHECK_ERROR(error);

        // Connect to a camera
        error = ppCameras[i]->Connect( &guid );
        CHECK_ERROR(error);

        // Get the camera information
        CameraInfo camInfo;
        error = ppCameras[i]->GetCameraInfo( &camInfo );
        CHECK_ERROR(error);

        // Set all cameras to a specific mode and frame rate so they
        // can be synchronized
        error = ppCameras[i]->SetVideoModeAndFrameRate(
            VIDEOMODE_1600x1200Y8,
            FRAMERATE_60 );
        CHECK_ERROR(error);


         unsigned int ulRegister = 0x12f8;
         unsigned int ulValue;



        error = ppCameras[i]->ReadRegister(ulRegister,&ulValue);
        CHECK_ERROR(error)
        error = ppCameras[i]->WriteRegister(ulRegister,ulValue | 1);
        CHECK_ERROR(error)

        FC2Config config;
        error = ppCameras[i]->GetConfiguration( &config );
        CHECK_ERROR(error);
        // Set the grab timeout to 100 mseconds
        config.grabTimeout = 200;
        error = ppCameras[i]->SetConfiguration( &config );
        CHECK_ERROR(error);

        qDebug() << CheckSoftwareTriggerPresence(ppCameras[i]);

        // error = ppCameras[i]->StartCapture();
        // CHECK_ERROR(error);
        // Get current trigger settings
        TriggerMode triggerMode;
        error = ppCameras[i]->GetTriggerMode( &triggerMode );
        CHECK_ERROR(error);

        // Set camera to trigger mode 0
        triggerMode.onOff = true;
        triggerMode.mode = 0;
        triggerMode.parameter = 0;
        triggerMode.source = 7;

        error = ppCameras[i]->SetTriggerMode( &triggerMode );
        CHECK_ERROR(error);
        //error = ppCameras[i]->SetStrobe(&control);
     //   CHECK_ERROR(error);
        error = ppCameras[i]->StartCapture();
        CHECK_ERROR(error);
    }

    qDebug() << "start sync" << numCameras;

  //  const unsigned int k_softwareTrigger = 0x62C;
  //  const unsigned int k_fireVal = 0x80000000;

 /*   unsigned int regVal = 0;
    do
    {

        unsigned int regValTmp = 0;
        #pragma omp for
        for ( unsigned int i = 0; i < numCameras; i++ )
        {
            error = ppCameras[i]->ReadRegister( k_softwareTrigger, &regValTmp );
            if(0!=regVal)
            {
                regVal = regVal&regValTmp;
            }else{
                regVal = regValTmp;
            }
            CHECK_ERROR(error)
        }
    } while ( (regVal >> 31) != 0 );*/




  //  error = Camera::StartSyncCapture( numCameras, (const Camera**)ppCameras );
  //  CHECK_ERROR(error);
}


PointGraySynchCameras::~PointGraySynchCameras()
{
    setEnable(false);
    for ( unsigned int i = 0; i < numCameras; i++ )
    {
        ppCameras[i]->StopCapture();
        ppCameras[i]->Disconnect();
        delete ppCameras[i];
    }

    delete [] ppCameras;
}

inline double readTimeStamp(unsigned char* data)
{
   /* strictly little endian... */
   unsigned int stamp;
   unsigned char* pStamp = (unsigned char*)&stamp;
   pStamp[0] = data[3];
   pStamp[1] = data[2];
   pStamp[2] = data[1];
   pStamp[3] = data[0];

   int nSecond      = (stamp >> 25) & 0x7F;   // get rid of cycle_* - keep 7 bits
   int nCycleCount  = (stamp >> 12) & 0x1FFF; // get rid of offset
   int nCycleOffset = (stamp >>  0) & 0xFFF;  // get rid of *_count

   return (double)nSecond + (((double)nCycleCount+((double)nCycleOffset/3072.0))/8000.0);
}

esp PointGraySynchCameras::get(const base::timestamp frame)
{
    FlyCapture2::Error error;
    const unsigned int k_softwareTrigger = 0x62C;
    const unsigned int k_fireVal = 0x80000000;


    unsigned int regVal = 0;
        do
        {

            unsigned int regValTmp = 0;
            #pragma omp parallel for
            for ( int i = 0; i < numCameras; i++ )
            {
                error = ppCameras[i]->ReadRegister( k_softwareTrigger, &regValTmp );
                if(0!=regVal)
                {
                    regVal = regVal&regValTmp;
                }else{
                    regVal = regValTmp;
                }
               // CHECK_ERRORX(error)
            }
        } while ( (regVal >> 31) != 0 );


    QElapsedTimer mt;
    mt.start();

    #pragma omp parallel for
    for (int i = 0; i < numCameras; i++ )
    {
        qDebug() << i << mt.nsecsElapsed()/1000000000.0f;;
        error = ppCameras[i]->WriteRegister( k_softwareTrigger, k_fireVal);

    }

    for ( unsigned int i = 0; i < numCameras; i++ )
    {

        Image image;

        error = ppCameras[i]->RetrieveBuffer( &image );
        CHECK_ERRORX(error)

        TimeStamp timestamp = image.GetTimeStamp();
        qDebug() <<frame << i << readTimeStamp(image.GetData()) << timestamp.cycleSeconds << timestamp.cycleCount;
    }

   // QtConcurrent::run(this,&PointGraySynchCameras::run);


    return esp();
}


void PointGraySynchCameras::tigger(int id)
{
    FlyCapture2::Error error;

    const unsigned int k_softwareTrigger = 0x62C;
    const unsigned int k_fireVal = 0x80000000;
    error = ppCameras[id]->WriteRegister( k_softwareTrigger, k_fireVal );
    CHECK_ERROR(error)
}

void PointGraySynchCameras::run()
{
    FlyCapture2::Error error;

    const unsigned int k_softwareTrigger = 0x62C;
    const unsigned int k_fireVal = 0x80000000;

        unsigned int regVal = 0;
        do
        {

            unsigned int regValTmp = 0;
            #pragma omp parallel for
            for ( int i = 0; i < numCameras; i++ )
            {
                error = ppCameras[i]->ReadRegister( k_softwareTrigger, &regValTmp );
                if(0!=regVal)
                {
                    regVal = regVal&regValTmp;
                }else{
                    regVal = regValTmp;
                }
            //    CHECK_ERROR(error)
            }
        } while ( (regVal >> 31) != 0 );


        #pragma omp parallel for
        for ( int i = 0; i < numCameras; i++ )
        {
            error = ppCameras[i]->WriteRegister( k_softwareTrigger, k_fireVal );
        //    CHECK_ERROR(error)
        }


        for ( unsigned int i = 0; i < numCameras; i++ )
        {

            Image image;
            error = ppCameras[i]->RetrieveBuffer( &image );
            CHECK_ERROR(error)

            TimeStamp timestamp = image.GetTimeStamp();
            qDebug() << i << readTimeStamp(image.GetData())<< timestamp.cycleSeconds << timestamp.cycleCount;
        }

  //      j++;
  //  }while(this->enable());
}
