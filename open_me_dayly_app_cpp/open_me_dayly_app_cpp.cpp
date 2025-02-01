// open_me_dayly_app_cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "check_opened.h"
#include "hey_I_checked.h"

int main(int argc, char* argv[]) {
    // "ben buradayým ve mutluyum" tek bir argüman olarak kabul edilsin
    std::string expected_phrase = "ben buradayým ve mutluyum";

    if (argc == 2 && std::string(argv[1]) == expected_phrase) {
        hey_I_checked sign_here;
        sign_here.runner();
    }
    else {
        check_opened check;
        check.runner();
    }

    return 0;
}
