#include "tcpabstractsender.h"
#include "utils/logs.h"
#include <QTcpServer>
#include <QTcpSocket>
#include "interfaces.h"
#include <types/general_tracking.h>

using namespace network;

TCPAbstractSender::TCPAbstractSender(QObject *parent) :
    io::OutputNode(parent)
{
    //TODO: Initiation source
    _tcpServer=0;
    setPort(54000);
}

int TCPAbstractSender::port()
{
    return _port;
}


void TCPAbstractSender::setPort(int value)
{
    if(_tcpServer)
    {
        _tcpServer->close();
        delete _tcpServer;
    }
    _tcpServer = new QTcpServer(this);
    if (!_tcpServer->listen(QHostAddress::Any,value)) {
             CRITICAL(_tcpServer->errorString())
             return;
    }
    connect(_tcpServer, SIGNAL(newConnection()),
                this, SLOT(newConnection()));
    _port = value;
}

void TCPAbstractSender::newConnection()
{
    qDebug() << "New connection";
    if(_tcpServer->hasPendingConnections())
    {
         qDebug() << "Has connection";
         QTcpSocket *socket = _tcpServer->nextPendingConnection();
         connect(socket, SIGNAL(disconnected()),
                    this, SLOT(clientDisconnected()));
         connect(socket, SIGNAL(disconnected()),
                    socket, SLOT(deleteLater()));

         if(socket){
             qDebug() << "Push connection";
             _sockets.push_back(socket);
         }
    }
}

void TCPAbstractSender::clientDisconnected()
{
    _sockets.removeOne((QTcpSocket*)this->sender());
}

TCPAbstractSender::~TCPAbstractSender()
{
    if(_tcpServer)
    {
        _tcpServer->close();
        delete _tcpServer;
    }
}

void TCPAbstractSender::broadcast(QByteArray data)
{
    foreach( QTcpSocket *socket,_sockets)
    {
//        socket->connectToHost();
        if(socket->isValid())
        {
            socket->write(data);
        }
    }
}

//This function is called if this node is enable
void TCPAbstractSender::receiveProcess(esp data)
{
    //TODO: Conversion to your type
  //  QString *var = data.getReadOnly<QString>();
    QByteArray block;
    QTextStream out(&block, QIODevice::WriteOnly);

    QSharedPointer<QString> readonly = data.getReadOnly<QString>();
    if(!readonly.isNull())
    {
        out << readonly.data();
        broadcast(block);
    }

    QSharedPointer<types::TrackedObjects> to = data.getReadOnly<types::TrackedObjects>();
    if(!to.isNull())
    {
        out << *to;
        out << endl;
        out.flush();
        qDebug() << block;
        broadcast(block);
    }
}

