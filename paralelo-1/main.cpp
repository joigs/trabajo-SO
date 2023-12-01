#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <chrono>
using namespace cv;
using namespace std;
using namespace std::chrono;

Mat conversion(Mat image){
    // cout << "Rows (height):" << image.rows << " Cols (width):" << image.cols << endl;
    
    Mat newImage(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    cout << "Start conversion ..." << endl;
    // read each pixel (RGB pixel)
    for (int r = 0; r < image.rows; r++) {
        for (int c = 0; c < image.cols; c++) {
            Point3_<uchar>* p = image.ptr<Point3_<uchar>>(r, c);
            // 0.07 B + 0.72 G + 0.21 R.
            uchar gray = static_cast<uchar>(p->x * 0.07 + p->y * 0.72 + p->z * 0.21);
            // x:B y:G z:R
            newImage.at<uchar>(r, c) = gray;
        }
    }
    return newImage;
}

int main(int argc, char** argv) {
    auto start = high_resolution_clock::now();
    // read the image file.
    //(B,G,R)
    if ( argc != 3 )
    {
        printf("Input Error");
        return -1;
    }

    Mat image;
    image = imread( argv[1], IMREAD_COLOR );

    cout << "Loading image ..." << endl;

    // check if the image was successfully loaded
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    
    cout << "Rows (height):" << image.rows << " Cols (width):" << image.cols << endl;
    // Divide image in 2
    // Obtain image dimensions
    int rows = image.rows;
    int cols = image.cols;
    
    // Define area of interest
    Rect leftRect(0, 0, cols/2, rows);
    Rect rightRect(cols/2, 0, cols/2, rows);
    // Create images using area of interest
    Mat leftHalf = image(leftRect);
    Mat rightHalf = image(rightRect);
    
    // Create vectors to store threads and results
    std::vector<std::thread> threads;
    std::vector<Mat> images(2);
    
    // Create threads, pass the function, arguments, and store the result
    threads.emplace_back([&images, leftHalf]() { images[0] = conversion(leftHalf);});
    threads.emplace_back([&images, rightHalf]() { images[1] = conversion(rightHalf);});
    
    // Wait for both threads to finish
    for (auto& t : threads) {
        t.join();
    }
    
    Mat newImage;
    hconcat(images[0], images[1], newImage);
    
    cout << "End conversion ..." << endl;

    imwrite(string(argv[2]), newImage);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    float duration2 = duration.count() / 1000000.0f;
    cout << "Total time spent in seconds is " << duration2 << endl;

    namedWindow(argv[2], WINDOW_AUTOSIZE );
    imshow(argv[2], newImage);
    
    waitKey(0);
    
    return 0;
}
