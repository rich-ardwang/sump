
This folder includes examples on how to use WSA API.

The sample projects are:
* adco_integration	sample code with ADCO integration
* aft_integration	sample code with AFT integration (windows only)
* example_dotNet	sample code in C# using wsa_nw.dll (windows only)
* example_minimal	sample code with minimal api calls to get cf
* example1		sample code with almost all api calls
* example2		sample code with multi-threading
* moodys_credit_model_example sample code with moodys credit models
* simulation_example          sample code with simulations

* DATA		Dir for sample deals as input to the sample projects.
N.B. WSA_API.DB must be downloaded from official site and put into DATA dir.


HOWTO COMPILE AND RUN

For Windows:
* Open examples.sln with vs2008
* Choose appropriate Solution Platform (Win32 or x64)
* Choose a sample project (e.g. example_minimal) as StartUp Project
* Build the project
* Run with Start Debugging (F5) or Start Without Debugging (Ctrl+F5)

For Linux:
* Open terminal (sh/bash) and type:
  # Choose a sample project (e.g. example_minimal)
  cd example_minimal
  # To build and run 32-bit exe
  make clean all
  export LD_LIBRARY_PATH=../../../lib
  ./example_minimal.exe
  # To build and run 64-bit exe
  make PLATFORM=x64 clean all
  export LD_LIBRARY_PATH=../../../lib64
  ./example_minimal.exe
