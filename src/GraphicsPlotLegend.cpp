#include "GraphicsPlotLegend.h"
#include <QStaticText>
class GraphicsPlotLegendPrivate
{
public:
    GraphicsPlotLegendPrivate() { m_brush = QBrush(Qt::white);}

    QRectF m_rect;
    QPen m_pen;
    QBrush m_brush;
    QHash<GraphicsDataItem*, QStaticText> addedItemHash;
    QFont m_font;
};

GraphicsPlotLegend::GraphicsPlotLegend(QGraphicsItem *parent) :
    QGraphicsObject(parent),
    d_ptr(new GraphicsPlotLegendPrivate())
{
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}

GraphicsPlotLegend::GraphicsPlotLegend(const QRectF &rect, QGraphicsItem *parent):
    QGraphicsObject(parent),
    d_ptr(new GraphicsPlotLegendPrivate())
{
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setRect(rect);
}

GraphicsPlotLegend::~GraphicsPlotLegend()
{
    delete d_ptr;
}

void GraphicsPlotLegend::addDataItem(GraphicsDataItem *item)
{
    d_ptr->addedItemHash.insert(item, QStaticText(item->title().toLower()));
    QObject::connect(item,  &QGraphicsObject::parentChanged, this, &GraphicsPlotLegend::itemRemove);
        QObject::connect(item, &GraphicsDataItem::titleChange, this, &GraphicsPlotLegend::dataItemChange);
        QObject::connect(item, &GraphicsDataItem::penItemChange, this, &GraphicsPlotLegend::dataItemChange);
}

void GraphicsPlotLegend::removeItem(GraphicsDataItem *item)
{
    d_ptr->addedItemHash.remove(item);
    QObject::disconnect(item);
    update();
}

void GraphicsPlotLegend::removeAllItem()
{
    for( QHash<GraphicsDataItem*, QStaticText>::Iterator iterator= d_ptr->addedItemHash.begin(); iterator != d_ptr->addedItemHash.end(); ++iterator){
        iterator.key()->disconnect(0,this);
    }
    d_ptr->addedItemHash.clear();
}


void GraphicsPlotLegend::setRect(const QRectF & rect)
{
    d_ptr->m_rect = rect;
}

QRectF GraphicsPlotLegend::rect()
{
    return d_ptr->m_rect;
}

void GraphicsPlotLegend::setPen(const QPen &pen)
{
    d_ptr->m_pen = pen;
}

QPen GraphicsPlotLegend::pen()
{
    return d_ptr->m_pen;
}

void GraphicsPlotLegend::setBrush(const QBrush &brush)
{
    d_ptr->m_brush = brush;
}

QBrush GraphicsPlotLegend::brush()
{
    return d_ptr->m_brush;
}


QRectF GraphicsPlotLegend::boundingRect() const
{
    return d_ptr->m_rect;
}

void GraphicsPlotLegend::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    Q_D(GraphicsPlotLegend);
    painter->setPen(d->m_pen);
        painter->setBrush(d->m_brush);
    painter->drawRect(d->m_rect);
    painter->setBrush(Qt::NoBrush);
    painter->setFont(d->m_font);
    const qreal fontSize = d->m_font.pointSizeF();
    const qreal fontSize_2 = 3*fontSize/4.0;

    int count =0; qreal zeroPosition;
    for( QHash<GraphicsDataItem*, QStaticText>::Iterator iterator= d->addedItemHash.begin(); iterator != d->addedItemHash.end(); ++iterator, ++count){
        zeroPosition = fontSize*count;
        painter->setPen(iterator.key()->pen());
        painter->drawLine( QLineF(10, fontSize_2 + zeroPosition, 30, zeroPosition+fontSize_2));
        painter->setPen(d_ptr->m_pen);
        painter->drawStaticText(40, zeroPosition, iterator.value());
    }
}

void GraphicsPlotLegend::itemRemove()
{
    removeItem(reinterpret_cast<GraphicsDataItem*>(sender()));
}

void GraphicsPlotLegend::dataItemChange()
{
    GraphicsDataItem * s=reinterpret_cast<GraphicsDataItem*>(sender());
    d_ptr->addedItemHash[s].setText(s->title().toLower());
    update();
}


QFont GraphicsPlotLegend::font()
{
    return d_ptr->m_font;
}

QPainterPath GraphicsPlotLegend::shape() const
{
    const Q_D(GraphicsPlotLegend);
    QPainterPath path;
    path.addRect(d->m_rect.x(), d->m_rect.y(), d->m_rect.width()+d->m_pen.width(), d->m_rect.height()+d->m_pen.width());
    return path;
}


void GraphicsPlotLegend::setFont(const QFont &font)
{
    d_ptr->m_font = font;
}
