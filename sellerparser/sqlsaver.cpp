#include "sqlsaver.h"

#include "sellergoods.h"
#include "settings/dbsettings.h"

#include <QDebug>
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

namespace garsale {

bool SqlSaver::save(const SellerGoods& sgoods)
{
  qDebug() << QSqlDatabase::drivers();
//  return false; TODO
  QSqlDatabase db = connectToDb();
  if (db.isValid() == true &&
      db.isOpen() == true) {
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
  DbSettings settings;
  if (settings.isNull() == true) {
    qDebug() << QObject::tr(QString("DB settings not found in %1").arg(settings.fileName())
                            .toStdString().c_str());
    return QSqlDatabase();
  }

  QSqlDatabase db = QSqlDatabase::addDatabase(settings.type());
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
