#include "abstractsaver.h"
#include "dummysaver.h"
#include "fdbsaver.h"

#include "settings/dbsettings.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

namespace {
  QString connectionName() { return QString("db-garsale"); }
}

namespace garsale {

std::unique_ptr<AbstractSaver> AbstractSaver::makeSaver(Type type)
{
  std::unique_ptr<AbstractSaver> result(nullptr);
  switch (type) {
    case Type::DUMMY:
      result.reset(new DummySaver());
    case Type::FDB:
      result.reset(new FdbSqlSaver());
    default:
      break;
  }
  return result;
}

QSqlDatabase connectToDb()
{
  DbSettings settings;
  if (settings.isNull() == true) {
    qDebug() << QObject::tr(QString("DB settings not found in %1").arg(settings.fileName())
                            .toStdString().c_str());
    return QSqlDatabase();
  }

  QSqlDatabase db = QSqlDatabase::contains(::connectionName()) ? QSqlDatabase::database(::connectionName())
                                                               : QSqlDatabase::addDatabase(settings.type(), ::connectionName());
  db.setHostName(settings.host());
  db.setDatabaseName(settings.name());
  db.setUserName(settings.user());
  db.setPassword(settings.password());

  if (db.open() == false) {
    qDebug() << QObject::tr(QString("Connecttion failed: %1").arg(db.lastError().text())
                            .toStdString().c_str());
  }
  return db;
}

} // garsale
