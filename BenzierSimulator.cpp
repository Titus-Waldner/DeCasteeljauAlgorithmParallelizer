//***************************
//Titus Waldner Code!!!
// Parallel Processing Assignment 1
//
//WHAT IT DOES:
//This code implements De Casteljau algorithm to generate a smooth human like curve for meant for mouse movement (basically vector based smooth curves)
//
// WHAT TO EXPECT:
// This is my private benzier generator I use to simulate movement to bypass bot detection on video games, built it for fun :)
// Figured free marks by modifying to not be LIVE (move and simulate at the same time) and throwing a #pragma statement on to parllelize it.
// To get actually good performance it is ususally better to move while simulating the points as otherwise it there will delay in your movement of the mouse
// but for the purposes of this assignment I calculated the points before hand to show the speed up :D have a great day
//
// INPUTS: 
// you need to give it number of processors and the time which is based on how many points in the line, thus more points longer run time, try 1000000
// if you are running this on WSL dont as it sues windows commands for gettings screen positions, just drag the exe into a windows terminal and type the parameters (explained more in assignment report)
//
//OTHER:
// This comments out the part that moves, feel free to uncomment it or just run the other exe in the folder provided
//
//Compile: 
// g++ test.cpp -o output_name -fopenmp -luser32 
//***************************
#include <windows.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include <thread>
#include <chrono>

// Declare global variable
std::string schedule_type;

//Generate random number? pretty simple bud
int random_range(int min, int max) {
    return min + (rand() % static_cast<int>(max - min + 1));
}

//
//For this assignment this is just the work each process does
void BezierAtTime(const std::vector<int>& controlX, const std::vector<int>& controlY, double t, int& outX, int& outY) 
{
    // get the degree of the bezier curve
    size_t degree = controlX.size();

    // temp vectors for storing intermediate values during the De Casteljau algorithm
    std::vector<int> tempX(controlX);
    std::vector<int> tempY(controlY);

    // De Casteljau's algorithm for Bézier curves
    for (size_t i = 1; i < degree; i++) 
    {
        for (size_t j = 0; j < degree - i; j++) 
        {
            // interpolate between each pair of control points using the parameter t
            // and store the result in the temporary vectors, effectively reducing the degree of the curve
            tempX[j] = static_cast<int>((1.0 - t) * tempX[j] + t * tempX[j + 1]);
            tempY[j] = static_cast<int>((1.0 - t) * tempY[j] + t * tempY[j + 1]);
        }
    }
    // thee final result after applying the De Casteljau algorithm multiple times 
    // gives the coordinates of the point on the Bézier curve at parameter t
    outX = tempX[0];
    outY = tempY[0];
}

