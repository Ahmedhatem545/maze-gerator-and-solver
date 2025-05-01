#include <raylib.h>
#include <vector>
#include <stack>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <limits>
#include "button.h"
#include "cell_grid.h"
#include "gen.h"
#include "solve.h"
using namespace std;

const int margin = 25;
void gameLoop();