#ifndef GARSALE_SELLERPARSER_MOSCOW_CSV_PARSER_H
#define GARSALE_SELLERPARSER_MOSCOW_CSV_PARSER_H

#include "abstractparser.h"

#include <QString>

class QByteArray;
template <typename T> class QList;
class QStringList;

namespace garsale {

class Good;
class SellerGoods;

class MoscowCsvParser : public AbstractParser
{
private:
  MoscowCsvParser();

public:
  const SellerGoods parse(const QByteArray& content) override;

private:
  static const QString fieldSeparator();
  const QString splitOneField(const QStringList& strlist, int fieldnum) const;
  const QString splitOneField(const QString& str, int fieldnum, const QString& separator = fieldSeparator()) const;
  const QList<Good> parseGoods(const QStringList& goodsList) const;
  const Good parseOneGood(const QString& str) const;

private:
  friend class AbstractParser;

};

} // garsale

#endif // GARSALE_SELLERPARSER_MOSCOW_CSV_PARSER_H
