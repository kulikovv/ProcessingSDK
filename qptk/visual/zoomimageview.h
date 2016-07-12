#ifndef ZOOMIMAGEVIEW_H
#define ZOOMIMAGEVIEW_H

#include <visual/imagevisualnode.h>
class QGraphicsView;
class QGraphicsScene;
namespace visual
{

class PROCESSINGFRAMEWORK_SHARED_EXPORT ZoomImageView : public ImageVisualNode {
    Q_OBJECT
public:
    explicit ZoomImageView(QObject *parent);
    ~ZoomImageView();
signals:
    void sendZoomCenter(QPointF center);
protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);

};
}
#endif // ZOOMIMAGEVIEW_H


