#include "PropertyDialog.h"
#include "PropertyTabWidget.h"
#include "PropertyStackedWidget.h"
#include  "QPropertyEditorWidget.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>


PropertyDialog::PropertyDialog(QList<QObject*> nodes,QMap<QObject*,QIcon> icons,DIALOG_TYPE dlg,QWidget *parent) :
    QDialog(parent)
{
    QDialogButtonBox* _buttonBox= new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
    QWidget* _view=0;
    QPropertyEditorWidget* pview=0;
    switch(dlg)
    {
        case TABVIEW: _view = new PropertyTabWidget(nodes,icons,this);break;
        case STACKED: _view = new PropertyStackedWidget(nodes,icons,this);break;
        case RESUME:  pview =  new QPropertyEditorWidget(this); pview->addResume(nodes); _view = pview; break;
        default: _view = new PropertyTabWidget(nodes,icons,this);break;
    }
    QVBoxLayout *hbox = new QVBoxLayout(this);
    this->setLayout(hbox);

    hbox->addWidget(_view);
    hbox->addWidget(_buttonBox);
    if(RESUME!=dlg){
        connect(_buttonBox,SIGNAL(rejected()),_view,SLOT(restoreObjectState()));
    }
    connect(_buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(_buttonBox,SIGNAL(rejected()),this,SLOT(reject()));
}
