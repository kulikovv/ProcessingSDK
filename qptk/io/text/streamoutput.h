#ifndef STREAMOUTPUT_H
#define STREAMOUTPUT_H

#include <io/outputnode.h>

#include <QFile>
#include <QTextStream>
namespace io
{

namespace text
{


class StreamOutput : public io::OutputNode
{
    Q_OBJECT
    Q_ENUMS(Mode)
    Q_PROPERTY(Mode writeMode READ writeMode WRITE setWriteMode)
public:
    enum Mode{
        NEW,
        APPEND,
        REWRITE
    };

    explicit StreamOutput(QObject* parent);
    ~StreamOutput();
    Mode writeMode();
public slots:
    virtual void setUrl(QString url);
    void setWriteMode(Mode mode);
    virtual void reset();
protected:
    virtual void receiveProcess(esp data);
private:
    Mode _writemode;

    int _counter;
    QFile _file;
    QTextStream _stream;
    QString _separator;
    QString _columns;
};

}
}
#endif // STREAMOUTPUT_H
