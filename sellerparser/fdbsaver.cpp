#include "fdbsaver.h"

#include "sellergoods.h"
#include "settings/fdbquerysettings.h"

#include <QDateTime>
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

    int docid = saveDocument(queries[FirebirdQuerySettings::insertDocumentKey()], sgoods, sellerid);
    if (sellerid == 0) {
      return false;
    }

    int savedCount = 0;
    for (auto it = sgoods.goods.constBegin(), end = sgoods.goods.constEnd(); it != end; ++it) {
      if (checkGoodExist(queries[FirebirdQuerySettings::selectGoodKey()], *it) != 0) {
        qDebug() << QObject::tr(QString("Warning: not saved good %1 %2 %3 %4")
                                .arg(it->label)
                                .arg(it->size)
                                .arg(it->price)
                                .arg(it->barcode)
                                .toStdString().c_str());
        continue;
      }

      int goodid = saveGood(queries[FirebirdQuerySettings::insertGoodKey()], *it);
      if (goodid == 0) {
        qDebug() << QObject::tr(QString("Warning: not saved good %1 %2 %3 %4")
                                .arg(it->label)
                                .arg(it->size)
                                .arg(it->price)
                                .arg(it->barcode)
                                .toStdString().c_str());
        continue;
      }
      if (saveDocContent(queries[FirebirdQuerySettings::insertDocContentKey()], *it, docid, goodid) == false) {
        qDebug() << QObject::tr(QString("Warning: not saved doc_content %1 %2 %3 %4")
                                .arg(it->label)
                                .arg(it->size)
                                .arg(it->price)
                                .arg(it->barcode)
                                .toStdString().c_str());
        continue;
      }
      if (saveStore(queries[FirebirdQuerySettings::insertStoreKey()], *it, sellerid, goodid) == false) {
        qDebug() << QObject::tr(QString("Warning: not saved in store %1 %2 %3 %4")
                                .arg(it->label)
                                .arg(it->size)
                                .arg(it->price)
                                .arg(it->barcode)
                                .toStdString().c_str());
        continue;
      }
      ++savedCount;
    }
    return savedCount == sgoods.goods.size();
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

int FdbSqlSaver::checkGoodExist(QSqlQuery& query, const Good& good) const
{
  int result = 0;

  query.bindValue(":barcode", good.barcode);

  if (execQuery(query) == true) {
    int sz = query.size();
    if (sz == 0) {
      result = 0;
    }
    else {
      qDebug() << QObject::tr(QString("Warning: exist %1 records with barcode = %2:\n%3 %4 %5 %6 %7")
                              .arg(sz).arg(good.barcode)
                              .arg(query.value(0).toString())
                              .arg(query.value(1).toString())
                              .arg(query.value(2).toString())
                              .arg(query.value(3).toString())
                              .arg(query.value(4).toString())
                              .toStdString().c_str());
      result = sz;
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

int FdbSqlSaver::saveDocument(QSqlQuery& query, const SellerGoods& sgoods, int sellerid) const
{
  int result = 0;

  QDateTime now = QDateTime::currentDateTime();

  query.bindValue(":date", now.date().toString("yyyy-MM-dd"));
  query.bindValue(":time", now.time().toString("hh:mm:ss"));
  query.bindValue(":person_id", sellerid);
  query.bindValue(":goods_sum", sgoods.goods.size());

  if (execQuery(query) == true) {
    if (query.first() == true) {
      result = query.value(0).toInt();
    }
    else {
      qDebug() << QObject::tr(QString("Error save document to DB. Query: '%1'")
                              .arg(query.lastQuery())
                              .toStdString().c_str());
    }

    if (query.isActive() == true) {
      query.finish();
    }
  }

  return result;
}

int FdbSqlSaver::saveGood(QSqlQuery& query, const Good& good) const
{
  int result = 0;

  query.bindValue(":label", good.label);
  query.bindValue(":barcode", good.barcode);
  query.bindValue(":price", good.price);
  query.bindValue(":size", good.size);

  if (execQuery(query) == true) {
    if (query.first() == true) {
      result = query.value(0).toInt();
    }
    else {
      qDebug() << QObject::tr(QString("Error save good to DB. Query: '%1'")
                              .arg(query.lastQuery())
                              .toStdString().c_str());
    }

    if (query.isActive() == true) {
      query.finish();
    }
  }

  return result;
}

bool FdbSqlSaver::saveDocContent(QSqlQuery& query, const Good& good, int docid, int goodid) const
{
  bool result = false;

  query.bindValue(":doc_id", docid);
  query.bindValue(":good_id", goodid);
  query.bindValue(":good_price", good.price);

  if (execQuery(query) == true) {
    result = true;
    if (query.isActive() == true) {
      query.finish();
    }
  }

  return result;
}

bool FdbSqlSaver::saveStore(QSqlQuery& query, const Good& good, int sellerid, int goodid) const
{
  bool result = false;

  query.bindValue(":person_id", sellerid);
  query.bindValue(":good_id", goodid);
  query.bindValue(":good_price", good.price);

  if (execQuery(query) == true) {
    result = true;
    if (query.isActive() == true) {
      query.finish();
    }
  }

  return result;
}

} // garsale
