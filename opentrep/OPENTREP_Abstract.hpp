#ifndef __OPENTREP_OPENTREP_ABSTRACT_HPP
#define __OPENTREP_OPENTREP_ABSTRACT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>

namespace OPENTREP {

  /** 
   * Base class for the OPENTREP interface structures. 
   */
  struct OPENTREP_Abstract {
  public:
    // /////////// Display support methods /////////
    /**
     * Dump a structure into an output stream.
     *
     * @param ostream& the output stream. 
     */
    virtual void toStream (std::ostream& ioOut) const = 0;

    /**
     * Read a structure from an input stream.
     *
     * @param istream& the input stream. 
     */
    virtual void fromStream (std::istream& ioIn) = 0;

    /**
     * Get the serialised version of the structure. 
     */
    virtual std::string toString() const = 0;
    

  protected:
    /**
     * Protected Default Constructor to ensure this class is abtract. 
     */
    OPENTREP_Abstract () {}
    OPENTREP_Abstract (const OPENTREP_Abstract&) {}

    /**
     * Destructor. 
     */
    virtual ~OPENTREP_Abstract() {}
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
            const OPENTREP::OPENTREP_Abstract& iStructure) {
  /**
   * string stream:
   *  - with same format
   *  - without special field width
   */
  std::basic_ostringstream<charT,traits> ostr;
  ostr.copyfmt (ioOut);
  ostr.width (0);

  // Fill string stream
  iStructure.toStream (ostr);

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
            OPENTREP::OPENTREP_Abstract& ioStucture) {
  // Fill Bom object with input stream
  ioStucture.fromStream (ioIn);
  return ioIn;
}

#endif // __OPENTREP_OPENTREP_ABSTRACT_HPP
