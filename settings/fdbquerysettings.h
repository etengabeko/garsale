#ifndef GARSALE_SETTINGS_FDBQUERYSETTINGS_H
#define GARSALE_SETTINGS_FDBQUERYSETTINGS_H

#include "settings.h"

class QString;
class QStringList;

namespace garsale
{
class FirebirdQuerySettings : public Settings
{
public:
  FirebirdQuerySettings() = default;

  bool isNull() const;
  QStringList keys() const;

  QString query(const QString& key) const;

  QString selectSellerQuery() const;
  QString selectGoodsQuery() const;

  QString insertSellerQuery() const;
  QString insertDocumentQuery() const;
  QString insertGoodQuery() const;
  QString insertDocContentQuery() const;
  QString insertStoreQuery() const;

  static QString selectSellerKey();
  static QString selectGoodsKey();
  static QString insertSellerKey();
  static QString insertDocumentKey();
  static QString insertGoodKey();
  static QString insertDocContentKey();
  static QString insertStoreKey();

};

} // garsale

#endif // GARSALE_SETTINGS_FDBQUERYSETTINGS_H
