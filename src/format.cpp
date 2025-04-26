#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{

    int HH = seconds / 3600;
    int MM = (seconds % 3600) /60;
    int SS = (seconds % 3600) %60;

    string HH_str = std::to_string(HH);
    string MM_str = std::to_string(MM);
    string SS_str = std::to_string(SS);




    HH_str.insert(0, 2 - HH_str.length(), '0');
    MM_str.insert(0, 2 - MM_str.length(), '0');
    SS_str.insert(0, 2 - SS_str.length(), '0');

    return HH_str + ":" + MM_str + ":" + SS_str;
    
}