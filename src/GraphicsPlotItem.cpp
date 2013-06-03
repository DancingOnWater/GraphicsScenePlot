#include "GraphicsPlotItem.h"
#include <QPainter>
#include <qmath.h>
#include <QDebug>


class Graphics2DPlotGrid: public QGraphicsRectItem
{
public:
    Graphics2DPlotGrid(QGraphicsItem * parent);
    void setRange(int axisNumber, double min, double max);
    inline void setRect(const QRectF &rect) {setRect(rect.x(), rect.y(), rect.width(), rect.height());}
    void setRect(qreal x, qreal y, qreal w, qreal h);

    void setMainGrid(int axisNumber, double zero, double step);
    void setSecondaryGrid(int axisNumber, double zero, double step);
    void setMainGridPen(const QPen & pen);
    void setSecondaryGridPen(const QPen &pen);
    void setNocksFont(const QFont & font);
    inline QFont nocksFont()const{return m_NocksFont;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    struct AxisGuideLines;
    void autoSetRange();
    void autoSetGrid();
    void calculateOrdinateGrid();
    void calculateAbscissGrid();
    void paintAxeGuiLines(const AxisGuideLines& axe, QPainter *painter);
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
    AxisGuideLines absicissGuideLines;
    AxisGuideLines ordinateGuideLines;

    QPen m_mainPen;
    QPen m_secondaryPen;

    QList<QGraphicsSimpleTextItem *> abscissNocks;
    QFont m_NocksFont;
    bool isAutoGrid;

    double ordinateFactor;
    double abscissFactor;
    qreal ordinateTranslate;
    qreal abscissTranslate;
};
void Graphics2DPlotGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(absicissGuideLines.showLines)
        paintAxeGuiLines(absicissGuideLines, painter);

    if(ordinateGuideLines.showLines)
        paintAxeGuiLines(ordinateGuideLines, painter);

    QGraphicsRectItem::paint(painter, option, widget);
}

class GraphicsPlotItemPrivate
{
    Q_DECLARE_PUBLIC(GraphicsPlotItem)
    GraphicsPlotItem* q_ptr;

    GraphicsPlotItemPrivate(GraphicsPlotItem* parent);
    void compose();

    Graphics2DPlotGrid * dataRect;
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
    dataRect = new Graphics2DPlotGrid(q);
    titleText = new QGraphicsSimpleTextItem(q);
        abscissText = new QGraphicsSimpleTextItem(q);
        ordinateText = new QGraphicsSimpleTextItem(q);
    ordinateText->setRotation(-90);
        dataRect->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
        dataRect->setRange(0, 1, 2);
        dataRect->setRange(1, 1, 2);
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
    dataRect->setRect(0, 0 , dataWidth, dataHeight);
        dataRect->setPos(rect.width()-dataWidth, 2*titleText->boundingRect().height());

