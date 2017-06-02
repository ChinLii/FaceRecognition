#include <QPushButton>
#include <QDialog>
#include <QGridLayout>
#include <QTableWidget>
#include <QLabel>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <iostream>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <WarningDialog.h>
#include <fstream>
#include <iostream>
#include <qfiledialog.h>
#include <qdebug.h>
#include <math.h>


using namespace std;
using namespace cv;

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

	//ui
	QLabel* labelInputVideo;
	QPushButton* openCam;
	QPushButton* eigenBtn;
	WarningDialog wd;

	
	//Object
private:
	
	void setupUi();
	void setupSignalAndSlot();
	void init();
	int faceDetection(Mat frame);
	void captureImage(Mat frame);
	vector<vector<Mat>> normaliseRange0To255(vector<vector<Mat>> eigenFaceOfPerson);
	Mat normaliseRange0To255(Mat eigenface);
	vector<int> convertVectorFloatToVectorInt(vector<float> input);
	vector<float> convertImageMatToVector(Mat image);
	vector<Mat> loadImage();
	string saveFile(Mat frame);
	void projectionInputImage();
	vector<vector<Mat>> findEigen();
	Mat normalizeToUnit(Mat y);
	float findMagnitude(Mat m);
	Mat findX(Mat A, Mat y);
	Mat combineVectorMatToMat(vector<Mat> listVector);
	Mat convertVectorImageToSingleMat(Mat input);
	void calculate(Mat input);
	int findMax(vector<float> result);
	float findDiff(vector<float> result);


	//variable
	cv::CascadeClassifier faceCascade;
	std::string face_cascade_name = "C:\\OpenCV2410\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	fstream file;
	Mat imgROI;
	vector<vector<Mat>> eigenFaceInDatabase;
	vector<Mat> matrixOfEigenFace;

	//text on frame
	string text = "";
	int fontFace = FONT_ITALIC;
	double fontScale = 1;
	int thickness = 2;


private slots:
	void recognition();
	void openCamera();
	
};

