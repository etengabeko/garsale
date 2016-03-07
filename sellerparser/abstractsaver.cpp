#include "abstractsaver.h"
#include "dummysaver.h"
#include "sqlsaver.h"

namespace garsale {

std::unique_ptr<AbstractSaver> AbstractSaver::makeSaver(Type type)
{
  std::unique_ptr<AbstractSaver> result(nullptr);
  switch (type) {
    case Type::DUMMY:
      result.reset(new DummySaver());
    case Type::SQL:
      result.reset(new SqlSaver());
    default:
      break;
  }
  return result;
}

} // garsale
