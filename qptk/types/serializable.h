#ifndef PROCESSINGFRAMEWORK_SERIALIZABLE_H
#define PROCESSINGFRAMEWORK_SERIALIZABLE_H

#include <sdk_global.h>
#include <exception>
#include <QByteArray>

namespace types
{
    class PROCESSINGFRAMEWORK_SHARED_EXPORT SerializationException: public std::exception
    {
      virtual const char* what() const throw()
      {
        return "Unimplemented method";
      }
    };

    class PROCESSINGFRAMEWORK_SHARED_EXPORT Serializable
    {
        virtual QByteArray serialize()
        {
            throw 1;
        }

    };
}

#endif // PROCESSINGFRAMEWORK_SERIALIZABLE_H
