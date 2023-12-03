#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>
#include <vector>

using namespace cv;
using namespace std;
using namespace std::chrono;

// Function to convert a region of the image to grayscale
void convertToGray(const Mat& input, Mat& output, int startRow, int endRow) {
    for (int r = startRow; r < endRow; r++) {
        for (int c = 0; c < input.cols; c++) {
            const Point3_<uchar>* p = input.ptr<Point3_<uchar>>(r, c);
            uchar gray = static_cast<uchar>(p->x * 0.07 + p->y * 0.72 + p->z * 0.21);
            output.at<uchar>(r, c) = gray;
        }
    }
}

int main(int argc, char** argv) {
    auto start = high_resolution_clock::now();
    
    // Make sure correct number of input parameters was passed correctly
    if (argc != 4) {
        printf("Usage: %s <input_image> <output_image> <num_threads>\n", argv[0]);
        return -1;
    }
    
    // Load image and make sure it was loaded properly
    Mat image = imread(argv[1], IMREAD_COLOR);
    
    cout << "Loading image ..." << endl;

    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    // Image dimensions
    cout << "Rows (height):" << image.rows << " Cols (width):" << image.cols << endl;

    Mat newImage(image.rows, image.cols, CV_8UC1, cv::Scalar(0));

    cout << "Start conversion ..." << endl;

    // Number of threads to use
    int numThreads = atoi(argv[3]);

    // Vectore to store threads
    vector<thread> threads;
    
    // In practice, this is how big the divisions are going to be
    // In an image of 1000 rows and 2 threads, the image is going to be divided
    // into two sub-images of 500 rows each
    int rowsPerThread = image.rows / numThreads;

    for (int i = 0; i < numThreads; i++) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? image.rows : (i + 1) * rowsPerThread;
        threads.emplace_back(convertToGray, ref(image), ref(newImage), startRow, endRow);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    cout << "End conversion ..." << endl;

    imwrite(string(argv[2]), newImage);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    float duration2 = duration.count() / 1000000.0f;
    cout << "Total time spent in seconds is " << duration2 << endl;


    return 0;
}
