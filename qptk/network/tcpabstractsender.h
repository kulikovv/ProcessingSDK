#ifndef TCPABSTRACTSENDER_H
#define TCPABSTRACTSENDER_H

#include <io/outputnode.h>



class QTcpServer;
class QTcpSocket;
namespace network{
class TCPAbstractSender : public io::OutputNode {
    Q_OBJECT
    //TODO: Add your own propery to see them in GraphEdit
    //Q_PROPERTY( QString property READ property WRITE setProperty  )
    Q_PROPERTY(int port READ port WRITE setPort )
    Q_ENUMS(Connection)
public:

    explicit TCPAbstractSender(QObject *parent);
    ~TCPAbstractSender();
    int port();
public slots:
    void setPort(int value);
protected slots:
    void newConnection();
    void clientDisconnected();
protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
    void broadcast(QByteArray data);
private:
    //Current data
    int _port;//!< Port used for TCP/IP communication
    QTcpServer *_tcpServer;
    QList<QTcpSocket*> _sockets;
};
}
#endif // TCPABSTRACTSENDER_H


