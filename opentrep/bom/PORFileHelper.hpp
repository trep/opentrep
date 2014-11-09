#ifndef __OPENTREP_BOM_PORFILEHELPER_HPP
#define __OPENTREP_BOM_PORFILEHELPER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {

  /**
   * @brief Utility class to ease the access to the POR (points of reference)
   * file.
   *
   * \see The POR file is optd_por_public.csv, located in the
   * http://github.com/opentraveldata/opentraveldata directory.
   */
  class PORFileHelper {
  public:
    /**
     * Get the underlying input file stream.
     *
     * An "uncompressor" filter (gzip, bzip2) may be used, if needed.
     */
    std::istream& getFileStreamRef() const;

  public:
    /**
     * Constructor.
     *
     * @param const PORFilePath_T&
     */
    PORFileHelper (const PORFilePath_T&);

    /**
     * Copy constructor.
     */
    PORFileHelper (const PORFileHelper&);

    /**
     * Destructor.
     */
    ~PORFileHelper();

  private:
    // ///////////////// Helper methods ////////////////
    /**
     * Set a file stream pointer on the POR (points of reference) file.
     *
     * @param const PORFilePath_T&
     */
    void init (const PORFilePath_T&);


  private:
    /**
     * Default constructor.
     */
    PORFileHelper();

  private:
    // /////////////// Attributes ////////////////
    /**
     * A pointer on the underlying input stream
     * An "uncompressor" filter (gzip, bzip2) may be used.
     * NULL when the file cannot be found or cannot be read
     */
    std::istream* _iStreamPtr;
  };

}
#endif // __OPENTREP_BOM_PORFILEHELPER_HPP
