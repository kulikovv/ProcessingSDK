#include "calibrationdialog.h"

#include <QImage>
#include <QDebug>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <visual/forms/simpleimageview.h>
#include <types/rgb.h>

using namespace visual::ui;

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent)
{
    _image=0;
    count=0;
    _fit = 0;

    resize(800, 600);
    verticalLayout_2 = new QVBoxLayout(this);
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
    view = new visual::ui::SimpleImageView(this);
    view->setObjectName(QString::fromUtf8("view"));

    smallview = new visual::ui::SimpleImageView(view);
    smallview->setObjectName(QString::fromUtf8("smallView"));
    smallview->setGeometry(QRect(0, 0, 320, 240));



    verticalLayout->addWidget(view);


    verticalLayout_2->addLayout(verticalLayout);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
    counter = new QLCDNumber(this);
    counter->setObjectName(QString::fromUtf8("count"));

    horizontalLayout->addWidget(counter);

    getFrame = new QPushButton(tr("Get Frame"),this);
    getFrame->setObjectName(QString::fromUtf8("getFrame"));

    horizontalLayout->addWidget(getFrame);

    bad = new QPushButton(tr("Bad Frame"),this);
    bad->setObjectName(QString::fromUtf8("bad"));

    horizontalLayout->addWidget(bad);

    Calibrate = new QPushButton(tr("Calibrate!"),this);
    Calibrate->setObjectName(QString::fromUtf8("Calibrate"));

    horizontalLayout->addWidget(Calibrate);


    verticalLayout_2->addLayout(horizontalLayout);

    bad->setEnabled(false);
    Calibrate->setEnabled(false);

    QMetaObject::connectSlotsByName(this);
}

CalibrationDialog::~CalibrationDialog()
{
    if(0!=_image)
    {
        delete _image;
        _image = 0;
    }
    if(0!=_fit)
    {
        delete _fit;
        _fit = 0;
    }
}
void CalibrationDialog::reciveSmallFeed(cv::Mat feed)
{
    _fit = types::Rgb(feed).createQImage(_fit);
    smallview->setData(_fit);
}


void CalibrationDialog::reciveFrame(cv::Mat mat)
{
    count++;
    _image = types::Rgb(mat).createQImage(_image);
    view->setData(_image);
  //view->setData(Functions::createQImage(mat,_image));
    counter->display(count);
    bad->setEnabled(true);
    if(count>5)
    {
        Calibrate->setEnabled(true);
    }
}

void CalibrationDialog::on_getFrame_clicked()
{
    bad->setEnabled(false);
    emit request();
}

void CalibrationDialog::on_bad_clicked()
{
    count--;
    counter->display(count);
    bad->setEnabled(false);
    if(count<5)
    {
        Calibrate->setEnabled(false);
    }
    emit badgrid();
}

void CalibrationDialog::on_Calibrate_clicked()
{
    emit callibrate();
}

