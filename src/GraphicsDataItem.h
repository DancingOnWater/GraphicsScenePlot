#pragma once
#include <QGraphicsItem>

class GraphicsDataItemPrivate;
class Graphics2DHistogramItemPrivate;
class Graphics2DGraphItemPrivate;
class GraphicsDataItem: public QGraphicsItem
{
public:
    GraphicsDataItem(QGraphicsItem *parent =0);
    ~GraphicsDataItem();

    void setPen(const QPen& pen);
    QPen pen();

    void setBrush(const QBrush & brush);
    QBrush brush();

    void ordinateRange(double *min, double *max);
    void abscissRange(double *min, double *max);
protected:
    void setOrdinateRange(double min, double max);
    void setAbscissRange(double min, double max);
private:
    Q_DECLARE_PRIVATE(GraphicsDataItem)
    GraphicsDataItemPrivate *d_ptr;
};


class Graphics2DGraphItem: public GraphicsDataItem
{
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

class Graphics2DHistogramItem: public GraphicsDataItem
{
public:
    Graphics2DHistogramItem(QGraphicsItem *parent =0);
    Graphics2DHistogramItem(double *absciss, double ordinate, int length, QGraphicsItem *parent =0);

    void setData(double *absciss, double ordinate, int length);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};
