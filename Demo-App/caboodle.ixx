// Modularization approach (and this particular code)
// are borrowed from (and inpired by) D.Engert's presentation at CppCon 2022.
//

module;

#include <filesystem>
#include <string>
#include <tuple>
#include <iostream>

export module the.whole.caboodle;
import boost.program_options;
import print;

namespace caboodle {

	// fs::path::string() has unspecified encoding on Windows
	// convert from UTF16 to UTF8 with guaranteed semantics
	export std::string utf8Path(const std::filesystem::path& Path);

	//-------------------------------------------------------------------

	boost::program_options::variables_map parseOptions();

	export auto getOptions() {
		const auto Option = parseOptions();
		return std::tuple{ Option["media"].as<std::string>(),
						   Option["server"].as<std::string>(),
		};
	}
} // caboodle

module : private; // names invisible, declarations unreachable!

using namespace std; // bad practice!

#ifdef _WIN32
#	define APICALL __declspec(dllimport) __stdcall
#else
#	define APICALL
#endif

namespace winapi {

	extern "C" {
		int APICALL WideCharToMultiByte(unsigned, unsigned long, const wchar_t*, int,
			char*, int, const char*, int*);
	}
	static constexpr auto UTF8 = 65001;

	static inline size_t estimateNarrow(wstring_view U16) noexcept {
		return WideCharToMultiByte(UTF8, 0, U16.data(),
			static_cast<int>(U16.size()), nullptr, 0,
			nullptr, nullptr);
	}
	static inline auto convertFromWide(wstring_view U16) noexcept {
		return [&](char* Buffer, size_t Size) -> size_t {
			WideCharToMultiByte(UTF8, 0, U16.data(), static_cast<int>(U16.size()),
				Buffer, static_cast<int>(Size), nullptr, nullptr);
			return Size;
		};
	}

	template <typename Str = string>
		requires(requires(Str s, size_t r, size_t(*f)(char*, size_t)) {
			{ s.resize_and_overwrite(r, f) };
	})
		decltype(auto) toUTF8(wstring_view Utf16, Str&& Utf8 = {}) {
		Utf8.resize_and_overwrite(estimateNarrow(Utf16), convertFromWide(Utf16));
		return static_cast<Str&&>(Utf8);
	}
} // namespace winapi


namespace caboodle {

	string utf8Path(const filesystem::path& Path) {
		if constexpr (_WIN32)
			return winapi::toUTF8(Path.wstring());
		else
			return Path.string();
	}

	auto getHelpText(auto OptionsDescription) {
		std::stringstream ss;
		OptionsDescription.print(ss);
		return ss.str();
	}

	namespace po = boost::program_options;
	auto parseCommandline(po::options_description& opts, po::positional_options_description& popts) -> po::variables_map
	{
#if defined (JUST_HELP_REQUEST)
	    char const * argv[] = { "test_app.exe", "--help" };
#else
		char const* argv[] = { "test_app.exe", "C:\\media_gif", "127.0.0.1"};
#endif

		po::variables_map vm;
		po::store(
			po::command_line_parser(sizeof(argv) / sizeof(argv[0]), argv)
			.options(opts)
			.positional(popts)
			.run(),
			vm
		);
		po::notify(vm);
		return vm;
	}

	auto parseOptions() -> po::variables_map
	{
		po::options_description OptionsDescription("Options available");
		// clang-format off
		OptionsDescription.add_options()
			("help", "produce help message")
			("media", po::value<std::string>()->default_value("media"), "media directory")
			("server", po::value<std::string>()->default_value(""), "server name or ip")
			;
		// clang-format on
		po::positional_options_description PositionalOptions;
		PositionalOptions.add("media", 1).add("server", 2);

		po::variables_map Option;
		bool needHelp = false;
		try {
			Option = parseCommandline(OptionsDescription, PositionalOptions);
		}
		catch (...) {
			needHelp = true;
		}
		try {
			if (Option.count("help") || Option["media"].as<std::string>().contains('?'))
			{
				needHelp = true;
			}
		}
		catch (...)
		{
			println("{}", "Invalid command line! ");
		}

 		if (needHelp) {
			try { 
				println("{}", getHelpText(OptionsDescription));
			} 
			catch (...) {}
			exit(-1);
		}
		else {
			return Option;
		}
	}
} // caboodle
