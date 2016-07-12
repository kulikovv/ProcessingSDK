#ifndef PROCESSINGFRAMEWORK_SIMPLEIMAGEVIEW_H
#define PROCESSINGFRAMEWORK_SIMPLEIMAGEVIEW_H

#include <QtWidgets/QWidget>
#include <QImage>
#include <sdk_global.h>
#include <types/controls.h>

namespace visual
{
/*! \brief Пространство имен visual::ui используется для объединения виждетов.
  Пространство имен visual::ui используется для объединения виждетов.
  Здесь хранятся виждеты адаптированные для использования в узлах из visual.
  */
namespace ui
{
    class PROCESSINGFRAMEWORK_SHARED_EXPORT SimpleImageView: public QWidget
    {
        Q_OBJECT
    public:
        explicit SimpleImageView(QWidget *parent = 0);
        void paintEvent(QPaintEvent *event);
    protected:
        void mousePressEvent( QMouseEvent* event );
        void mouseMoveEvent(QMouseEvent *event);
        QSize _resized;
        QSize _originalSize;
    signals:
        void imageClicked(const MouseControl coord);
        void saveScreen();
    public slots:
        void setData(QImage* var);
    private:
        QImage m_im;

    };
}
}

#endif // WIDGET_H

