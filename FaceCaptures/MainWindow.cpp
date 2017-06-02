#include "MainWindow.h"
#include "qdebug.h"

MainWindow::MainWindow()
{
	init();
	resize(300, 200);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi(){
	openCam = new QPushButton(this);
	openCam->setText("Face Capture");
	openCam->setGeometry(100, 40, 100, 50);

	eigenBtn = new QPushButton(this);
	eigenBtn->setText("Recognition Face");
	eigenBtn->setGeometry(100, 100, 100, 50);

}

//part of face detection 
int MainWindow::faceDetection(Mat frame){
	std::vector<cv::Rect> faces;
	Mat temp = frame.clone();
	Mat frameGray;
	cvtColor(frame, frameGray, CV_BGR2GRAY);
	equalizeHist(frameGray, frameGray);
	faceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));

	for (size_t i = 0; i < faces.size(); i++){
		Mat faceROI = frameGray(faces[i]);
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		rectangle(temp,
			cv::Point(faces[i].x, faces[i].y),
			cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
			cv::Scalar(0, 255, 0),
			4,
			8,
			0);
	}
	if (faces.size() > 0){
		//imgROI = faceROI.clone();
		imgROI = frame(Rect(faces[0].x,faces[0].y, faces[0].width,faces[0].height));
		return 1;
	}
	return 0;
}
void MainWindow::captureImage(Mat frame){
	string fileName;
	// new file system to record the data of images
	fileName = saveFile(frame);
	if (fileName == ""){
		qDebug() << "fileName is empty";
		return;
	}
	//change file name//
	file.open("DataBase\\test.txt", fstream::app);
	file << fileName << "\n";
	file.close();
}
void MainWindow::openCamera(){
	CvCapture* capture;
	Mat frame;
	Mat temp;
	if (!faceCascade.load(face_cascade_name)){
		std::cout << "Error!!!" << std::endl;
		return;
	}
	std::cout << "pass" << std::endl;
	//-- 2. Read the video stream
	capture = cvCaptureFromCAM(0);
	if (capture)
	{
		while (true)
		{
			frame = cvQueryFrame(capture);
			//-- 3. Apply the classifier to the frame
			if (!frame.empty())
			{
				std::vector<cv::Rect> faces;
				Mat frameGray;
				temp = frame.clone();
				cvtColor(frame, frameGray, CV_BGR2GRAY);
				equalizeHist(frameGray, frameGray);
				faceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
				for (size_t i = 0; i < faces.size(); i++){
					Mat faceROI = frameGray(faces[i]);
					Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
					rectangle(frame,
						cv::Point(faces[i].x, faces[i].y),
						cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
						cv::Scalar(0, 255, 0),
						4,
						8,
						0);
				}
				cv::namedWindow("Face capture");
				cv::imshow("Face capture", frame);
			}
			else
			{
				break;
			}
			int c = cv::waitKey(5);
			if ((char)c == 'x') {
				cvReleaseCapture(&capture);
				cvDestroyAllWindows();
				break;

			}
			if ((char)c == 'c'){
				int check;
				check = faceDetection(frame);
				if (check == 1){
					captureImage(temp);
				}
				else{
					wd.show();
				}
			}
		}
	}
}

