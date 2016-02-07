#ifndef GARSALE_SELLERPARSER_ABSTRACTPARSER_H
#define GARSALE_SELLERPARSER_ABSTRACTPARSER_H

class QByteArray;
template <typename T> class QSharedPointer;

namespace garsale {

class SellerGoods;

class AbstractParser
{
public:
  enum Type {
    CSV = 0
  };

public:
  AbstractParser();
  virtual ~AbstractParser();

  static QSharedPointer<AbstractParser> factory(Type type);
  
  virtual const SellerGoods parse(const QByteArray& content) = 0;

};

} // garsale

#endif // GARSALE_SELLERPARSER_ABSTRACTPARSER_H
