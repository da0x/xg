//
//  x_process.hpp
//  x
//
//  Created by Daher Alfawares on 8/29/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef x_process_hpp
#define x_process_hpp

#include <string>
#include <sstream>
#include <vector>

namespace x {
    class process {
        std::string process_name;
    public:
        process(std::string process):process_name(process) {}
        std::string  run(const std::string& args) const;
        std::string output();
    private:
        std::stringstream out;
    };
}

#endif /* x_process_hpp */