//**********
//This code generates a benzier curve and stores all the points in an two vectors (you can iterate over them with mouse move commands or plot them form cool curve ~~~~~~~~)
//**********
void RandomBezier(int X0, int Y0, int Xf, int Yf, int T, const std::string& O = "", double base1 = 400.0, double base2 = 550.0, int path = 100) 
{
    // generate a bunch of random numbers
    int base = random_range(static_cast<int>(base1), static_cast<int>(base2));
    T += base;

    // Define the degree of the Bézier curve. This determines the number of control points minus one. For squigally :)
    int N = 2; 

    // <Advanced User> Check for certain options/flags in the string O
    bool RO = (O.find("RO") != std::string::npos);
    bool RD = (O.find("RD") != std::string::npos);

    // demand the the current position of the mouse cursor
    int XM, YM;
    POINT pt;
    GetCursorPos(&pt);
    XM = pt.x;
    YM = pt.y;

    // <Advanced User> If the RO flag is set, adjust the initial coordinates relative to the current mouse position
    if (RO) 
    {
        X0 += XM;
        Y0 += YM;
    }

    // <Advanced User> If the RD flag is set, adjust the final coordinates relative to the current mouse position
    if (RD) 
    {
        Xf += XM;
        Yf += YM;
    }

    // determine bounds for control points based on initial and final positions and path variable
    int sX, bX, sY, bY;
    if (X0 < Xf) 
    {
        sX = X0 - path; // min for x
        bX = Xf + path; // max for x
    } 
    else 
    {
        sX = Xf - path;
        bX = X0 + path;
    }

    if (Y0 < Yf)
    {
        sY = Y0 - path; // min for y
        bY = Yf + path; // max for y
    }
    else 
    {
        sY = Yf - path;
        bY = Y0 + path;
    }

    //define control points
    std::vector<int> controlX = {X0};
    std::vector<int> controlY = {Y0};

    //put random values in control points within the boundary
    for (int i = 1; i < N; ++i) {
        controlX.push_back(random_range(sX, bX));
        controlY.push_back(random_range(sY, bY));
    }

    // make damn sure the last control point matches the final position
    controlX.push_back(Xf);
    controlY.push_back(Yf);

    std::vector<int> xPoints(T);
    std::vector<int> yPoints(T);


    

    // Compute all the points of the BENZIER CURVE and store dem all in vectors
    if(schedule_type == "static")
    {
        double startTime, endTime;
        startTime = omp_get_wtime();
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < T; ++i)
        {
            double U = static_cast<double>(i) / T;
            int X, Y;
            // git dat coordiantes for at TIME U for X,Y
            BezierAtTime(controlX, controlY, U, X, Y);
            xPoints[i] = X;
            yPoints[i] = Y;
        }
        endTime = omp_get_wtime();
        std::cout << "Time taken: " << (endTime - startTime) << " seconds." << std::endl;
    }
    else if(schedule_type == "dynamic")
    {
        double startTime, endTime;
        startTime = omp_get_wtime();
        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < T; ++i)
        {
            double U = static_cast<double>(i) / T;
            int X, Y;
            // git dat coordiantes for at TIME U for X,Y
            BezierAtTime(controlX, controlY, U, X, Y);
            xPoints[i] = X;
            yPoints[i] = Y;
        }
        endTime = omp_get_wtime();
        std::cout << "Time taken: " << (endTime - startTime) << " seconds." << std::endl;
    }
    else if(schedule_type == "guided")
    {
        
        double startTime, endTime;
        startTime = omp_get_wtime();
        #pragma omp parallel for schedule(guided)
        for (int i = 0; i < T; ++i)
        {
            double U = static_cast<double>(i) / T;
            int X, Y;
            // git dat coordiantes for at TIME U for X,Y
            BezierAtTime(controlX, controlY, U, X, Y);
            xPoints[i] = X;
            yPoints[i] = Y;
        }
        endTime = omp_get_wtime();
        std::cout << "Time taken: " << (endTime - startTime) << " seconds." << std::endl;
    }
    else
    {
        std::cerr << "Invalid schedule type!" << std::endl;
        Sleep(1000);
    }

    //move cursor through stored points
    //for (int i = 0; i < T; ++i)
    //{
    //   SetCursorPos(xPoints[i], yPoints[i]);
    //}

    // set final position
    //SetCursorPos(Xf, Yf);
}



int main(int argc, char* argv[]) 
{
    if(argc < 4) 
    {
        std::cerr << "You did not give me the correct number of parameters!!!! OGRE SOUNDS!!!" << std::endl;
        return 1;
    }
    
    int num_processors = std::stoi(argv[1]);
    int timeBenzier = std::stoi(argv[2]);
    
    // Assign value to the global variable bacuase I am lazy and didnt want to change the parameters :/
    schedule_type = argv[3];

    omp_set_num_threads(num_processors);

    srand(static_cast<unsigned int>(time(NULL))); // Seed random number generator if you feel like it
    POINT pt;
    if (GetCursorPos(&pt))
    {
        std::cout << "Current mouse position: X=" << pt.x << ", Y=" << pt.y << std::endl;
        RandomBezier(pt.x, pt.y, 0, 0, timeBenzier);
    }
    
    return 0;
}

