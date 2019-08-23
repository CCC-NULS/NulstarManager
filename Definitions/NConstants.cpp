#include "NConstants.h"

QString NConstants::colorToRgbText(const QColor& color)
{
  QString blue = QString::number(color.blue());
  QString green = QString::number(color.green());
  QString red = QString::number(color.red());

  QString text = QString("(%1, %2, %3)").arg(red).arg(green).arg(blue);
  return text;
}
