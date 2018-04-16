#include "benchmarkview.h"
#include <QHeaderView>
#include <QAccessible>
#include <private/qabstractitemview_p.h>
#include <private/qtableview_p.h>
#include <QElapsedTimer>
#include <QApplication>
QTableViewPrivate *BenchMarkView::d_func() { return reinterpret_cast<QTableViewPrivate *>(qGetPtrHelper(d_ptr)); }

const QTableViewPrivate *BenchMarkView::d_func() const { return reinterpret_cast<const QTableViewPrivate *>(qGetPtrHelper(d_ptr)); }

BenchMarkView::BenchMarkView(BenchMarkType typ, QWidget *parent):QTableView(parent),m_tpy(typ){}

void BenchMarkView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    QElapsedTimer elapsedCalc;
    if(m_tpy==NoChange){
        elapsedCalc.start();
        QTableView::dataChanged(topLeft,bottomRight,roles);
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        dataChangedElapsed(elapsedCalc.elapsed());
        return;
    }
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
            if(m_tpy==JoinRects){
                elapsedCalc.start();
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
                        const int maxColumn = bottomRight.column() - ((i==maxRow-1) ? 1:0);
                        for (int j = topLeft.column()+ ((i==topLeft.row()) ? 1:0); j < maxColumn; ++j)  {
                            dirty |= visualRect(d->model->index(i, j, parent));
                        }
                    }
                    dirty &= viewportRect;
                    if (!dirty.isEmpty())
                        viewport()->update(dirty);
                }
                QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
                dataChangedElapsed(elapsedCalc.elapsed());
            }
            else if(m_tpy==UpdateOnEach){
                elapsedCalc.start();
                const int maxRow = bottomRight.row();
                const QModelIndex parent = topLeft.parent();
                for (int i = topLeft.row(); i < maxRow; ++i)  {
                    const int maxColumn = bottomRight.column() - ((i==maxRow-1) ? 1:0);
                    for (int j = topLeft.column()+ ((i==topLeft.row()) ? 1:0); j < maxColumn; ++j)  {
                        viewport()->update(visualRect(model()->index(i, j, parent)));
                    }
                }
                QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
                dataChangedElapsed(elapsedCalc.elapsed());
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
