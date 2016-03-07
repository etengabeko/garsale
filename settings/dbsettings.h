#ifndef GARSALE_SETTINGS_DBSETTINGS_H
#define GARSALE_SETTINGS_DBSETTINGS_H

#include "settings.h"

class QString;

namespace garsale
{
class DbSettings : public Settings
{
public:
  DbSettings() = default;

  bool isNull() const;

  QString type() const;
  QString host() const;
  QString name() const;
  QString user() const;
  QString password() const;

  void setType(const QString& type);
  void setHost(const QString& host);
  void setName(const QString& name);
  void setUser(const QString& user);
  void setPassword(const QString& passwd);

};

} // garsale

#endif // GARSALE_SETTINGS_DBSETTINGS_H
