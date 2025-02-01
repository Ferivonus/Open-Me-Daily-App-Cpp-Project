#include <iostream>
#include "check_opened.h"
#include "hey_I_checked.h"

int main(int argc, char* argv[]) {
    // Expected phrase for password entry or to view logs
    std::string expected_phrase = "ben buradayým ve mutluyum";
    std::string logs_phrase = "my logs";

    // Check if the correct phrase is passed for password entry
    if (argc == 2 && std::string(argv[1]) == expected_phrase) {
        hey_I_checked sign_here;
        sign_here.runner();
    }
    // Check if the user wants to view the logs
    else if (argc == 2 && std::string(argv[1]) == logs_phrase) {
        hey_I_checked view_logs;
        view_logs.showLogs(); // Function to show all user logs
    }
    else {
        check_opened check;
        check.runner();
    }

    return 0;
}
