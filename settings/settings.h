#ifndef GARSALE_SETTINGS_SETTINGS_H
#define GARSALE_SETTINGS_SETTINGS_H

#include <QVariant>
#include <QString>

#include <memory>

class QSettings;
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
  QStringList childKeys(const QString& groupName = QString::null) const;

protected:
  void setValue(const QString& key, const QVariant& value);
  QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;

private:
  std::unique_ptr<QSettings> settings_;

};

} // garsale

#endif // GARSALE_SETTINGS_SETTINGS_H
