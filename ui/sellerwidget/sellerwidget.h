#ifndef GARSALE_UI_SELLERWIDGET_H
#define GARSALE_UI_SELLERWIDGET_H

#include <QtWidgets/QWidget>

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

private:
  Ui::SellerWidget* ui_;

};

} // garsale

#endif // GARSALE_UI_SELLERWIDGET_H
