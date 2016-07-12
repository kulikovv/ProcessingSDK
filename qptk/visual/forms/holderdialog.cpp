#include "holderdialog.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QRegExpValidator>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QComboBox>

using namespace visual::ui::utils;

HolderDialog::HolderDialog(QStringList items,QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout* vbox = new QVBoxLayout();
    QHBoxLayout* hbox = new QHBoxLayout();
    QHBoxLayout* hbox2 = new QHBoxLayout();
    QHBoxLayout* hbox3 = new QHBoxLayout();
    QHBoxLayout* hbox4 = new QHBoxLayout();
    QHBoxLayout* hbox5 = new QHBoxLayout();
    QLabel* label = new QLabel(tr("Label:"),this);
    QLabel* label2 = new QLabel(tr("X cm:"),this);
    QLabel* label3 = new QLabel(tr("Y cm:"),this);
    QLabel* label4 = new QLabel(tr("Resize to:"),this);
    QLabel* label5 = new QLabel(tr("Percent:"),this);
    _name = new QLineEdit(this);
    _x = new QLineEdit(this);
    _y = new QLineEdit(this);
    _objects = new QComboBox(this);
    _percent = new QSpinBox(this);
    _x->setValidator(new QDoubleValidator(_x));
    _y->setValidator(new QDoubleValidator(_y));
    _objects->addItem(tr("None"));
    _objects->addItems(items);

    _percent->setMinimum(1);
    _percent->setMaximum(300);
    _percent->setValue(100);

    hbox->addWidget(label);
    hbox->addWidget(_name);

    hbox2->addWidget(label2);
    hbox2->addWidget(_x);

    hbox3->addWidget(label3);
    hbox3->addWidget(_y);

    hbox4->addWidget(label4);
    hbox4->addWidget(_objects);

    hbox5->addWidget(label5);
    hbox5->addWidget(_percent);

    QDialogButtonBox* _buttonBox= new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);

    vbox->addLayout(hbox);
    vbox->addLayout(hbox2);
    vbox->addLayout(hbox3);
    vbox->addLayout(hbox4);
    vbox->addLayout(hbox5);
    vbox->addWidget(_buttonBox);
    this->setLayout(vbox);

    connect(_buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(_buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
}

float HolderDialog::getSizeX()
{
    return _x->text().toFloat();
}

float HolderDialog::getSizeY()
{
    return _y->text().toFloat();
}

QString HolderDialog::getSourceObject()
{
    if(0==_objects->currentIndex())
    {
        return "";
    }
    return _objects->currentText();
}

int HolderDialog::getPercent()
{
    return _percent->value();
}

void HolderDialog::setSizeX(float x)
{
    _x->setText(QString("%1").arg(x));
}

void HolderDialog::setSizeY(float y)
{
    _y->setText(QString("%1").arg(y));
}

void HolderDialog::setName(QString name)
{

    _name->setText(name);
}

QString HolderDialog::getName()
{
    return _name->text();
}
