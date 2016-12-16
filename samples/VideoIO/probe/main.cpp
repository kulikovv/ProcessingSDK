#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <VideoIO/VideoIO.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define VIDEO_FILES_DIR "../../data/"

const char *outputFileName = "C:/TEMP/output.mkv";

const char *inputFileNames[] = {
    VIDEO_FILES_DIR "clipcanvas_14348_offline.mp4",
    VIDEO_FILES_DIR "good.mpg"
};

static int const nbInputFiles = sizeof(inputFileNames) / sizeof(inputFileNames[0]);

const char *outputCodecNames[] = {
    "ffv1",
    "ffv1"
};

const char *outputPixelFormats[] = {
    "gray16",
    "bgra"
};

// "Кривые" FPS для проверки.
const double outputFrameRates[] = {
    10.1,
    17.7
};

void printVideoReaderInfo(const char *fileName, video_io::VideoReader const &videoReader)
{
    std::cout << "**************************************************" << std::endl;
    std::cout << std::endl;

    std::cout << "fileName: " << fileName << std::endl;
    std::cout << std::endl;

    std::cout << "nbStreams              : " << videoReader.nbStreams() << std::endl;
    std::cout << "nbVideoStreams         : " << videoReader.nbVideoStreams() << std::endl;
    std::cout << "nbReadableVideoStreams : " << videoReader.nbReadableVideoStreams() << std::endl;
    std::cout << std::endl;

    video_io::Metadata metadata;
  //  std::string x = metadata["xxx"];

    videoReader.getMetadata(metadata);

    if (!metadata.empty())
    {
        video_io::Metadata::const_iterator it;

        std::cout << "Metadata" << std::endl;
        std::cout << std::endl;

        for (it = metadata.begin(); it != metadata.end(); ++it)
            std::cout << "  " << it->first << " : " << it->second << std::endl;

        std::cout << std::endl;
    }

    std::cout << "Stream info" << std::endl;
    std::cout << std::endl;

    for (int id = 0; id < videoReader.nbStreams(); ++id)
    {
        std::cout << "  stream id: " << id << std::endl;
        std::cout << "    nbThreads        : " << videoReader.nbThreads(id) << std::endl;
        std::cout << "    codecDescription : " << videoReader.codecDescription(id) << std::endl;
        std::cout << "    codecName        : " << videoReader.codecName(id) << std::endl;
        std::cout << "    fourCC           : " << videoReader.fourCC(id) << std::endl;
        std::cout << "    isVideoStream    : " << videoReader.isVideoStream(id) << std::endl;
        std::cout << "    nbFrames         : " << videoReader.nbFrames(id) << std::endl;
        std::cout << "    duration         : " << videoReader.duration(id) << std::endl;
        std::cout << "    frameRate        : " << videoReader.frameRate(id) << std::endl;
        std::cout << "    avgFrameRate     : " << videoReader.avgFrameRate(id) << std::endl;
        std::cout << "    pixelFormat      : " << videoReader.pixelFormat(id) << std::endl;
        std::cout << "    width            : " << videoReader.width(id) << std::endl;
        std::cout << "    height           : " << videoReader.height(id) << std::endl;
        std::cout << "    aspectRatio      : " << videoReader.aspectRatio(id) << std::endl;
        std::cout << "    bitRate          : " << videoReader.bitRate(id) << std::endl;
        std::cout << "    bitRateTolerance : " << videoReader.bitRateTolerance(id) << std::endl;
        std::cout << "    gopSize          : " << videoReader.gopSize(id) << std::endl;
        std::cout << "    maxBFrames       : " << videoReader.maxBFrames(id) << std::endl;
        std::cout << std::endl;
    }
}

void printVideoWriterInfo(const char *fileName, video_io::VideoWriter const &videoWriter)
{
    std::cout << "**************************************************" << std::endl;
    std::cout << std::endl;

    std::cout << "fileName: " << fileName << std::endl;
    std::cout << std::endl;

    std::cout << "nbStreams: " << videoWriter.nbStreams() << std::endl;
    std::cout << std::endl;

    std::cout << "Stream info" << std::endl;
    std::cout << std::endl;

    for (int id = 0; id < videoWriter.nbStreams(); ++id)
    {
        std::cout << "  stream id: " << id << std::endl;
        std::cout << "    nbThreads        : " << videoWriter.nbThreads(id) << std::endl;
        std::cout << "    codecDescription : " << videoWriter.codecDescription(id) << std::endl;
        std::cout << "    codecName        : " << videoWriter.codecName(id) << std::endl;
        std::cout << "    fourCC           : " << videoWriter.fourCC(id) << std::endl;
        std::cout << "    frameRate        : " << videoWriter.frameRate(id) << std::endl;
        std::cout << "    pixelFormat      : " << videoWriter.pixelFormat(id) << std::endl;
        std::cout << "    width            : " << videoWriter.width(id) << std::endl;
        std::cout << "    height           : " << videoWriter.height(id) << std::endl;
        std::cout << "    aspectRatio      : " << videoWriter.aspectRatio(id) << std::endl;
        std::cout << "    bitRate          : " << videoWriter.bitRate(id) << std::endl;
        std::cout << "    bitRateTolerance : " << videoWriter.bitRateTolerance(id) << std::endl;
        std::cout << "    gopSize          : " << videoWriter.gopSize(id) << std::endl;
        std::cout << "    maxBFrames       : " << videoWriter.maxBFrames(id) << std::endl;
        std::cout << std::endl;
    }
}

