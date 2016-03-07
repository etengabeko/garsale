#ifndef GARSALE_SETTINGS_SETTINGS_H
#define GARSALE_SETTINGS_SETTINGS_H

#include <QVariant>

#include <memory>

class QSettings;
class QString;
class QStringList;

namespace garsale
{
class Settings
{
public:
  Settings();
  virtual ~Settings();

  Settings(Settings&& rhs) = default;
  Settings& operator= (Settings&& rhs) = default;

  bool contains(const QString& key) const;
  QString fileName() const;
  QStringList childGroups() const;

protected:
  void setValue(const QString& key, const QVariant& value);
  QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;

private:
  std::unique_ptr<QSettings> settings_;

};

} // garsale

#endif // GARSALE_SETTINGS_SETTINGS_H