// part to find eigen vector
string MainWindow::saveFile(Mat frame){
	QString filters("Images files (*.jpg)");
	QString defaultFilter("Images files (*.jpg)");
	/* Static method approach */
	QString fn = QFileDialog::getSaveFileName(this, "Save file", "C:\\Users\\kanan_000\\Documents\\Visual Studio 2013\\Projects\\FaceCaptures\\FaceCaptures\\DataBase",
		filters, &defaultFilter);

	if (fn.isEmpty()){
		qDebug() << "fileName is empty";
		return "";
	}
	if (!(fn.endsWith(".jpg", Qt::CaseInsensitive)))
		fn += ".jpg"; // default
	Size size(283, 283);
	cv::resize(imgROI, imgROI, size);
	imwrite(fn.toStdString(), imgROI);
	imgROI.release();
	qDebug() << fn; 
	eigenFaceInDatabase.clear();
	eigenFaceInDatabase = findEigen();
	for (int i = 0; i < eigenFaceInDatabase.size(); i++){
		matrixOfEigenFace.push_back(normalizeToUnit(combineVectorMatToMat(eigenFaceInDatabase.at(i))));
	}
	return fn.toStdString().substr(92);

}
//load image from txt file & return in vector of Matrix
vector<Mat> MainWindow::loadImage(){
	file.open("DataBase\\test.txt", ios::in);
	vector<string> picsName;
	string line;
	if (file.is_open()){
		while (getline(file, line)){
			picsName.push_back(line);
		}
		file.close();
	}
	else{
		qDebug() << "Can not open file";
	}


	vector<Mat> pics;
	for (int i = 0; i < picsName.size(); i++){
		string imageName = "DataBase\\" + picsName.at(i);
		pics.push_back(imread(imageName, IMREAD_GRAYSCALE));

	}

	return pics;

}
vector<vector<Mat>> MainWindow::findEigen(){
	vector<Mat> imageList = loadImage();
	vector<vector<float>> allArray;
	vector<vector<vector<float>>> imageVectors;
	int count = 1;
	vector<float> vectorImage;
	for (int i = 0; i < imageList.size(); i++){
		Mat temp = imageList.at(i);
		vectorImage = convertImageMatToVector(temp);

		allArray.push_back(vectorImage);
		if (count == 4){
			//qDebug() << count;
			imageVectors.push_back(allArray);
			allArray.clear();
			count = 0;
		}
		count++;
	}
	vector<Mat> matrixs;
	vector<Mat> eigenVectors;
	vector<Mat> eigenValues;
	vector<Mat> covarianceMatrix;
	cout << "Number of person: " << imageVectors.size() << endl;
	for (int i = 0; i < imageVectors.size(); i++){
		cout << "Number of Image per person: " << imageVectors.at(i).size() << endl;
		//convert Vector to Matrix
		//Matrix A transpose;
		/////////////////rows///////////////cols///////////////////////float type///
		Mat At(imageVectors.at(i).size(), imageVectors.at(i).at(0).size(), CV_32F);
		for (int j = 0; j < imageVectors.at(i).size(); j++){
			for (int k = 0; k < imageVectors.at(i).at(j).size(); k++){
				At.at<float>(j, k) = imageVectors.at(i).at(j).at(k);
			}
		}
		//create matrix A 
		Mat A = Mat(At.rows, At.cols, CV_32F);
		A = At.t();
		matrixs.push_back(A);
		At.release();
	}
	for (int i = 0; i < matrixs.size(); i++){
		Mat At = matrixs.at(i).t();
		Mat result = Mat(matrixs.at(i).cols, At.rows, CV_32F);
		cout << "Person" << i + 1 << endl;
		cout << "Row of the matrix multiply matrix transpone:" << result.rows << endl;
		cout << "Col of the matrix multiply matrix transpone:" << result.cols << endl;
		//find Covariance matrix
		result = matrixs.at(i).t()*matrixs.at(i);
		cout << "Result of A x At = " << endl << " " << result << endl << endl;
		Mat eigenvalue, eigenvector;
		//add to list covariance matrix
		covarianceMatrix.push_back(result);
		//find eigen value and eigen vector
		eigen(result, true, eigenvalue, eigenvector);
		cout << "eigenvalue" << eigenvalue << endl;
		cout << "eigenvector" << eigenvector << endl;
		//add to list of eigen vector and list eigen value
		eigenVectors.push_back(eigenvector);
		eigenValues.push_back(eigenvalue);
		eigenvalue.release();
		eigenvector.release();
		result.release();
	}
	//keep 3 diamension first keep number matrix second and third keep eigenVector
	vector<vector<Mat>> eigenVectorsList;
	vector<Mat> ev;
	Mat temp;

	for (int i = 0; i < eigenVectors.size(); i++){
		for (int j = 0; j < eigenVectors.at(i).rows; j++){
			//copy j row
			temp = eigenVectors.at(i).row(j);
			ev.push_back(temp);
			//cout <<"test temp" <<temp << endl;
		}
		eigenVectorsList.push_back(ev);
		ev.clear();
	}

	//find eigen face
	Mat eigenFace;
	vector<Mat> tempEigenFace;
	vector<vector<Mat>> eigenFaceOfPerson;
	for (int i = 0; i < eigenVectorsList.size(); i++){
		for (int j = 0; j < eigenVectorsList.at(i).size(); j++){
			eigenFace = matrixs.at(i) * eigenVectorsList.at(i).at(j).t();
			tempEigenFace.push_back(eigenFace);
			eigenFace.release();
		}
		eigenFaceOfPerson.push_back(tempEigenFace);
		tempEigenFace.clear();
	}

	// this line below just show the eigen Face image (not important)

	//vector<vector<Mat>> showImage = normaliseRange0To255(eigenFaceOfPerson);
	//vector<vector<vector<int>>> listVectorImage;
	//vector<vector<int>> image1Person;
	//vector<float> image1Col;
	//vector<int> image1ColInt;

	////convert from Mat 1x80,089 to vector 
	//for (int i = 0; i < showImage.size(); i++){
	//	//person
	//	for (int j = 0; j < showImage.at(i).size(); j++){
	//		//image per person
	//		image1Col = convertImageMatToVector(showImage.at(i).at(j));
	//		//convert vector float to int 
	//		image1ColInt = convertVectorFloatToVectorInt(image1Col);
	//		image1Person.push_back(image1ColInt);
	//	}
	//	listVectorImage.push_back(image1Person);
	//	image1Person.clear();
	//	image1Col.clear();
	//}

	//vector<Mat> listEigenImage;
	//for (int i = 0; i < listVectorImage.size(); i++){
	//	//person
	//	for (int j = 0; j < listVectorImage.at(i).size(); j++){
	//		//per image
	//		Mat eigenFaceImage(283, 283, CV_8UC1);
	//		int rowOFImage = 0;
	//		int colOfImage = 0;
	//		int countCols = 0;
	//		for (int k = 0; k < listVectorImage.at(i).at(j).size(); k++){
	//			eigenFaceImage.at<uchar>(rowOFImage, colOfImage) = listVectorImage.at(i).at(j).at(k);
	//			colOfImage++;
	//			countCols++;
	//			if (countCols == 283){
	//				rowOFImage++;
	//				colOfImage = 0;
	//				countCols = 0;
	//			}
	//		}
	//		//namedWindow("TEST");
	//		//imshow("TEST", eigenFaceImage);
	//		//waitKey();
	//		listEigenImage.push_back(eigenFaceImage);
	//		eigenFaceImage.release();
	//	}
	//}
	//show image
	/*for (int i = 0; i < listEigenImage.size(); i++){
	String name = cv::format("Example%d", i);
	namedWindow(name);
	imshow(name, listEigenImage.at(i));
	}
	waitKey();*/

	return eigenFaceOfPerson;
}

