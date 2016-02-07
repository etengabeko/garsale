#include "sellergoods.h"

#include <QTextStream>

namespace garsale {

bool Good::isValid() const
{
  bool notValid = label.isEmpty() ||
                  size.isEmpty() ||
                  price.isEmpty() ||
//                  discount.isEmpty() ||
//                  charity.isEmpty() ||
                  barcode.isEmpty();
  return !notValid;
}

bool Good::isEmpty() const
{
  return label.isEmpty() &&
         size.isEmpty() &&
         price.isEmpty() &&
//         discount.isEmpty() &&
//         charity.isEmpty() &&
         barcode.isEmpty();
}

QTextStream& operator<< (QTextStream& stream, const SellerGoods& sgoods)
{
  stream << sgoods.nickname << "\n"
         << sgoods.name << "\n"
         << sgoods.phone << "\n"
         << sgoods.email << "\n"
         << sgoods.payment_kind << "\n"
         << sgoods.payment_to << "\n"
         << sgoods.inspector_code << "\n"
         << sgoods.id << "\n";
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
