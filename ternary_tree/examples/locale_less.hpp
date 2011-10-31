#ifndef LOCALE_LESS_HPP_INCLUDED
#define LOCALE_LESS_HPP_INCLUDED

#include <locale>
#include <string>
#include <functional>
#include <boost/scoped_array.hpp>

//!
//! Localization comparator
//! \file
//!


namespace utility {

// STLPORT, MSVC+Dinkumware, or GCC?
#ifdef _STLPORT_VERSION
	const char* swedish_locale_name = "sv_SE";
#elif (defined _MSC_VER && defined _CPPLIB_VER)
	const char* swedish_locale_name = "swedish";
//#elif defined __GLIBCXX__
#else
	const char* swedish_locale_name = "C";
//# error Must define locale name compatible with your std::locale implementation (see file __FILE__ for context)
#endif

namespace detail {

	//! Expensive single-character comparison by std::locale<> facet collate
	template<class CharT>
	struct collate_comp : public std::binary_function<CharT, CharT, bool>
	{
		collate_comp(const std::string& name)
			: m_locale(name.c_str())
		{}

		bool operator()(const CharT& a, const CharT& b) const
		{
			const CharT * a0 = &a, *b0 = &b;
			return std::use_facet<std::collate<CharT> >(m_locale).compare(a0, a0+1, b0, b0+1) < 0;
		}
	  private:
		std::locale m_locale;
	};

}  // end namespace detail


//! Localized less-comparison (using table lookup initialized by slow collate_comp). \ingroup stringutils
template<class CharT, size_t MaxVal = 256>
struct locale_less : public std::binary_function<CharT, CharT, bool>
{
	enum { max_val = MaxVal };
	
	locale_less(const std::string& name = "") { fill_table(name.c_str()); }

	bool operator()(const wchar_t& a, const wchar_t& b) const {
		return table()[a] < table()[b];
	}

	bool operator()(const char& a, const char& b) const {
		return table()[(unsigned char)a] < table()[(unsigned char)b];
	}

	const CharT& operator[](size_t index) const { return table()[index]; }

	const CharT* table() const { return const_cast<locale_less&>(*this).mutable_table(); }


private:
	enum { mask = CharT(-1) & ((sizeof(CharT)<<8) - 1) };

	void fill_table(const char* name)
	{
		static bool init;
		if (init)
			return;
		init = true;
		// iota operator over table
		CharT tmp[max_val];
		for (unsigned i = 0; i < max_val; ++i)
			tmp[i] = (CharT)i;
		std::sort(tmp, tmp + max_val, detail::collate_comp<CharT>(name));
		// Write inverse mapping to stored table: table[N] == sort_order_index
		CharT* tab = mutable_table();
		for (unsigned i = 1; i < max_val; ++i) {
			tab[ mask & tmp[i] ] = i;
		}
	}
	CharT* mutable_table() {
		static boost::scoped_array<CharT> t(new CharT[MaxVal]);
		return t.get();
	}

};

}  // namespace utility


#endif  // LOCALE_LESS_HPP_INCLUDED
