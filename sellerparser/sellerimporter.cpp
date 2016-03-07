#include "sellerimporter.h"

#include "abstractparser.h"
#include "abstractsaver.h"
#include "sellergoods.h"

#include <QDebug>
#include <QFile>
#include <QObject>
#include <QString>

#include <memory>

namespace garsale {

bool SellerImporter::importFromFile(const QString& fileName)
{
  return saveSellerGoods(loadFromFile(fileName));
}

const SellerGoods SellerImporter::loadFromFile(const QString& fileName)
{
  QFile input(fileName);
  if (input.open(QIODevice::ReadOnly) == true) {
    auto parser = AbstractParser::makeParser(AbstractParser::Type::CSV);
    if (parser != nullptr) {
      return parser->parse(input.readAll());
    }
  }
   
  qDebug() << QObject::tr(QString("Error open file %1: %2")
                          .arg(fileName)
                          .arg(input.errorString())
                          .toStdString().c_str());
  return SellerGoods();
}

bool SellerImporter::saveSellerGoods(const SellerGoods& sgoods)
{
  auto saver = AbstractSaver::makeSaver(AbstractSaver::Type::SQL);
  return saver != nullptr ? saver->save(sgoods) : false;
}

} // garsale
