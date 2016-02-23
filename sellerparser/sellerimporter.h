#ifndef GARSALE_SELLERPARSER_SELLERIMPORTER_H
#define GARSALE_SELLERPARSER_SELLERIMPORTER_H

class QString;

namespace garsale {

class SellerGoods;

class SellerImporter
{
  public:
    static bool importFromFile(const QString& fileName);
    static const SellerGoods loadFromFile(const QString& fileName);
    static bool saveSellerGoods(const SellerGoods& sgoods);

};

} // garsale

#endif // GARSALE_SELLERPARSER_SELLERIMPORTER_H
