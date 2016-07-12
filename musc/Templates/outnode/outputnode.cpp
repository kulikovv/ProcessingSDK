#include "%ClassName:l%.%CppHeaderSuffix%"
#include <tools.h>

%ClassName%::%ClassName%(QObject *parent) :
    OutputNode(parent)
{
    //TODO: Initiation source
}

%ClassName%::~%ClassName%()
{
    //TODO: Destruction
}

//This function is called if this node is enable
void %ClassName%::receiveProcess(esp data)
{
    //TODO: Conversion to your type
    %Datatype% *var = data.getReadOnly<%Datatype%>();
    if(0!=var)
    {
        //TODO: storing data code here
    }
}
