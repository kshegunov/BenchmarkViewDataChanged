#ifndef BENCHMARKMODEL_H
#define BENCHMARKMODEL_H
#include <QStandardItemModel>
class BenchmarkModel : public QStandardItemModel{
    Q_OBJECT
    Q_DISABLE_COPY(BenchmarkModel)
public:
    explicit BenchmarkModel(QObject* parent = Q_NULLPTR) : QStandardItemModel(parent){}
public Q_SLOTS:
    void dataChangeAll(){dataChanged(index(0,0),index(rowCount()-1,columnCount()-1),{Qt::EditRole, Qt::DisplayRole});}
    void dataChangeTop4(){dataChanged(index(0,0),index(1,1),{Qt::EditRole, Qt::DisplayRole});}
};
#endif // BENCHMARKMODEL_H
