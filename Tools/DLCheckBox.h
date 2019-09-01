#ifndef DLCHECKBOX_H
#define DLCHECKBOX_H

#include <QModelIndex>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

#include "Tools.h"

class TOOLSSHARED_EXPORT DLCheckBox : public QStyledItemDelegate
{
  Q_OBJECT

  public:
    DLCheckBox(QObject* parent);

    void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex& index ) const;
};




#endif // DLCHECKBOX_H
