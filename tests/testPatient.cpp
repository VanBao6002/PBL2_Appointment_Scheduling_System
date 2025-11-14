#include "all.h"
#include <string>
#include <iostream>


using namespace std;

int main () {
    string temp = "14/2/2006";

    Date current(Date::fromString(temp));
    
    cout << current.getDay() << current.getMonth() << current.getYear();
}