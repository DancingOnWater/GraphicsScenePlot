#include "GraphicsPlotItem.h"

#include <QPainter>
#include <QStaticText>
#include <qmath.h>
#include <QDebug>
#include "GraphicsDataItem.h"


class GraphicsPlotNocksTube : public QGraphicsItem
{
public:
    GraphicsPlotNocksTube(QGraphicsItem *parent): QGraphicsItem(parent){}
    void updateNocks(const QList<QGraphicsSimpleTextItem*>& nocks);
    QRectF boundingRect()const {return m_boundRect;}
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *){}
    inline const QFont &font(){return m_NocksFont;}
private:
    QList<QGraphicsSimpleTextItem*> m_nocks;
    QFont m_NocksFont;
    QPen m_nockPen;
    QRectF m_boundRect;
};

class Graphics2DPlotGrid: public QGraphicsItem
{
public:
    Graphics2DPlotGrid(QGraphicsItem * parent);
    QRectF boundingRect() const;
    const QRectF & rect() const;
    void setRange(int axisNumber, double min, double max);

    void setMainGrid(int axisNumber, double zero, double step);
    void setSecondaryGrid(int axisNumber, double zero, double step);
    void setMainGridPen(const QPen & pen);
    void setSecondaryGridPen(const QPen &pen);
    inline QPen mainGridPen(){return m_mainPen;}
    inline QPen secondaryGridPen(){return m_secondaryPen;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public:
    struct AxisGuideLines {
        AxisGuideLines(): showLines(true){}
        QVector<QLineF> lines;
        bool showLines;
    };
    AxisGuideLines abscissMainLines;
    AxisGuideLines abscissSecondaryLines;
    AxisGuideLines ordinateMainLines;
    AxisGuideLines ordinateSecondaryLines;
private:

    void paintAxeGuidLines(const AxisGuideLines& axe, QPainter *painter, const QPen &linePen);

    QPen m_mainPen;
    QPen m_secondaryPen;

    QRectF m_rect;
};

class GraphicsPlotItemPrivate
{
    Q_DECLARE_PUBLIC(GraphicsPlotItem)
    GraphicsPlotItem* q_ptr;

    GraphicsPlotItemPrivate(GraphicsPlotItem* parent);
    void compose();
    void calculateAndSetTransForm();
    void autoSetRange();
    void autoSetGrid();
    void calculateOrdinateGrid();
    void calculateAbscissGrid();
    void setAxisRange(int axisNumber, double min, double max);

    Graphics2DPlotGrid * gridItem;
    QGraphicsSimpleTextItem * abscissText;
    QGraphicsSimpleTextItem * ordinateText;
    QGraphicsSimpleTextItem *titleText;
    QFont titleFont;
    QFont ordinaateFont;
    QFont abscissFont;

    QRectF rect;
    QRectF m_sceneDataRect;
    GraphicsPlotLegend *m_legend;
    GraphicsPlotNocksTube* ordinateMainNocks;
    GraphicsPlotNocksTube* ordinateSecondaryNocks;
    GraphicsPlotNocksTube* abscissSecondaryNocks;
    GraphicsPlotNocksTube* abscissMainNocks;

    struct Range{
        double min;
        double max;
    };
    struct AxisGuideLines {
        AxisGuideLines():baseValue(0.0), step(0.0){}
        double baseValue;
        double step;
    };
    AxisGuideLines abscissMainLines;
    AxisGuideLines abscissSecondaryLines;
    AxisGuideLines ordinateMainLines;
    AxisGuideLines ordinateSecondaryLines;

