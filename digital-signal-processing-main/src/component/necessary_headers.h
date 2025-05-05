#pragma once

#define _USE_MATH_DEFINES

/*
data - структура для хранения информации 
о частоте гармоники и ее времени
*/
#define SignalTime      data.time
#define SignalFrequency data.frequency

#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <fftw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <filesystem>
