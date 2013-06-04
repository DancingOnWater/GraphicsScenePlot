#include "GraphicsPlotItem.h"

#include <QPainter>
#include <qmath.h>


#include "GraphicsDataItem.h"
class Graphics2DPlotGrid: public QGraphicsItem
{
public:
    Graphics2DPlotGrid(QGraphicsItem * parent);
    QRectF boundingRect() const;
    const QRectF & rect() const;
    void setRange(int axisNumber, double min, double max);
    void range(int axisNumber, double *min, double* max);

    void setMainGrid(int axisNumber, double zero, double step);
    void setSecondaryGrid(int axisNumber, double zero, double step);
    void setMainGridPen(const QPen & pen);
    void setSecondaryGridPen(const QPen &pen);
    inline QPen mainGridPen(){return m_mainPen;}
    inline QPen secondaryGridPen(){return m_secondaryPen;}
    void setNocksFont(const QFont & font);
    inline QFont nocksFont()const{return m_NocksFont;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    struct AxisGuideLines;
    void autoSetRange();
    void autoSetGrid();
    void calculateOrdinateGrid();
    void calculateAbscissGrid();
    void paintAxeGuidLines(const AxisGuideLines& axe, QPainter *painter);
private:
    struct Range{
        double min;
        double max;
    };
    struct AxisGuideLines {
        AxisGuideLines():mainBaseValue(0.0), mainStep(0.0), secondaryStep(0.0), showMainLines(true), showSecondaryLine(true), showLines(true){}
        double mainBaseValue;
        double mainStep;
        double secondaryBaseValue;
        double secondaryStep;
        QVector<QLineF> main;
        QVector<QLineF> secondary;
        QList<QGraphicsSimpleTextItem *> nocks;
        bool showMainLines;
        bool showSecondaryLine;
        bool showLines;
    };
    Range abscissRange;
    Range ordinateRange;
    AxisGuideLines abscissGuideLines;
    AxisGuideLines ordinateGuideLines;

    QPen m_mainPen;
    QPen m_secondaryPen;

    QList<QGraphicsSimpleTextItem *> abscissNocks;
    QFont m_NocksFont;
    bool isAutoGrid;

    QRectF m_rect;
};
void Graphics2DPlotGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(abscissGuideLines.showLines)
        paintAxeGuidLines(abscissGuideLines, painter);

    if(ordinateGuideLines.showLines)
        paintAxeGuidLines(ordinateGuideLines, painter);

    painter->setPen(m_mainPen);
    painter->drawRect(m_rect);
}

class GraphicsPlotItemPrivate
{
    Q_DECLARE_PUBLIC(GraphicsPlotItem)
    GraphicsPlotItem* q_ptr;

    GraphicsPlotItemPrivate(GraphicsPlotItem* parent);
    void compose();
    void calculateAndSetTransForm();

    Graphics2DPlotGrid * gridItem;
    QGraphicsSimpleTextItem * abscissText;
    QGraphicsSimpleTextItem * ordinateText;
    QGraphicsSimpleTextItem *titleText;
    QFont titleFont;
    QFont ordinaateFont;
    QFont abscissFont;

    QRectF rect;
    QRectF m_sceneDataRect;
};

GraphicsPlotItemPrivate::GraphicsPlotItemPrivate(GraphicsPlotItem* parent)
{
    q_ptr = parent;
    Q_Q(GraphicsPlotItem);
    gridItem = new Graphics2DPlotGrid(q);
    titleText = new QGraphicsSimpleTextItem(q);
        abscissText = new QGraphicsSimpleTextItem(q);
        ordinateText = new QGraphicsSimpleTextItem(q);
    ordinateText->setRotation(-90);
        gridItem->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
        gridItem->setRange(0, 1, 2);
        gridItem->setRange(1, 1, 2);
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
    m_sceneDataRect.setRect(rect.width()-dataWidth, 2*titleText->boundingRect().height() , dataWidth, dataHeight);

    abscissText->setPos( (dataWidth - abscissText->boundingRect().width())/2.0 + m_sceneDataRect.y(), rect.bottom() - abscissText->boundingRect().height());
        ordinateText->setPos(0, (dataHeight - ordinateText->boundingRect().width())/2.0 + m_sceneDataRect.y());
        q_ptr->update();
    calculateAndSetTransForm();

}

void GraphicsPlotItemPrivate::calculateAndSetTransForm()
{
    double  scaleX = m_sceneDataRect.width()/gridItem->rect().width();
        double scaleY = m_sceneDataRect.height()/gridItem->rect().height();
    QTransform transform = QTransform::fromTranslate( - gridItem->rect().x()*scaleX + m_sceneDataRect.x(), - gridItem->rect().y()*scaleY +m_sceneDataRect.y());
        transform.scale(scaleX, scaleY);
    gridItem->setTransform(transform);
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
    d_ptr->gridItem->setRange(axisNumber, min, max);
    d_ptr->calculateAndSetTransForm();
}

