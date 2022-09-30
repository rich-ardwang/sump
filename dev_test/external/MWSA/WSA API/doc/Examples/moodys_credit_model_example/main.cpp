#pragma warning(disable : 4996)

#include "wsa.h"

#include <iostream>

// input path
#ifdef _WIN32
const char* input_path="..\\DATA";
const char* mpa_data_path="..\\DATA\\MPA\\data";
const char* pa_data_path="..\\DATA\\PA\\data";
#else
const char* input_path="../DATA";
const char* mpa_data_path="../DATA/MPA/data";
const char* pa_data_path="../DATA/PA/data";
#endif

// cmm
extern void run_cmm_example();
// mpa
extern void run_mpa_example();
// pa
extern void run_pa_example();
// sedf
extern void run_sedf_example();

int main()
{
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "--------------- Moodys Credit Model Examples -------------" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    std::cout << MOODYS_STANDARD_SETTINGS << " -- Standard model example" << std::endl;
    std::cout << MOODYS_DPLC_SETTINGS << " -- DPLC model example" << std::endl;
    std::cout << MOODYS_CMM_SETTINGS << " -- CMM model example" << std::endl;
    std::cout << MOODYS_MPA_SETTINGS << " -- MPA model example" << std::endl;
    std::cout << MOODYS_PA_SETTINGS << " -- PA model example" << std::endl;
    std::cout << MOODYS_SEDF_SETTINGS << " -- SEDF model example" << std::endl;
    std::cout << "Please input the credit model number: ";

    int credit_type = -1;
    std::cin >> credit_type;

    //Set the path of deals.
    set_input_path(input_path);
    //Set the path of mpa data.
    set_mpa_data_path(mpa_data_path);
    //Set the path of pa data.
    set_pa_data_path(pa_data_path);

    switch(credit_type)
    {
    case MOODYS_STANDARD_SETTINGS:
        std::cout << "[WARN]run_standard_example() not implemented." << std::endl;
        break;
    case MOODYS_DPLC_SETTINGS:
        std::cout << "[WARN]run_dplc_example() not implemented." << std::endl;
        break;
    case MOODYS_CMM_SETTINGS:
        run_cmm_example();
        break;
    case MOODYS_MPA_SETTINGS:
        run_mpa_example();
        break;
    case MOODYS_PA_SETTINGS:
        run_pa_example();
        break;
    case MOODYS_SEDF_SETTINGS:
        run_sedf_example();
        break;
    default:
        std::cerr << "Invalid credit type number: " << credit_type << std::endl;
        break;
    }

    std::cout << "Sample code program finished." << std::endl;
    return 0;
}
