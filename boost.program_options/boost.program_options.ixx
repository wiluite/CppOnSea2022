//<-- Just to highlight C++ code in FAR Manager
module;

#pragma warning(push)
//#pragma warning(disable: <number>) 

#define BOOST_PROGRAM_OPTIONS_NO_LIB
#define PROGRAM_OPTIONS_MODULE
#define BOOST_NO_STD_WSTRING // not necessary; if used add utf8_codecvt_facet.cpp into src.hpp to compile as well.

#include <fstream>

#include <boost/program_options/options_description.hpp> 
#include <boost/program_options/detail/utf8_codecvt_facet.hpp>
#include <boost/program_options/variables_map.hpp> 
#include <boost/program_options/parsers.hpp> 

#pragma warning(pop)

export module boost.program_options;

#ifdef _MSC_VER
#	pragma comment(lib, "boost.program_options.lib")
#endif


namespace po = boost::program_options;

template class po::typed_value<std::string, char>; // explicit instantiation
template class po::typed_value<int, char>; // as well

// and so on whatever it is required by a client
template std::string& po::variable_value::as<std::string>(); 
template std::string const& po::variable_value::as<std::string>() const;
template int& po::variable_value::as<int>();
template int const& po::variable_value::as<int>() const;

export {
	namespace boost::program_options {
		using po::options_description;
		using po::variables_map;

		using po::value;
		using po::parse_command_line;

		// Unnecessary to export explicitly because it is exported via ADL's parse_command_line return value 
		// basic_parsed_options<>, visible from already included <boost/program_options/variables_map.hpp>  
		using po::store;
		// NOTE: this is true only in a standalone batch build.

		using po::notify;

		using po::positional_options_description;
		using po::command_line_parser;

		std::ostream& operator<<(std::ostream&, const po::options_description&);
	}
}

module :private;

#include "libs/program_options/src/config_file.cpp"
#include "libs/program_options/src/cmdline.cpp"
#include "libs/program_options/src/options_description.cpp"
#include "libs/program_options/src/positional_options.cpp"
#include "libs/program_options/src/convert.cpp"
#include "libs/program_options/src/parsers.cpp"
#include "libs/program_options/src/variables_map.cpp"
#include "libs/program_options/src/value_semantic.cpp"
#include "libs/program_options/src/winmain.cpp"
#include "libs/program_options/src/split.cpp"
