#include "abstractsaver.h"
#include "dummysaver.h"
#include "firebirdsaver.h"

namespace garsale {

std::unique_ptr<AbstractSaver> AbstractSaver::makeSaver(Type type)
{
  std::unique_ptr<AbstractSaver> result(nullptr);
  switch (type) {
    case Type::DUMMY:
      result.reset(new DummySaver());
    case Type::FIREBIRD:
      result.reset(new FirebirdSaver());
    default:
      break;
  }
  return result;
}

} // garsale
