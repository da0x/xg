//
//  x_probability.hpp
//  x
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#ifndef x_probability_hpp
#define x_probability_hpp

#include <vector>

namespace x {
    class probability {
        float normal;
    public:
        probability()
        {
            this->normal = 0;
        }
        probability(float normal)
        {
            this->normal = normal;
        }
        operator bool() const {
            float random = std::rand() / static_cast<float>(RAND_MAX);
            return random <= this->normal;
        }
        probability& operator = (float value){
            normal = value;
            return *this;
        }
        float value() const {
            return normal;
        }
    };
}

#include <ostream>
inline
std::ostream & operator << (std::ostream& stream, const x::probability& probability){
    return stream << probability.value();
}

            
#endif /* x_probability_hpp */
