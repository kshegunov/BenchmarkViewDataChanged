#include "benchmarkview.h"
#include <QHeaderView>
#include <QAccessible>
//#include <private/qabstractitemview_p.h>
#include <QElapsedTimer>
#include <QApplication>

#include <private/qtableview_p.h>

class BenchmarkViewPrivate : public QTableViewPrivate
{
    Q_DECLARE_PUBLIC(BenchmarkView)

    inline BenchmarkViewPrivate(BenchmarkView * q)
        : QTableViewPrivate(), elapsed(0)
    {
        q_ptr = q;  // It is defined in QObjectData
    }

    QElapsedTimer timer;
    qint64 elapsed;
};

BenchmarkView::BenchmarkView(QWidget * parent)
    : QTableView(*new BenchmarkViewPrivate(this), parent)
{
}

void BenchmarkView::clearTimer()
{
    Q_D(BenchmarkView);
    d->elapsed = 0;
}

void BenchmarkView::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles)
{
    Q_D(BenchmarkView);
    d->timer.start();
    doUpdate(topLeft, bottomRight, roles);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    d->elapsed += d->timer.elapsed();

    dataChangedElapsed(d->elapsed);
}

void BenchmarkView::doUpdate(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles)
{
    QTableView::dataChanged(topLeft, bottomRight, roles);
}

void BenchmarkViewJoinRects::doUpdate(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles)
{
    Q_UNUSED(roles);

    // Single item changed
    Q_D(QTableView);
    if (topLeft == bottomRight && topLeft.isValid()) {
        const QEditorInfo &editorInfo = d->editorForIndex(topLeft);
        //we don't update the edit data if it is static
        if (!editorInfo.isStatic && editorInfo.widget) {
            QAbstractItemDelegate *delegate = d->delegateForIndex(topLeft);
            if (delegate) {
                delegate->setEditorData(editorInfo.widget.data(), topLeft);
            }
        }
        if (isVisible() && !d->delayedPendingLayout) {
            // otherwise the items will be update later anyway
            update(topLeft);
        }
    } else {
        d->updateEditorData(topLeft, bottomRight);
        if (isVisible() && !d->delayedPendingLayout)  {
            const QRect viewportRect = viewport()->rect();
            const QModelIndex parent = topLeft.parent();
            const QRect cornerRect = visualRect(topLeft) | visualRect(bottomRight);
            // If dirty is 1/4th or more of the viewport rect, just trigger a full update
            const QRect cornerRectViewPort = cornerRect & viewportRect;
            if(cornerRectViewPort.width() * cornerRectViewPort.height() * 4 > viewportRect.width() * viewportRect.height())  {
                viewport()->update();
            }
            else  { // Just fall back to iterating over the model indices
                QRect dirty(cornerRect);
                const int maxRow = bottomRight.row();
                for (int i = topLeft.row(); i < maxRow; ++i)  {
                    const int maxColumn = bottomRight.column() - ((i == maxRow -1 ) ? 1 : 0);
                    for (int j = topLeft.column() + ((i == topLeft.row()) ? 1 : 0); j < maxColumn; ++j)  {
                        dirty |= visualRect(d->model->index(i, j, parent));
                    }
                }
                dirty &= viewportRect;
                if (!dirty.isEmpty())
                    viewport()->update(dirty);
            }
        }
    }
#ifndef QT_NO_ACCESSIBILITY
    if (QAccessible::isActive()) {
        QAccessibleTableModelChangeEvent accessibleEvent(this, QAccessibleTableModelChangeEvent::DataChanged);
        accessibleEvent.setFirstRow(topLeft.row());
        accessibleEvent.setFirstColumn(topLeft.column());
        accessibleEvent.setLastRow(bottomRight.row());
        accessibleEvent.setLastColumn(bottomRight.column());
        QAccessible::updateAccessibility(&accessibleEvent);
    }
#endif
    d->updateGeometry();
}

void BenchmarkViewUpdateEach::doUpdate(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles)
{
    Q_UNUSED(roles);

    // Single item changed
    Q_D(QTableView);
    if (topLeft == bottomRight && topLeft.isValid()) {
        const QEditorInfo &editorInfo = d->editorForIndex(topLeft);
        //we don't update the edit data if it is static
        if (!editorInfo.isStatic && editorInfo.widget) {
            QAbstractItemDelegate *delegate = d->delegateForIndex(topLeft);
            if (delegate) {
                delegate->setEditorData(editorInfo.widget.data(), topLeft);
            }
        }
        if (isVisible() && !d->delayedPendingLayout) {
            // otherwise the items will be update later anyway
            update(topLeft);
        }
    } else {
        d->updateEditorData(topLeft, bottomRight);
        if (isVisible() && !d->delayedPendingLayout)  {
            const QModelIndex parent = topLeft.parent();
            for (int i = topLeft.row(), maxRow = bottomRight.row(); i < maxRow; ++i)  {
                for (int j = topLeft.column(), maxColumn = bottomRight.column(); j < maxColumn; ++j)  {
                    viewport()->update(visualRect(model()->index(i, j, parent)));
                }
            }
        }
    }
#ifndef QT_NO_ACCESSIBILITY
    if (QAccessible::isActive()) {
        QAccessibleTableModelChangeEvent accessibleEvent(this, QAccessibleTableModelChangeEvent::DataChanged);
        accessibleEvent.setFirstRow(topLeft.row());
        accessibleEvent.setFirstColumn(topLeft.column());
        accessibleEvent.setLastRow(bottomRight.row());
        accessibleEvent.setLastColumn(bottomRight.column());
        QAccessible::updateAccessibility(&accessibleEvent);
    }
#endif
    d->updateGeometry();
}
