#ifndef GRAPHICVISITOR_H
#define GRAPHICVISITOR_H

#include <QPixmap>
#include <QPointF>
#include <QPainter>

#include "cameravisitor.h"
#include "objectmodel.h"

class GameView;

class GraphicVisitor : public CameraVisitor
{
private:
    QPainter painter;

public:
    GraphicVisitor(GameView * gv);
    void paint();
};

#endif // GRAPHICVISITOR_H
