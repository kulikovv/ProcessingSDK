#ifndef CONTROLS_H
#define CONTROLS_H

#include <QPointF>
#include <QMouseEvent>

struct MouseControl{
    QPointF imgCoordinates;//Координаты клика на изображении
    QPoint  widgetCoordinates;//Координаты на окошке
    QPoint globalCoordinates;//Глобальные координаты
    Qt::MouseButtons button;//Нажатые кнопки
    double delta;//Колесико мыши

};

#endif // CONTROLS_H
