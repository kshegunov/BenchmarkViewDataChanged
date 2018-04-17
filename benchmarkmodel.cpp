#include "benchmarkmodel.h"

static const int iterations = 1000;

BenchmarkModel::BenchmarkModel(QObject * parent)
    : QStandardItemModel(parent)
{
}

void BenchmarkModel::dataChangeAll()
{
    for (qint32 i = 0; i < iterations; i++)
        dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1), { Qt::EditRole, Qt::DisplayRole });
}

void BenchmarkModel::dataChangeTop4()
{
    for (qint32 i = 0; i < iterations; i++)
        dataChanged(index(0, 0), index(1, 1), {Qt::EditRole, Qt::DisplayRole});
}

void BenchmarkModel::dataCgangedCustom(int row, int col)
{
    for (qint32 i = 0; i < iterations; i++)
        dataChanged(index(0, 0), index(row, col), {Qt::EditRole, Qt::DisplayRole});
}
