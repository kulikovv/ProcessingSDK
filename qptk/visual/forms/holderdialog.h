#ifndef HOLDERDIALOG_H
#define HOLDERDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class QSpinBox;

namespace visual{
namespace ui{
namespace utils{

/*! \brief Диалог параметров зоны
 * Диалог, позволяющий задать параметры зоны (название, ) на LayoutWidget
 */
class HolderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HolderDialog(QStringList items, QWidget *parent = 0);
    /*! Поставить название области
      \param name название области
      */
    void setName(QString name);
    /*! Получать название области, введенное пользователем
      \return название области
      */
    QString getName();
    /*! Ширина физической арены в метрах, заданная пользователем
      \return получить заданную пользователем ширину арены
      */
    float getSizeX();
    /*! Высота физической арены в метрах, заданная пользователем
      \return получить высоту пользователем ширину арены
      */
    float getSizeY();

    QString getSourceObject();
    int getPercent();
    /*! Начальная ширина физической арены в метрах
      \param x ширина физической арены в метрах
      */
    void setSizeX(float x);
    /*! Начальная высота физической арены в метрах
      \param y высота физической арены в метрах
      */
    void setSizeY(float y);
signals:

public slots:
protected:
    QLineEdit* _name; //!
    QLineEdit* _x;
    QLineEdit* _y;
    QComboBox* _objects;
    QSpinBox* _percent;
};
}
}
}
#endif // HOLDERDIALOG_H
