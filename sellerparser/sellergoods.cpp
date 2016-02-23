#include "sellergoods.h"

#include <QObject>
#include <QTextStream>

namespace garsale {

bool Good::isValid() const
{
  bool notValid = label.trimmed().isEmpty() ||
                  size.trimmed().isEmpty() ||
                  price.trimmed().isEmpty() ||
//                  discount.trimmed().isEmpty() ||
//                  charity.trimmed().isEmpty() ||
                  barcode.trimmed().isEmpty();
  return !notValid;
}

bool Good::isEmpty() const
{
  return label.trimmed().isEmpty() &&
         size.trimmed().isEmpty() &&
         price.trimmed().isEmpty() &&
//         discount.trimmed().isEmpty() &&
//         charity.trimmed().isEmpty() &&
         barcode.trimmed().isEmpty();
}

QTextStream& operator<< (QTextStream& stream, const SellerGoods& sgoods)
{
  stream << QObject::tr("Nickname: ") << sgoods.nickname << "\n"
         << QObject::tr("Name: ") << sgoods.name << "\n"
         << QObject::tr("Phone: ") << sgoods.phone << "\n"
         << QObject::tr("Email: ") << sgoods.email << "\n"
         << QObject::tr("Payment kind: ") << sgoods.payment_kind << "\n"
         << QObject::tr("Payment to: ") << sgoods.payment_to << "\n"
         << QObject::tr("Inspector code: ") << sgoods.inspector_code << "\n"
         << QObject::tr("ID: ") << sgoods.id << "\n"
         << QObject::tr("Goods:") << "\n";
  int i = 0;
  for (auto it = sgoods.goods.begin(), end = sgoods.goods.end(); it != end; ++it) {
    stream << ++i << "\t";
    if (it->label.isEmpty() == false) stream << it->label << "\t";
    if (it->size.isEmpty() == false) stream << it->size << "\t";
    if (it->price.isEmpty() == false) stream << it->price << "\t";
    if (it->discount.isEmpty() == false) stream << it->discount << "\t";
    if (it->charity.isEmpty() == false) stream << it->charity << "\t";
    if (it->barcode.isEmpty() == false) stream << it->barcode << "\n";
  }
  return stream;
}

} // garsale 
