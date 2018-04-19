#include "benchmarkmodel.h"

BenchmarkModel::BenchmarkModel(QObject * parent)
    : QStandardItemModel(parent)
{
}

void BenchmarkModel::dataChangeAll()
{
    dataChangeSpan(0, 0, rowCount() - 1, columnCount() - 1);
}

void BenchmarkModel::dataChangeTop(int rows, int cols)
{
    dataChangeSpan(0, 0, rows, cols);
}

void BenchmarkModel::dataChangeSpan(int topX, int topY, int bottomX, int bottomY)
{
    dataChanged(index(topX, topY), index(bottomX, bottomY), {Qt::EditRole, Qt::DisplayRole});
}

inline int BenchmarkModel::rand(int max)
{
    return rand(0, max);
}

inline int BenchmarkModel::rand(int min, int max)
{
    return min + static_cast<qreal>(qrand()) / RAND_MAX * (max - min);
}

/*    const int rows = rowCount(), cols = columnCount(), x = rand(rows), y = rand(cols);
//    dataChanged(index(x, y), index(rand(x, rows), rand(y, cols)), {Qt::EditRole, Qt::DisplayRole});*/

