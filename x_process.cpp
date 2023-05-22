//
//  xctest_process.cpp
//  xctest
//
//  Created by Daher Alfawares on 8/29/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#include "x_process.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <array>

static int count = 0;
using namespace x;

std::string process::run(const std::string& args) const{

    std::string command = this->process_name+" "+args;
    count ++;
    std::array<char, 128> buffer;
    std::string result;
#ifdef _WIN32
	FILE* pipe = _popen(command.c_str(), "r");
#else
	FILE* pipe = popen(command.c_str(), "r");
#endif
    if (!pipe)
    {
        std::cout << "Error popen("<< count << ") : "/* << strerror_s(errno) */ << std::endl;
        return 0;
    }
    while (fgets(buffer.data(), 128, pipe) != NULL) {
        result += buffer.data();
    }
    fclose(pipe);
    return result;
}

std::string process::output(){
    auto output = this->out.str();
    output.erase(output.size()-1);
    //output.erase(output.size()-1);
    return output;
}
