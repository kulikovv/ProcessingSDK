#ifndef LAYOUTNODE_H
#define LAYOUTNODE_H

#include <visual/zoomimageview.h>

namespace visual
{

class PROCESSINGFRAMEWORK_SHARED_EXPORT LayoutNode : public ZoomImageView {
    Q_OBJECT
    Q_PROPERTY( QString layout READ layout WRITE setLayout )
    Q_PROPERTY( double mpx READ getMpx WRITE setMpx USER true )
    Q_PROPERTY( double mpy READ getMpy WRITE setMpy USER true )
public:
    explicit LayoutNode(QObject *parent);
    ~LayoutNode();
    QString layout();

public slots:
    double getMpx();
    double getMpy();
    QList<double> getCoordinates(QString name);
    void setLayout(QString data);
    void setMpx(double value);
    void setMpy(double value);
    double isInside(QString name,double x,double y);
    virtual QIcon getIcon();
protected slots:
    void reciveMpxy(float x,float y);
protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
    void checkAssignForm(esp data);
    double _mpx;
    double _mpy;

};
}

#endif // LAYOUTNODE_H


