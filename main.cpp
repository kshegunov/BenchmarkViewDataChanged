#include "benchmarkview.h"
#include "benchmarkmodel.h"

#include "ui_benchmarkview.h"

#include <QApplication>
#include <QWidget>
#include <functional>
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
    label->setText(QStringLiteral("dataChanged took %1 msec").arg(mseconds));
}

int main(int argc, char *argv[])
{
    qsrand(88);

    QApplication app(argc, argv);
    QWidget window;

    Q_UNUSED(app);

    BenchmarkModel model;
    fillModel(model);

    Ui::Window ui;
    ui.setupUi(&window);

    ui.defaultView->setModel(&model);
    ui.rectJoinView->setModel(&model);
    ui.updateCompressionView->setModel(&model);

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

    window.show();
    return QApplication::exec();
}
