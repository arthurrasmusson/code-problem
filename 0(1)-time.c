#include <ctime>

static char values[ARRAY_SIZE] = {};
static std::time_t updatedTimestamps[ARRAY_SIZE] = {};
static bool overrideActive = false;
static char overrideValue = 0;
static std::time_t overrideTime = 0;

void set_value(int index, char value) {
    // store the new value
    values[index] = value;
    // record the current time for this slot
    updatedTimestamps[index] = std::time(nullptr);
}

void set_all_values(char value) {
    // enable override mode
    overrideActive = true;
    // remember the override value
    overrideValue = value;
    // record when the override was applied
    overrideTime = std::time(nullptr);
}

char get_value(int index) {
    if (overrideActive) {
        // if this element was individually updated after the override,
        // return its own stored value
        if (updatedTimestamps[index] > overrideTime){
            return values[index];
        }
        // otherwise, return the global override value
        return overrideValue;
    }
    // no override in effect: return the element’s stored value
    return values[index];
}