    Range abscissRange;
    Range ordinateRange;
    bool isAutoGrid;
    bool isAutoSecondaryGrid;

public:
    void range(int axisNumber, double *min, double *max);
};

GraphicsPlotItemPrivate::GraphicsPlotItemPrivate(GraphicsPlotItem* parent)
{
    q_ptr = parent;
    Q_Q(GraphicsPlotItem);
    gridItem = new Graphics2DPlotGrid(q);
    titleText = new QGraphicsSimpleTextItem(q);
        abscissText = new QGraphicsSimpleTextItem(q);
        ordinateText = new QGraphicsSimpleTextItem(q);
    ordinateMainNocks = new GraphicsPlotNocksTube(q);
        ordinateSecondaryNocks= new GraphicsPlotNocksTube(q);
        abscissSecondaryNocks = new GraphicsPlotNocksTube(q);
        abscissMainNocks = new GraphicsPlotNocksTube(q);
    ordinateText->setRotation(-90);
        gridItem->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
        gridItem->setRange(0, 1, 2);
        gridItem->setRange(1, 1, 2);
    ordinateSecondaryNocks->hide();
    abscissSecondaryNocks->hide();
    m_legend = nullptr;
    isAutoGrid = true;

}

void GraphicsPlotItemPrivate::compose()
{
    titleText->setFont(titleFont);
        abscissText->setFont(abscissFont);
    if(titleText->boundingRect().width() > rect.width()){
        //TODO case when titleText too long
    }

    //Composite by height
    qreal dataHeight = rect.height() - 2*titleText->boundingRect().height() - 2*(abscissText->boundingRect().height());
    if(dataHeight < 0.5*rect.height()){
        //TODO decrease font size
    }

    titleText->setPos((rect.width()-titleText->boundingRect().width())/2.0, rect.y());

    //Compose by width
    qreal dataWidth = rect.width()-2*ordinateText->boundingRect().height();
    if(dataWidth< 0.5*rect.width()){
        //TODO decrease font size
    }
    ordinateMainNocks->setPos(-ordinateMainNocks->boundingRect().width(), -5*ordinateMainNocks->font().pointSizeF()/4.0);

    m_sceneDataRect.setRect(rect.width()-dataWidth, 2*titleText->boundingRect().height() , dataWidth, dataHeight);

    abscissText->setPos( (dataWidth - abscissText->boundingRect().width())/2.0 + m_sceneDataRect.y(), rect.bottom() - abscissText->boundingRect().height());
        ordinateText->setPos(0, (dataHeight - ordinateText->boundingRect().width())/2.0 + m_sceneDataRect.y());
    calculateAndSetTransForm();
    q_ptr->update();
}

void GraphicsPlotItemPrivate::calculateAndSetTransForm()
{
    double  scaleX = m_sceneDataRect.width()/gridItem->rect().width();
        double scaleY = m_sceneDataRect.height()/gridItem->rect().height();
    QTransform transform = QTransform::fromTranslate( - gridItem->rect().x()*scaleX + m_sceneDataRect.x(), - gridItem->rect().y()*scaleY +m_sceneDataRect.y());
        transform.scale(scaleX, -scaleY);
    gridItem->setTransform(transform);
    ordinateMainNocks->setTransform(transform);
//        ordinateSecondaryNocks->setTransform(transform);
    abscissMainNocks->setTransform(transform);
//    abscissSecondaryNocks->setTransform(transform);
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
    return d_ptr->titleText->text();
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
    d_ptr->compose();
}

QFont GraphicsPlotItem::titleFont()
{
    return  d_ptr->titleFont;
}

QString GraphicsPlotItem::axisText(int axisNumber)
{
    QGraphicsSimpleTextItem *useText = *(&(d_ptr->abscissText) + axisNumber);
    return useText->text();
}

void GraphicsPlotItem::setAxisTextFont(int axisNumber, const QFont &font)
{   if(axisNumber == 0)
        d_ptr->abscissFont= font;
    else
        d_ptr->ordinaateFont = font;
    d_ptr->compose();
}

QFont GraphicsPlotItem::axisTextFont(int axisNumber)
{
    if(axisNumber == 0)
        return d_ptr->abscissFont;
     return d_ptr->ordinaateFont;
}

void GraphicsPlotItem::setRect(const QRectF &rect)
{
    Q_D(GraphicsPlotItem);
    prepareGeometryChange();
    d->rect = rect;
    d->compose();
}

QRectF GraphicsPlotItem::rect()
{
    return d_ptr->rect;
}

void GraphicsPlotItem::setAxisRange(int axisNumber, double min, double max)
{
    d_ptr->setAxisRange(axisNumber, min, max);
}

void GraphicsPlotItem::axisRange(int axisNumber, double *min, double *max)
{
    d_ptr->range(axisNumber, min, max);
}

void GraphicsPlotItem::setAxisAutoRange(int axisNumber, bool isAuto)
{
}

void GraphicsPlotItem::setMainGridLinePen(const QPen &pen)
{
    d_ptr->gridItem->setMainGridPen(pen);
}

QPen GraphicsPlotItem::mainGridLinePen()
{
    return d_ptr->gridItem->mainGridPen();
}

void GraphicsPlotItem::setSecondaryGridLine(const QPen &pen)
{
    d_ptr->gridItem->setSecondaryGridPen(pen);
}

QPen GraphicsPlotItem::secondayGridLinePen()
{
    return d_ptr->gridItem->secondaryGridPen();
}

void GraphicsPlotItem::setMainGridLine(int axisNumber, double baseValue, double step)
{
}

void GraphicsPlotItem::setSecondaryLineAuto(bool isAuto)
{
}

Graphics2DGraphItem *GraphicsPlotItem::addGraph(double *absciss, double *ordinate, qint32 length)
{
    Graphics2DGraphItem * ptr = new Graphics2DGraphItem(absciss, ordinate, length);
        addDataItem(ptr);
    return ptr;
}

void GraphicsPlotItem::addDataItem(GraphicsDataItem *item)
{
    item->setParentItem(d_ptr->gridItem);
    if(d_ptr->m_legend != nullptr)
        d_ptr->m_legend->addDataItem(item);
}

void GraphicsPlotItem::setLegend(GraphicsPlotLegend *legend)
{
    Q_D(GraphicsPlotItem);
    if(d->m_legend != nullptr){
        d->m_legend->setParent(nullptr);
        d->m_legend->removeAllItem();
    }
    d->m_legend = legend;
    if(d->m_legend != nullptr)
    {
        d->m_legend->setParentItem(this);
        QList<QGraphicsItem*> items = d->gridItem->childItems();
        for(int i =0; i< items.size(); ++i){
            if(items.at(i)->type() == GraphicsPlot::DataType){
                d->m_legend->addDataItem(static_cast<GraphicsDataItem*>(items.at(i)));
            }
        }
    }
}

GraphicsPlotLegend *GraphicsPlotItem::legend()
{
    return d_ptr->m_legend;
}

void GraphicsPlotItem::removeLegend()
{
    Q_D(GraphicsPlotItem);

    if(d->m_legend != nullptr){
        d->m_legend->setParent(nullptr);
        d->m_legend->removeAllItem();
    }
    d->m_legend = nullptr;
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


Graphics2DPlotGrid::Graphics2DPlotGrid(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    m_mainPen.setCosmetic(true);
        m_secondaryPen = m_mainPen;
        m_secondaryPen.setColor(Qt::gray);
}

QRectF Graphics2DPlotGrid::boundingRect() const
{
    return m_rect;
}

const QRectF &Graphics2DPlotGrid::rect() const
{
    return m_rect;
}

void Graphics2DPlotGrid::setRange(int axisNumber, double min, double max)
{
    if(axisNumber == 0){
        m_rect.setX(min);
        m_rect.setWidth(max-min);
    }
    else{
        m_rect.setY(min);
        m_rect.setHeight(max-min);
    }
}

void GraphicsPlotItemPrivate::range(int axisNumber, double *min, double *max)
{
    if(axisNumber == 0){
        *min = abscissRange.min;
        *max = abscissRange.max;
    }
    else{
         *min = ordinateRange.min;
        *max = ordinateRange.max;
    }
}


void GraphicsPlotItemPrivate::calculateAbscissGrid()
{
    const QRectF  r = gridItem->boundingRect();
    if(fabs(r.width()) < std::numeric_limits<float>::min()*5.0 || fabs(r.height()) < std::numeric_limits<float>::min()*5.0)
        return;
    QList<QGraphicsSimpleTextItem*> nocksList;

    auto calculteLine = [&] (AxisGuideLines* guides, QVector<QLineF> *lines )
    {
        int k;
        double minValue;
        int count;
        nocksList.clear();

        if(fabs(guides->step) > std::numeric_limits<double>::min()*5.0 )
        {
            k = (abscissRange.min - guides->baseValue)/guides->step;
            minValue = k*guides->step+guides->baseValue;
            count = (abscissRange.max - minValue)/guides->step;

            //TODO додумать что делать, если направляющая всего одна
            if( count >0){
                lines->resize(count);
                nocksList.reserve(count);
                double guidCoordinate;
                for(int i = 0; i< count; i++){
                    guidCoordinate = minValue+i*guides->step;
                    lines->operator[](i) = QLineF( guidCoordinate , ordinateRange.max, guidCoordinate, ordinateRange.min);
                    nocksList.append(new QGraphicsSimpleTextItem(QString::number(guidCoordinate)));
                    nocksList.last()->setPos(guidCoordinate, ordinateRange.min);
                }
            }
            else
                lines->clear();
        }
        else
            lines->clear();
    };

    calculteLine(&abscissMainLines, &(gridItem->abscissMainLines.lines));
    abscissMainNocks->updateNocks(nocksList);
        calculteLine(&abscissSecondaryLines, &(gridItem->abscissSecondaryLines.lines));
        abscissSecondaryNocks->updateNocks(nocksList);
}

void GraphicsPlotItemPrivate::calculateOrdinateGrid()
{
    const QRectF  r = gridItem->boundingRect();
    if(fabs(r.width()) < std::numeric_limits<float>::min()*5.0 || fabs(r.height()) < std::numeric_limits<float>::min()*5.0)
        return;
    QList<QGraphicsSimpleTextItem*> nocksList;

    auto calculteLine = [&] (AxisGuideLines* guides, QVector<QLineF> *lines)
    {
        int k;
        double minValue;
        int count;

        nocksList.clear();
        if(fabs(guides->step) > std::numeric_limits<double>::min()*5.0 )
        {
            k = (ordinateRange.min - guides->baseValue)/guides->step;
            minValue = k*guides->step+guides->baseValue;
            count = (ordinateRange.max - minValue)/guides->step;

            //TODO додумать что делать, если направляющая всего одна
            if( count >0){
                lines->resize(count);
                nocksList.reserve(count);
                double guidCoordinate;
                for(int i = 0; i< count; i++){
                    guidCoordinate = minValue+i*guides->step;
                    lines->operator[](i) = QLineF(abscissRange.max, guidCoordinate, abscissRange.min, guidCoordinate);
                    nocksList.append(new QGraphicsSimpleTextItem(QString::number(guidCoordinate)));
                    nocksList.last()->setPos(abscissRange.min, guidCoordinate);
                }
            }
            else
                lines->clear();
        }
        else
            lines->clear();
    };
    calculteLine(&ordinateMainLines, &(gridItem->ordinateMainLines.lines));
    ordinateMainNocks->updateNocks(nocksList);
        calculteLine(&ordinateSecondaryLines, &(gridItem->ordinateSecondaryLines.lines));
        ordinateSecondaryNocks->updateNocks(nocksList);
}

void Graphics2DPlotGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    paintAxeGuidLines(abscissSecondaryLines, painter, m_secondaryPen);
        paintAxeGuidLines(abscissMainLines, painter, m_mainPen);
        paintAxeGuidLines(ordinateSecondaryLines, painter, m_secondaryPen);
        paintAxeGuidLines(ordinateMainLines, painter, m_mainPen);
    painter->setPen(m_mainPen);
    painter->drawRect(m_rect);
}

void Graphics2DPlotGrid::paintAxeGuidLines(const AxisGuideLines& axe, QPainter *painter, const QPen &linePen)
{
    if(axe.showLines){
        painter->setPen(linePen);
        painter->drawLines(axe.lines);
    }
}

void Graphics2DPlotGrid::setSecondaryGridPen(const QPen &pen)
{
    m_secondaryPen = pen;
    m_secondaryPen.setCosmetic(true);
}


void Graphics2DPlotGrid::setMainGridPen(const QPen &pen)
{
    m_mainPen = pen;
    m_mainPen.setCosmetic(true);
}


void GraphicsPlotItemPrivate::setAxisRange(int axisNumber, double min, double max)
{
    if(min >= max)
        return;
    gridItem->setRange(axisNumber, min, max);

    auto autoGridSetValue = [&](AxisGuideLines * guidesMain, AxisGuideLines *guidesSecondary)
    {
        if(isAutoGrid){
            guidesMain->baseValue = min;
            guidesMain->step = (max-min)/5.0;
        }
        if(isAutoSecondaryGrid || isAutoGrid){
                guidesSecondary->baseValue = guidesMain->baseValue;
                guidesSecondary->step = guidesMain->step/5.0;
        }
    };

    if( axisNumber ==0){
        abscissRange.min = min;
            abscissRange.max = max;
        autoGridSetValue(&abscissMainLines, &abscissSecondaryLines);

    }
    else{
        ordinateRange.min = min;
            ordinateRange.max = max;
        autoGridSetValue(&ordinateMainLines, &ordinateSecondaryLines);
    }
    calculateOrdinateGrid();
        calculateAbscissGrid();

    compose();
}


void GraphicsPlotNocksTube::updateNocks(const QList<QGraphicsSimpleTextItem *> &nocks)
{
    while(m_nocks.size() >0)
        delete m_nocks.takeLast();
    m_nocks = nocks;
    if(nocks.size() == 0){
        m_boundRect = QRectF();
        return;
    }
    QGraphicsSimpleTextItem *item= nocks.first();
    QPointF nockPos = item->pos();
    m_boundRect = item->boundingRect().translated(nockPos.x(), nockPos.y());
    item->setParentItem(this);
    item->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    for(int i = 1; i <nocks.size(); ++i){
        item = nocks.at(i);
        nockPos = item->pos();
        m_boundRect|= item->boundingRect().translated(nockPos.x(), nockPos.y());
        item->setParentItem(this);
        item->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        item->setFont(m_NocksFont);
        item->setPen(m_nockPen);
    }

    update();
}
