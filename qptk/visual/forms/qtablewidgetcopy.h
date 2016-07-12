#ifndef QTABLEWIDGETCOPY_H
#define QTABLEWIDGETCOPY_H

#include <QTableWidget>
#include <sdk_global.h>
namespace visual
{
/*! \brief Пространство имен visual::ui используется для объединения виждетов.
  Пространство имен visual::ui используется для объединения виждетов.
  Здесь хранятся виждеты адаптированные для использования в узлах из visual.
  */
namespace ui
{
class PROCESSINGFRAMEWORK_SHARED_EXPORT QTableWidgetCopy : public QTableWidget
{
    Q_OBJECT
public:
    explicit QTableWidgetCopy(QWidget *parent = 0);

signals:

private slots:
  void copy();
  void copyWithHeaders();
  void saveExcel();
protected:
  QString toText(QLatin1Char sep);
  virtual void keyPressEvent(QKeyEvent * event);
};
}
}
#endif // QTABLEWIDGETCOPY_H
