#include "%ClassName:l%.%CppHeaderSuffix%"

%ClassName%::%ClassName%(QObject *parent) :
    ProcessNode(parent)
{
    //TODO: Initiation source
}
%ClassName%::~%ClassName%()
{
    //TODO: Destruction	
}

void %ClassName%::receiveProcess(esp data)
{
    //TODO: Copy readonly data into working buffer (for example workcopy)

    //TODO: processing job

    //TODO: send to next node using macros SEND()
}
