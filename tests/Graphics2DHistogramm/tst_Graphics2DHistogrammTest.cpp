#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <GraphicsDataItem.h>

class Graphics2DHistogrammTest : public QObject
{
    Q_OBJECT
    
public:
    Graphics2DHistogrammTest();
    
private Q_SLOTS:
    void simpleTest();
};

Graphics2DHistogrammTest::Graphics2DHistogrammTest()
{
}

void Graphics2DHistogrammTest::simpleTest()
{
    QGraphicsView graphicsView;
    QGraphicsScene * scene = new QGraphicsScene();
    graphicsView.setScene(scene);
        graphicsView.setGeometry(0,0, 520, 520);
    graphicsView.show();

    Graphics2DHistogramItem *histogramm = new Graphics2DHistogramItem();
    histogramm->setBrush(QBrush(Qt::red));
    QList<float> absciss, ordinate;
    for(int i = 0; i<=10; ++i){
        absciss.append(-20+2*i);
        ordinate.append(-30+2*i);
    }

    histogramm->setData(absciss, ordinate);

    scene->addItem(histogramm);
    histogramm->setScale(4);
    qApp->exec();
}

QTEST_MAIN(Graphics2DHistogrammTest)

#include "tst_Graphics2DHistogrammTest.moc"
