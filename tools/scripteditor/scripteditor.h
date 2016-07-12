 #ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

/*Project:      EthoStudio 2010
 *File:         scripteditor.h
 *Author:       Kulikov Victor
 *Copyright:    Kulikov Victor
 *Contact:      kulikov.victor@gmail.com
 */


#include "ScriptEditor_global.h"

#include <QWidget>
#include <QAction>
#include <QListWidget>
#include <QMap>

class QStringListModel;
class QCompleter;
class ScriptHighlight;
class QTextEdit;
class ScriptTextEditor;
class QVBoxLayout;
class ScriptWorker;
class LineCounter;
/*! Класс ScriptEditor - представляет собой инструмент для редактирования и
  запуска скриптов написанных на языке JavaScript.
  Основные функции:
  1. Хранение всех используемых скриптов
  2. Редактирование скриптов
  3. Исполнение скриптов
  4. Добавление объектов системы в контекст исполнения

  Так как ScriptEditor является единственным объектом своего типа в программе он
  создан по технологии Singleton.
  */
class SCRIPTEDITORSHARED_EXPORT ScriptEditor : public QWidget
{
Q_OBJECT
public:
    //! Неявный конструктор объекта: создает новый объект если его нет или возвращает уже созданный
    static ScriptEditor* createEditor(QWidget* parent=0);
    //! Ручной деструктор вызвается при закрытии приложения см. mainwindow.cpp
    static void deleteEditor();
    //! Неявный конструктор объекта: создает новый объект если его нет или возвращает уже созданный
    static ScriptEditor* cloneEditor(QWidget* parent=0);

    //! Возвращает текущий текст из редактора
    QString getScript(QString key=QString());

    //! Установить скрипт в редактор
    void setScript(QString script,QString key=QString());

    //! Для того,чтобы не видно было двух колонок список скриптов можно вынести в плавающую панель
    QWidget* getControlPanel();
    //! Список доступных пользователю скриптов в данной настройке см. Preset
    QStringList getScripts();

    static void copy(ScriptEditor* from,ScriptEditor* to);
    void clear();
    /*!
     * \brief setScriptDir выбрать дирректорию, в которой храняться скрипты
     * \param dir диррекстория со скриптами
     */
    void setScriptDir(QString dir);
signals:

public slots:
    //! Обрывание исполнения скрипта
    void stopExecuting();

    void hideOutput(bool on);
    void saveScriptFile(QString script);
    void openScriptFile(QString path);
    //! Добавление объекта из приложения в виде переменной скрипта obj - указатель на объект,
    //! name - псевдоним объекта, decription - описание (временно не используется)
    void addObject(QObject* obj,QString name="",QString decription="");
    //! Запуск текущего скрипта
    void run();
    //! Запуск скрипта по имени из списка доступных пользователю
    void runScript(QString script);
    //! Вывод на консоль
    void printText(QString x);
protected slots:
    void save();
    /*!
     * \brief onTextChanged при изменении текста в редакторе все сохраняется в scripts
     */
    void onTextChanged();
    void nhideOutput(bool hide);
    //! Создать новый скрипт
    void newScript();
    //! Удалить выбранные скрипты
    void removeScript();
    //! Текущий элемент заменен
    void currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
protected:
    //! Запуск исполнения конкретного текста (используется методами run и runScript)
    void runScriptText(QString script);
    //! Новый скрипт с названием
    void newScript(QString name);
    //! Контейнер ассоциирующий скрипты с их названиями
    QMap<QString,QString> scripts;  
private:
    //! Сам редактор скриптов
    static ScriptEditor* script;
    //! Скрытый конструктор и деструктор
    explicit ScriptEditor(QWidget *parent = 0);
    ~ScriptEditor();

    ScriptHighlight* highlight;
    //! Консольный вывод
    QTextEdit* output;
    //! Редактор
    ScriptTextEditor* editor;
    LineCounter* numberPage;
    //! Движок скриптовый
    ScriptWorker* interpreter;
    //! Представление для показа списка скриптов
    QListWidget* listview;
    //! Фрагмет который наверное стоит вынисти из интерфейса редактора
    QVBoxLayout *over;
    QWidget* outpanel;
    QString scriptdir;

};

#endif // SCRIPTEDITOR_H
