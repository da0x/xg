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
//
//  x_clamp.hpp
//  x
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright � 2018 Daher Alfawares. All rights reserved.
//

#pragma once

namespace x
{
	template<typename _T> const _T& clamp(const _T& value, const _T& min, const _T& max)
	{
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}
}

//
//  x.cpp
//  x
//
//  Created by Daher Alfawares on 6/2/19.
//  Copyright � 2019 Daher Alfawares. All rights reserved.
//

#pragma once

namespace x
	{
	class fixed_float {
		const float & f;
		const int p;
	public:
		fixed_float(const float& floatValue, const int precision = 2) :
			f(floatValue), p(precision) {}
		const float& value() const { return f; }
		const int& precision() const { return p; }
		};
	}

#include <iomanip>

inline
std::ostream& operator << (std::ostream& stream, const x::fixed_float& f)
	{
	return stream << std::fixed << std::setprecision(f.precision()) << f.value();
	}

//C++
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  Filename     : x_table.hpp                                          //
//  Created by   : Daher Alfawares                                          //
//  Created Date : 06/20/2009                                               //
//  License      : Apache License 2.0                                       //
//                                                                          //
// Copyright 2009 Daher Alfawares                                           //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
// http://www.apache.org/licenses/LICENSE-2.0                               //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/*
 
 X Table is a C++ class that allows you to format your data in an ASCII dynamic table structure.
 
 Example:
 
 x::table myTable("Table Title");
 myTable ("header 1")("header 2")++;
 myTable ("value 11")("value 12")++;
 myTable ("value 21")("value 22")++;
 
 std::cout << myTable;
 
 Output:
 
 +---------------------+
 |Table Title          |
 +----------+----------+
 | header 1 | header 2 |
 +----------+----------+
 | value 11 | value 12 |
 | value 21 | value 22 |
 +----------+----------+
 
 */


#ifndef _X_Table__0a61eabe_d038_4d30_a6fb_e202b2dfd6a9
#define _X_Table__0a61eabe_d038_4d30_a6fb_e202b2dfd6a9

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <numeric>
#include <sstream>

namespace x
{
    // forward:
    
    // the following allows adding a C style string to an std::string and returns an std::string.
    // example:
    // std::string a = "abc";
    // std::string b = "123" + a;
    //
    
    inline std::string operator + ( const char*a, const std::string& b );
    inline std::string itoa( const int &i );
    inline int atoi( const std::string&s );
    
    // lexical cast.
    template<typename _To, typename _From> class lexical_cast;
    struct scoped_mute_s;
    // formats the output.
    std::size_t length( std::string _String);
    std::string fill(std::string _Str, std::size_t _Size);
    
    class endl {};

    class table
    {
    public:
        std::string					title;
        size_t						column;
        std::vector<std::string>	columns;
        std::vector<std::size_t>	column_widths;
        std::string					comment;
        std::string					Prefix; // line prefix.
        
        table( std::string t ):title(t),column(0){}
        
        
        std::vector< std::vector<std::string> > rows;
        
        table & operator<<(std::string t){
            return (*this)(t);
        }
        table & operator<<(int t){
            std::stringstream fstr;
            fstr << t;
            return (*this)<< fstr.str();
        }
        table & operator()(std::string t)
        {
            if( column >= columns.size() )
            {
                columns.push_back("");
                column_widths.push_back(0);
            }
            
            columns[column] = t;
            if( column_widths[column] < length(t) )
                column_widths[column] = length(t);
            
            ++column;
            
            return *this;
        }
        table & operator()(int i)
        {
            return (*this)(itoa(i));
        }
        
        void operator ++(int)
        {
            column = 0;
            rows.push_back( columns );
        }
        
        std::string prefix(){ return Prefix; }
        void prefix( std::string _P ){ Prefix=_P; }
        
        std::string endl(){ return "\r\n" + prefix() + " "; }
    };
    
