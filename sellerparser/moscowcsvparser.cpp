#include "moscowcsvparser.h"
#include "sellergoods.h"

#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <QStringList>

namespace {
  int sellerStringsCount() { return 30; }
  int goodFieldsCount() { return 5; }
}

namespace garsale {

MoscowCsvParser::MoscowCsvParser() :
  AbstractParser()
{
}

const QString MoscowCsvParser::fieldSeparator()
{
  return "|";
}

const SellerGoods MoscowCsvParser::parse(const QByteArray& content)
{
  QStringList contentList(QString(content).split("\n"));
  if (contentList.size() < ::sellerStringsCount()) {
    qDebug() << QObject::tr(QString("Row count less than needed: %1 from %2")
                            .arg(contentList.size())
                            .arg(::sellerStringsCount())
                            .toStdString().c_str());
    return SellerGoods();
  }
  
  SellerGoods sg;
  sg.id = splitOneField(contentList.at(22), 1);
  sg.nickname = splitOneField(contentList.at(1), 2);
  sg.name = splitOneField(contentList.at(3), 2);
  sg.phone = splitOneField(contentList.at(5), 2);
  sg.email = splitOneField(contentList.at(7), 2);
  sg.payment_kind = splitOneField(contentList.at(13), 2);
  sg.payment_to = splitOneField(contentList.at(14), 1);
  sg.inspector_code = splitOneField(contentList.at(19), 2);

  auto firstRemoved = contentList.begin();
  auto lastRemoved = firstRemoved + ::sellerStringsCount()-1;
  contentList.erase(firstRemoved, lastRemoved);
  sg.goods = parseGoods(contentList);
 
  return sg;
}

const QList<Good> MoscowCsvParser::parseGoods(const QStringList& goodsList) const
{
 QList<Good> result;
 foreach(auto entry, goodsList) {
   auto each(parseOneGood(entry));
   if (each.isValid() == true) {
     result.append(each);
   }
   else {
     if (each.isEmpty() == false) {
       qDebug() << QObject::tr(QString("Good has empty fields:\n\tname %1\n\tsize %2\n\tcount %3\n\tbarcode %4")
                               .arg(each.label)
                               .arg(each.size)
                               .arg(each.price)
                               .arg(each.barcode)
                               .toStdString().c_str());
     }
   }
 }
 return result;
}

const Good MoscowCsvParser::parseOneGood(const QString& str) const
{
  QStringList goodFields(str.split(fieldSeparator(), QString::SkipEmptyParts));
  if (goodFields.size() < ::goodFieldsCount()) {
    return Good();
  }
  
  Good g;
  g.label = splitOneField(goodFields, 2);
  g.size = splitOneField(goodFields, 3);
  g.price = splitOneField(goodFields, 4);
  g.barcode = splitOneField(goodFields, 5);
  return g;
}

const QString MoscowCsvParser::splitOneField(const QStringList& strlist, int fieldnum) const
{
  if (strlist.size() < fieldnum) {
    qDebug() << QObject::tr(QString("Filed %1 not found in: %2")
                            .arg(fieldnum)
                            .arg(strlist.join(fieldSeparator()))
                            .toStdString().c_str());
    return QString::null;
  }
  return strlist.at(fieldnum-1);
}

const QString MoscowCsvParser::splitOneField(const QString& str, int fieldnum, const QString& separator) const
{
  QStringList fields(str.split(separator, QString::SkipEmptyParts));
  return splitOneField(fields, fieldnum);
}

} // garsale
