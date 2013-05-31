#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <QGraphicsView>

#include <GraphicsPlotItem.h>
class GraphicsPlotItemTest : public QObject
{
    Q_OBJECT
    
public:
    GraphicsPlotItemTest();
    
private Q_SLOTS:
    void testCase1();
    void simpleTestGrid();
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
        graphicsView.setGeometry(0,0, 300, 300);
    graphicsView.show();

    GraphicsPlotItem *plot = new GraphicsPlotItem();
        scene->addItem(plot);
    plot->setRect(QRect(0, 0, 200, 200));
    plot->setTitle(QString("Test title"));
        plot->setAxisText(0, QString("x"));
        plot->setAxisText(1, QString("y"));
        plot->setAbscissaRange(-10, 10);
        plot->setOrdinateRange(-20, 20);
    scene->addRect(plot->boundingRect());
    qApp->exec();
}

QTEST_MAIN(GraphicsPlotItemTest)

#include "tst_GraphicsPlotItemTest.moc"
