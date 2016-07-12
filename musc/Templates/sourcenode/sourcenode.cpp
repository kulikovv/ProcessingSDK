#include "%ClassName:l%.%CppHeaderSuffix%"

%ClassName%::%ClassName%(QObject *parent) :
    %Datatype%(parent)
{
    //TODO: Initiation source
}

%ClassName%::~%ClassName%()
{
    //TODO: Destroy data
}

esp %ClassName%::get(const timestamp frame)
{   
    //TODO: load data here
    return esp();
}

void %ClassName%::receiveProcess(esp data)
{
    //TODO: reaction on reciving command
    get(data.getFrameId());
}
