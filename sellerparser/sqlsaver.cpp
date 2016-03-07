#include "sqlsaver.h"

#include "sellergoods.h"

#include <QDebug>
#include <QDir>
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QStringList>

namespace {

// TODO settings
const QString databaseType() { return QString("QIBASE"); }
const QString databaseHost() { return QString("localhost"); }
const QString databaseName() { return QString("%1/tmp/OPSURT_DB_2.0.7.FDB").arg(QDir::homePath()); }
const QString databaseUser() { return QString("SYSDBA"); }
const QString databasePassword() { return QString("masterkey"); }

}

namespace garsale {

bool SqlSaver::save(const SellerGoods& sgoods)
{
  qDebug() << QSqlDatabase::drivers();
  return false;
  QSqlDatabase db = connectToDb();
  if (db.isOpen() == true) {
    // TODO check exists
    QList<QSqlQuery> queries = makeQueries(db, sgoods);
    bool ok = false;
    if (db.transaction() == true) {
      auto it = queries.begin(), end = queries.end();
      for (; it != end; ++it) {
        if (execOneQuery(*it) == false) {
          break;
        }
      }

      if (it == end) {
        if (db.commit() == true) {
          ok = true;
        }
        else {
          qDebug() << QObject::tr(QString("Commit transaction failed: %1").arg(db.lastError().text())
                                  .toStdString().c_str());
        }
      }

      if (ok == false) {
        if (db.rollback() == false) {
          qDebug() << QObject::tr(QString("Rollback transaction failed: %1").arg(db.lastError().text())
                                  .toStdString().c_str());
        }
      }
    }
    else {
      qDebug() << QObject::tr(QString("Create transaction failed: %1").arg(db.lastError().text())
                              .toStdString().c_str());
    }
    return ok;
  }
  return false;
}

QSqlDatabase SqlSaver::connectToDb() const
{
  QSqlDatabase db = QSqlDatabase::addDatabase(::databaseType());
  db.setHostName(::databaseHost());
  db.setDatabaseName(::databaseName());
  db.setUserName(::databaseUser());
  db.setPassword(::databasePassword());

  if (db.open() == false) {
    qDebug() << QObject::tr(QString("Connecttion failed: %1").arg(db.lastError().text())
                            .toStdString().c_str());
  }
  return db;
}

QList<QSqlQuery> SqlSaver::makeQueries(const QSqlDatabase& /*db*/, const SellerGoods& /*sgoods*/) const
{
  QList<QSqlQuery> result;
  // TODO
  return result;
}

bool SqlSaver::execOneQuery(QSqlQuery &query) const
{
  bool result = false;
  if (query.exec() == true) {
    result = true;
  }
  else {
    qDebug() << QObject::tr(QString("Query failed: %1").arg(query.lastError().text())
                            .toStdString().c_str());
  }

  if (query.isActive() == true) {
    query.finish();
  }
  return result;
}

} // garsale
