#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <qlabel.h>

class WarningDialog : public QWidget
{
	Q_OBJECT
public:
	WarningDialog();
	~WarningDialog();

private:
	QLabel* textWarning;
	QPushButton* closeBtn;

};

#endif // WARNINGDIALOG_H
