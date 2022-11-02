#include <iostream>
#include <utility>
#include <vector>

extern "C" {
    #include <tomophantom/TomoP3DModelSino_core.h>
}

// #include <astra/FilteredBackProjectionAlgorithm.h>

#include <opencv2/opencv.hpp>
#include "TomoP3DModel.hpp"


void generate_partition(std::vector<std::pair<int, int>>& partition, int parts_num, int overlay, int full_height) {
    partition.resize(parts_num);

    if (parts_num > 1) {
        int scan_height = full_height / parts_num;
        int half_overlay = overlay / 2;
        
        partition[0] = {0, scan_height + overlay};

        for (int scan_id = 1; scan_id < parts_num - 1; ++scan_id) {
            int offset = scan_id * scan_height;
            partition[scan_id] = {offset - half_overlay, offset + scan_height + half_overlay};
        }

        partition[parts_num - 1] = {full_height - scan_height - overlay, full_height};
    }
    else {
        partition[0] = {0, full_height};
    }
}


int main(int argc, char** argv) {
    if (argc != 4)
    {
        printf("Usage: ./strg \n");
        return 1;
    }

    std::vector<std::pair<int, int>> partition;
    generate_partition(partition, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    for (auto part : partition) {
        std::cout << part.first << ' ' << part.second << '\n';
    }

    return 0;
}
