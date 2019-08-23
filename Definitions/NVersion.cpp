#include <QStringList>

#include "WCConstants.h"
#include "WCVersion.h"

const QString version = "1.0.0";
const QString alias = "Potato Killer";
const QString qtversion = "5";

WCVersion::WCVersion(QObject* parent)
         : QObject(parent)
{
}

QString WCVersion::fileRegion(const QString& fileName)
{
  QString region = fileName.section("__",1,1); // Para la region se usa __
  return region;
}

QString WCVersion::fileToObject(int os, const QString& fileName)
{
  QString file;
  if(os == WCConstants::Linux64)
  {
    file = fileName.section(".so",0,0).section("__",0,0); // Para la region se usa __
    if(file.startsWith("lib"))
      file.remove(0,3);
    if(file.endsWith(".zip"))
      file.remove(".zip");
    if(file.endsWith(".pdz"))
      file.remove(".pdz");
    if(file.endsWith(".pem"))
      file.remove(".pem");
    if(file.endsWith(".cfg"))
      file.remove(".cfg");

  }
  if(os == WCConstants::Windows32)
  {
    file = fileName.section(".",0,0).section("__",0,0); // Para la region se usa __
    if(file.endsWith(qtversion))
      file.chop(1);
  }
  if(os == WCConstants::Windows64)
  {
    file = fileName.section(".",0,0).section("__",0,0); // Para la region se usa __
    if(file.endsWith(qtversion))
      file.chop(1);
  }
  return file;
}

QString WCVersion::normalizeFile(const QString& fileName)
{
  QString file = fileName;
  if(fileName.contains("__"))
  {
    file = fileName.section("__",0,0);
    file.append(fileName.section("__",-1,-1));
  }
  return file;
}

QString WCVersion::worldcoinManagerVersion()
{
  return version;
}

QString WCVersion::worldcoinManagerVersionCompressed(const QString& sversion)
{
  QStringList values = sversion.split(".");
  Q_ASSERT(values.size() == 3);
  int mayor = values.at(0).toInt();
  int minor = values.at(1).toInt();
  int bugs = values.at(2).toInt();

  values[0] = QString::number(mayor);
  values[1] = QString::number(minor);
  values[2] = QString::number(bugs);

  return values.join(".");
}

QString WCVersion::worldcoinManagerVersionExtended()
{
  QStringList values = version.split(".");
  Q_ASSERT(values.size() == 3);
  if(values.at(0).size() == 1)
     values[0].prepend("0")  ;
  if(values.at(1).size() == 1)
     values[1].prepend("0")  ;
  if(values.at(2).size() == 1)
     values[2].prepend("0")  ;
  return values.join(".");
}

QString WCVersion::worldcoinManagerVersionExtended(const QString& sversion)
{
  QStringList values = sversion.split(".");
  Q_ASSERT(values.size() == 3);
  if(values.at(0).size() == 1)
     values[0].prepend("0")  ;
  if(values.at(1).size() == 1)
     values[1].prepend("0")  ;
  if(values.at(2).size() == 1)
     values[2].prepend("0")  ;
  return values.join(".");
}

QString WCVersion::worldcoinManagerAlias()
{
  return alias;
}
