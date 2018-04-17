#ifndef BENCHMARKMODEL_H
#define BENCHMARKMODEL_H

#include <QStandardItemModel>

class BenchmarkModel : public QStandardItemModel
{
    Q_OBJECT
    Q_DISABLE_COPY(BenchmarkModel)

public:
    explicit BenchmarkModel(QObject * parent = nullptr);

public slots:
    void dataChangeAll();
    void dataChangeTop4();
    void dataCgangedCustom(int row, int col);
};
#endif // BENCHMARKMODEL_H