    inline std::ostream& operator << ( std::ostream& _Str, table& T )
    {
        
        int width = std::accumulate( T.column_widths.begin(), T.column_widths.end(), 0 );
        width += 3*T.columns.size();
        
        _Str << std::setiosflags(std::ios::left);
        
        _Str << T.endl();
        
        // top border.
//        _Str
//        << ' ';
//        for( int b=0; b< width-1; b++ )
//            _Str
//            << '-';
//        _Str
//        << ' '
//        << T.endl();
        
        // title.
        _Str
        << "  "
        << std::setw(width-1)
        << T.title
        << ""
        << T.endl();
        
        // middle border
        for( size_t i=0; i< T.columns.size(); i++ )
            _Str
            << "  "
            << std::setw( static_cast<int>(T.column_widths[i]) )
            << std::setfill('-')
            << ""
            << std::setfill(' ')
            << " ";
        _Str
        << " "
        << T.endl();
        
        
        if( !T.rows.empty() || !T.rows[0].empty() )
        {
            size_t i=0,j=0;
            
            // first row as header.
            for( i=0; i< T.columns.size(); i++ )
            {
                _Str
                << "  "
                << fill( T.rows[j][i], T.column_widths[i] )
                << " ";
            }
            _Str
            << " "
            << T.endl();
            
            // middle border
            for( size_t i=0; i< T.columns.size(); i++ )
            {
                _Str
                << "  "
                << std::setw( static_cast<int>(T.column_widths[i]) )
                << std::setfill(' ')
                << ""
                << std::setfill(' ')
                << " ";
            }
            _Str
            << " "
            << T.endl();
            
            // comment if available.
            if( !T.comment.empty() )
                _Str
                << " "
                << std::setw(width-1)
                << T.comment
                << " "
                << T.endl();
            
            // full table.
            for( size_t j=1; j< T.rows.size(); j++ )
            {
                for( size_t i=0; i< T.columns.size(); i++ )
                {
                    _Str
                    << "  "
                    << fill( T.rows[j][i], T.column_widths[i] )
                    << " ";
                }
                _Str
                << " "
                << T.endl();
            }
        }
        
        // bottom border.
        for( size_t i=0; i< T.columns.size(); i++ )
        {
            _Str
            << "  "
            << std::setw( static_cast<int>(T.column_widths[i]) )
            << std::setfill('-')
            << ""
            << std::setfill(' ')
            << " ";
        }
        _Str << std::resetiosflags(std::ios::left);
        _Str
        << " "
        << std::endl;
        
        return _Str;
    }
    
    inline std::string operator + ( const char*a, const std::string& b )
    {
        std::string c;
        c += a;
        c += b;
        return c;
    }
    
    inline std::string itoa( const int &i )
    {
        std::stringstream Str;
        Str << i;
        return Str.str();
    }
    
    inline int atoi( const std::string&s )
    {
        int i;
        std::istringstream Str(s);
        Str >> i;
        return i;
    }
    
    // lexical cast.
    template<typename _To, typename _From> class lexical_cast
    {
        _From * from;
    public:
        lexical_cast<_To, _From>( _From From )
        {
            this->from = & From;
        }
        
        operator _To()
        {
            throw( "Bad lexical cast: " );
        }
    };
    
    inline
    std::size_t length( std::string _String){
        std::size_t length = 0;
        //while (*s)
        for(auto Char:_String)
            length += (Char++ & 0xc0) != 0x80;
        return length;
    }
    
    inline
    std::string fill(std::string _Str, std::size_t _Size){
        
        if( length(_Str) < _Size ){
            std::size_t Difference = _Size - length(_Str);
            std::stringstream _Out;
            _Out << _Str;
            for(std::size_t i=0; i< Difference; i++){
                _Out << " ";
            }
            return _Out.str();
        }
        
        return _Str;
    }
    
} /* ascii */

#endif

//
//  x_options.hpp
//  x
//
//  Created by Daher Alfawares on 8/28/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef x_options_hpp
#define x_options_hpp

/* : Example :
 #include <iostream>
 #include "xg.hpp"
 namespace arg {
     std::string help = "-help";
      std::string name = "-name";
      std::string x = "-x";
 }
 int main(int argc, const char * argv[]) {
      auto arguments = x::options(argc,argv);
      arguments.map_to({
           {arg::help,    x::option("produces the usage of this tool.")},
           {arg::name,    x::option("takes your first and last names.")}
      });
      if(arguments[arg::help]){
           std::cout << arguments.print() << std::endl;
      }
      if(arguments[arg::name]){
           std::cout<< arguments[arg::name].value() << std::endl;
      }
      return 0;
 }
 */

#include <string>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

namespace x {
    
    class option {
    public:
        typedef std::string             string;
        typedef std::vector<string>     vector;
        typedef std::map<string,option> map;
        
    public:
        option(string description){
            option_description = description;
            option_is_enabled  = false;
        }
        void operator +=(const string& value){
            this->option_values.push_back(value);
        }
        const vector values() const { return this->option_values; }
        const string value() const {
            if(this->option_values.size()>0)
                return this->option_values[0];
            else
                return string();
        }
        void enable(){ option_is_enabled = true; }
        bool enabled(){ return option_is_enabled; }
        std::string description(){ return option_description; }
        operator bool () { return enabled(); }
        
