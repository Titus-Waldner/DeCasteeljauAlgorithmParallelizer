#include <windows.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>

int random_range(int min, int max) {
    return min + (rand() % static_cast<int>(max - min + 1));
}

void BezierAtTime(const std::vector<int>& controlX, const std::vector<int>& controlY, double t, int& outX, int& outY) {
    size_t degree = controlX.size();
    std::vector<int> tempX(controlX);
    std::vector<int> tempY(controlY);

    for (size_t i = 1; i < degree; i++) {
        for (size_t j = 0; j < degree - i; j++) {
            tempX[j] = static_cast<int>((1.0 - t) * tempX[j] + t * tempX[j + 1]);
            tempY[j] = static_cast<int>((1.0 - t) * tempY[j] + t * tempY[j + 1]);
        }
    }

    outX = tempX[0];
    outY = tempY[0];
}

void RandomBezier(int X0, int Y0, int Xf, int Yf, int T, const std::string& O = "",
                  double base1 = 400.0, double base2 = 550.0, int path = 100) {
    int base = random_range(static_cast<int>(base1), static_cast<int>(base2));
    T += base;

    int N = 2; // degree

    bool RO = (O.find("RO") != std::string::npos);
    bool RD = (O.find("RD") != std::string::npos);

    int XM, YM;
    POINT pt;
    GetCursorPos(&pt);
    XM = pt.x;
    YM = pt.y;

    if (RO) {
        X0 += XM;
        Y0 += YM;
    }
    
    if (RD) {
        Xf += XM;
        Yf += YM;
    }

    int sX, bX, sY, bY;
    if (X0 < Xf) {
        sX = X0 - path;
        bX = Xf + path;
    } else {
        sX = Xf - path;
        bX = X0 + path;
    }

    if (Y0 < Yf) {
        sY = Y0 - path;
        bY = Yf + path;
    } else {
        sY = Yf - path;
        bY = Y0 + path;
    }

    std::vector<int> controlX = {X0};
    std::vector<int> controlY = {Y0};
    for (int i = 1; i < N; ++i) {
        controlX.push_back(random_range(sX, bX));
        controlY.push_back(random_range(sY, bY));
    }
    controlX.push_back(Xf);
    controlY.push_back(Yf);

    DWORD start = GetTickCount();
    DWORD end = start + T;

    while (GetTickCount() < end) 
    {
        double U = static_cast<double>(GetTickCount() - start) / T;
        int X, Y;
        BezierAtTime(controlX, controlY, U, X, Y);
        SetCursorPos(X, Y);
        Sleep(10);  // Sleep a bit to make the motion perceivable
    }

    SetCursorPos(Xf, Yf);
}

int main() 
{
    srand(static_cast<unsigned int>(time(NULL))); // Seed random number generator
    POINT pt;
    if (GetCursorPos(&pt))
    {
        RandomBezier(pt.x, pt.y, 900, 900, 200); 
    }
    
    return 0;
}
