#include "%ProjectName:l%.h"
#include <QtPlugin>

//Include here all nodes headers

%ProjectName:c%::%ProjectName:c%()
{

}

QString %ProjectName:c%::getLibraryName()
{
    return "%ProjectName:c%";
}

void %ProjectName:c%::registrate()
{
    //TODO: register nodes to export from your library
    //REGISTER_DYN_NODE(Nodename)
}

Q_EXPORT_PLUGIN2(%ProjectName:c%,%ProjectName:c%)
