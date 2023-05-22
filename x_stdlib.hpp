//
//  stdlib.hpp
//  x
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#ifndef x_stdlib_hpp
#define x_stdlib_hpp

#include <cstdlib>

namespace x {

    template<typename T> T abs(const T& t){
        return t < 0 ? -t : t;
    }
    
    template<typename T> T rand(){
        return std::rand() / static_cast<T>(RAND_MAX);
    }
    
    template<typename T> T clamp(const T& v, const T& min, const T& max){
        if(v < min){return min;}
        if(v > max){return max;}
        return v;
    }
    
    class fixed_float {
        const float & f;
        const int p;
    public:
        fixed_float(const float& floatValue, const int precision = 2):
        f(floatValue),p(precision) {}
        const float& value() const { return f; }
        const int& precision() const { return p; }
    };
}

#include <iomanip>

inline
std::ostream& operator << (std::ostream& stream, const x::fixed_float& f){
    return stream << std::fixed << std::setprecision(f.precision()) << f.value();
}

#endif /* x_stdlib_hpp */
