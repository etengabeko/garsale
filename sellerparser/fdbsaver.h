#ifndef GARSALE_SELLERPARSER_FDBSAVER_H
#define GARSALE_SELLERPARSER_FDBSAVER_H

#include "abstractsaver.h"

template <typename K, typename V> class QMap;
class QSqlDatabase;
class QSqlQuery;
class QString;

namespace garsale {

class SellerGoods;

class FdbSqlSaver : public AbstractSaver
{
public:
  FdbSqlSaver() = default;

  virtual bool save(const SellerGoods& sgoods) override;

private:
  QMap<QString, QSqlQuery> makeQueries(const QSqlDatabase& db) const;
  bool execQuery(QSqlQuery& query) const;

  int checkSellerExist(QSqlQuery& query, const SellerGoods& sgoods) const; //! return: id if seller exist, 0 if not exist, -1 if error
  int saveSeller(QSqlQuery& query, const SellerGoods& sgoods) const; //! return id if write success, else return 0

};

} // garsale

#endif // GARSALE_SELLERPARSER_SQLSAVER_H