//part of face recognition
vector<float> MainWindow::convertImageMatToVector(Mat image){
	// convert mat from uchar mat to float mat
	image.convertTo(image, CV_32FC1);
	vector<float> imageVector;
	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			imageVector.push_back(image.at<float>(i, j));
		}
	}
	return imageVector;
}
vector<int> MainWindow::convertVectorFloatToVectorInt(vector<float> input){
	vector<int> intVector;
	for (int i = 0; i < input.size(); i++){
		intVector.push_back(static_cast<int>(input.at(i)));
	}
	return intVector;
}
Mat MainWindow::normaliseRange0To255(Mat eigenface){ //normalise range of data in 0 - 255 (Matrix)
	float max, min, a, b;
	max = eigenface.at<float>(0, 0);
	min = eigenface.at<float>(0, 0);
	for (int i = 0; i < eigenface.rows; i++){ //find max and min of data
		for (int j = 0; j < eigenface.cols; j++){
			if (max < eigenface.at<float>(i, j))
				max = eigenface.at<float>(i, j);
			if (min > eigenface.at<float>(i, j))
				min = eigenface.at<float>(i, j);
		}
	}

	a = 255 / (max - min); //assign "a" by range of data
	eigenface *= a; //multiply all data with "a"

	min = eigenface.at<float>(0, 0); //new min's default
	for (int i = 0; i < eigenface.rows; i++){  //find a new min
		for (int j = 0; j < eigenface.cols; j++){
			if (min > eigenface.at<float>(i, j))
				min = eigenface.at<float>(i, j);
		}
	}

	b = -min; //assign "b" by new min
	eigenface += b; //add all data with "b"

	return eigenface;
}
vector<vector<Mat>> MainWindow::normaliseRange0To255(vector<vector<Mat>> eigenFaceOfPerson){ //normalise range of all data in 0 - 255 (vector of vector of Matrix)
	//normalise each matrix into range of 0 - 255 with normaliseRange0To255(Mat eigenface)
	for (int i = 0; i < eigenFaceOfPerson.size(); i++){
		for (int j = 0; j < eigenFaceOfPerson.at(i).size(); j++){
			normaliseRange0To255(eigenFaceOfPerson.at(i).at(j));
		}
	}
	return eigenFaceOfPerson;
}
Mat MainWindow::normalizeToUnit(Mat y){ //normalize input "y" into unit vector
	float sq_sum = 0;
	for (int i = 0; i < y.rows; i++){
		for (int j = 0; j < y.cols; j++){
			sq_sum += pow(y.at<float>(i, j), 2);
		}
	}
	return y / sqrt(sq_sum);
}
float MainWindow::findMagnitude(Mat m){ //find magnitude(||m||) of matrix
	float sq_sum = 0;
	for (int i = 0; i < m.rows; i++){
		for (int j = 0; j < m.cols; j++){
			sq_sum += pow(m.at<float>(i, j), 2);
		}
	}
	//qDebug() << sqrt(sq_sum);
	return sqrt(sq_sum);
}
Mat MainWindow::findX(Mat A, Mat y){ //find "x" that is transpose of "a" multiply with input "y"
	return A*y;
}
Mat MainWindow::combineVectorMatToMat(vector<Mat> listVector){
	Mat result = Mat(4, 80089, CV_32F);

	for (int i = 0; i < result.rows; i++){ // 4
		for (int j = 0; j < result.cols; j++){ // 80089
			result.at<float>(i, j) = listVector.at(i).at<float>(j, 0);
		}
	}
	return result;
}
Mat MainWindow::convertVectorImageToSingleMat(Mat input){
	Mat result = Mat(80089,1,CV_32FC1);
	int row = 0;
	int col = 0;
	for (int i = 0; i < result.rows; i++){
		result.at<float>(i, 0) = input.at<float>(row, col);
		col++;
		if (col == 283){
			row++;
			col = 0;
		}
	}
	return result;
}
int MainWindow::findMax(vector<float> result){
	float max = result.at(0);
	int index = 0;
	for (int i = 0; i < result.size(); i++){
		if (max < result.at(i)){
			index = i;
			max = result.at(i);
		}
	}
	return index;
}
float MainWindow::findDiff(vector<float> result){
	float max = result.at(0);
	float min = result.at(0);
	float diff = 0;
	for (int i = 0; i < result.size(); i++){
		if (max < result.at(i)){
			max = result.at(i);
		}
		if (min > result.at(i)){
			min = result.at(i);
		}
	}
	diff = max - min;
	return diff;
}
void MainWindow::calculate(Mat input){
	vector<float> result;
	for (int i = 0; i < matrixOfEigenFace.size(); i++){
		float temp;
		temp = findMagnitude(findX(matrixOfEigenFace.at(i), input));
		result.push_back(temp);
	}
	if (result.size() > 1){
		float diff = findDiff(result);
		if (diff < 0.025){
			qDebug() << "not found in database";
			text = "Not found in database";
		}
		else{
			qDebug() << "person" << findMax(result);
			text = "Person: " + to_string(findMax(result));
			
		}
	}
	else if (result.size() == 1){
		if (result.at(0) >= 0.8){
			qDebug() << "person" << 0;
		}
		else{
			qDebug() << "not found in database";
		}
	}
	else{
		qDebug() << "database is empty";
	}

	
}
void MainWindow::projectionInputImage(){
	// projection input image //
	Size size(283, 283);
	cv::resize(imgROI, imgROI, size);
	Mat convertInput;
	cv::cvtColor(imgROI, convertInput, CV_BGR2GRAY);

	//namedWindow("TEST");
	//cv::imshow("TEST", convertInput);
	convertInput.convertTo(convertInput, CV_32FC1); 

	Mat frame = normalizeToUnit(convertInput);
	//qDebug() << frame.at<float>(0, 0);
	Mat singleRow = convertVectorImageToSingleMat(frame);
	//qDebug() << singleRow.at<float>(0, 0);
	calculate(singleRow);
}
void MainWindow::recognition(){
	// open cam and detect face and send frame to rocognization 
	CvCapture* capture;
	Mat frame;
	Mat temp;
	if (!faceCascade.load(face_cascade_name)){
		std::cout << "Error!!!" << std::endl;
		return;
	}
	std::cout << "pass" << std::endl;
	//-- 2. Read the video stream
	capture = cvCaptureFromCAM(0);
	if (capture)
	{
		while (true)
		{
			frame = cvQueryFrame(capture);
			//-- 3. Apply the classifier to the frame
			if (!frame.empty())
			{
				std::vector<cv::Rect> faces;
				Mat frameGray;
				temp = frame.clone();
				cvtColor(frame, frameGray, CV_BGR2GRAY);
				equalizeHist(frameGray, frameGray);
				faceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
				Mat faceROI;
				for (size_t i = 0; i < faces.size(); i++){
					cv::Point textOrg(faces[i].x, faces[i].y - 10);
					cv::putText(frame, text, textOrg, fontFace, fontScale, Scalar(0,0,255), thickness, 8);
					faceROI = frameGray(faces[i]);
					Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
					rectangle(frame,
						cv::Point(faces[i].x, faces[i].y),
						cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
						cv::Scalar(0, 255, 0),
						4,
						8,
						0);
				}
				cv::namedWindow("Face recognition");
				cv::imshow("Face recognition", frame);
			}
			else
			{
				break;
			}
			int check;
			check = faceDetection(frame);
			if (check == 1){
				projectionInputImage();
			}
			int c = cv::waitKey(5);
			if ((char)c == 'x') {
				cvReleaseCapture(&capture);
				cvDestroyAllWindows();
				break;
			}
		}
	}
}

//init of ui and value
void MainWindow::setupSignalAndSlot(){
	connect(openCam, SIGNAL(clicked()), this, SLOT(openCamera()));
	connect(eigenBtn, SIGNAL(clicked()), this, SLOT(recognition()));
}
void MainWindow::init(){
	setupUi();
	setupSignalAndSlot();
	eigenFaceInDatabase = findEigen();
	for (int i = 0; i < eigenFaceInDatabase.size(); i++){
		matrixOfEigenFace.push_back(normalizeToUnit(combineVectorMatToMat(eigenFaceInDatabase.at(i))));
	}
}