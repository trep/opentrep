// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <istream>
#include <exception>
// Boost
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
// OpenTrep
#include <opentrep/bom/PORFileHelper.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  PORFileHelper::PORFileHelper() : _iStreamPtr (NULL) {
  }

  // //////////////////////////////////////////////////////////////////////
  PORFileHelper::PORFileHelper (const PORFileHelper& iPORFileHelper)
    : _iStreamPtr (iPORFileHelper._iStreamPtr) {
  }

  // //////////////////////////////////////////////////////////////////////
  PORFileHelper::PORFileHelper (const PORFilePath_T& iPORFilePath)
    : _iStreamPtr (NULL) {
    init (iPORFilePath);
  }

  // //////////////////////////////////////////////////////////////////////
  PORFileHelper::~PORFileHelper() {
    delete _iStreamPtr; _iStreamPtr = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  std::istream& PORFileHelper::getFileStreamRef() const {
    assert (_iStreamPtr != NULL);
    return *_iStreamPtr;
  }

  // //////////////////////////////////////////////////////////////////////
  void PORFileHelper::init (const PORFilePath_T& iPORFilePath) {
    // DEBUG
    OPENTREP_LOG_DEBUG ("Checking whether the POR file ('" << iPORFilePath
                        << "') exists, is readable and/or compressed.");

    // Check whether the POR file exists and is readable.
    boost::filesystem::path lPORFilePath (iPORFilePath.begin(),
                                          iPORFilePath.end());
    if (!(boost::filesystem::exists (lPORFilePath)
          && boost::filesystem::is_regular_file (lPORFilePath))) {
      OPENTREP_LOG_ERROR ("The POR file " << iPORFilePath
                          << " does not exist or cannot be open." << std::endl);

      throw FileNotFoundException ("The POR file " + iPORFilePath
                                   + " does not exist or cannot be read");
    }

    /**
     * Check whether the POR file is compressed.
     *
     * Note: that check is not very robust, as it is based on the
     * file extension. A more robust approach would be to extract
     * the magic number (first 4 bytes).
     */
    const boost::filesystem::path& lPORFileExtPath = lPORFilePath.extension();
    const std::string& lPORFileExt = lPORFileExtPath.string();
    if (lPORFileExt == ".bz2") {
      // Open the file
      boost::iostreams::file_source cprdPORFile (iPORFilePath, std::ios_base::in
                                                 | std::ios_base::binary);

      // Uncompress the file with the BZ2 library and its Boost wrapper
      boost::iostreams::filtering_istream* bunzip2Filter_ptr =
        new boost::iostreams::filtering_istream();
      bunzip2Filter_ptr->push (boost::iostreams::bzip2_decompressor());
      bunzip2Filter_ptr->push (cprdPORFile);
      _iStreamPtr = bunzip2Filter_ptr;

    } else if (lPORFileExt == ".gz") {
      // Open the file
      boost::iostreams::file_source cprdPORFile (iPORFilePath, std::ios_base::in
                                                 | std::ios_base::binary);

      // Uncompress the file with the BZ2 library and its Boost wrapper
      boost::iostreams::filtering_istream* gunzipFilter_ptr =
        new boost::iostreams::filtering_istream();
      gunzipFilter_ptr->push (boost::iostreams::gzip_decompressor());
      gunzipFilter_ptr->push (cprdPORFile);
      _iStreamPtr = gunzipFilter_ptr;

    } else if (lPORFileExt == ".csv") {
      // Open the file
      _iStreamPtr = new boost::filesystem::ifstream (iPORFilePath,
                                                     std::ios_base::in);

    } else {
      //
      std::ostringstream errorStr;
      errorStr << "The POR file " << iPORFilePath
               << " has got an unknown extension ('" << lPORFileExt
               << "'). Recognised extensions: .csv, .bz2, .gz";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw FileExtensionUnknownException (errorStr.str());
    }
  }

}
