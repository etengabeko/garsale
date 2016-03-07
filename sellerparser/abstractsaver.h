#ifndef GARSALE_SELLERPARSER_ABSTRACTSAVER_H
#define GARSALE_SELLERPARSER_ABSTRACTSAVER_H

#include <memory>

namespace garsale {

class SellerGoods;

class AbstractSaver
{
public:
  enum class Type {
    DUMMY = 0,
    SQL
  };

public:
  AbstractSaver() = default;
  virtual ~AbstractSaver() = default;

  AbstractSaver(AbstractSaver&& other) = default;
  AbstractSaver& operator= (AbstractSaver&& other) = default;

  AbstractSaver(const AbstractSaver& other) = default;
  AbstractSaver& operator= (const AbstractSaver& other) = default;

  static std::unique_ptr<AbstractSaver> makeSaver(Type type);
  
  virtual bool save(const SellerGoods& sgoods) = 0;

};

} // garsale

#endif // GARSALE_SELLERPARSER_ABSTRACTSAVER_H
