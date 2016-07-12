#ifndef PROCESSINGFRAMEWORK_IMAGEVIEW_H
#define PROCESSINGFRAMEWORK_IMAGEVIEW_H

#include <visual/imagevisualnode.h>
#include <types/controls.h>
namespace cv
{
    class Mat;
}
namespace visual
{

class PROCESSINGFRAMEWORK_SHARED_EXPORT ImageView : public ImageVisualNode {
    Q_OBJECT
    Q_PROPERTY( QString prefered READ prefered WRITE setPrefered )
    Q_CLASSINFO("prefered",tr("Describes prefered source"))
public:
    explicit ImageView(QObject *parent);
    QString prefered() const;
public slots:
    void setPrefered(QString value);
    void userReaction();
    void onImageClicked(const MouseControl point);

protected:
    /*!
     * \brief preDraw функция для переопределения, по началу ничего не делает.
     * Ее можно переопределить если необходимо дополнить изображение, какими либо статическими данными, например мишень и т.д.
     * \param img изображение поступившее
     */
    virtual void preDraw(cv::Mat& img);
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
private:
    QString _prefered;//!<
    int _w;
    int _h;
    esp _innerdata;
};
}
#endif // IMAGEVIEW_H


