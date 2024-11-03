#include <opencv2/opencv.hpp>
#include <vector>
#include <opencv2/aruco.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>

using namespace cv;
using namespace std;

// Function to find ArUco markers in an image
tuple<vector<vector<Point2f>>, vector<int>, vector<vector<Point2f>>> findArucoMarkers(Mat& img, int markerSize = 6, int totalMarkers = 100, bool draw = true)
{
    // Convert image to grayscale
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Initialize variables to store marker information
    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;

    // Define detector parameters and ArUco dictionary
    aruco::DetectorParameters detectorParams = aruco::DetectorParameters();
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);

    // Create ArUco detector and detect markers
    aruco::ArucoDetector detector(dictionary, detectorParams);
    detector.detectMarkers(gray, markerCorners, markerIds, rejectedCandidates);

    return make_tuple(markerCorners, markerIds, rejectedCandidates);
}

int main()
{
    // Load the image
    Mat image = imread("/Users/chaitanyajain/Desktop/ChaitanyaJain_426633_mscrob_2024/aruco_card.jpeg");

    // Check if the image is loaded successfully
    if (image.empty())
    {
        cout << "Error: Unable to load image." << endl;
        return -1;
    }

    // Convert BGR image to gray scale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Remove Gaussian noise from the image
    GaussianBlur(gray, gray, Size(7, 7), 0);

    // Perform edge detection, then perform a dilation + erosion
    Mat edged;
    Canny(gray, edged, 50, 100);
    dilate(edged, edged, Mat(), Point(-1, -1), 1);
    erode(edged, edged, Mat(), Point(-1, -1), 1);

    // Find contours in the edge map
    vector<vector<Point>> contours;
    findContours(edged.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Declare and initialize pixelsPerMetric to 0
    double pixelsPerMetric = 0;

    // Detect Aruco marker and use its dimension to calculate the pixel to inch ratio
    auto arucofound = findArucoMarkers(image, 100);
    
    if (!get<0>(arucofound).empty())
    {
        // Assuming the first ArUco marker found is used for calibration
        double aruco_perimeter = arcLength(get<0>(arucofound)[0], true);
        

        // Pixel to Inch ratio
        pixelsPerMetric = aruco_perimeter /4.5;
    }
    else
    {
        // Default value for pixel to inch ratio
        pixelsPerMetric = 66;
        cout << "No ArUco marker found. Using default pixel to inch ratio." << endl;
    }

    // Loop over the contours individually
    for (size_t i = 0; i < contours.size(); i++)
    {
        // If the contour is not sufficiently large, ignore it
        if (contourArea(contours[i]) < 2000)
            continue;

        // Compute the rotated bounding box of the contour
        RotatedRect box = minAreaRect(contours[i]);
        Point2f vertices[4];
        box.points(vertices);

        // Draw the contour and the bounding box
        vector<vector<Point>> boxContours = { vector<Point>(begin(vertices), end(vertices)) };
        drawContours(image, boxContours, 0, Scalar(0, 0, 255), 2);

        // Draw the centroid
        Moments M = moments(contours[i]);
        Point2f centroid = Point2f(M.m10 / M.m00, M.m01 / M.m00);
        circle(image, centroid, 5, Scalar(255, 255, 255), -1);

        // Order the points in the contour
        vector<Point2f> orderedVertices(vertices, vertices + 4);
        sort(orderedVertices.begin(), orderedVertices.end(), [](const Point2f& p1, const Point2f& p2) {
            return (p1.x + p1.y) < (p2.x + p2.y);
        });
        
        Point2f tl = orderedVertices[0];
        Point2f tr = orderedVertices[1];
        Point2f br = orderedVertices[2];
        Point2f bl = orderedVertices[3];

        // Calculate width and height in inches
        double width_1 = norm(tr - tl);
        double height_1 = norm(bl - tl);
        double d_wd = width_1 / pixelsPerMetric;
        double d_ht = height_1 / pixelsPerMetric;
        
        // Display the width and height of the object in inches
        cout << "Object "  " - Width: " << d_wd << " inch, Height: " << d_ht << " inch" << endl;

    }
    imshow("Output_image", image);
    
    waitKey(0);
    
    // Close all OpenCV windows
    destroyAllWindows();
    
    return 0;
}