    private:
        string option_description;
        vector option_values;
        bool   option_is_enabled;
    };
    
    class options {
    public:
        option::map     map;
        option::vector  arguments;
        options(int argc, const char * argv[])
            {
            for(int i=1; i< argc; i++)
                this->arguments.push_back(argv[i]);
            }
        void map_to(option::map option_map){
            this->map = option_map;
            option::map::iterator option_iterator = map.end();
            for(auto argument : this->arguments){
                // what is it?
                if(argument[0] == '-'){
                    // then it's an option.
                    // find it in the map.
                    option_iterator = this->map.find(argument);
                    if( option_iterator == this->map.end()){
                        std::cout << "Invalid switch: " << argument << std::endl;
                        continue;
                    }
                    auto option_name      = option_iterator->first;
                    option& option_object = option_iterator->second;
                    option_object.enable();
                } else {
                    if(option_iterator==map.end()){
                        std::cout << "Ignoring " << argument << std::endl;
                        continue;
                    }
                    // it's a value
                    auto option_name      = option_iterator->first;
                    option& option_object = option_iterator->second;
                    option_object += argument;
                }
            }
        }
        std::string print(){
            std::stringstream stream;
            x::table out("available options:");
            out("switch")("description")++;
            for(auto iter:map){
                out(iter.first)(iter.second.description())++;
            }
            stream << "Usage: -[switch] value1 value2 ..." << std::endl;
            stream << out;
            return stream.str();
        }
        std::string print_values(){
            std::stringstream stream;
            x::table out("available options:");
            out("switch")("value")++;
            for(auto iter:map){
                std::stringstream stream;
                for(auto value:iter.second.values()) stream << "\"" << value << "\" ";
                out(iter.first)(stream.str())++;
            }
            stream << out;
            return stream.str();
        }
        
        inline
        option& operator[](option::string key){
            auto iterator = map.find(key);
            if(iterator != map.end()){
                return iterator->second;
            }
            
            throw -1; // You forgot to add a switch to the map.
        }
    };
}

#endif /* x_options_hpp */
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
#include <array>

namespace x {
    class process {
        std::string process_name;
        public:
        process(std::string process):process_name(process) {}
        std::string run(const std::string& args) const{
            static int count = 0;
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
        std::string output(){
            auto output = this->out.str();
            output.erase(output.size()-1);
            //output.erase(output.size()-1);
            return output;
        }
        private:
        std::stringstream out;
    };
}

#endif /* x_process_hpp */
//
//  x_shell.hpp
//  x
//
//  Created by Daher Alfawares on 12/13/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef x_shell_hpp
#define x_shell_hpp

#include <string>
#include <iostream>
#include <sstream>

namespace x {
    namespace shell {
        
        using std::string;
        
        inline string remove(string file){
            std::stringstream command;
            command
            << "rm "
            << "-f "
            << "-r "
            << "-v "
            << "\"" << file << "\" ";
            return command.str();
        }
        inline string mkdir(string dir){
            std::stringstream command;
            command
            << "mkdir "
            << "\"" << dir << "\" ";
            return command.str();
        }
        inline string copy(string src, string dst){
            std::stringstream command;
            command
            << "cp "    // copy
            << "-f "    // force
            << "-r "    // recursive
            << "-v "    // verbose
            << "\"" << src << "\" "
            << "\"" << dst << "\" ";
            
          //  std::cout << command.str() << std::endl;
            return command.str();
        }
        
        inline string diff(string src, string dst){
            std::stringstream command;
            command
            << "diff " // diff
            << "-q "   // quiet
            << "\"" << src << "\" "
            << "\"" << dst << "\" ";
          //  std::cout << command.str() << std::endl;
            return command.str();
        }
        inline string move(string src, string dst){
            std::stringstream command;
            command
            << "mv "
            << "-f "
            << "-v "
            << "\"" << src << "\" "
            << "\"" << dst << "\" ";
            return command.str();
           // std::cout << command.str() << std::endl;
        }
    }
}

#endif /* x_shell_hpp */
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
    
}

#endif /* x_stdlib_hpp */


//
//  x_test_probability.hpp
//  x
//
//  Created by Daher Alfawares on 1/26/18.
//  Copyright © 2018 Daher Alfawares. All rights reserved.
//

