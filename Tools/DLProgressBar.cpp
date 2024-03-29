#include <QApplication>

#include "DLProgressBar.h"

DLProgressBar::DLProgressBar(QObject* parent)
             : QStyledItemDelegate(parent)
{
}

void DLProgressBar::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex& index ) const
{
    // Set up a QStyleOptionProgressBar to precisely mimic the
    // environment of a progress bar.
    QStyleOptionProgressBar progressBarOption;
    progressBarOption.state = QStyle::State_Enabled;
    progressBarOption.direction = QApplication::layoutDirection();
    progressBarOption.rect = option.rect;
    progressBarOption.fontMetrics = QApplication::fontMetrics();
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.textAlignment = Qt::AlignCenter;
    progressBarOption.textVisible = true;

    // Set the progress and text values of the style option.
    int progress = index.data(Qt::UserRole).toInt();
    progressBarOption.progress = progress;
    progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

    // Draw the progress bar onto the view.
    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}
