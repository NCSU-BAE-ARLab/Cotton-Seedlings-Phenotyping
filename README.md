# Filtering Cotton Seeding Trays
The filter script utilizes the Point Cloud Library to remove noise in point clouds of cotton seedling trays. It first uses ConditionalRemoval to filter based on position and color, then RadiusOutlierRemoval, then StatisticalOutlierRemoval.

The following video shows the filtering at each step:



## Codebase Walkthrough
### Clouds
The `clouds` folder holds point clouds of the tray Rep1Tray15_2.

The `example` subfolder has the example clouds used in the video, with original.ply being zipped due to its large size.

The `optimization` subfolder has the resultant point clouds of the unselected parameter combinations from the filter parameter optimization process. For instance, `ror0.01_300.ply` is the result of setting the RadiusOutlierRemoval parameters to 0.01 for radius and 300 for neighbors, `sor2_2.4` is StatsticalOutlierRemoval with 2 neighbors and 2.4 distance, and `sor_ror.ply` is having StatisticalOutlierRemoval come before RadiusOutlierRemoval instead of the other way around.

### Program
The `program` folder includes the files needed to run the filter.


## How to Filter
1. Ensure you have PCL and CMake installed
2. In program/filter.cpp, replace the path with the PLY file that you are trying to filter
3. In terminal, `cd` into `program/build`
4. run `cmake ..`
5. run `make`
6. run `./filter`
7. After the program is finished running, the filtered PLY will be located at `program/build/filtered.ply`