#ifndef x_test_probability_hpp
#define x_test_probability_hpp

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

#endif

//
//  x_uuid.hpp
//  x
//
//  Created by Daher Alfawares on 12/14/17.
//  Copyright © 2017 Daher Alfawares. All rights reserved.
//

#ifndef x_uuid_hpp
#define x_uuid_hpp

#include <string>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace x {
    namespace uuid {
        inline std::string string(){
            std::stringstream str;
            boost::uuids::uuid u = boost::uuids::random_generator()();
            str << u;
            return str.str();
        }
    }
}

#endif

//
//  xg.hpp
//  life
//
//  Created by Daher Alfawares on 1/1/2019.
//  Copyright � 2019 Daher Alfawares. All rights reserved.
//

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <exception>
#include <glm/glm.hpp>

#ifdef main
#undef main
#endif

namespace xg {

	using glm::vec2;
	using glm::vec1;
	typedef glm::vec2 v;
	constexpr auto pi = 3.14159265358979323846f;
	using std::vector;

	enum align
	{
		left = 0x0001,
		right = 0x0002,
		center_h = 0x0004,
		center_v = 0x0008,
		top = 0x0010,
		bottom = 0x0020,
	};

	const SDL_BlendMode blend_none = SDL_BLENDMODE_NONE;
	const SDL_BlendMode blend = SDL_BLENDMODE_BLEND;
	const SDL_BlendMode blend_add = SDL_BLENDMODE_ADD;
	const SDL_BlendMode blend_mod = SDL_BLENDMODE_MOD;

	class color
	{
	public:
		Uint8 r = 255, g = 255, b = 255, a = 255;
		std::string name;
		color() {}
		color(int red, int green, int blue, int alpha) : r(red), g(green), b(blue), a(alpha) {}
		color(std::string name, int red, int green, int blue, int alpha) : name(name), r(red), g(green), b(blue), a(alpha) {}
		static color white() { return color("White", 255, 255, 255, 255); }
		static color black() { return color("Black", 0, 0, 0, 255); }
		static color gray() { return color("Gray", 211, 211, 211, 255); }
		static color dark_gray() { return color("DarkGray", 60, 60, 60, 255); }

		static color red() { return color("Red", 255, 0, 0, 255); }
		static color orange() { return color("Orange", 236, 63, 20, 255); }
		static color yellow() { return color("Yellow", 255, 242, 0, 255); }
		static color green() { return color("Green", 0, 255, 0, 255); }
		static color blue() { return color("Blue", 144, 19, 254, 255); }

		static color peach() { return color("Peach", 211, 129, 131, 255); }
		static color blood() { return color("Blood", 136, 0, 21, 255); }
		static color bronze() { return color("Bronze", 168, 100, 26, 255); }
		static color lavender() { return color("Lavender", 206, 160, 248, 255); }
		static color phisphorus() { return color("Phisphorus", 191, 244, 51, 255); }
		static color purple() { return color("Purple", 255, 198, 205, 255); }
		static color hot_pink() { return color("Hot Pink", 255, 0, 128, 255); }
		static color sky_blue() { return color("Sky Blue", 0, 163, 232, 255); }
		static color aqua_green() { return color("Aqua Green", 78, 201, 176, 255); }
		static color grass_green() { return color("Grass Green", 34, 177, 76, 255); }
		static color olive_green() { return color("Olive Green", 128, 128, 0, 255); }
		static color gold() { return color("Gold", 218, 165, 32, 255); }
		static color tan() { return color("Tan", 210, 180, 140, 255); }
		color shaded_by(float factor) { return color(static_cast<Uint8>(r*factor), static_cast<Uint8>(g*factor), static_cast<Uint8>(b*factor), a); }
//		color tinted_by(float factor) { return color(r+r*factor, g+g*factor, b+b*factor, a); }
		static color random()
			{
			int random = int(19 * rand() / float(RAND_MAX));
			color c = color::black();
			switch (random)
				{
				case 0: c = gold(); break;
				case 1: c = tan(); break;
				case 2: c = gold(); break;
				case 3: c = red(); break;
				case 4: c = orange(); break;
				case 5: c = yellow(); break;
				case 6: c = green(); break;
				case 7: c = blue(); break;
				case 8: c = peach(); break;
				case 9: c = blood(); break;
				case 10: c = bronze(); break;
				case 11: c = lavender(); break;
				case 12: c = phisphorus(); break;
				case 13: c = purple(); break;
				case 14: c = hot_pink(); break;
				case 15: c = sky_blue(); break;
				case 16: c = aqua_green(); break;
				case 17: c = grass_green(); break;
				case 18: c = olive_green(); break;
				}
			return c;
			}
	};

