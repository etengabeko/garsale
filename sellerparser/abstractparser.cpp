#include "abstractparser.h"
#include "moscowcsvparser.h"

#include <QSharedPointer>

namespace garsale {

AbstractParser::AbstractParser()
{
}

AbstractParser::~AbstractParser()
{
}

QSharedPointer<AbstractParser> AbstractParser::factory(Type type)
{
  switch (type) {
    case CSV:
      return QSharedPointer<AbstractParser>(new MoscowCsvParser());
    default:
      break;
  }
  return QSharedPointer<AbstractParser>();
}

} // garsale
