//
//  x_block_measure.hpp
//  x
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#ifndef x_block_measure_hpp
#define x_block_measure_hpp

#include <iostream>
#include <ctime>
#include <chrono>

namespace x {
    namespace block {
        class measure {
            const char* block_name;
            std::chrono::high_resolution_clock::time_point start;
        public:
            measure(const char* block_name){
                this->block_name = block_name;
                this->start = std::chrono::high_resolution_clock::now();
            }
            ~measure(){
                auto end = std::chrono::high_resolution_clock::now();
                
                std::cout << "block execution: " << this->block_name << "(" << std::chrono::duration<double, std::milli>(end-start).count()
                << " ms)" << std::endl;
            }
        };
    }
}

#endif /* x_block_measure_hpp */
