#include "%ClassName:l%.%CppHeaderSuffix%"


%ClassName%::%ClassName%(QObject *parent) :
    VisualNode(parent)
{
    //TODO: Initiation source
    //Create your widget
    // _widget = new ...
}
%ClassName%::~%ClassName%()
{
    //TODO: Destruction code
    //Do not delete _widget it will be destroyed by VisualNode
}

//This function is called if this node is enable
void %ClassName%::receiveProcess(esp data)
{
    // Yourtype *var = data.getReadOnly<Yourtype>();
    //if(0!=var)
    //TODO: show data code inside your _widget
}

