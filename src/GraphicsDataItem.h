#pragma once
#include <QGraphicsObject>
#include <QPainter>

#include "Global.h"


/*!
 * \class GraphicsDataItem
 *\brief base class for  items-data.
 *\fn GraphicsDataItem::setTitle
 *Set item title. Use in legend
 *\fn GraphicsDataItem:: title()
 *return title
 *\fn GraphicsDataItem::dataItemChange()
 *emit when data change. Must be used inc hild classes.
 *\fn GraphicsDataItem::penItemChange()
 *Emits when pen change
 *\fn GraphicsDataItem::titleChange()
 *Emits when title change
 */

/*!
 *\class Graphics2DHistogramItem
 *\fn Graphics2DHistogramItem::setData(double *absciss, double *ordinate, int length)
 *Draw hisogramm in next manner:
 *ordinate(i) correspond interval absciss(i+1) - absciss(i)
 */
class GraphicsDataItemPrivate;
class Graphics2DHistogramItemPrivate;
class Graphics2DGraphItemPrivate;
class SRCSHARED_EXPORT GraphicsDataItem: public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsDataItem(QGraphicsItem *parent =0);
    ~GraphicsDataItem();

    void setPen(const QPen& pen);
    QPen pen();

    void setBrush(const QBrush & brush);
    QBrush brush();

    void ordinateRange(double *min, double *max);
    void abscissRange(double *min, double *max);

    void setTitle(const QString & title);
    QString title();

    inline int type() const {return GraphicsPlot::DataType;}
Q_SIGNALS:
    void dataItemChange();
    void penItemChange();
    void titleChange();
protected:
    void setOrdinateRange(double min, double max);
    void setAbscissRange(double min, double max);
private:
    Q_DECLARE_PRIVATE(GraphicsDataItem)
    GraphicsDataItemPrivate *d_ptr;
};


class SRCSHARED_EXPORT Graphics2DGraphItem: public GraphicsDataItem
{
    Q_OBJECT
public:
    Graphics2DGraphItem(QGraphicsItem *parent =0);
    Graphics2DGraphItem(double *absciss, double *ordinate, int length, QGraphicsItem *parent =0);
    ~Graphics2DGraphItem();

    void setData(double *absciss, double *ordinate, int length);
    void setData(QList<double> absciss, QList<double> ordinate);
    void setData(QVector<double> absciss, QVector<double> ordinate);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    Q_DECLARE_PRIVATE(Graphics2DGraphItem)
    Graphics2DGraphItemPrivate *d_ptr;
};

class SRCSHARED_EXPORT Graphics2DHistogramItem: public GraphicsDataItem
{
    Q_OBJECT
public:
    Graphics2DHistogramItem(QGraphicsItem *parent =0);
    Graphics2DHistogramItem(float *absciss, float *ordinate, int length, QGraphicsItem *parent =0);
    ~Graphics2DHistogramItem();

    void setData(float *absciss, float *ordinate, int length);
    void setData(QList<float> absciss, QList<float> ordinate);
    void setData(QVector<float> absciss, QVector<float> ordinate);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    Q_DECLARE_PRIVATE(Graphics2DHistogramItem)
    Graphics2DHistogramItemPrivate *d_ptr;

};
