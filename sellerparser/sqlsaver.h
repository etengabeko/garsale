#ifndef GARSALE_SELLERPARSER_SQLSAVER_H
#define GARSALE_SELLERPARSER_SQLSAVER_H

#include "abstractsaver.h"

template <typename T> class QList;
class QString;
class QSqlDatabase;
class QSqlQuery;

namespace garsale {

class SellerGoods;

class SqlSaver : public AbstractSaver
{
public:
  SqlSaver() = default;

  bool save(const SellerGoods& sgoods) override;

private:
  QSqlDatabase connectToDb() const;
  QList<QSqlQuery> makeQueries(const QSqlDatabase& db, const SellerGoods& sgoods) const;
  bool execOneQuery(QSqlQuery& query) const;

};

} // garsale

#endif // GARSALE_SELLERPARSER_SQLSAVER_H
