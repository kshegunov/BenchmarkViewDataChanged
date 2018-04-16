#include "benchmarkview.h"
#include "benchmarkmodel.h"
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
int main(int argc, char *argv[])
{
    qsrand(88);
    QApplication a(argc, argv);
    QWidget w;
    auto baseModel = new BenchmarkModel(&w);
    QPixmap bluePix(20,20);
    bluePix.fill(Qt::blue);
    baseModel->insertRows(0,1000);
    baseModel->insertColumns(0,100);
    for(int i=0;i<baseModel->rowCount();++i){
        for(int j=0;j<baseModel->columnCount();++j){
            baseModel->setData(baseModel->index(i,j),QString::number(i)+";"+QString::number(j));
            if(qrand()%2==0)
                baseModel->setData(baseModel->index(i,j),bluePix,Qt::DecorationRole);
        }

    }
    auto changeAllButton=new QPushButton(QStringLiteral("Data Change All"),&w);
    auto change4Button=new QPushButton(QStringLiteral("Data Change 4 Items"),&w);
    auto buttonLay= new QHBoxLayout;
    buttonLay->addWidget(changeAllButton);
    buttonLay->addWidget(change4Button);
    buttonLay->addStretch();
    QObject::connect(changeAllButton,&QPushButton::clicked,baseModel,&BenchmarkModel::dataChangeAll);
    QObject::connect(change4Button,&QPushButton::clicked,baseModel,&BenchmarkModel::dataChangeTop4);
    auto viewsLay= new QGridLayout;
    auto noChangeView = new BenchMarkView(BenchMarkView::NoChange,&w);
    auto joinRectsView = new BenchMarkView(BenchMarkView::JoinRects,&w);
    auto updateOnEachView = new BenchMarkView(BenchMarkView::UpdateOnEach,&w);
    auto noChangeLabel = new QLabel(&w);
    auto joinRectsLabel = new QLabel(&w);
    auto updateOnEachLabel = new QLabel(&w);
    noChangeView->setModel(baseModel);
    joinRectsView->setModel(baseModel);
    updateOnEachView->setModel(baseModel);
    QObject::connect(noChangeView,&BenchMarkView::dataChangedElapsed,noChangeLabel,[noChangeLabel](qint64 mSec){noChangeLabel->setText(QStringLiteral("dataChanged took %1msec").arg(mSec));});
    QObject::connect(joinRectsView,&BenchMarkView::dataChangedElapsed,noChangeLabel,[joinRectsLabel](qint64 mSec){joinRectsLabel->setText(QStringLiteral("dataChanged took %1msec").arg(mSec));});
    QObject::connect(updateOnEachView,&BenchMarkView::dataChangedElapsed,noChangeLabel,[updateOnEachLabel](qint64 mSec){updateOnEachLabel->setText(QStringLiteral("dataChanged took %1msec").arg(mSec));});
    viewsLay->addWidget(noChangeView,0,0);
    viewsLay->addWidget(joinRectsView,0,1);
    viewsLay->addWidget(updateOnEachView,0,2);
    viewsLay->addWidget(noChangeLabel,1,0);
    viewsLay->addWidget(joinRectsLabel,1,1);
    viewsLay->addWidget(updateOnEachLabel,1,2);
    auto mainLay = new QVBoxLayout(&w);
    mainLay->addLayout(buttonLay);
    mainLay->addLayout(viewsLay);
    w.show();

    return a.exec();
}
