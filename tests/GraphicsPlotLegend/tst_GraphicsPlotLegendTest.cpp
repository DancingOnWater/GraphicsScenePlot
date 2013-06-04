#include <QString>
#include <QtTest>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <GraphicsPlotLegend.h>
class GraphicsPlotLegendTest : public QObject
{
    Q_OBJECT
    
public:
    GraphicsPlotLegendTest();
    
private Q_SLOTS:
    void baseTest();
};

GraphicsPlotLegendTest::GraphicsPlotLegendTest()
{

}

void GraphicsPlotLegendTest::baseTest()
{
    QGraphicsView view;
    view.setMinimumSize(300, 300);
    QGraphicsScene * scene = new QGraphicsScene();
    view.setScene(scene);
    GraphicsPlotLegend * legend = new GraphicsPlotLegend();
    scene->addItem(legend);
    legend->setRect(QRectF(0, 0, 250, 250));

    Graphics2DGraphItem dataItem;

    dataItem.setTitle(QString("Test title"));
        dataItem.setPen(QColor(Qt::blue));
    legend->addDataItem(&dataItem);
    view.show();
    dataItem.setPen(QColor(Qt::red));


    qApp->exec();
}

QTEST_MAIN(GraphicsPlotLegendTest)

#include "tst_GraphicsPlotLegendTest.moc"
