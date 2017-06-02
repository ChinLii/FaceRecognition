#include <QtWidgets/QApplication>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <QtWidgets\qlabel.h>
#include <MainWindow.h>
//version 1.1 
using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
