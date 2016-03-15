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
  QSqlDatabase db = connectToDb();
  if (db.isValid() == true &&
      db.isOpen() == true) {
    QMap<QString, QSqlQuery> queries = makeQueries(db);
    if (checkExistsAllQueries(queries) == false) {
      return false;
    }

    int sellerid = checkSellerExist(queries[FirebirdQuerySettings::selectSellerKey()], sgoods);
    if (sellerid == -1) {
      return false;
    }
    else if (sellerid == 0) {
      sellerid = getMaxId(queries[FirebirdQuerySettings::selectMaxSellerIdKey()]);
      if (sellerid < 0) {
        return false;
      }
      sellerid = saveSeller(queries[FirebirdQuerySettings::insertSellerKey()], sgoods, sellerid+1);
      if (sellerid < 0) {
        return false;
      }
    }

    int docid = getMaxId(queries[FirebirdQuerySettings::selectMaxDocumentIdKey()]);
    if (docid < 0) {
      return false;
    }
    docid = saveDocument(queries[FirebirdQuerySettings::insertDocumentKey()], sgoods, sellerid, docid+1);
    if (sellerid == 0) {
      return false;
    }

    int savedCount = 0;
    for (auto it = sgoods.goods.constBegin(), end = sgoods.goods.constEnd(); it != end; ++it) {
      int goodExists = checkGoodExist(queries[FirebirdQuerySettings::selectGoodKey()], *it);
      if (goodExists == -1 || goodExists > 0) {
        qDebug() << QObject::tr(QString("Warning: not saved good %1 %2 %3 %4")
                                .arg(it->label)
                                .arg(it->size)
                                .arg(it->price)
                                .arg(it->barcode)
                                .toStdString().c_str());
        continue;
      }

      int goodid = getMaxId(queries[FirebirdQuerySettings::selectMaxGoodIdKey()]);
      if (goodid < 0) {
        return false;
      }
      goodid = saveGood(queries[FirebirdQuerySettings::insertGoodKey()], *it, goodid+1);
      if (goodid == 0) {
        qDebug() << QObject::tr(QString("Warning: not saved good %1 %2 %3 %4")
                                .arg(it->label)
                                .arg(it->size)
                                .arg(it->price)
                                .arg(it->barcode)
                                .toStdString().c_str());
        continue;
      }

      int doccontentid = getMaxId(queries[FirebirdQuerySettings::selectMaxDocContentIdKey()]);
      if (saveDocContent(queries[FirebirdQuerySettings::insertDocContentKey()], *it, docid, goodid, doccontentid+1) == false) {
        qDebug() << QObject::tr(QString("Warning: not saved doc_content %1 %2 %3 %4")
                                .arg(it->label)
                                .arg(it->size)
                                .arg(it->price)
                                .arg(it->barcode)
                                .toStdString().c_str());
        continue;
      }

      int storeid = getMaxId(queries[FirebirdQuerySettings::selectMaxStoreIdKey()]);
      if (saveStore(queries[FirebirdQuerySettings::insertStoreKey()], *it, sellerid, goodid, storeid+1) == false) {
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

bool FdbSqlSaver::checkExistsAllQueries(const QMap<QString, QSqlQuery>& queries) const
{
  if (queries.isEmpty() == false) {
    bool result = checkExistsOneQuery(queries, FirebirdQuerySettings::insertDocContentKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::insertDocumentKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::insertGoodKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::insertSellerKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::insertStoreKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::selectGoodKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::selectMaxDocContentIdKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::selectMaxDocumentIdKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::selectMaxGoodIdKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::selectMaxSellerIdKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::selectMaxStoreIdKey())
               && checkExistsOneQuery(queries, FirebirdQuerySettings::selectSellerKey());
    return result;
  }
  return false;
}

bool FdbSqlSaver::checkExistsOneQuery(const QMap<QString, QSqlQuery>& queries, const QString& key) const
{
  if (key.isEmpty() == false) {
    if (queries.contains(key) == true) {
      return true;
    }
    qDebug() << QObject::tr(QString("Query '%1' not found")
                            .arg(key)
                            .toStdString().c_str());
  }
  return false;
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

int FdbSqlSaver::getMaxId(QSqlQuery& query) const
{
  int result = -1;

  if (execQuery(query) == true) {
    if (query.first() == true) {
      result = query.value(0).toInt();
      qDebug() << "max id = " << result;
    }
    else {
      qDebug() << QObject::tr(QString("Error get last id")
                              .toStdString().c_str());
    }
  }

  return result;
}

int FdbSqlSaver::checkSellerExist(QSqlQuery& query, const SellerGoods& sgoods) const
{
  int result = -1;

  query.bindValue(":id", sgoods.id);

  if (execQuery(query) == true) {
    int sz = 0;
    while (query.next() == true) {
      ++sz;
      result = 0;
      int fid = query.value(0).toInt();
      QString sellerNickname = query.value(1).toString();
      qDebug() << "last seller fid" << fid << "name" << sellerNickname;
      if (sellerNickname == sgoods.nickname) {
        result = fid;
      }
      else {
        qDebug() << QObject::tr(QString("Error: nickname in DB '%1' not equal '%2'")
                                .arg(sellerNickname).arg(sgoods.nickname)
                                .toStdString().c_str());
      }
    }

    if (sz > 1) {
      qDebug() << QObject::tr(QString("Error: exist %1 records with seller id = %2")
                              .arg(sz).arg(sgoods.id)
                              .toStdString().c_str());
      result = sz;
    }
    else if (sz == 0) {
      qDebug() << QObject::tr(QString("Error: not result of exec query: %1")
                              .arg(query.lastError().text())
                              .toStdString().c_str());
    }
  }

  if (query.isActive() == true) {
    query.finish();
  }
  return result;
}

int FdbSqlSaver::checkGoodExist(QSqlQuery& query, const Good& good) const
{
  int result = -1;

  query.bindValue(":barcode", good.barcode);

  if (execQuery(query) == true) {
    if (query.first() == true) {
      result = 0;
      int sz = 1;
      qDebug() << QObject::tr(QString("Warning: exist records with barcode = %2")
                              .arg(good.barcode)
                              .toStdString().c_str());
      qDebug() << QObject::tr(QString("%1 %2 %3 %4 %5")
                              .arg(query.value(0).toString())
                              .arg(query.value(1).toString())
                              .arg(query.value(2).toString())
                              .arg(query.value(3).toString())
                              .arg(query.value(4).toString())
                              .toStdString().c_str());
      while (query.next() == true) {
        ++sz;
        qDebug() << QObject::tr(QString("%1 %2 %3 %4 %5")
                                .arg(query.value(0).toString())
                                .arg(query.value(1).toString())
                                .arg(query.value(2).toString())
                                .arg(query.value(3).toString())
                                .arg(query.value(4).toString())
                                .toStdString().c_str());
      }
      result = sz;
    }

    if (query.isActive() == true) {
      query.finish();
    }
  }
  return result;
}

int FdbSqlSaver::saveSeller(QSqlQuery& query, const SellerGoods& sgoods, int sellerid) const
{
  int result = 0;

  query.bindValue(":fid", sellerid);
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

int FdbSqlSaver::saveDocument(QSqlQuery& query, const SellerGoods& sgoods, int sellerid, int docid) const
{
  int result = 0;

  QDateTime now = QDateTime::currentDateTime();

  query.bindValue(":fid", docid);
  query.bindValue(":dt", now.date().toString("yyyy-MM-dd"));
  query.bindValue(":tm", now.time().toString("hh:mm:ss"));
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

int FdbSqlSaver::saveGood(QSqlQuery& query, const Good& good, int goodid) const
{
  int result = 0;

  query.bindValue(":fid", goodid);
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

bool FdbSqlSaver::saveDocContent(QSqlQuery& query, const Good& good, int docid, int goodid, int doccontentid) const
{
  bool result = false;

  query.bindValue(":fid", doccontentid);
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

bool FdbSqlSaver::saveStore(QSqlQuery& query, const Good& good, int sellerid, int goodid, int storeid) const
{
  bool result = false;

  query.bindValue(":fid", storeid);
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
