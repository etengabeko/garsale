#include "abstractsaver.h"
#include "dummysaver.h"

#include <QSharedPointer>

namespace garsale {

AbstractSaver::AbstractSaver()
{
}

AbstractSaver::~AbstractSaver()
{
}

QSharedPointer<AbstractSaver> AbstractSaver::factory(Type type)
{
  switch (type) {
    case DUMMY:
      return QSharedPointer<AbstractSaver>(new DummySaver());
    default:
      break;
  }
  return QSharedPointer<AbstractSaver>();
}

} // garsale
