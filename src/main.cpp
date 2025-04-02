#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <time.h>
#include <cstdlib>
#include <string>

using namespace cv;
using namespace std;

// Global matrices for grayscale image and output image
Mat grayImage;
Mat outputImage;

// Sobel X filter for edge detection
int sobelX[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

// Structure to hold thread arguments
struct ThreadData {
    int startRow; // Starting row for the thread
    int endRow;   // Ending row for the thread
};

// Function to perform convolution operation for each thread
void* convolutionThread(void* arg) {
    // Cast the argument to ThreadData to access row range for the thread
    ThreadData* data = (ThreadData*) arg;
    
    // Loop through rows assigned to the thread
    for (int i = data->startRow; i < data->endRow; i++) {
        // Skip the first and last rows to avoid boundary issues
        if (i == 0 || i == grayImage.rows - 1)
            continue;
        
        // Loop through the columns, skipping the boundary pixels
        for (int j = 1; j < grayImage.cols - 1; j++) {
            int sum = 0;
            // Apply the 3x3 Sobel filter
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    int pixel = (int)grayImage.at<uchar>(i + m, j + n); // Get pixel value
                    sum += pixel * sobelX[m + 1][n + 1]; // Multiply by the corresponding filter value
                }
            }
            // Clamp the result to the 0-255 range (grayscale pixel range)
            sum = max(0, min(255, sum));
            outputImage.at<uchar>(i, j) = (uchar)sum; // Assign the computed value to the output image
        }
    }
    pthread_exit(NULL); // Terminate the thread
}

int main(int argc, char** argv) {
    // Check for correct number of arguments
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input_image> <num_threads>" << endl;
        return -1;
    }
    
    // Load the input image (in color)
    Mat colorImg = imread(argv[1], IMREAD_COLOR);
    if (colorImg.empty()) {
        cout << "Error: Could not open or find the image" << endl;
        return -1;
    }
    
    // Convert the image to grayscale
    cvtColor(colorImg, grayImage, COLOR_BGR2GRAY);
    // Initialize the output image with zeros (black)
    outputImage = Mat::zeros(grayImage.size(), grayImage.type());
    
    // Parse the number of threads from the command line arguments
    int numThreads = atoi(argv[2]);
    pthread_t threads[numThreads]; // Array to hold thread IDs
    ThreadData threadData[numThreads]; // Array to hold data for each thread
    
    // Calculate rows to process, excluding boundary rows
    int rowsToProcess = grayImage.rows - 2;
    // Divide the rows between threads
    int rowsPerThread = rowsToProcess / numThreads;
    int remainingRows = rowsToProcess % numThreads;
    int currentRow = 1; // Start from row 1 (skip boundary row)
    
    // Start measuring execution time
    clock_t startTime = clock();
    
    // Create threads for parallel convolution
    for (int i = 0; i < numThreads; i++) {
        // Assign the start and end row for each thread
        threadData[i].startRow = currentRow;
        int extra = (i < remainingRows) ? 1 : 0; // Distribute extra rows if any
        threadData[i].endRow = currentRow + rowsPerThread + extra;
        currentRow = threadData[i].endRow;
        
        // Create the thread and start the convolution operation
        pthread_create(&threads[i], NULL, convolutionThread, (void*)&threadData[i]);
    }
    
    // Wait for all threads to finish
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Stop measuring execution time
    clock_t endTime = clock();
    double elapsed = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    cout << "Execution Time: " << elapsed << " seconds" << endl;
    
    // Derive the output file name based on the input file name
    string inputPath = argv[1];
    size_t pos = inputPath.find_last_of("/\\"); // Get the last file separator position
    string baseName = (pos == string::npos) ? inputPath : inputPath.substr(pos + 1);
    
    // Output file name with "output_" prefix
    string outputFileName = "output_" + baseName;
    
    // Save the resulting image to file
    if (!imwrite(outputFileName, outputImage)) {
        cerr << "Error: Could not save the output image: " << outputFileName << endl;
        return -1;
    }
    cout << "Output saved as: " << outputFileName << endl;
    
    return 0; // Successful execution
}
