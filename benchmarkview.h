#ifndef BENCHMARKVIEW_H
#define BENCHMARKVIEW_H

#include <QTableView>

class BenchmarkViewPrivate;
class BenchmarkView : public QTableView
{
    Q_OBJECT
    Q_DISABLE_COPY(BenchmarkView)
    Q_DECLARE_PRIVATE(BenchmarkView)

public:
    explicit BenchmarkView(QWidget * parent = nullptr);

signals:
    void dataChangedElapsed(qint64 mSec);

public slots:
    void clearTimer();

protected:
    virtual void doUpdate(const QModelIndex &, const QModelIndex &, const QVector<int> &);

protected slots:
    void dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &) override;
};

class BenchmarkViewJoinRects : public BenchmarkView
{
    Q_OBJECT
    Q_DISABLE_COPY(BenchmarkViewJoinRects)
    Q_DECLARE_PRIVATE(BenchmarkView)

public:
    using BenchmarkView::BenchmarkView;

protected:
    void doUpdate(const QModelIndex &, const QModelIndex &, const QVector<int> &) override;
};

class BenchmarkViewUpdateEach : public BenchmarkView
{
    Q_OBJECT
    Q_DISABLE_COPY(BenchmarkViewUpdateEach)
    Q_DECLARE_PRIVATE(BenchmarkView)

public:
    using BenchmarkView::BenchmarkView;

protected:
    void doUpdate(const QModelIndex &, const QModelIndex &, const QVector<int> &) override;
};

class BenchmarkViewUpdateHeuristic : public BenchmarkView
{
    Q_OBJECT
    Q_DISABLE_COPY(BenchmarkViewUpdateEach)
    Q_DECLARE_PRIVATE(BenchmarkView)

public:
    using BenchmarkView::BenchmarkView;

protected:
    void doUpdate(const QModelIndex &, const QModelIndex &, const QVector<int> &) override;
};

class BenchmarkViewMashup : public BenchmarkView
{
    Q_OBJECT
    Q_DISABLE_COPY(BenchmarkViewUpdateEach)
    Q_DECLARE_PRIVATE(BenchmarkView)

public:
    using BenchmarkView::BenchmarkView;

protected:
    void doUpdate(const QModelIndex &, const QModelIndex &, const QVector<int> &) override;
};

#endif // BENCHMARKVIEW_H
