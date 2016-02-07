#ifndef GARSALE_SELLERPARSER_ABSTRACTSAVER_H
#define GARSALE_SELLERPARSER_ABSTRACTSAVER_H

template <typename T> class QSharedPointer;

namespace garsale {

class SellerGoods;

class AbstractSaver
{
public:
  enum Type {
    DUMMY = 0
  };

public:
  AbstractSaver();
  virtual ~AbstractSaver();

  static QSharedPointer<AbstractSaver> factory(Type type);
  
  virtual bool save(const SellerGoods& sgoods) = 0;

};

} // garsale

#endif // GARSALE_SELLERPARSER_ABSTRACTSAVER_H
