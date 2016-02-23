#include "sellerimporter.h"

#include "abstractparser.h"
#include "abstractsaver.h"
#include "sellergoods.h"

#include <QDebug>
#include <QFile>
#include <QObject>
#include <QSharedPointer>
#include <QString>

namespace garsale {

bool SellerImporter::importFromFile(const QString& fileName)
{
  return saveSellerGoods(loadFromFile(fileName));
}

const SellerGoods SellerImporter::loadFromFile(const QString& fileName)
{
  QFile input(fileName);
  if (input.open(QIODevice::ReadOnly) == true) {
    QSharedPointer<AbstractParser> parser(AbstractParser::factory(AbstractParser::CSV));
    if (parser.isNull() == false) {
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
  QSharedPointer<AbstractSaver> saver(AbstractSaver::factory(AbstractSaver::DUMMY));
  return saver.isNull() == false ? saver->save(sgoods) : false;
}

} // garsale
