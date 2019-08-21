#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QTextOption>

#include "DLCheckBox.h"

DLCheckBox::DLCheckBox(QObject* parent)
          : QStyledItemDelegate(parent)
{
}

void DLCheckBox::paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex& index ) const
{
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  if(option.state & QStyle::State_Selected)
    painter->fillRect(option.rect, option.palette.highlight());
  bool state = index.data(Qt::CheckStateRole).toBool();

  QPixmap cbIcon;
  if(state)
   cbIcon.load(":/Resources/Images/Tick.png");
  else
   cbIcon.load(":/Resources/Images/Error.png");

  QVariant iconVar = index.data(Qt::DecorationRole);
  QPixmap icon = iconVar.value<QPixmap>();
  QString text = index.data(Qt::DisplayRole).toString();
  QVariant brushVar = index.data(Qt::ForegroundRole);
  QBrush brush = brushVar.value<QBrush>();
  QColor color = brush.color();

  int iconX = 0;
  QRect rect(option.rect.x(), option.rect.y(), option.rect.height(), option.rect.height());
  if(iconVar.isValid())
    iconX = rect.width();
  QRect iconRect(rect.x() + iconX + 5,rect.y(),iconX,rect.height());

  painter->drawPixmap(rect,cbIcon);
  painter->setPen(option.state & QStyle::State_Selected ? option.palette.highlightedText().color() : color);
  if(iconX)
  {
    painter->drawPixmap(iconRect,icon);
    painter->drawText(option.rect.adjusted(option.rect.height() + iconRect.width() + 10,0,0,0), text, QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
  }
  else
    painter->drawText(option.rect.adjusted(option.rect.height() + iconRect.width() + 5,0,0,0), text, QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
  painter->restore();
}
