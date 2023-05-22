//
//  x_sort.hpp
//  x
//
//  Created by Daher Alfawares on 2/3/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#ifndef x_sort_hpp
#define x_sort_hpp

namespace x { namespace sort {
    
    template<typename functor>
    struct by
    {
        functor f;
        template<class T>
        bool operator()(T const &a, T const &b) const {
            return f(a) > f(b);
        }
    };
    
}}

#endif /* x_sort_hpp */
