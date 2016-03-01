#ifndef GARSALE_SELLERPARSER_FIREBIRDSAVER_H
#define GARSALE_SELLERPARSER_FIREBIRDSAVER_H

#include "abstractsaver.h"

class QString;

namespace garsale {

class SellerGoods;

class FirebirdSaver : public AbstractSaver
{
public:
  FirebirdSaver() = default;

  bool save(const SellerGoods& sgoods) override;

private:

};

} // garsale

#endif // GARSALE_SELLERPARSER_FIREBIRDSAVER_H
