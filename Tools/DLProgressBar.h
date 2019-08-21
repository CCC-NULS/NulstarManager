#ifndef DLPROGRESSBAR_H
#define DLPROGRESSBAR_H

#include <QModelIndex>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

#include "Tools.h"

class DLProgressBar : public QStyledItemDelegate
{
  Q_OBJECT

  public:
    DLProgressBar(QObject* parent);

    void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex& index ) const;
};




#endif // DLPROGRESSBAR_H
