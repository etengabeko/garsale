#ifndef GARSALE_SELLERPARSER_FDBSAVER_H
#define GARSALE_SELLERPARSER_FDBSAVER_H

#include "abstractsaver.h"

#include <QStringList>

template <typename K, typename V> class QMap;
template <typename K, typename V> class QPair;
template <typename T> class QList;
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
  class Record
  {
  public:
    void append(const QString& column, const QString& value);
    int columnsCount() const;
    const QStringList& values() const;
    const QStringList& columns() const;

  private:
    QStringList columns_;
    QStringList values_;

  };

  QMap<QString, QSqlQuery> makeQueries(const QSqlDatabase& db) const;
  bool checkExistsAllQueries(const QMap<QString, QSqlQuery>& queries) const;
  bool checkExistsOneQuery(const QMap<QString, QSqlQuery>& queries, const QString& key) const;

  QList<Record> execQuery(QSqlQuery& query, bool* ok) const;

  //! return max id or -1 if error
  int getMaxId(QSqlQuery& query) const;

  //! functions below return: id if seller exist, 0 if not exist, -1 if error
  int checkSellerExist(QSqlQuery& query, const SellerGoods& sgoods) const;

  //! functions below return true if write success, else return false
  bool saveSeller(QSqlQuery& query, const SellerGoods& sgoods, int sellerid) const;
  bool saveDocument(QSqlQuery& query, const SellerGoods& sgoods, int sellerid, int docid) const;
  bool saveGood(QSqlQuery& query, const Good& good, int goodid) const;

  bool checkGoodExist(QSqlQuery& query, const Good& good) const;
  bool saveDocContent(QSqlQuery& query, const Good& good, int docid, int goodid, int doccontentid) const;
  bool saveStore(QSqlQuery& query, const Good& good, int sellerid, int goodid, int storeid) const;

};

} // garsale

#endif // GARSALE_SELLERPARSER_SQLSAVER_H
