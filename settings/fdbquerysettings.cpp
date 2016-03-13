#include "fdbquerysettings.h"

#include <QSettings>
#include <QString>
#include <QStringList>

namespace {

QString fdbQueriesKey() { return QString("fdb-queries"); }

}

namespace garsale {

bool FirebirdQuerySettings::isNull() const
{
  return !childGroups().contains(::fdbQueriesKey());
}

QStringList FirebirdQuerySettings::keys() const
{
  return childKeys(::fdbQueriesKey());
}

QString FirebirdQuerySettings::query(const QString& key) const
{
  return value(key).toString();
}

QString FirebirdQuerySettings::selectSellerQuery() const
{
  return query(selectSellerKey());
}

QString FirebirdQuerySettings::selectGoodsQuery() const
{
  return query(selectGoodKey());
}

QString FirebirdQuerySettings::insertSellerQuery() const
{
  return query(insertSellerKey());
}

QString FirebirdQuerySettings::insertDocumentQuery() const
{
  return query(insertDocumentKey());
}

QString FirebirdQuerySettings::insertGoodQuery() const
{
  return query(insertGoodKey());
}

QString FirebirdQuerySettings::insertDocContentQuery() const
{
  return query(insertDocContentKey());
}

QString FirebirdQuerySettings::insertStoreQuery() const
{
  return query(insertStoreKey());
}

QString FirebirdQuerySettings::selectSellerKey()
{
  return QString("%1/%2").arg(::fdbQueriesKey()).arg("select_seller");
}

QString FirebirdQuerySettings::selectGoodKey()
{
  return QString("%1/%2").arg(::fdbQueriesKey()).arg("select_goods");
}

QString FirebirdQuerySettings::insertSellerKey()
{
  return QString("%1/%2").arg(::fdbQueriesKey()).arg("insert_seller");
}

QString FirebirdQuerySettings::insertDocumentKey()
{
  return QString("%1/%2").arg(::fdbQueriesKey()).arg("insert_document");
}

QString FirebirdQuerySettings::insertGoodKey()
{
  return QString("%1/%2").arg(::fdbQueriesKey()).arg("insert_good");
}

QString FirebirdQuerySettings::insertDocContentKey()
{
  return QString("%1/%2").arg(::fdbQueriesKey()).arg("insert_doc_content");
}

QString FirebirdQuerySettings::insertStoreKey()
{
  return QString("%1/%2").arg(::fdbQueriesKey()).arg("insert_store");
}

} // garsale
