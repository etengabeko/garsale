#include "sellerimporter.h"

#include "abstractparser.h"
#include "abstractsaver.h"
#include "sellergoods.h"

#include <QDebug>
#include <QFile>
#include <QSharedPointer>
#include <QString>

namespace garsale {

SellerImporter::SellerImporter()
{
}

bool SellerImporter::importFromFile(const QString& filename)
{
  QFile input(filename);
  if (input.open(QIODevice::ReadOnly) == true) {
    QSharedPointer<AbstractParser> parser(AbstractParser::factory(AbstractParser::CSV));
    if (parser.isNull() == false) {
      QSharedPointer<AbstractSaver> saver(AbstractSaver::factory(AbstractSaver::DUMMY));
      return saver->save(parser->parse(input.readAll()));
    }
  }
   
  qDebug() << QString::fromUtf8("Ошибка открытия файла %1: %2")
              .arg(filename)
              .arg(input.errorString());
  return false;
}

}
