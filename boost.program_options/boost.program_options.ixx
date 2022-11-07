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


template class boost::program_options::typed_value<int, char>;
template class boost::program_options::typed_value<std::string, char>;

namespace po = boost::program_options;
template std::string& po::variable_value::as<std::string>();
template std::string const& po::variable_value::as<std::string>() const;

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

#include "libs/program_options/src/src.hpp"
