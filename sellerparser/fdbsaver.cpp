#include "fdbsaver.h"

#include "sellergoods.h"
#include "settings/fdbquerysettings.h"

#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QPair>
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

QList<FdbSqlSaver::Record> FdbSqlSaver::execQuery(QSqlQuery &query, bool* ok) const
{
  if (ok != nullptr) {
    *ok = false;
  }

  QList<Record> result;
  if (query.exec() == false) {
    qDebug() << QObject::tr(QString("Execute query failed: %2")
                            .arg(query.lastError().text())
                            .toStdString().c_str());
    return result;
  }

  if (ok != nullptr) {
    *ok = true;
  }

  while (query.next() == true) {
    Record rec;
    for ( int i = 0, sz = query.record().count(); i < sz; ++i) {
      rec.append(query.record().field(i).name(),
                 query.record().field(i).value().toString());
    }
    result.append(rec);
  }

  if (query.isActive() == true) {
    query.finish();
  }

  return result;
}

int FdbSqlSaver::getMaxId(QSqlQuery& query) const
{
  int result = -1;
  bool ok = false;

  auto queryResult = execQuery(query, &ok);
  if (ok == false) {
    return result;
  }

  result = queryResult.isEmpty() ? 0 : queryResult.first().values().first().toInt();

  return result;
}

int FdbSqlSaver::checkSellerExist(QSqlQuery& query, const SellerGoods& sgoods) const
{
  int result = -1;
  bool ok = false;

  query.addBindValue(sgoods.id);
  auto queryResult = execQuery(query, &ok);
  if (ok == false) {
    return result;
  }

  int sz = queryResult.size();
  if (sz > 1) {
    QStringList idlist;
    idlist.reserve(sz);
    for (auto it = queryResult.constBegin(), end = queryResult.constEnd(); it != end; ++it) {
      idlist.append(it.values().first());
    }
    qDebug() << QObject::tr(QString("Error: exist %1 records with seller id = %2: %3")
                            .arg(sz).arg(sgoods.id)
                            .arg(idlist.join(", "))
                            .toStdString().c_str());
  }
  else if (sz == 0) {
    result = 0;
  }
  else {
    int fid = queryResult.first().values().first().toInt();
    QString sellerNickname = queryResult.first().values().at(1).toInt();
    if (sellerNickname == sgoods.nickname) {
      result = fid;
    }
    else {
      qDebug() << QObject::tr(QString("Error: nickname in DB '%1' not equal '%2'")
                              .arg(sellerNickname).arg(sgoods.nickname)
                              .toStdString().c_str());
    }
  }
  return result;
}

bool FdbSqlSaver::checkGoodExist(QSqlQuery& query, const Good& good) const
{
  bool ok = false;

  query.addBindValue(good.barcode);
  auto queryResult = execQuery(query, &ok);
  if (ok == false) {
    return false;
  }

  if (queryResult.isEmpty() == true) {
    return true;
  }

  qDebug() << QObject::tr(QString("Warning: exist records with barcode = %2")
                          .arg(good.barcode)
                          .toStdString().c_str());
  for (auto it = queryResult.constBegin(), end = queryResult.constEnd(); it != end; ++it) {
    const Record& rec = *it;
    qDebug() << QObject::tr(QString("%1 %2 %3 %4 %5")
                            .arg(rec.values().at(0))
                            .arg(rec.values().at(1))
                            .arg(rec.values().at(2))
                            .arg(rec.values().at(3))
                            .arg(rec.values().at(4))
                            .toStdString().c_str());
  }
  return false;
}

bool FdbSqlSaver::saveSeller(QSqlQuery& query, const SellerGoods& sgoods, int sellerid) const
{
  query.addBindValue(sellerid);
  query.addBindValue(sgoods.nickname);
  query.addBindValue(sgoods.name);
  query.addBindValue(sgoods.inspector_code);
  query.addBindValue(sgoods.phone);
  query.addBindValue(sgoods.email);
  query.addBindValue(sgoods.id);
  query.addBindValue(sgoods.payment_to);
  query.addBindValue(sgoods.payment_kind);

  bool ok = false;
  auto queryResult = execQuery(query, &ok);
  Q_UNUSED(queryResult);
  return ok;
}

bool FdbSqlSaver::saveDocument(QSqlQuery& query, const SellerGoods& sgoods, int sellerid, int docid) const
{
  QDateTime now = QDateTime::currentDateTime();

  query.addBindValue(docid);
  query.addBindValue(now.date().toString("yyyy-MM-dd"));
  query.addBindValue(now.time().toString("hh:mm:ss"));
  query.addBindValue(sellerid);
  query.addBindValue(sgoods.goods.size());

  bool ok = false;
  auto queryResult = execQuery(query, &ok);
  Q_UNUSED(queryResult);
  return ok;
}

bool FdbSqlSaver::saveGood(QSqlQuery& query, const Good& good, int goodid) const
{
  query.addBindValue(goodid);
  query.addBindValue(good.label);
  query.addBindValue(good.barcode);
  query.addBindValue(good.price);
  query.addBindValue(good.size);

  bool ok = false;
  auto queryResult = execQuery(query, &ok);
  Q_UNUSED(queryResult);
  return ok;
}

bool FdbSqlSaver::saveDocContent(QSqlQuery& query, const Good& good, int docid, int goodid, int doccontentid) const
{
  query.addBindValue(doccontentid);
  query.addBindValue(docid);
  query.addBindValue(goodid);
  query.addBindValue(good.price);

  bool ok = false;
  auto queryResult = execQuery(query, &ok);
  Q_UNUSED(queryResult);
  return ok;
}

bool FdbSqlSaver::saveStore(QSqlQuery& query, const Good& good, int sellerid, int goodid, int storeid) const
{
  query.addBindValue(storeid);
  query.addBindValue(sellerid);
  query.addBindValue(goodid);
  query.addBindValue(good.price);

  bool ok = false;
  auto queryResult = execQuery(query, &ok);
  Q_UNUSED(queryResult);
  return ok;
}

} // garsale
