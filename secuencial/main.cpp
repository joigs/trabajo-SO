#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    // read the image file.
    //(B,G,R)
    if ( argc != 3 )
    {
        printf("Input Error");
        return -1;
    }
    Mat image;
    image = imread( argv[1], IMREAD_COLOR );

    // check if the image was successfully loaded
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat newImage(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    // read each pixel (RGB pixel)
    for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
            Point3_<uchar>* p = image.ptr<Point3_<uchar>>(r, c);
            uchar gray = static_cast<uchar>(p->x * 0.07 + p->y * 0.72 + p->z * 0.21);
            // x:B y:G z:R
            newImage.at<uchar>(r, c) = gray;
        }
    }
    // 0.07 B + 0.72 G + 0.21 R.
    imwrite(string(argv[2]), newImage);
    namedWindow(argv[2], WINDOW_AUTOSIZE );
    imshow(argv[2], newImage);
    waitKey(0);

    return 0;
}
