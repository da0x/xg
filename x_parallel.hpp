//
//  x_parallel.hpp
//  life
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#ifndef x_parallel_hpp
#define x_parallel_hpp

#include <vector>
#include <thread>

namespace x {
    template<typename function>
    class parallel {
        typedef std::thread thread;
        typedef std::vector<thread> vector;
        vector threads;
        
        parallel(std::vector<function> functions){
            for(auto f: functions){
                threads.push_back(thread(f));
            }
        }
        ~parallel(){
            for(auto t: threads){
                t.join();
            }
        }
    };
}


#endif /* x_parallel_hpp */
