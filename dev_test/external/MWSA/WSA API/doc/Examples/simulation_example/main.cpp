#pragma warning(disable : 4996)

#include "wsa.h"

#include <iostream>

// monte carlo
extern void run_monte_carlo_example();
// default probability distribution
extern void run_dpd_example();

// input path
#ifdef _WIN32
const char* input_path="..\\DATA";
#else
const char* input_path="../DATA";
#endif

int main()
{
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "--------------- Simulation Examples -------------" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // Set the path to deals.
    set_input_path(input_path);
    // Set the error handling level.
    set_error_handling_level(ERROR_HANDLING_LEVEL_LOG_IT); // ERROR_HANDLING_LEVEL_STOP_CALCULATION
    // Set the rule to handle missing interst rates.
    set_missing_interest_rates_handling(MISSING_INTEREST_RATES_USE_ZERO); // MISSING_INTEREST_RATES_TREAT_AS_ERROR
    // Set the rule to handle missing exchange rates.
    set_missing_exchange_rates_handling(MISSING_EXCHANGE_RATES_USE_ONE); // MISSING_EXCHANGE_RATES_TREAT_AS_ERROR

    run_monte_carlo_example();
    run_dpd_example();

    return 0;
}
