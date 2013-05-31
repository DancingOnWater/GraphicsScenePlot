#include "GraphicsPlotItem.h"
#include <QPainter>
#include <qmath.h>
class GraphicsPlotItemPrivate
{
    Q_DECLARE_PUBLIC(GraphicsPlotItem)
    GraphicsPlotItem* q_ptr;

    GraphicsPlotItemPrivate(GraphicsPlotItem* parent);
    void compose();
    QGraphicsRectItem * dataRect;
    QGraphicsSimpleTextItem * abscissText;
    QGraphicsSimpleTextItem * ordinateText;
    QGraphicsSimpleTextItem *titleText;
    QFont titleFont;
    QFont ordinaateFont;
    QFont abscissFont;

    QRectF rect;
};

GraphicsPlotItemPrivate::GraphicsPlotItemPrivate(GraphicsPlotItem* parent)
{
    q_ptr = parent;
    Q_Q(GraphicsPlotItem);
    dataRect = new QGraphicsRectItem(q);
    titleText = new QGraphicsSimpleTextItem(q);
        abscissText = new QGraphicsSimpleTextItem(q);
        ordinateText = new QGraphicsSimpleTextItem(q);
    dataRect->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    ordinateText->setRotation(-90);
}

void GraphicsPlotItemPrivate::compose()
{
    if(titleText->boundingRect().width() > rect.width()){
        //TODO case when titleText too long
    }

    //Composite with each orther
    qreal dataHeight = rect.height() - 2*titleText->boundingRect().height() - 2*(abscissText->boundingRect().height());
    if(dataHeight < 0.5*rect.height()){
        //TODO decrease font size
    }

    titleText->setPos((rect.width()-titleText->boundingRect().width())/2.0, rect.y());
    qreal dataWidth = rect.width();
    dataRect->setRect(0, 2*titleText->boundingRect().height() , dataWidth, dataHeight);
    q_ptr->update();
}


GraphicsPlotItem::GraphicsPlotItem(QGraphicsItem *parent):
    QGraphicsItem(parent),
    d_ptr(new GraphicsPlotItemPrivate(this))
{

}

GraphicsPlotItem::~GraphicsPlotItem()
{
}

void GraphicsPlotItem::setTitle(const QString &text)
{
    d_func()->titleText->setText(text);
    d_ptr->compose();
}

QString GraphicsPlotItem::title() const
{
}

void GraphicsPlotItem::setAxisText(int axisNumber, const QString &text)
{
    QGraphicsSimpleTextItem *useText = *(&(d_ptr->abscissText) + axisNumber);
    useText->setText(text);
    d_ptr->compose();
}

void GraphicsPlotItem::setTitleFont(const QFont &font)
{
    d_ptr->titleFont = font;
}

QFont GraphicsPlotItem::titleFont()
{
    d_ptr->titleFont;
}

QString GraphicsPlotItem::axisText(int axisNumber)
{
}

void GraphicsPlotItem::setAxisTextFont(int axisNumber, const QFont &text)
{
}

QFont GraphicsPlotItem::axisTextFont(int axisNumber)
{
}

void GraphicsPlotItem::setRect(const QRectF &rect)
{
    Q_D(GraphicsPlotItem);
    prepareGeometryChange();
    d->rect = rect;
    d_func()->titleText->setFont(d_func()->titleFont);
        d_func()->abscissText->setFont(d_func()->abscissFont);

}

QRectF GraphicsPlotItem::rect()
{
    return d_ptr->rect;
}

void GraphicsPlotItem::setDataRect(const QRectF &rect)
{
}


QRectF GraphicsPlotItem::dataRect()
{
}

void GraphicsPlotItem::setAxisRange(int axisNumber, double min, double max)
{
}

void GraphicsPlotItem::axisRange(int axisNumber, double *min, double *max)
{
}

void GraphicsPlotItem::setMainGridLinePen(const QPen &pen)
{
}

void GraphicsPlotItem::setSecondaryGridLine(const QPen &pen)
{
}

QPen GraphicsPlotItem::secondayGridLinePen()
{
}

void GraphicsPlotItem::setMainGridLine(int axisNumber, double zero, double step)
{
}

void GraphicsPlotItem::setSecondaryLineAuto(bool isAuto)
{
}


GraphicsGraphItem *GraphicsPlotItem::addGraph(double *absciss, double *ordinate, qint32 length)
{
}

void GraphicsPlotItem::addDataItem(GraphicsDataItem *item)
{
}

QRectF GraphicsPlotItem::boundingRect() const
{
    return d_ptr->rect;
}

void GraphicsPlotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    Q_UNUSED(painter)
}
