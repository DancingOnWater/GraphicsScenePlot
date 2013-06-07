#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <QGraphicsView>

#include <GraphicsPlotItem.h>
#include <GraphicsDataItem.h>
#include <GraphicsPlotLegend.h>

#define COMMONPART QGraphicsView graphicsView; \
QGraphicsScene * scene = new QGraphicsScene(); \
graphicsView.setScene(scene); \
    graphicsView.setGeometry(0,0, 520, 520); \
graphicsView.show(); \
GraphicsPlotItem *plot = new GraphicsPlotItem(); \
    scene->addItem(plot); \
plot->setRect(QRect(0, 0, 500, 500)); \
plot->setTitle(QString("Test title")); \
    plot->setAxisText(0, QString("x")); \
    plot->setAxisText(1, QString("y")); \
    plot->setAbscissaRange(-10, 10); \
    plot->setOrdinateRange(-20, 20); \
scene->setSceneRect(plot->boundingRect());

class GraphicsPlotItemTest : public QObject
{
    Q_OBJECT
    
public:
    GraphicsPlotItemTest();
private Q_SLOTS:
    void testCase1();
    void simpleTestGrid();
    void simple2DGraphTest();
    void simple2Histogrammtest();
    void simpleLegendTest();
};

GraphicsPlotItemTest::GraphicsPlotItemTest()
{
}

void GraphicsPlotItemTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

void GraphicsPlotItemTest::simpleTestGrid()
{
    COMMONPART
    scene->addRect(plot->boundingRect());
    qApp->exec();
}

void GraphicsPlotItemTest::simple2DGraphTest()
{
    COMMONPART

    QVector<double> absciss; QVector<double> ordinate;
    for(int i = 0; i<=10; ++i){
        absciss.append(-20+2*i);
        ordinate.append(-30+2*i);
    }

    Graphics2DGraphItem *dataItem = new Graphics2DGraphItem();
    dataItem->setPen(QColor(Qt::blue));
    dataItem->setData(absciss, ordinate);

    plot->addDataItem(dataItem);
    qApp->exec();
}

void GraphicsPlotItemTest::simple2Histogrammtest()
{
    COMMONPART
    QVector<float> absciss; QVector<float> ordinate;
    for(int i = 0; i<=10; ++i){
        absciss.append(-20+2*i);
        ordinate.append(-30+2*i);
    }

    Graphics2DHistogramItem *dataItem = new Graphics2DHistogramItem();
    dataItem->setBrush(QColor(Qt::red));
    dataItem->setData(absciss, ordinate);

    plot->addDataItem(dataItem);
    qApp->exec();
}

void GraphicsPlotItemTest::simpleLegendTest()
{
    COMMONPART

    QVector<double> absciss; QVector<double> ordinate;
    for(int i = 0; i<=10; ++i){
        absciss.append(-20+2*i);
        ordinate.append(-30+2*i);
    }

    Graphics2DGraphItem *dataItem = new Graphics2DGraphItem();
    dataItem->setPen(QColor(Qt::blue));
    dataItem->setData(absciss, ordinate);

    plot->addDataItem(dataItem);

    plot->setLegend(new GraphicsPlotLegend(QRectF(0, 0, 100, 100)));
    plot->legend()->setPos(400, 370);
    dataItem->setPen(QColor(Qt::red));

    qApp->exec();
}

QTEST_MAIN(GraphicsPlotItemTest)

#include "tst_GraphicsPlotItemTest.moc"
