#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H

#ifdef WIN32_MSVC
    #define EXT "dll"
#endif
#ifdef UNIX_BUILD
    #define EXT "so"
#endif

#include <QString>

class CrossPlatform
{
public:
    static QString dynLibExtension()
    {
        return EXT;
    }
    static QString dynLibName(QString basename)
    {
        return QString("%1.%2").arg(basename).arg(dynLibExtension());
    }
};

#endif // CROSSPLATFORM_H
