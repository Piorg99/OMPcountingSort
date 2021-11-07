# FirstProjectOMP

## Dependencies

* CMake 3.9+
* OpenMP
* Python3 3.9.7
* Pipenv

## How to run

1. Create a build directory and launch cmake

   ```batch
   mkdir build
   cd build
   cmake ..
   ```

2. Generate executables with `make`
3. To generate measures run 
   - `make generate_measuresOn2` to run the execution with time complexity O(n^2)
   - `make generate_measuresOn` to run the execution with time complexity O(n)
4. To extract graphics and tables for speedup and efficiency from the .csv files run
   - `make extract_measuresOn2` to run the extraction for the algorithm with time complexity O(n^2)
   - `make generate_measuresOn` to run the extraction for the algorithm with time complexity O(n)

Results can be found in the `measures/measure`  or `measures/measureOn` directory, divided by problem size and the gcc optimization option used.


