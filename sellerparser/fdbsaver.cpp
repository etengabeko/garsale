#include "fdbsaver.h"

#include "sellergoods.h"
#include "settings/fdbquerysettings.h"

#include <QDebug>
#include <QMap>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace garsale {

bool FdbSqlSaver::save(const SellerGoods& sgoods)
{
  qDebug() << QSqlDatabase::drivers();
//  return false; TODO build driver

  QSqlDatabase db = connectToDb();
  if (db.isValid() == true &&
      db.isOpen() == true) {
    QMap<QString, QSqlQuery> queries = makeQueries(db);
    if (queries.contains(FirebirdQuerySettings::selectSellerKey()) == false) {
      qDebug() << QObject::tr(QString("Query '%1' not found")
                              .arg(FirebirdQuerySettings::selectSellerKey())
                              .toStdString().c_str());
      return false;
    }

    int sellerid = checkSellerExist(queries[FirebirdQuerySettings::selectSellerKey()], sgoods);
    if (sellerid == -1) {
      return false;
    }
    else if (sellerid == 0) {
      if (queries.contains(FirebirdQuerySettings::insertSellerKey()) == false) {
        qDebug() << QObject::tr(QString("Query '%1' not found")
                                .arg(FirebirdQuerySettings::insertSellerKey())
                                .toStdString().c_str());
        return false;
      }
      sellerid = saveSeller(queries[FirebirdQuerySettings::insertSellerKey()], sgoods);
      if (sellerid == 0) {
        return false;
      }
    }

    bool ok = false;
    if (db.transaction() == true) {
      // TODO
//      auto it = queries.begin(), end = queries.end();
//      for (; it != end; ++it) {
//        if (execQuery(*it) == false) {
//          break;
//        }
//      }

//      if (it == end) {
//        if (db.commit() == true) {
//          ok = true;
//        }
//        else {
//          qDebug() << QObject::tr(QString("Commit transaction failed: %1").arg(db.lastError().text())
//                                  .toStdString().c_str());
//        }
//      }

//      if (ok == false) {
//        if (db.rollback() == false) {
//          qDebug() << QObject::tr(QString("Rollback transaction failed: %1").arg(db.lastError().text())
//                                  .toStdString().c_str());
//        }
//      }
    }
    else {
      qDebug() << QObject::tr(QString("Create transaction failed: %1").arg(db.lastError().text())
                              .toStdString().c_str());
    }
    return ok;
  }
  return false;
}

QMap<QString, QSqlQuery> FdbSqlSaver::makeQueries(const QSqlDatabase& db) const
{
  QMap<QString, QSqlQuery> result;

  FirebirdQuerySettings settings;
  QStringList queryKeys = settings.keys();
  for (auto it = queryKeys.constBegin(), end = queryKeys.constEnd(); it != end; ++it) {
    QSqlQuery q(db);
    if (q.prepare(settings.query(*it)) == true) {
      result.insert(*it, q);
    }
    else {
      qDebug() << QObject::tr(QString("Fail preparing SQL query from %1: %2")
                              .arg(*it).arg(q.lastError().text())
                              .toStdString().c_str());
    }
  }
  return result;
}

bool FdbSqlSaver::execQuery(QSqlQuery &query) const
{
  bool result = false;
  if (query.exec() == true) {
    result = true;
  }
  else {
    qDebug() << QObject::tr(QString("Execute query '%1' failed: %2")
                            .arg(query.lastQuery()).arg(query.lastError().text())
                            .toStdString().c_str());
    if (query.isActive() == true) {
      query.finish();
    }
  }
  return result;
}

int FdbSqlSaver::checkSellerExist(QSqlQuery& query, const SellerGoods& sgoods) const
{
  int result = 0;

  query.bindValue(":id", sgoods.id);

  if (execQuery(query) == true) {
    int sz = query.size();
    if (sz == 0) {
      result = 0;
    }
    else if (sz == 1) {
      if (query.first() == true) {
        int fid = query.value(0).toInt();
        QString sellerNickname = query.value(1).toString();
        if (sellerNickname == sgoods.nickname) {
          result = fid;
        }
        else {
          qDebug() << QObject::tr(QString("Error: nickname in DB '%1' not equal '%2'")
                                  .arg(sellerNickname).arg(sgoods.nickname)
                                  .toStdString().c_str());
          result = -1;
        }
      }
    }
    else {
      qDebug() << QObject::tr(QString("Error: exist %1 records with seller id = %2")
                              .arg(sz).arg(sgoods.id)
                              .toStdString().c_str());
      result = -1;
    }

    if (query.isActive() == true) {
      query.finish();
    }
  }
  else {
    result = -1;
  }
  return result;
}

int FdbSqlSaver::saveSeller(QSqlQuery& query, const SellerGoods& sgoods) const
{
  int result = 0;

  query.bindValue(":nickname", sgoods.nickname);
  query.bindValue(":name", sgoods.name);
  query.bindValue(":inspector_code", sgoods.inspector_code);
  query.bindValue(":phone", sgoods.phone);
  query.bindValue(":email", sgoods.email);
  query.bindValue(":id", sgoods.id);
  query.bindValue(":payment_to", sgoods.payment_to);
  query.bindValue(":payment_kind", sgoods.payment_kind);

  if (execQuery(query) == true) {
    if (query.first() == true) {
      result = query.value(0).toInt();
    }
    else {
      qDebug() << QObject::tr(QString("Error save seller to DB. Query: '%1'")
                              .arg(query.lastQuery())
                              .toStdString().c_str());
    }

    if (query.isActive() == true) {
      query.finish();
    }
  }

  return result;
}

} // garsale
