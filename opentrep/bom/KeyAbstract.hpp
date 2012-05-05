#ifndef __OPENTREP_KEY_KEYABSTRACT_HPP
#define __OPENTREP_KEY_KEYABSTRACT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <sstream>
#include <string>

namespace OPENTREP {

  /**
   * @brief Base class for the Business Object Model (KEY) layer.
   */
  class KeyAbstract {
  public:
    // /////////// Display support methods /////////
    /**
     * Dump a Business Object into an output stream.
     *
     * @param ostream& the output stream.
     */
    virtual void toStream (std::ostream&) const = 0;

    /**
     * Read a Business Object from an input stream.
     *
     * @param istream& the input stream.
     */
    virtual void fromStream (std::istream&) = 0;

    /**
     * Get the serialised version of the Business Object.
     */
    virtual std::string toString() const = 0;
    
    /**
     * Get a string describing the key (differentiating two objects
     * at any level).
     */
    virtual std::string describe() const = 0;

    
  protected:
    /**
     * Protected Default Constructor to ensure this class is abtract.
     */
    KeyAbstract() {}
    KeyAbstract (const KeyAbstract&) {}

    /**
     * Destructor.
     */
    virtual ~KeyAbstract() {}
  };
}

/**
 * Piece of code given by Nicolai M. Josuttis, Section 13.12.1 "Implementing
 * Output Operators" (p653) of his book "The C++ Standard Library: A Tutorial
 * and Reference", published by Addison-Wesley.
 */
template <class charT, class traits>
inline
std::basic_ostream<charT, traits>&
operator<< (std::basic_ostream<charT, traits>& ioOut,
            const OPENTREP::KeyAbstract& iKey) {
  /**
   *  string stream:
   *  - with same format
   *  - without special field width
   */
  std::basic_ostringstream<charT,traits> ostr;
  ostr.copyfmt (ioOut);
  ostr.width (0);

  // Fill string stream
  iKey.toStream (ostr);

  // Print string stream
  ioOut << ostr.str();

  return ioOut;
}

/**
 * Piece of code given by Nicolai M. Josuttis, Section 13.12.1 "Implementing
 * Output Operators" (pp655-657) of his book "The C++ Standard Library:
 * A Tutorial and Reference", published by Addison-Wesley.
 */
template <class charT, class traits>
inline
std::basic_istream<charT, traits>&
operator>> (std::basic_istream<charT, traits>& ioIn,
            OPENTREP::KeyAbstract& ioKey) {
  // Fill Key object with input stream
  ioKey.fromStream (ioIn);
  return ioIn;
}

#endif // __OPENTREP_KEY_KEYABSTRACT_HPP
