int main()
{
	vector<pair<int, string>> v;
	
	v.push_back(pair<int ,string>(3, "Dok2"));
	v.push_back(pair<int, string>(6, "DMask"));
	v.push_back(pair<int, string>(1, "Tiger JK"))
	v.push_back(pair<int, string>(4, "Block"))
	v.push_back(pair<int, string>(2, "banana"))
	v.push_back(pair<int, string>(2, "apple"))
	
	cout <<"=== After sort ==="<<endl;
	
	vector<pair<int, string>>::iterator iter;
	for(iter = v.begin(); iter!=v.end(); iter++)
	{
		cout << "[" <<iter->first <<", "<<iter->second<<"]"<<endl;
	}
	
	cout <<endl;
	sort(v.begin(), v.end());
	
	cout<<"=== After sort ==="<<endl;
	
	for(iter = v.begin(); iter!=v.end(); iter++)
	{
		cout <<"[" <<iter->first<<", "<<iter->second<<"]"<<endl;
	}
}#include<opencv2/opencv.hpp>
#include<iostream>
#include<sstream>
#include <Windows.h> ////엔터시 문자열 출력 유지 용도로 sleep함수 사용, 렉이 걸리긴함. time에 대한 함수 사용법을 간파하지 못해서 대신 사용
using namespace std;  ////using std::right; using std::setw; 오른쪽정렬
using namespace cv;

//방법1.

//반복문으로 각 화소 모두 비교하는 방법

Mat getHandMask1(const Mat& image, int minCr, int maxCr, int minCb, int maxCb) {

	//컬러 공간 변환 BGR->YCrCb

	Mat YCrCb;

	cvtColor(image, YCrCb, COLOR_BGR2YCrCb);



	//각 채널별로 분리

	vector<Mat> planes;

	split(YCrCb, planes);



	//각 채널별로 화소마다 비교

	Mat mask(image.size(), CV_8U, Scalar(0));   //결과 마스크를 저장할 영상

	int nr = image.rows;    //전체 행의 수

	int nc = image.cols;

	for (int i = 0; i < nr; i++) {

		uchar* CrPlane = planes[1].ptr<uchar>(i);   //Cr채널의 i번째 행 주소

		uchar* CbPlane = planes[2].ptr<uchar>(i);   //Cb채널의 i번째 행 주소

		for (int j = 0; j < nc; j++) {

			if ((minCr < CrPlane[j]) && (CrPlane[j] < maxCr) && (minCb < CbPlane[j]) && (CbPlane[j] < maxCb))

				mask.at<uchar>(i, j) = 255;
		}
	}
	return mask;
}



//방법2.

//비교연산자, 논리 연산자를 이용한 방법

Mat getHandMask2(const Mat& image, int minCr, int maxCr, int minCb, int maxCb) {

	//컬러 공간 변환 BGR->YCrCb

	Mat YCrCb;

	cvtColor(image, YCrCb, COLOR_BGR2YCrCb);



	//각 채널별로 분리

	vector<Mat> planes;

	split(YCrCb, planes);



	Mat mask = (minCr < planes[1]) & (planes[1] < maxCr) & (minCb < planes[2]) & (planes[2] < maxCb);



	return mask;

}

//손바닥의 중심점과 반지름 반환

//입력은 8bit 단일 채널(CV_8U), 반지름을 저장할 double형 변수

Point getHandCenter(const Mat& mask, double& radius) {

	//거리 변환 행렬을 저장할 변수

	Mat dst;

	distanceTransform(mask, dst, DIST_L2, 5); //결과는 CV_32SC1 타입


	//거리 변환 행렬에서 값(거리)이 가장 큰 픽셀의 좌표와, 값을 얻어온다.

	int maxIdx[2]; //좌표 값을 얻어올 배열(행, 열 순으로 저장됨)

	minMaxIdx(dst, NULL, &radius, NULL, maxIdx, mask); //최소값은 사용 X


	return Point(maxIdx[1], maxIdx[0]);

}

//손목을 제거하지 않은 상태에서 손가락 개수 세기

int getFingerCount(const Mat& mask, Point center, double radius, double scale = 2.0) {

	//손가락 개수를 세기 위한 원 그리기

	Mat cImg(mask.size(), CV_8U, Scalar(0));

	circle(cImg, center, radius * scale, Scalar(255));


	//원의 외곽선을 저장할 벡터

	vector<vector<Point>> contours;

	findContours(cImg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);



	if (contours.size() == 0)   //외곽선이 없을 때 == 손 검출 X

		return -1;



	//외곽선을 따라 돌며 mask의 값이 0에서 1로 바뀌는 지점 확인

	int fingerCount = 0;

	for (int i = 1; i < contours[0].size(); i++) {

		Point p1 = contours[0][i - 1];

		Point p2 = contours[0][i];

		if (mask.at<uchar>(p1.y, p1.x) == 0 && mask.at<uchar>(p2.y, p2.x) > 1)
		{
			fingerCount++;
		}
	}


	//손목과 만나는 개수 1개 제외

	return fingerCount - 1;

}


