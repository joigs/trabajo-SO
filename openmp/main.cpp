#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <omp.h>
using namespace cv;
using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {
    auto start = high_resolution_clock::now();
    // read the image file.
    //(B,G,R)
    if ( argc != 4 )
    {
        printf("Input Error");
        return -1;
    }
    Mat image;
    image = imread( argv[1], IMREAD_COLOR );

    int n_threads = atoi(argv[3]);
    int r;
    int c;
    uchar gray;

    cout << "Loading image ..." << endl;

    // check if the image was successfully loaded
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    cout << "Rows (height):" << image.rows << " Cols (width):" << image.cols << endl;

    Mat newImage(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    cout << "Start conversion ..." << endl;
    // read each pixel (RGB pixel)

    #pragma omp parallel num_threads(n_threads)
    {
        #pragma omp for private(c,gray)
        for (r = 0; r < image.rows; r++) {
            for (c = 0; c < image.cols; c++) {
                Point3_<uchar>* p = image.ptr<Point3_<uchar>>(r, c);
                // 0.07 B + 0.72 G + 0.21 R.
                gray = static_cast<uchar>(p->x * 0.07 + p->y * 0.72 + p->z * 0.21);
                // x:B y:G z:R
                newImage.at<uchar>(r, c) = gray;
            }
        }
    }
    cout << "End conversion ..." << endl;

    imwrite(string(argv[2]), newImage);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    float duration2 = duration.count() / 1000000.0f;
    cout << "Total time spent in seconds is " << duration2 << endl;

    return 0;
}
