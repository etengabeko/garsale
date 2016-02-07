#ifndef GARSALE_SELLERPARSER_SELLERGOODS_H
#define GARSALE_SELLERPARSER_SELLERGOODS_H

#include <QList>
#include <QString>

class QTextStream;

namespace garsale {

struct Good
{
  QString label;
  QString size;
  QString price;
  QString discount;
  QString charity;
  QString barcode;

  bool isValid() const;
  bool isEmpty() const;
};

struct SellerGoods
{
  QString id; 
  QString nickname;
  QString name;
  QString phone;
  QString email;
  QString payment_kind;
  QString payment_to;
  QString inspector_code;

  QList<Good> goods;
};

QTextStream& operator<< (QTextStream& stream, const SellerGoods& sgoods);

} // garsale

#endif // GARSALE_SELLERPARSER_SELLERGOODS_H
