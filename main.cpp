#include "benchmarkview.h"
#include "benchmarkmodel.h"

#include "ui_benchmarkview.h"

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>
#include <QElapsedTimer>

#include <functional>

void fillModel(QAbstractItemModel & model);
void updateIndicator(qint64 mseconds, QLabel * label);

void testUpdateFull(BenchmarkModel *, QLabel *);
void testUpdateTop(BenchmarkModel *, QLabel *, int, int);
void testUpdateRandom(BenchmarkModel *, QLabel *);

void testView(BenchmarkModel *, QLabel *, QLabel *, QLabel *, int, int);
void testAll(Ui::Window *);

static int iterations = 1000;

int main(int argc, char *argv[])
{
    qsrand(88);

    QApplication app(argc, argv);
    Q_UNUSED(app);

    QWidget window;
    Ui::Window ui;
    ui.setupUi(&window);
    window.show();

    QObject::connect(ui.startButton, &QPushButton::clicked, std::bind(&testAll, &ui));
/*



    ui.defaultView->setModel(&model);
    ui.rectJoinView->setModel(&model);
    ui.updateCompressionView->setModel(&model);
    ui.updateHeuristicView->setModel(&model);
    ui.mashupView->setModel(&model);

    for(QPushButton* singleButton : {ui.update4ItemsButton,ui.updateAllButton,ui.customUpdateButton}){
        QObject::connect(singleButton, &QPushButton::clicked, ui.defaultView, &BenchmarkView::clearTimer);
        QObject::connect(singleButton, &QPushButton::clicked, ui.rectJoinView, &BenchmarkView::clearTimer);
        QObject::connect(singleButton, &QPushButton::clicked, ui.updateCompressionView, &BenchmarkView::clearTimer);
    }


    QObject::connect(ui.update4ItemsButton, &QPushButton::clicked, &model, &BenchmarkModel::dataChangeTop4);
    QObject::connect(ui.updateAllButton, &QPushButton::clicked, &model, &BenchmarkModel::dataChangeAll);
    QObject::connect(ui.customUpdateButton, &QPushButton::clicked, &model,[&ui,&model](){model.dataCgangedCustom(ui.rowSpin->value(),ui.colSpin->value());});

    QObject::connect(ui.defaultView, &BenchmarkView::dataChangedElapsed, ui.defaultElapsed, std::bind(&updateIndicator, std::placeholders::_1, ui.defaultElapsed));
    QObject::connect(ui.rectJoinView, &BenchmarkView::dataChangedElapsed, ui.rectJoinElapsed, std::bind(&updateIndicator, std::placeholders::_1, ui.rectJoinElapsed));
    QObject::connect(ui.updateCompressionView, &BenchmarkView::dataChangedElapsed, ui.updateCompressionElapsed, std::bind(&updateIndicator, std::placeholders::_1, ui.updateCompressionElapsed));
    QObject::connect(ui.updateHeuristicView, &BenchmarkView::dataChangedElapsed, ui.updateHeuristicElapsed, std::bind(&updateIndicator, std::placeholders::_1, ui.updateHeuristicElapsed));
    QObject::connect(ui.mashupView, &BenchmarkView::dataChangedElapsed, ui.mashupElapsed, std::bind(&updateIndicator, std::placeholders::_1, ui.mashupElapsed));

*/
    return QApplication::exec();
}


void fillModel(QAbstractItemModel & model)
{
    QPixmap bluePix(20, 20);
    bluePix.fill(Qt::blue);

    const int rows = 1000;
    const int cols = 100;

    model.insertRows(0, rows);
    model.insertColumns(0, cols);
    for (int i = 0; i < rows; ++i)  {
        for (int j = 0; j < cols; ++j)  {
            model.setData(model.index(i, j), QString::number(i) + ";" + QString::number(j));
            if (qrand() & 1)
                model.setData(model.index(i, j), bluePix, Qt::DecorationRole);
        }
    }
}

void updateIndicator(qint64 mseconds, QLabel * label)
{
    label->setText(QStringLiteral("%1 ms").arg(mseconds));
}

void testUpdateFull(BenchmarkModel * model, QLabel * label)
{
    QElapsedTimer timer;
    timer.start();
    for (int i = 0; i < iterations; i++)
        model->dataChangeAll();
    updateIndicator(timer.elapsed(), label);
}

void testUpdateTop(BenchmarkModel * model, QLabel * label, int rows, int cols)
{
    testUpdateFull(model, label);
}

void testUpdateRandom(BenchmarkModel * model, QLabel * label)
{
    testUpdateFull(model, label);
}

void testAll(Ui::Window * ui)
{
    QSize size = QApplication::desktop()->size() * 0.9;

    BenchmarkModel model;
    fillModel(model);

    // Create and test each view
    {
        BenchmarkView view;
        view.setModel(&model);
        view.move(0, 0);
        view.resize(size);
        view.show();

        QCoreApplication::processEvents();

        testView(&model, ui->resultDefaultFull, ui->resultDefaultTop, ui->resultDefaultRandom, ui->topRowsSpinbox->value(), ui->topColsSpinbox->value());
    }
}

void testView(BenchmarkModel * model, QLabel * resultFull, QLabel * resultTop, QLabel * resultRandom, int topRows, int topCols)
{
    testUpdateFull(model, resultFull);
    testUpdateTop(model, resultTop, topRows, topCols);
    testUpdateRandom(model, resultRandom);
}
