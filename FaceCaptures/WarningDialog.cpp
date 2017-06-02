#include "WarningDialog.h"

WarningDialog::WarningDialog()
{
	textWarning = new QLabel(this);
	textWarning->setText("Can not detect your face! Please try again.");
	textWarning->setGeometry(60, 30, 220, 50);

	closeBtn = new QPushButton(this);
	closeBtn->setText("Close");
	closeBtn->setGeometry(100, 90, 100, 30);

	resize(300,150);
	
	connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

WarningDialog::~WarningDialog()
{

}

