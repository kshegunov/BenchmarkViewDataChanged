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
    void dataChangeTop(int, int);
    void dataChangeSpan(int, int, int, int);

private:
    int rand(int);
    int rand(int, int);
};
#endif // BENCHMARKMODEL_H