void playVideoFile(const char *fileName, const char *wndName = 0)
{
    video_io::VideoReader videoReader;
    video_io::VideoReader::ReadParams readParams;

   // video_io::Metadata mdata;
  //  videoReader.getMetadata(mdata);
   // mdata.

    videoReader.open(fileName);
    readParams.fixAspectRatio = true;

    printVideoReaderInfo(fileName, videoReader);

    if (videoReader.nbReadableVideoStreams() > 0)
    {
        cv::Mat image;

        while (true)
        {
            int id = videoReader.read(image, readParams);

            std::cout << "id: " << id;

            if (id < 0)
            {
                std::cout << std::endl;
                break;
            }

            std::cout << "  frameNumber: " << videoReader.frameNumber(id);
            std::cout << "  timestamp: " << videoReader.timestamp(id);
            std::cout << std::endl;

            if (!wndName)
                cv::imshow(inputFileNames[id], image);
            else
                cv::imshow(wndName, image);

            cv::waitKey(1);
        }
    }
}

void muxVideoFiles()
{
    video_io::VideoReader videoReaders[nbInputFiles];
    int inputIds[nbInputFiles];
    video_io::VideoReader::ReadParams readParams[nbInputFiles];
    video_io::VideoWriter videoWriter;

    // Открываем входные файлы.

    for (int n = 0; n < nbInputFiles; ++n)
    {
        videoReaders[n].open(inputFileNames[n]);

        if (!videoReaders[n].nbReadableVideoStreams())
            throw std::runtime_error("bad input file");

        printVideoReaderInfo(inputFileNames[n], videoReaders[n]);
    }


    // Находим во входных файлах по одному видеопотоку, который можем декодировать и
    // устанавливаем параметры чтения.

    for (int n = 0; n < nbInputFiles; ++n)
        for (int i = 0; i < videoReaders[n].nbStreams(); ++i)
            if (videoReaders[n].haveVideoDecoder(i))
            {
                inputIds[n] = i;
                readParams[n].fixAspectRatio = true;
                break;
            }

    // Открываем выходной файл.

    videoWriter.open(outputFileName);

    // Добавляем метаданные.

    video_io::Metadata metadata;

    metadata.insert(video_io::Metadata::value_type("title", "aaaaaaaaaaaabbbbbbbbbbbbbbb"));
    metadata.insert(video_io::Metadata::value_type("Camera0", "f: 1000, k: 1, m0: (255, 270), gamma: 1"));
    metadata.insert(video_io::Metadata::value_type("Camera1", "f: 2000, k: 1, m0: (235, 288), gamma: 1"));
    metadata.insert(video_io::Metadata::value_type("Camera2", "f: 2500, k: 1, m0: (225, 271), gamma: 1"));
    videoWriter.setMetadata(metadata);

    // Создаем в выходном файле видеопотоки.

    for (int n = 0; n < nbInputFiles; ++n)
    {
        video_io::VideoWriter::VideoStreamParams videoStreamParams;

        videoStreamParams.codecName = outputCodecNames[n];
        videoStreamParams.pixelFormat = outputPixelFormats[n];
        videoStreamParams.frameRate = outputFrameRates[n];
        videoStreamParams.width = videoReaders[n].width(inputIds[n], readParams[n]);
        videoStreamParams.height = videoReaders[n].height(inputIds[n], readParams[n]);
        //videoStreamParams.options = "b=3000000:bt=3000000:bf=2"; // Для проверки.

        std::cout << "adding stream " << n << std::endl;
        videoWriter.addVideoStream(videoStreamParams);
    }

    // Читаем входные файлы и записываем изображения в соответствующие потоки выходного файла.

    cv::Mat image;

    while (true)
    {
        int s = 0;

        for (int n = 0; n < nbInputFiles; ++n)
        {
            int id = videoReaders[n].read(image, inputIds[n], readParams[n]);

            if (id < 0)
            {
                ++s;
                continue;
            }

            videoWriter.write(image, n);
        }

        if (s == nbInputFiles)
            break;
    }

    printVideoWriterInfo(outputFileName, videoWriter);

    // Явно закрываем выходной файл, чтобы не пропустить возможные ошибки записи.

    videoWriter.close();
}
void gray16test()
{
/*    video_io::VideoWriter videoWriter;
    cv::Mat gray16(480,640,CV_16UC1);

   // videoWriter.open("E:/gray16demo.mkv");
    video_io::VideoWriter::VideoStreamParams videoStreamParams;

    videoStreamParams.codecName = "ffv1";
    videoStreamParams.pixelFormat = "gray16";
    videoStreamParams.frameRate = 30;
    videoStreamParams.width = 640;
    videoStreamParams.height = 480;
    videoWriter.addVideoStream(videoStreamParams);

    gray16.setTo(0);
    videoWriter.write(gray16,0);
    gray16.setTo(1);
    videoWriter.write(gray16,0);
    gray16.setTo(2);
    videoWriter.write(gray16,0);
    gray16.setTo(3);
    videoWriter.write(gray16,0);
    gray16.setTo(4);
    videoWriter.write(gray16,0);

    videoWriter.close();*/
    video_io::VideoReader videoReader;
    video_io::VideoReader videoReaderDp;
    const char *fnames[] =  {
        "G:/2.avi"
        "E:/ABC.mkv",
        "C:/TEMP/Output.mkv"
    };


  //  videoReader.open("D:/of1.avi");
    videoReader.open("G:/2.mkv");
   // videoReaderDp.open("G:/2.mkv");
     printVideoReaderInfo("G:/2.mkv",videoReader);
    cv::namedWindow("XXX");
 //   cv::namedWindow("YYY");
    int seekpos=0;
    while(true)
    {

        cv::Mat im;
        cv::Mat dp;
        std::cout << videoReader.read(im)<< '\n';
    //    videoReaderDp.read(dp);
        if(!im.empty())//&&!dp.empty())
        {
            cv::imshow("XXX",im);

         //   cv::imshow("YYY",dp);
        }
        else
            std::cout << "Image is empty" << '\n';

        char c = cv::waitKey(20);
        if('a'== c)
        {
            seekpos+=500;
            videoReader.seek(seekpos);
        }
        if('s'== c)
        {
            seekpos-=500;
            videoReader.seek(seekpos);
        }
        if(27== c)
        {
            break;
        }
    }

  //  cv::Mat savedgray;
   // bool d = videoReader.seek(100);
   // videoReader.read(savedgray,0);
  //  std::cout << "Sum=" << cv::sum(savedgray)[0] << std::endl;
  //  videoReader.seek(115);
  /*  for(int i=0;i<1000;i+=5)
    {


        videoReader.seek(i);
        videoReader.read(savedgray,0);
        cv::imshow("XXX",savedgray);
        std::cout << "Sum=" << (int)savedgray.at<uchar>(0,0) << std::endl;
        cv::waitKey(100);
    }*/


}
void ocvtest()
{
    {
        cv::VideoWriter writer;
        if(writer.open("E:/ocv.mkv",CV_FOURCC('D','I','V','X'),30,cv::Size(640,480)))
        {
            std::cout << "ASS" << std::endl;
            cv::Mat gray8(480,640,CV_8UC3);
            for(int i=0;i<1000;i++)
            {
                gray8.setTo(i%255);
                writer.write(gray8);
            }
        }
    }
    {
        video_io::VideoReader videoReader;
        videoReader.open("E:/ocv.mkv");

        printVideoReaderInfo("E:/ocv.mkv",videoReader);
    }
    {
        cv::VideoCapture cap;
        if(cap.open("E:/ocv.mkv"))
        {

            std::cout << "Length" << cap.get(CV_CAP_PROP_FRAME_COUNT);
        }
    }

}

int main()
{
#ifdef _MSC_VER
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

   /* try
    {
        // Показываем исходные файлы.
        for (int n = 0; n < nbInputFiles; ++n)
            playVideoFile(inputFileNames[n], inputFileNames[n]);

        // Создаем выходной файл.
        muxVideoFiles();

        // Демонстрируем то, что получилось.
        playVideoFile(outputFileName);

        cv::waitKey();
    }
    catch (video_io::VideoReader::Error const &e)
    {
        std::cout << "VideoReader error: " << e.what() << std::endl;

        return 1;
    }
    catch (video_io::VideoWriter::Error const &e)
    {
        std::cout << "VideoWriter error: " << e.what() << std::endl;

        return 1;
    }
    catch (std::exception const &e)
    {
        std::cout << "Error: " << e.what() << std::endl;

        return 1;
    }*/

    gray16test();
 //   ocvtest();
    return 0;
}