    abscissText->setPos( (dataWidth - abscissText->boundingRect().width())/2.0+dataRect->x(), rect.bottom() - abscissText->boundingRect().height());
        ordinateText->setPos(0, (dataHeight - ordinateText->boundingRect().width())/2.0+dataRect->y());
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

void GraphicsPlotItem::setDataRect(const QRectF &rect)
{
}


QRectF GraphicsPlotItem::dataRect()
{
    return d_ptr->dataRect->rect();
}

void GraphicsPlotItem::setAxisRange(int axisNumber, double min, double max)
{
    d_ptr->dataRect->setRange(axisNumber, min, max);
}

void GraphicsPlotItem::axisRange(int axisNumber, double *min, double *max)
{
}

void GraphicsPlotItem::setAxisAutoRange(int axisNumber, bool isAuto)
{
}

void GraphicsPlotItem::setMainGridLinePen(const QPen &pen)
{
   d_ptr->dataRect->setMainGridPen(pen);
}

void GraphicsPlotItem::setSecondaryGridLine(const QPen &pen)
{
    d_ptr->dataRect->setSecondaryGridPen(pen);
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


Graphics2DPlotGrid::Graphics2DPlotGrid(QGraphicsItem *parent):
    QGraphicsRectItem(parent),
    isAutoGrid(true)
{
    ordinateFactor = 1;
    m_secondaryPen = m_mainPen;
    m_secondaryPen.setColor(Qt::gray);
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
        autoGridSetValue(&absicissGuideLines);

    }
    else{
        ordinateRange.min = min;
        ordinateRange.max = max;
        autoGridSetValue(&ordinateGuideLines);
    }
    QRectF r = rect();
    abscissFactor =  (abscissRange.max - abscissRange.min)/r.width();
        ordinateFactor = (ordinateRange.max-ordinateRange.min)/r.height();
        abscissTranslate = abscissRange.min + r.x();
        ordinateTranslate = ordinateRange.min + r.y();
    calculateOrdinateGrid();
        calculateAbscissGrid();
}


void Graphics2DPlotGrid::calculateAbscissGrid()
{
    const QRectF r = rect();
    if(fabs(r.width()) < std::numeric_limits<float>::min()*5.0 || fabs(r.height()) < std::numeric_limits<float>::min()*5.0)
        return;

    auto calculteLine = [&] (double step, double baseValue, QVector<QLineF> *drawLinesArray)
    {
        int k;
        double minValue;
        int count;
        qreal stepInScene;

        if(fabs(step) > std::numeric_limits<double>::min()*5.0 )
        {
            k = (abscissRange.min - baseValue)/step;
            minValue = k*step+baseValue;
            count = (abscissRange.max - minValue)/step;

            stepInScene = step/abscissFactor;
            //TODO додумать что делать, если направляющая всего одна
            if( count >0){
                minValue -=abscissTranslate;
                minValue /=abscissFactor;
                drawLinesArray->resize(count);
                for(int i = 0; i< count; i++){
                    (*drawLinesArray)[i] = QLineF( minValue+i*stepInScene, r.y(), minValue+i*stepInScene, r.y()+r.height());
                }
            }
            else
                drawLinesArray->clear();
        }
        else
            drawLinesArray->clear();
    };

    calculteLine(absicissGuideLines.mainStep, absicissGuideLines.mainBaseValue, &(absicissGuideLines.main));
        calculteLine(absicissGuideLines.secondaryStep, absicissGuideLines.secondaryBaseValue, &(absicissGuideLines.secondary));
}

void Graphics2DPlotGrid::calculateOrdinateGrid()
{
    const QRectF r = rect();
    if(fabs(r.width()) < std::numeric_limits<float>::min()*5.0 || fabs(r.height()) < std::numeric_limits<float>::min()*5.0)
        return;

    auto calculteLine = [&] (double step, double baseValue, QVector<QLineF> *drawLinesArray)
    {
        int k;
        double minValue;
        int count;
        qreal stepInScene;

        if(fabs(step) > std::numeric_limits<double>::min()*5.0 )
        {
            k = (ordinateRange.min - baseValue)/step;
            minValue = k*step+baseValue;
            count = (ordinateRange.max - minValue)/step;

            stepInScene = step/ordinateFactor;
            //TODO додумать что делать, если направляющая всего одна
            if( count >0){
                minValue -=ordinateTranslate;
                minValue /=ordinateFactor;
                drawLinesArray->resize(count);
                for(int i = 0; i< count; i++){
                    (*drawLinesArray)[i] = QLineF(r.x(), minValue+i*stepInScene, r.x()+r.width(), minValue+i*stepInScene);
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

void Graphics2DPlotGrid::paintAxeGuiLines(const AxisGuideLines& axe, QPainter *painter)
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

void Graphics2DPlotGrid::setRect(qreal x, qreal y, qreal w, qreal h)
{
    abscissFactor = (abscissRange.max - abscissRange.min)/w;
        ordinateFactor = (ordinateRange.max-ordinateRange.min)/h;
        abscissTranslate = (abscissRange.min + x);
        ordinateTranslate = ordinateRange.min +y;
    QGraphicsRectItem::setRect(x, y, w, h);
    calculateOrdinateGrid();
        calculateAbscissGrid();
}


void Graphics2DPlotGrid::setSecondaryGridPen(const QPen &pen)
{
    m_secondaryPen = pen;
}


void Graphics2DPlotGrid::setMainGridPen(const QPen &pen)
{
    m_mainPen = pen;
}
