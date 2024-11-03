# Vision-Based Object Measurement System using ArUco Markers

## Overview
This project is a machine vision-based system implemented using **C++** and **OpenCV** to measure the dimensions of objects. It uses ArUco markers to calibrate the pixel-to-real-world unit ratio (inches) and detects object contours to determine their width and height.

## Features
- **Accurate Object Measurement**: Calculates the width and height of objects based on the pixel-to-inch ratio using ArUco marker detection.
- **Contour Detection**: Uses edge detection and contour finding to identify and measure the objects.
- **Aruco Marker Calibration**: Automatically adjusts pixel-to-inch conversion using detected ArUco markers, with a fallback default ratio if none are found.
- **Edge Detection & Smoothing**: Pre-processes the image to remove noise and detect object edges clearly.

## Requirements
To run the system, you'll need:
- **C++ Compiler** (e.g., GCC)
- **OpenCV 4.x** (or above)
- A camera or image containing objects to measure
- An ArUco marker for accurate measurement calibration (optional but recommended)

## How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/jainchaitanya19/Aruco-based-object-measurement.git
