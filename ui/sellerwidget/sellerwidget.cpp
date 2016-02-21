#include "sellerwidget.h"
#include "ui_sellerwidget.h"

namespace garsale {

SellerWidget::SellerWidget(QWidget* parent) :
  QWidget(parent),
  ui_(new Ui::SellerWidget())
{
  ui_->setupUi(this);
}

SellerWidget::~SellerWidget()
{
  delete ui_;
  ui_ = 0;
}

} // garsale
