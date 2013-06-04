#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <QGraphicsView>

#include <GraphicsPlotItem.h>
#include <GraphicsDataItem.h>
class GraphicsPlotItemTest : public QObject
{
    Q_OBJECT
    
public:
    GraphicsPlotItemTest();
private:
    void drawItem(GraphicsDataItem *dataItem)
    {
        QGraphicsView graphicsView;
        QGraphicsScene * scene = new QGraphicsScene();
        graphicsView.setScene(scene);
            graphicsView.setGeometry(0,0, 520, 520);
        graphicsView.show();

        GraphicsPlotItem *plot = new GraphicsPlotItem();
            scene->addItem(plot);
        plot->setRect(QRect(0, 0, 500, 500));
        plot->setTitle(QString("Test title"));
            plot->setAxisText(0, QString("x"));
            plot->setAxisText(1, QString("y"));
            plot->setAbscissaRange(-10, 10);
            plot->setOrdinateRange(-20, 20);
        scene->setSceneRect(plot->boundingRect());
        plot->addDataItem(dataItem);
        qApp->exec();
    }

private Q_SLOTS:
    void testCase1();
    void simpleTestGrid();
    void simple2DGraphTest();
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
    QGraphicsView graphicsView;
    QGraphicsScene * scene = new QGraphicsScene();
    graphicsView.setScene(scene);
        graphicsView.setGeometry(0,0, 520, 520);
    graphicsView.show();

    GraphicsPlotItem *plot = new GraphicsPlotItem();
        scene->addItem(plot);
    plot->setRect(QRect(0, 0, 500, 500));
    plot->setTitle(QString("Test title"));
        plot->setAxisText(0, QString("x"));
        plot->setAxisText(1, QString("y"));
        plot->setAbscissaRange(-10, 10);
        plot->setOrdinateRange(-20, 20);
    scene->addRect(plot->boundingRect());
    qApp->exec();
}

void GraphicsPlotItemTest::simple2DGraphTest()
{
    QVector<double> absciss; QVector<double> ordinate;
    for(int i = 0; i<=10; ++i){
        absciss.append(-20+2*i);
        ordinate.append(-30+2*i);
    }

    Graphics2DGraphItem *dataItem = new Graphics2DGraphItem();
    dataItem->setPen(QColor(Qt::blue));
    dataItem->setData(absciss, ordinate);
    drawItem(dataItem);
}

QTEST_MAIN(GraphicsPlotItemTest)

#include "tst_GraphicsPlotItemTest.moc"
