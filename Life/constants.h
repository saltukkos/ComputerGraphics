#ifndef CONSTANTS
#define CONSTANTS

namespace Constants {
    const int LIVE_BEGIN = 2000;
    const int LIVE_END = 3300;
    const int BIRTH_BEGIN = 2300;
    const int BIRTH_END = 2900;
    const int FIRST_IMPACT = 1000;
    const int SECOND_IMPACT = 300;

    const int IMPACT_MULT = 1000;

    const int FIELD_WIDTH = 400;
    const int FIELD_HEIGHT = 400;
    const int MAX_FIELD_WIDTH = 5000;
    const int MAX_FIELD_HEIGHT = 5000;
    const int MIN_FIELD_WIDTH = 1;
    const int MIN_FIELD_HEIGHT = 1;

    const float CELL_STEP = 1.35;
    const int CELL_SIZE = 35;
    const int MIN_CELL_SIZE = 3;
    const int MAX_CELL_SIZE = 400;
    const int MIN_TEXT_CELL_SIZE = 23;
    const int WHEEL_FORCE = 1;

    const int TIMER_PERIOD = 0;
    const int MAX_TIMER_PERIOD = 1000;

    const float WIDTH_K = 0.86602540378f; //sqrt(3)/2
    const float ANGLE_TAN = 0.57735026919f; //tan(Pi/6)

}

#endif // CONSTANTS