void main()
{

	VideoCapture cap(0); //카메라 성능에 따라 색을 잘 구분,

	if (!cap.isOpened())
	{
		cerr << "Can't open the cam";
		return;
	}
	Mat frame;
	Mat frame_src;
	int minCr = 128; int maxCr = 170; int minCb = 73; int maxCb = 158;

	int flag = 0;

	clock_t start = clock();
	double ptime3 = 3; //timer 3초용
	double ptime2 = 2; //timer 2초용.
	double ptime1 = 1; //1초용

	double a, b; //고정좌표용

	string zero = "0";
	string one = "1";
	string two = "2";
	string three = "3";
	string four = "4";
	string five = "5";
	string six = "6";
	string seven = "7";
	string eight = "8";
	string nine = "9";
	string clr = "clr";
	string enter = "enter";

	string result_num = "";


	int Pnum = 1;//Pointnum

	int minusPoint = 0; // 문자열에 문자 수만큼 좌표를 왼쪽으로 밀려는 용도 .result_num은 문자열로 문자가 들어올때마다 사이즈가 증가, 이를 minusPoint에 할당해서 minusPoint 변수가 result_num문자열 사이즈를 가짐, 사이즈값은 값입력 시 계속증가.

	while (1)
	{
		cap >> frame;
		frame_src = frame.clone();



		//add(dialPadIMG, frame_src, frame_src, noArray(), -1);



		//getHandMask2(frame_src, minCr, maxCr, minCb, maxCb);
		Mat mask = getHandMask1(frame_src, minCr, maxCr, minCb, maxCb);  //스킨 컬러로 피부색 검출
		Mat handImage(frame_src.size(), CV_8UC3, Scalar(0));

		add(frame_src, Scalar(0), handImage, mask);

		//imshow("mask1", mask);
		/*손바닥 중심점 좌표*/
		erode(mask, mask, Mat(3, 3, CV_8U, Scalar(1)), Point(-1, -1), 2);
		//imshow("mask2", mask);
		double radius;  // = 3.5;

		Point center = getHandCenter(mask, radius);

		//cout << "손바닥 중심점 좌표:" << center << ", 반지름:" << radius << endl;


		//손바닥 중심점 그리기

		circle(frame_src, center, 2, Scalar(0, 255, 0), -1);


		//손바닥 영역 그리기

		circle(frame_src, center, (int)(radius + 0.5), Scalar(255, 0, 0), 2);
		//cout<<getFingerCount(mask, center, radius);

		Mat hand_dst = frame.clone();// mask를 이진화해서 hand_dst에 저장할 목적.
		threshold(mask, hand_dst, 100, 255, THRESH_BINARY); //  이진화적용 이거 안하면 에러남, contours는 이진화 전제하에 이뤄짐.


		vector<vector<Point>> contours;
		findContours(hand_dst, contours, noArray(), RETR_TREE, CHAIN_APPROX_SIMPLE);
		if (contours.size() < 1)
			continue;  //에러방지
		int maxK = 0;
		double maxArea = contourArea(contours[1]); //[0], [1],...뭔지 파악.

		for (int k = 0; k < contours.size(); k++)
		{
			double area = contourArea(contours[k]);
			if (area > maxArea)
			{
				maxK = k;
				maxArea = area;
			}
			else continue;  // 작아도 계속 하게 하기. 별도  설정
		}
		vector<Point> handContour = contours[maxK]; // hand_dst에서 찾은 contours를 handContour 좌표들로 저장.

		vector<int>hull;
		convexHull(handContour, hull);

		vector<Point>ptsHull;
		for (int k = 0; k < hull.size(); k++)
		{
			int i = hull[k];
			ptsHull.push_back(handContour[i]);
			if (hull[k] < 1)  //hull 이 없어도 continue하기 , 별도설정.
				continue;
		}

		drawContours(frame_src, vector<vector<Point>>(1, ptsHull), 0, Scalar(255, 0, 0), 2);
		//ptsHull에 담겨진건 handContour들.  hull[k]는 ptsHull의 인덱스 .. ptsHull을 frame_Src영상에 적용
		//57라인

		vector<Vec4i> defects; //https://thebook.io/006939/ch12/02/01-02/ vec4i란.

		convexityDefects(handContour, hull, defects); //ptsHull 이아닌 hull을써야함.

		Point ptEnd;
		Point ptFar;
		for (int k = 0; k < defects.size(); k++) // defects.size()를 5로 바꿨음
		{
			Vec4i v = defects[0]; //k , 0이나 특정 숫자 쓰면 포인트점이 1나 나옴.
		//	Point ptStart = handContour[v[0]];
			ptEnd = handContour[v[1]];
			//ptFar = handContour[v[2]];

			circle(frame_src, ptEnd, 3, Scalar(0, 0, 255), 3, 8);
			//circle(frame_src, ptFar, 3, Scalar(0, 0, 255), 2, 8);
			if (1 > ptEnd.x || ptEnd.x > frame_src.cols || ptEnd.y < 1 || ptEnd.y >frame_src.rows)
				continue; //에러방지
			if (1 > ptFar.x || ptFar.x > frame_src.cols || ptFar.y < 1 || ptFar.y >frame_src.rows)
				continue; //에러방지
		}



		if (flag == 0)
		{
			clock_t end = clock();
			double time = double(end - start) / CLOCKS_PER_SEC; //초단위 변환

			if (time == ptime3 || time > ptime3) //핵심, time == p 만하면, time이 너무 빨라 3.000이 안나올수도있고 인지가 안됨 그래서 time > p를 조건으로 추가
			{
				ptime3 += 3; //3초가 지날때마다 p가 3씩 증가  

				if (0 < center.x && center.x < frame_src.cols && 0 < center.y && center.y < frame_src.rows)
				{
					a = center.x;
					b = center.y;
					//3초
					if (center.x)
						cout << "delay:3 sec" << endl;
					flag = 1;
					// 3초여야 함, 근데 시간이 켜지고 3초지나면서 켜짐,-> 손이 특정영역에 정지한상태에서 3초지나야 함! //
				}
			} //문제점: 논리식이 틀림, 현재는 영역을 전 구간으로 정했으나, 만약 특정 구간으로 정할 시 바로 켜짐. 3초 안머물러도.... 다 끝내고 해결해보자
			if (1 > center.x || center.x > frame_src.cols || center.y < 1 || center.y >frame_src.rows)
				continue;
		}

		if (flag == 1)
		{

			rectangle(frame_src, Rect(a, b, 195, 50), Scalar(0, 0, 255), 2); //입력칸

			/*문자열 오른쪽 정렬 */
			minusPoint = result_num.size(); // 문자열길이만큼 좌측으로 이동해야 됨. 0사이즈면 0으로 이동, 1사이즈면 그 사이즈만큼 이동. 이걸 안하면 왼쪽으로 자동으로가버림.  // size_num과 대등한 변수.
			//putText(frame_src, result_num, Point((a + 180- ((minusPoint)*10)), (b + 40)), FONT_HERSHEY_PLAIN, 1.3, Scalar(255, 0, 0), 1, 8); //문제해결 ! 문자열 왼쪽으로 밀려짐.
			putText(frame_src, result_num, Point(a + 180 - (minusPoint * 12), b + 40), FONT_HERSHEY_PLAIN, 1.3, Scalar(255, 0, 0), 1, 8); //문제  ,  
			if (result_num.size() > 13)
			{
				result_num = "";  // 입력창 공간 10사이즈로 제한.
			}

			minusPoint += 5;

			rectangle(frame_src, Rect(a, b + 50, 65, 50), Scalar(0, 0, 255), 2); // 1
			putText(frame_src, one, Point(a + 25, b + 85), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);
			// a < pt.x < a+ 65    b+50< pt.y< b+50 + 50
			rectangle(frame_src, Rect(a + 65, b + 50, 65, 50), Scalar(0, 0, 255), 2); //2
			putText(frame_src, two, Point(a + 90, b + 85), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a + 130, b + 50, 65, 50), Scalar(0, 0, 255), 2);//Rect (x, y, width, height)
			putText(frame_src, three, Point(a + 155, b + 85), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a, b + 100, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, four, Point(a + 25, b + 135), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a + 65, b + 100, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, five, Point(a + 90, b + 135), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a + 130, b + 100, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, six, Point(a + 155, b + 135), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a, b + 150, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, seven, Point(a + 25, b + 185), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a + 65, b + 150, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, eight, Point(a + 90, b + 185), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a + 130, b + 150, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, nine, Point(a + 155, b + 185), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a, b + 200, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, clr, Point(a + 10, b + 230), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);

			rectangle(frame_src, Rect(a + 65, b + 200, 65, 50), Scalar(0, 0, 255), 2);//0  // (x, y) = (a+65, b+200)  width : 65/ height: 50
			putText(frame_src, zero, Point(a + 90, b + 230), FONT_HERSHEY_DUPLEX, 0.7, Scalar(255, 0, 0), 1, 8);
			//a+65 < pt.x < a + 65 + 65      b+200 < pt.y < b+ 200+ 50

			rectangle(frame_src, Rect(a + 130, b + 200, 65, 50), Scalar(0, 0, 255), 2);
			putText(frame_src, enter, Point(a + 133, b + 230), FONT_HERSHEY_DUPLEX, 0.6, Scalar(255, 0, 0), 1, 8);



			//영역 전범위에 1초 time .
			clock_t end2 = clock();
			double time = double(end2 - start) / CLOCKS_PER_SEC; //초단위 변환

			if (time == ptime1 || time > ptime1)  // 수정하기.
			{
				ptime1++;  // p1 1씩 증가, 1초용

			 // flag가 0일때도 p1 ++, flag ==1일때도 ++ 해야.

				if ((a < ptEnd.x) && (ptEnd.x < a + 65) && (b + 50 < ptEnd.y) && (ptEnd.y < b + 50 + 50)) //해결 - 조건문 성립 안되는 이유가 궁금.//초기화를 두번했음..
				{
					result_num += "1";
				}
				if ((a + 65 < ptEnd.x) && (ptEnd.x < a + 130) && (b + 50 < ptEnd.y) && (ptEnd.y < b + 50 + 50)) //해결 - 조건문 성립 안되는 이유가 궁금.//초기화를 두번했음..
				{
					result_num += "2";
				}
				if ((a + 130 < ptEnd.x) && (ptEnd.x < a + 195) && (b + 50 < ptEnd.y) && (ptEnd.y < b + 50 + 50))
				{
					result_num += "3";
				}
				if ((a < ptEnd.x) && (ptEnd.x < a + 65) && (b + 100 < ptEnd.y) && (ptEnd.y < b + 150))
				{
					result_num += "4";
				}
				if ((a + 65 < ptEnd.x) && (ptEnd.x < a + 130) && (b + 100 < ptEnd.y) && (ptEnd.y < b + 150))
				{
					result_num += "5";
				}
				if ((a + 130 < ptEnd.x) && (ptEnd.x < a + 195) && (b + 100 < ptEnd.y) && (ptEnd.y < b + 150))
				{
					result_num += "6";

				}
				if ((a < ptEnd.x) && (ptEnd.x < a + 65) && (b + 150 < ptEnd.y) && (ptEnd.y < b + 200))
				{
					result_num += "7";

				}
				if ((a + 65 < ptEnd.x) && (ptEnd.x < a + 130) && (b + 150 < ptEnd.y) && (ptEnd.y < b + 200))
				{
					result_num += "8";

				}
				if ((a + 130 < ptEnd.x) && (ptEnd.x < a + 195) && (b + 150 < ptEnd.y) && (ptEnd.y < b + 200))
				{
					result_num += "9";

				}
				if ((a < ptEnd.x) && (ptEnd.x < a + 65) && (b + 200 < ptEnd.y) && (ptEnd.y < b + 250)) //clr
				{
					result_num = "";

				}//0
				if ((a + 65 < ptEnd.x) && (ptEnd.x < a + 130) && (b + 200 < ptEnd.y) && (ptEnd.y < b + 250))
				{
					result_num += "0";

				}


				if ((a + 130 < ptEnd.x) && (ptEnd.x < a + 195) && (b + 200 < ptEnd.y) && (ptEnd.y < b + 250)) //enter // 문제2
				{

					//size_num = result_num.size(); // 3가지 문제, 시간. 문자열 정렬. 엔터누를 시 입력창 삭제 및 상단에 puttexxt로 문자열 넣기.
					putText(frame_src, result_num, Point(15, 30), FONT_HERSHEY_COMPLEX, 0.7, Scalar(0, 0, 255)); //바로 없어짐. num = "" ; 해서. 수정2
					Sleep(1000);//엔터시 문자열 출력 유지 용도.
					clock_t end3 = clock();
					double time = double(end3 - start) / CLOCKS_PER_SEC; //초단위 변환

					if (time == ptime2 || time > ptime2)
					{
						ptime2 += 2; //
						result_num = "";
					}
					//2초정도 time넣어서 입력창 초기화.  내 논리는 1초간 번호 누르면 번호가 입력창에 생기고, 엔터누르는 순간 상단에 문자열 나오고
					//2초간 enter를 누를떄, 입력창이 클리어됨
					//

				}
			}
		} //1초타임범위.

			//imshow("handIMG",handImage);
		imshow("frame_src", frame_src);
		//imshow("mask", mask);
		if (waitKey(1) == 27)
			break;
	}

}


//문제 ptEnd , 여러점 놓을 때 인지안됨 //손가락 사이 각도가 손가락끝점과 가장 먼것을 끝점으로 인지.
//문제 enter, time개념 없어서 그런듯. 엔터 시 입력창 clr 동시에 상단에 입력된거 puttext로 올려야. 
//문제 시간


//sleep함수로 해결했다. 다만, 엔터시 1초 렉걸린다. 당연한 결과.

