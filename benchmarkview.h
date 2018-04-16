#ifndef BENCHMARKVIEW_H
#define BENCHMARKVIEW_H

#include <QTableView>
class BenchMarkView : public QTableView
{
    Q_OBJECT
    Q_DISABLE_COPY(BenchMarkView)
    QTableViewPrivate* d_func();
    const QTableViewPrivate* d_func() const;
public:
    enum BenchMarkType{
      NoChange
      , JoinRects
      , UpdateOnEach
    };
    explicit BenchMarkView(BenchMarkType typ,QWidget* parent = Q_NULLPTR);
Q_SIGNALS:
    void dataChangedElapsed(qint64 mSec);
protected Q_SLOTS:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
private:
    const BenchMarkType m_tpy;
};

#endif // BENCHMARKVIEW_H
