
# Multithreaded Convolution for Edge Detection

This project performs edge detection on images using a multithreaded convolution algorithm.

## Prerequisites

- **OpenCV 4**  
- **C++11** compliant compiler  
- **pthread** library  

Ensure that all prerequisites are installed before proceeding.

## Compilation

### Using Makefile

From the project's root directory, open a terminal and run:

```bash
make
```

### Manual Compilation

Alternatively, compile manually with:

```bash
g++ -std=c++11 -O2 -Wall -o edge_detection src/main.cpp `pkg-config --cflags --libs opencv4` -lpthread
```

## Running the Program

After a successful compilation, run the program using:

```bash
./edge_detection <input_image> <num_threads>
```

For example, to process different images with 4 threads:

```bash
./edge_detection images/1MP.jpg 4
./edge_detection images/2MP.jpg 4
./edge_detection images/4MP.jpg 4
./edge_detection images/8MP.jpg 4
./edge_detection images/16MP.jpg 4
./edge_detection images/32MP.jpg 4
```

The command takes the specified input image (e.g., `images/1MP.jpg`), processes it using the designated number of threads (e.g., `4`), and saves the output image as `output_1MP.jpg` in the current directory.

