#ifndef GARSALE_SELLERPARSER_SELLERIMPORTER_H
#define GARSALE_SELLERPARSER_SELLERIMPORTER_H

class QString;

namespace garsale {

class SellerImporter
{
  public:
    SellerImporter();
    
    bool importFromFile(const QString& filename);

};

} // garsale

#endif // GARSALE_SELLERPARSER_SELLERIMPORTER_H
