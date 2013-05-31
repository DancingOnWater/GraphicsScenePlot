#ifndef GRAPHICSPLOTITEM_H
#define GRAPHICSPLOTITEM_H
#include <QGraphicsItem>
#include "Global.h"

class GraphicsPlotItemPrivate;
class GraphicsGraphItem;
class GraphicsDataItem;

/*!
  \class GraphicsPlotItem
 * \brief The GraphicsPlotItem class
 */
class SRCSHARED_EXPORT GraphicsPlotItem: public QGraphicsItem
{
public:
    GraphicsPlotItem(QGraphicsItem *parent=0);
    ~GraphicsPlotItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setTitle(const QString & text);
    QString title() const;
    void setTitleFont(const QFont & font);
    QFont titleFont();

    void setAxisText(int axisNumber, const QString & text);
    QString axisText(int axisNumber);
    void setAxisTextFont(int axisNumber, const QFont & font);
    QFont axisTextFont(int axisNumber);

    void setRect(const QRectF& rect);
    QRectF rect();

    void setDataRect(const QRectF &rect);
    QRectF dataRect();

    void setAxisRange(int axisNumber, double min, double max);
    inline void setAbscissaRange(double min, double max){setAxisRange(0, min, max);}
    inline void setOrdinateRange(double min, double max){setAxisRange(1, min, max);}

    void axisRange(int axisNumber, double *min, double *max);

    void setMainGridLinePen(const QPen& pen);
    QPen mainGridLinePen();
    void setSecondaryGridLine(const QPen &pen);
    QPen secondayGridLinePen();
    void setMainGridLine(int axisNumber, double zero, double step);
    void setSecondaryLineAuto(bool isAuto);
    void setSecondaryGridLine(int axisNumber, double step);



    GraphicsGraphItem * addGraph(double *absciss, double *ordinate, qint32 length);
    void addDataItem(GraphicsDataItem *item);


private:
    Q_DECLARE_PRIVATE(GraphicsPlotItem)
    GraphicsPlotItemPrivate *d_ptr;
};

#endif // GRAPHICSPLOTITEM_H
