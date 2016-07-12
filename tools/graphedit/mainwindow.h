#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "editorscene.h"
#include <QTreeWidget>
#include <QSignalMapper>

#include <QHBoxLayout>
#include <typeinfo>

#include <QPropertyEditorWidget.h>
#include "graphproxyeditor.h"

namespace Ui {
    class MainWindow;
}
namespace synch
{
    class Synch;
}
namespace utils
{
    struct NodeDecoration;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*!
     * \brief saveFile сохранить файл
     * \param fname название файла
     */
    void saveFile(QString fname);


public slots:
    /*!
     * \brief loadFile загрузить файл
     * \param fname название файла
     */
    void loadFile(QString fname);

private slots:
    void on_actionOpen_triggered();
    void on_actionInsert_Line_triggered(bool checked);
    void onItemDoubleClicked(QTreeWidgetItem* item,int value);

    void on_actionSave_triggered();
    void on_actionShow_Preview_triggered();
    void on_actionScreen_shot_triggered();
    void on_actionNew_triggered();
    void on_actionSaveAs_triggered();
    void on_actionScientific_pictures_triggered();    
    void on_actionScript_Editor_triggered();
    void on_addBaseGraph_triggered();

    void onObjectSelection(QObject* obj);
    void onWidgetPreview(QWidget* preview);

    void onContextMenu(const QPoint& pos);

    void on_actionRead_Log_triggered();

    void on_actionAggrigate_Properies_triggered();

private:
    GraphProxyEditor _proxyEditor;//!< Удаленный редактор для графа
    QMenu* _menu;//!< Контекстное меню

    Ui::MainWindow *uix;//!<
    QGraphicsView view;
    QTreeWidget tree;

    QPropertyEditorWidget* pedit;//!< Редактор свойств

    QSignalMapper _mapper;//!< Сопоставление файлов открытых

    QString _lastdir;//!< Последняя открытая директория
    QString _filename;//!< Название откртого файла
    QStringList _fileList;//!< Список файлов для быстрого доступа
};

#endif // MAINWINDOW_H
