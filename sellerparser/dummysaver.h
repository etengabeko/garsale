#ifndef GARSALE_SELLERPARSER_DUMMYSAVER_H
#define GARSALE_SELLERPARSER_DUMMYSAVER_H

#include "abstractsaver.h"

class QString;

namespace garsale {

class SellerGoods;

class DummySaver : public AbstractSaver
{
public:
  DummySaver() = default;

  bool save(const SellerGoods& sgoods) override;

private:
  const QString getOutputFileName(QString sellerName) const;

};

} // garsale

#endif // GARSALE_SELLERPARSER_DUMMYSAVER_H