	class sdl
	{
	public:
		sdl()
		{
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
			{
				std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
				return;
			}

			auto init_value = IMG_INIT_PNG;
			if ((IMG_Init(init_value) & init_value) != init_value)
			{
				std::cout << "IMG_Init failed" << std::endl;
				SDL_Quit();
				return;
			}

			if (TTF_Init() != 0) {
				std::cout << "TTF_Init failed" << std::endl;
				SDL_Quit();
				return;
			}
		}
		~sdl()
		{
			IMG_Quit();
			SDL_Quit();
		}
	};

	class quit { };

	class window
	{
		SDL_Window *win;
	public:
		window(xg::vec2 dimensions)
		{
			// Settings
            SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			this->win = SDL_CreateWindow("xg", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, int(dimensions.x), int(dimensions.y), SDL_RENDERER_ACCELERATED | /*SDL_WINDOW_FULLSCREEN |*/ SDL_WINDOW_ALLOW_HIGHDPI);
			if (win == nullptr)
			{
				std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
				SDL_Quit();
				return;
			}
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
		}

		operator SDL_Window* ()
		{
			return this->win;
		}

		bool waiting_to_start()
		{
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					throw quit();
				}
				if (e.type == SDL_KEYDOWN)
				{
					if (e.key.keysym.sym == SDLK_DOWN)
						return false;
				}
			}
			return true;
		}

		int new_frame()
		{
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					throw quit();
				}
				if (e.type == SDLK_KP_PLUS)
				{
					return 0;
				}
				if (e.type == SDLK_KP_MINUS)
				{
					return -1;
				}
			}

			return true;
		}
		void show_cursor() { SDL_ShowCursor(SDL_ENABLE); }
		void hide_cursor() { SDL_ShowCursor(SDL_DISABLE); }
		~window()
		{
			SDL_DestroyWindow(this->win);
		}
	};

	class renderer
	{
		SDL_Renderer *ren;
	public:
		float gamma = 0.75f;
		xg::color background_color = xg::color::black();
		xg::color color;
		renderer(window& Window)
		{
			this->ren = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (ren == nullptr)
			{
				std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			}

			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		}

		operator SDL_Renderer* ()
		{
			return this->ren;
		}

		void new_frame()
		{
			auto& c = this->background_color;
			SDL_SetRenderDrawColor(this->ren, c.r, c.g, c.b, c.a);
			SDL_RenderClear(this->ren);
		}

		void present()
		{
			SDL_RenderPresent(this->ren);
		}

		void set_color(const xg::color& c)
		{
			// make a copy (sorry, SDL extensions such as thick line, require a color in the function call)
			this->color = c;
			// activate color
			SDL_SetRenderDrawColor(*this,
				static_cast<Uint8>(c.r*gamma),
				static_cast<Uint8>(c.g*gamma),
				static_cast<Uint8>(c.b*gamma),
				c.a);
		}

		template<typename T> void render(T& t)
		{
			t.render(*this);
		}

		~renderer()
		{
			SDL_DestroyRenderer(this->ren);
		}
	};

	class texture
	{
		SDL_Texture *tex;
		renderer& ren;
	public:
		xg::vec2 origin = { 0,0 };
		xg::vec2 dimensions;
		float alpha = 1;
		int align = center_h | center_v;
		SDL_BlendMode blend = blend_none;
		texture(std::string name, renderer& Renderer) : ren(Renderer)
		{
			this->tex = IMG_LoadTexture(this->ren, name.c_str());
			if (this->tex == nullptr)
			{
				std::cout << "texture failed:" + name << std::endl;
			}

			int w, h;
			SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
			this->dimensions.x = static_cast<float>(w);
			this->dimensions.y = static_cast<float>(h);
		}

		texture(std::string name, xg::color color, renderer& Renderer) : texture(name, Renderer)
		{
			//SDL_SetTextureColorMod(this->tex, color.r, color.g, color.b);
		}

		void render(xg::vec2 origin, xg::vec2 dimensions)
		{
			int x, y, w, h;
			x = static_cast<int>(origin.x);
			y = static_cast<int>(origin.y);
			w = static_cast<int>(dimensions.x);
			h = static_cast<int>(dimensions.y);

			if (this->align & center_h) x -= w / 2;
			if (this->align & center_v) y -= h / 2;
			if (this->align & right)	x -= w;
			if (this->align & bottom)	y -= h;

			SDL_Rect dst = { x,y,w,h };
			SDL_SetTextureBlendMode(this->tex, this->blend);
			SDL_RenderCopy(this->ren, this->tex, nullptr, &dst);
			SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_NONE);
		}

		void render()
		{
			this->render(this->origin, this->dimensions);
		}
	};

	class static_text
	{
		TTF_Font *font;
		SDL_Texture* texture;
	public:

		vec2 origin;
		int align = top | left;
		static_text(xg::renderer& renderer, std::string font, int size, std::string message)
		{
			this->font = TTF_OpenFont(font.c_str(), size);
			if (this->font == nullptr)
			{
				std::cout << "TTF_OpenFont can't open: " << font << std::endl;
				return;
			}

			SDL_Color color = { 255,255,255,255 };
			SDL_Color color0 = { 0,0,0,255 };

			SDL_Surface* surf = TTF_RenderText_Blended(this->font, message.c_str(), color);
			if (surf == nullptr)
				{
				std::cout << "text::render: TTF_RenderText failed" << std::endl;
				return;
				}

			texture = SDL_CreateTextureFromSurface(renderer, surf);
			if (texture == nullptr)
				{
				std::cout << "text::render: CreateTexture failed" << std::endl;
				}

			SDL_FreeSurface(surf);
		}

		void render(xg::renderer& renderer)
		{
			int x, y, w, h;
			SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
			x = static_cast<int>(this->origin.x);
			y = static_cast<int>(this->origin.y);

			if (this->align & center_h) x -= w / 2;
			if (this->align & center_v) y -= h / 2;
			if (this->align & right)	x -= w;
			if (this->align & bottom)	y -= h;

			SDL_Rect dst = { x,y,w,h };
			SDL_RenderCopy(renderer, texture, nullptr, &dst);
		}

		~static_text()
		{
			SDL_DestroyTexture(texture);
			TTF_CloseFont(this->font);
		}
	};


	class text
		{
		TTF_Font* font;
		public:
			vec2 origin;
			int align = top | left;
			xg::color color;
			xg::color background_color;
			float spacing;
			text(renderer& renderer, std::string font, int size): spacing(float(size))
				{
				this->font = TTF_OpenFont(font.c_str(), size);
				if (this->font == nullptr)
					{
					std::cout << "TTF_OpenFont can't open: " << font << std::endl;
					return;
					}
				}

			void render(xg::renderer& renderer, std::string message)
				{
				SDL_Color tcolor = { color.r, color.b, color.b, color.a };
				SDL_Color bcolor = { background_color.r, background_color.b, background_color.b, background_color.a };

				SDL_Surface* surf = TTF_RenderText_Blended(this->font, message.c_str(), tcolor);
				if (surf == nullptr)
					{
					std::cout << "text::render: TTF_RenderText failed" << std::endl;
					return;
					}

				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
				if (texture == nullptr)
					{
					std::cout << "text::render: CreateTexture failed" << std::endl;
					}

				int x, y, w, h;
				SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
				x = static_cast<int>(this->origin.x);
				y = static_cast<int>(this->origin.y);

				if (this->align & center_h) x -= w / 2;
				if (this->align & center_v) y -= h / 2;
				if (this->align & right)	x -= w;
				if (this->align & bottom)	y -= h;

				SDL_Rect dst = { x,y,w,h };
				SDL_RenderCopy(renderer, texture, nullptr, &dst);
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surf);
				}

			~text()
				{
				TTF_CloseFont(this->font);
				}
		};

	struct mouse_state
		{
		int x = 0;
		int y = 0;
		mouse_state()
			{
			SDL_CaptureMouse(SDL_TRUE);
			}
		void new_frame()
			{
			SDL_PumpEvents();
			SDL_GetMouseState(&x, &y);
			}
		};

	struct mouse_state_display
		{
		mouse_state state;
		text text;
		mouse_state_display(renderer& renderer, std::string font_name, int font_size) :
			text(renderer, font_name, font_size)
			{
			text.origin = v{ 10,10 };
			text.color = color::white();
			}

		void render(renderer& renderer)
			{
			state.new_frame();
			std::stringstream str;
			str << state.x << "," << state.y;
			text.origin = v{ 1000,10 };
			text.render(renderer, str.str());
			}
		};

	class time
	{
		Uint64 time_value = 0;
		Uint64 time_seconds = 0;
		Uint64 time_delta = 0;
		Uint64 time_gameplay = 0;
		std::list<Uint64> history;
	public:
		bool pause = false;
		time()
		{
			this->time_value = SDL_GetTicks();
			this->time_delta = 0;
		}
		bool new_frame()
		{
			auto now = SDL_GetTicks();
			this->time_delta = now - this->time_value;
			this->time_value = now;

			if (!this->pause)
				this->time_gameplay += this->time_delta;

			this->history.push_back(this->time_delta);
			if (history.size() > 60)
				this->history.pop_front();

			Uint64 seconds = this->time_gameplay / 1000;
			if (time_seconds != seconds)
				{
				time_seconds = seconds;
				return true;
				}
			return false;
		}
		Uint64 delta() const
		{
			return this->time_delta;
		}
		float seconds() const
		{
			return this->time_gameplay / 1000.0f;
		}
		int fps() const
		{
			auto total = std::accumulate(this->history.begin(), this->history.end(), 0.0f);
			return int(std::ceilf(1000.0f * float(this->history.size()) / float(total)));
		}
        operator float() const
        {
            return time_gameplay;
        }
	};

	class framerate
	{
		const time& t;
		const int size = 10;
		std::size_t counter = 0;
		vector<float> frames;
	public:
		framerate(const time& Time) : t(Time), frames(size, 0)
		{}

		void new_frame()
		{
			frames[++counter % size] = static_cast<float>(t.delta());
		}

		float per_second() const
		{
			float sum = static_cast<float>(std::accumulate(frames.begin(), frames.end(), 0.0));
			return sum / static_cast<float>(size);
		}
	};

	class point
		{
		public:
			vec2 origin;
			void render(renderer& ren)
				{
				SDL_RenderDrawPointF(ren, origin.x, origin.y);
				}
		};

	class line
	{
	public:
		vec2 p0, p1;
		void render(renderer& ren)
		{
			SDL_RenderDrawLine(ren, static_cast<int>(p0.x), static_cast<int>(p0.y), static_cast<int>(p1.x), static_cast<int>(p1.y));
		}
	};

	class thick_line
	{
	public:
		vec2 p0, p1;
		int width;
		void render(renderer& ren)
		{
			color& color = ren.color;
			thickLineRGBA(ren, static_cast<int>(p0.x), static_cast<int>(p0.y), static_cast<int>(p1.x), static_cast<int>(p1.y), width, color.r, color.g, color.b, color.a);
		}
	};

	class rect
	{
	public:
		vec2 origin;
		vec2 dimensions;
		int align = center_h | center_v;
		void render(renderer& ren)
		{
			int x, y, w, h;
			x = static_cast<int>(this->origin.x);
			y = static_cast<int>(this->origin.y);
			w = static_cast<int>(this->dimensions.x);
			h = static_cast<int>(this->dimensions.y);

			if (this->align & center_h) x -= w / 2;
			if (this->align & center_v) y -= h / 2;
			if (this->align & right)	x -= w;
			if (this->align & bottom)	y -= h;

			SDL_Rect rect = { x, y, w, h };
			SDL_RenderFillRect(ren, &rect);
		}
	};


	class path
	{
		std::size_t size = 0;
		std::unique_ptr<SDL_Point[]> point_buffer;
		vector<vec2> points;
		line line;
	public:
		float scale = 1;
		vec2 origin;
		void clear()
		{
			this->points.clear();
		}
		void push(vec2 point)
		{
			this->points.push_back(point);
		}

		void render(renderer& ren)
		{
			if (this->points.size() > size)
			{
				this->point_buffer.reset();
				this->point_buffer = std::make_unique<SDL_Point[]>(points.size());
				this->size = this->points.size();
			}

			for (std::size_t i = 0; i < this->points.size(); i++)
			{
				vec2 point = this->origin + this->points[i] * this->scale;
				this->point_buffer.get()[i] = { static_cast<int>(point.x), static_cast<int>(point.y) };
			}

			for (std::size_t i = 0; i < this->points.size() - 1; i++)
			{
				line.p0 = this->origin + scale * points[i];
				line.p1 = this->origin + scale * points[i + 1];
				ren.render(line);
				SDL_RenderDrawPoint(ren, static_cast<int>(line.p0.x), static_cast<int>(line.p0.y));
			}
		}
	};

	class polygon
		{
		vector<Sint16> xs;
		vector<Sint16> ys;
		vector<Sint16> active_xs;
		vector<Sint16> active_ys;
		public:
			color color;
			bool filled = true;
			bool outlined = false;
			v origin;
			v active_origin;
			polygon(vector<v> points, xg::color c):color(c)
				{
				for (auto p : points)
					{
					xs.push_back(static_cast<Sint16>(p.x));
					ys.push_back(static_cast<Sint16>(p.y));
					}

				active_xs = xs;
				active_ys = ys;
				}
			void render(renderer& renderer)
				{
				if (active_origin != origin)
					{
					for (size_t i=0; i< xs.size(); i++)
						{
						active_xs[i] = xs[i] + Sint16(origin.x);
						active_ys[i] = ys[i] + Sint16(origin.y);
						}
					active_origin = origin;
					}

				if (xs.size() > 0 && xs.size() == ys.size())
					{
					if (filled)
						{
						filledPolygonRGBA(renderer, &*active_xs.begin(), &*active_ys.begin(), xs.size(), color.r, color.g, color.b, color.a);
						if(outlined)
							polygonRGBA(renderer, &*active_xs.begin(), &*active_ys.begin(), xs.size(), 200, 200, 200, 255);
						}
					else
						{
						polygonRGBA(renderer, &*active_xs.begin(), &*active_ys.begin(), xs.size(), color.r, color.g, color.b, color.a);
						}
					}
				}
		};

	class circle
	{
		static const int point_count = 360;
		path circle_path;
	public:
		float radius;
		vec2 origin;
		circle()
		{
			for (int i = 0; i < point_count; i++)
			{
				this->circle_path.push({ std::sin(pi * i / 180.0f),  std::cos(pi * i / 180.0f) });
			}
		}
		void render(renderer& ren)
		{
			this->circle_path.origin = origin;
			this->circle_path.scale = radius;

			ren.render(this->circle_path);
		}
	};

	class grid
	{
		v origin = { 0, 0 };
		v position = { 0, 0 };
		v space = { 1, 1 };
	public:
		grid& start_at(v at)
		{
			this->origin = at;
			this->position = { 0,0 };
			return *this;
		}
		grid& reset(v reset) {
			this->position *= reset;
			return *this;
		}
		grid& space_by(v by)
		{
			this->space = by;
			return *this;
		}
		grid& step_by(xg::vec2 points)
		{
			this->position += points;
			return *this;
		}
		xg::vec2 operator ++()
		{
			this->step_by(space);
			return origin + position;
		}
		xg::vec2 operator ++(int)
		{
			auto v = origin + position;
			this->step_by(space);
			return v;
		}
		xg::vec2 operator --()
		{
			this->step_by(-space);
			return origin + position;
		}
		xg::vec2 operator --(int)
		{
			auto v = origin + position;
			this->step_by(-space);
			return v;
		}
	};

	class linear_curve
	{
		float start_time;
		float duration = 0;
	public:
		void start(const xg::time& time, float duration)
		{
			this->start_time = time.seconds();
			this->duration = duration;
		}

		float value(const xg::time& time)
		{
			if (!duration)
				return 0.0f;
			float delta = time.seconds() - this->start_time;
			float value = 1 - delta / duration;
			const float min = 0.0f;
			const float max = 1.0f;
			return x::clamp<float>(value, min, max);
		}
	};

	class music
	{
		Mix_Music *sound;
		int channel = 0;
	public:
		music(std::string filename)
		{
			// open 44.1KHz, signed 16bit, system byte order,
			//      stereo audio, using 1024 byte chunks
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
			{
				printf("Mix_OpenAudio: %s\n", Mix_GetError());
			}
			Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG);
			printf("There are %d music deocoders available\n", Mix_GetNumMusicDecoders());
			// print music decoders available
			int i, max = Mix_GetNumMusicDecoders();
			for (i = 0; i < max; ++i)
				printf("Music decoder %d is for %s", i, Mix_GetMusicDecoder(i));

			this->sound = Mix_LoadMUS(filename.c_str());

			if (!sound) {
				printf("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
				// this might be a critical error...
			}

			this->channel = Mix_PlayMusic(this->sound, 1);

		}

		bool new_frame()
		{
			return Mix_PlayingMusic();
		}

		~music()
		{
			Mix_FreeMusic(sound);
			Mix_Quit();
		}
	};

	class mouse
	{
	public:
		v position;
		v delta_position;
		bool left_button_down;
		bool right_button_down;
	};

	class keyboard
	{
	public:
		int button_flags;
		int button_filters;

		enum key
		{
			a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,v,w,x,y,z
		};
	};

	class input
	{
	public:
		mouse mouse;
		keyboard keyboard;
	};

} /* xg */
