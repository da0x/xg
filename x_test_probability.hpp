//
//  x_test_probability.hpp
//  x
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#ifndef x_test_probability_hpp
#define x_test_probability_hpp

#include "x_block_measure.hpp"
#include <iomanip>
#include <vector>

namespace x {
namespace test {
    inline void probability(){
        x::block::measure m("x::test::probability");
        std::vector<float> probabilities = {
            0.1f, 0.25f, 0.5f, 0.75f, 1.0f
        };
        int iterations = 1000000;
        std::cout << "testing probability for " << iterations << " iterations" << std::endl;
        for(auto probability:probabilities){
            float total = 0;
            for(int iteration = 0; iteration < iterations; iteration++){
                if(x::probability(probability)){
                    total += 1;
                }
            }
            auto result = total/static_cast<float>(iterations);
            auto diff = result - probability;
            if(diff<0) diff = -diff;
            
            std::cout
            << "probability("
            << std::fixed << std::setprecision(2) << probability
            << ") -> "
            << result
            << std::endl;
        }
    }
}}


#endif /* x_test_probability_hpp */
