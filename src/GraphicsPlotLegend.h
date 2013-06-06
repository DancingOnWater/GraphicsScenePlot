#ifndef GRAPHICSPLOTLEGEND_H
#define GRAPHICSPLOTLEGEND_H
#include <QGraphicsObject>
#include "Global.h"
#include "GraphicsDataItem.h"

class GraphicsPlotLegendPrivate;
class SRCSHARED_EXPORT GraphicsPlotLegend : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit GraphicsPlotLegend(QGraphicsItem *parent = 0);
    GraphicsPlotLegend(const QRectF & rect, QGraphicsItem *parent =0);
    ~GraphicsPlotLegend();
    void addDataItem(GraphicsDataItem * item);
    void removeItem(GraphicsDataItem *item);
    void removeAllItem();
    void setRect(const QRectF &rect);
    QRectF rect();
    void setPen(const QPen & pen);
    QPen pen();
    void setBrush(const QBrush & brush);
    QBrush brush();
    void setFont(const QFont & font);
    QFont font();

    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected Q_SLOTS:
    void itemRemove();
    void dataItemChange();
private:
    Q_DECLARE_PRIVATE(GraphicsPlotLegend)
    GraphicsPlotLegendPrivate *d_ptr;
};

#endif // GRAPHICSPLOTLEGEND_H
