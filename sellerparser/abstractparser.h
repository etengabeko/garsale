#ifndef GARSALE_SELLERPARSER_ABSTRACTPARSER_H
#define GARSALE_SELLERPARSER_ABSTRACTPARSER_H

#include <memory>

class QByteArray;

namespace garsale {

class SellerGoods;

class AbstractParser
{
public:
  enum class Type {
    CSV = 0
  };

public:
  AbstractParser() = default;
  virtual ~AbstractParser() = default;

  AbstractParser(AbstractParser&& other) = default;
  AbstractParser& operator= (AbstractParser&& other) = default;

  AbstractParser(const AbstractParser& other) = default;
  AbstractParser& operator= (const AbstractParser& other) = default;

  static std::unique_ptr<AbstractParser> makeParser(Type type);
  
  virtual const SellerGoods parse(const QByteArray& content) = 0;

};

} // garsale

#endif // GARSALE_SELLERPARSER_ABSTRACTPARSER_H
