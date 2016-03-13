#ifndef GARSALE_SELLERPARSER_FDBSAVER_H
#define GARSALE_SELLERPARSER_FDBSAVER_H

#include "abstractsaver.h"

template <typename K, typename V> class QMap;
class QSqlDatabase;
class QSqlQuery;
class QString;

namespace garsale {

class SellerGoods;
class Good;

class FdbSqlSaver : public AbstractSaver
{
public:
  FdbSqlSaver() = default;

  virtual bool save(const SellerGoods& sgoods) override;

private:
  QMap<QString, QSqlQuery> makeQueries(const QSqlDatabase& db) const;
  bool execQuery(QSqlQuery& query) const;

  //! functions below return: id if seller exist, 0 if not exist, -1 if error
  int checkSellerExist(QSqlQuery& query, const SellerGoods& sgoods) const;
  int checkGoodExist(QSqlQuery& query, const Good& good) const;

  //! functions below return id if write success, else return 0
  int saveSeller(QSqlQuery& query, const SellerGoods& sgoods) const;
  int saveDocument(QSqlQuery& query, const SellerGoods& sgoods, int sellerid) const;
  int saveGood(QSqlQuery& query, const Good& good) const;

  //! functions below return true if write success, else return false
  bool saveDocContent(QSqlQuery& query, const Good& good, int docid, int goodid) const;
  bool saveStore(QSqlQuery& query, const Good& good, int sellerid, int goodid) const;

};

} // garsale

#endif // GARSALE_SELLERPARSER_SQLSAVER_H
