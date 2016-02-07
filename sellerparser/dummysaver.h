#ifndef GARSALE_SELLERPARSER_DUMMYSAVER_H
#define GARSALE_SELLERPARSER_DUMMYSAVER_H

#include "abstractsaver.h"

class QString;

namespace garsale {

class SellerGoods;

class DummySaver : public AbstractSaver
{
private:
  DummySaver();

public:
  bool save(const SellerGoods& sgoods) override;

private:
  const QString getOutputFileName(QString sellerName) const;

private:
  friend class AbstractSaver;

};

} // garsale

#endif // GARSALE_SELLERPARSER_DUMMYSAVER_H
