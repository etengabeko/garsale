#include "dbsettings.h"

#include <QDir>
#include <QSettings>
#include <QString>
#include <QStringList>

namespace {

const QString databaseKey() { return QString("database"); }
const QString typeKey() { return QString("%1/%2").arg(databaseKey()).arg("type"); }
const QString hostKey() { return QString("%1/%2").arg(databaseKey()).arg("host"); }
const QString nameKey() { return QString("%1/%2").arg(databaseKey()).arg("name"); }
const QString userKey() { return QString("%1/%2").arg(databaseKey()).arg("user"); }
const QString passwordKey() { return QString("%1/%2").arg(databaseKey()).arg("password"); }

}

namespace garsale {

bool DbSettings::isNull() const
{
  return !childGroups().contains(::databaseKey());
}

QString DbSettings::type() const
{
  return value(::typeKey()).toString();
}

QString DbSettings::host() const
{
  return value(::hostKey()).toString();
}

QString DbSettings::name() const
{
  QString result = value(::nameKey()).toString();
  return result.contains('~') ? result.replace('~', QDir::homePath()) : result;
}

QString DbSettings::user() const
{
  return value(::userKey()).toString();
}

QString DbSettings::password() const
{
  return value(::passwordKey()).toString();
}

void DbSettings::setType(const QString& type)
{
  setValue(::typeKey(), type);
}

void DbSettings::setHost(const QString& host)
{
  setValue(::hostKey(), host);
}

void DbSettings::setName(const QString& name)
{
  setValue(::nameKey(), name);
}

void DbSettings::setUser(const QString& user)
{
  setValue(::userKey(), user);
}

void DbSettings::setPassword(const QString& passwd)
{
  setValue(::passwordKey(), passwd);
}

} // garsale