void GraphicsPlotItem::axisRange(int axisNumber, double *min, double *max)
{
    d_ptr->gridItem->range(axisNumber, min, max);
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
    return (new Graphics2DGraphItem(absciss, ordinate, length, d_ptr->gridItem));
}

void GraphicsPlotItem::addDataItem(GraphicsDataItem *item)
{
    item->setParentItem(d_ptr->gridItem);
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
    QGraphicsItem(parent),
    isAutoGrid(true)
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
    if(min >= max)
        return;
    auto autoGridSetValue = [&](AxisGuideLines * axe)
    {
        if(isAutoGrid){
            axe->mainBaseValue = min;
            axe->mainStep = (max-min)/5.0;
                axe->secondaryBaseValue = axe->mainBaseValue;
                axe->secondaryStep = axe->mainStep/5.0;
        }
    };

    if( axisNumber ==0){
        abscissRange.min = min;
            abscissRange.max = max;
        autoGridSetValue(&abscissGuideLines);

    }
    else{
        ordinateRange.min = min;
            ordinateRange.max = max;
        autoGridSetValue(&ordinateGuideLines);
    }
    m_rect.setRect(abscissRange.min, ordinateRange.min, abscissRange.max - abscissRange.min, ordinateRange.max - ordinateRange.min);
    calculateOrdinateGrid();
        calculateAbscissGrid();
}

void Graphics2DPlotGrid::range(int axisNumber, double *min, double *max)
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


void Graphics2DPlotGrid::calculateAbscissGrid()
{
    const QRectF & r = m_rect;
    if(fabs(r.width()) < std::numeric_limits<float>::min()*5.0 || fabs(r.height()) < std::numeric_limits<float>::min()*5.0)
        return;

    auto calculteLine = [&] (double step, double baseValue, QVector<QLineF> *drawLinesArray)
    {
        int k;
        double minValue;
        int count;

        if(fabs(step) > std::numeric_limits<double>::min()*5.0 )
        {
            k = (abscissRange.min - baseValue)/step;
            minValue = k*step+baseValue;
            count = (abscissRange.max - minValue)/step;

            //TODO додумать что делать, если направляющая всего одна
            if( count >0){

                drawLinesArray->resize(count);
                for(int i = 0; i< count; i++){
                    (*drawLinesArray)[i] = QLineF( minValue+i*step, ordinateRange.min, minValue+i*step, ordinateRange.max);
                }
            }
            else
                drawLinesArray->clear();
        }
        else
            drawLinesArray->clear();
    };

    calculteLine(abscissGuideLines.mainStep, abscissGuideLines.mainBaseValue, &(abscissGuideLines.main));
        calculteLine(abscissGuideLines.secondaryStep, abscissGuideLines.secondaryBaseValue, &(abscissGuideLines.secondary));
}

void Graphics2DPlotGrid::calculateOrdinateGrid()
{
    const QRectF & r = m_rect;
    if(fabs(r.width()) < std::numeric_limits<float>::min()*5.0 || fabs(r.height()) < std::numeric_limits<float>::min()*5.0)
        return;

    auto calculteLine = [&] (double step, double baseValue, QVector<QLineF> *drawLinesArray)
    {
        int k;
        double minValue;
        int count;

        if(fabs(step) > std::numeric_limits<double>::min()*5.0 )
        {
            k = (ordinateRange.min - baseValue)/step;
            minValue = k*step+baseValue;
            count = (ordinateRange.max - minValue)/step;

            //TODO додумать что делать, если направляющая всего одна
            if( count >0){
                drawLinesArray->resize(count);
                for(int i = 0; i< count; i++){
                    (*drawLinesArray)[i] = QLineF(abscissRange.min, minValue+i*step, abscissRange.max,minValue+i*step);
                }
            }
            else
                drawLinesArray->clear();
        }
        else
            drawLinesArray->clear();
    };

    calculteLine(ordinateGuideLines.mainStep, ordinateGuideLines.mainBaseValue, &(ordinateGuideLines.main));
        calculteLine(ordinateGuideLines.secondaryStep, ordinateGuideLines.secondaryBaseValue, &(ordinateGuideLines.secondary));
}

void Graphics2DPlotGrid::paintAxeGuidLines(const AxisGuideLines& axe, QPainter *painter)
{
    if(axe.showSecondaryLine){
        painter->setPen(m_secondaryPen);
        painter->drawLines(axe.secondary);
    }
    if(axe.showMainLines){
        painter->setPen(m_mainPen);
        painter->drawLines(axe.main);
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
