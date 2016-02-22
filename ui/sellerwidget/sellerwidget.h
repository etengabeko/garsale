#ifndef GARSALE_UI_SELLERWIDGET_H
#define GARSALE_UI_SELLERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
  class SellerWidget;
} // Ui

namespace garsale {

class SellerWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SellerWidget(QWidget* parent = 0);
  ~SellerWidget();

private slots:
  void slotOpenFiles();
  void slotClose();
  void slotApply();

private:
  Ui::SellerWidget* ui_;

  QStandardItemModel filesModel_;
  QStandardItemModel sellersModel_;

};

} // garsale

#endif // GARSALE_UI_SELLERWIDGET_H
