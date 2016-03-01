#include "abstractparser.h"
#include "moscowcsvparser.h"

namespace garsale {

std::unique_ptr<AbstractParser> AbstractParser::makeParser(Type type)
{
  std::unique_ptr<AbstractParser> result(nullptr);
  switch (type) {
    case Type::CSV:
      result.reset(new MoscowCsvParser());
    default:
      break;
  }
  return result;
}

} // garsale
