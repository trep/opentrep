// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OpenTREP
#include <opentrep/bom/Document.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Document::~Document () {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Document::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _queryString;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Document::describeKey() const {
    std::ostringstream oStr;
    oStr << "`" << describeShortKey() << "'";
    if (_correctedQueryString.empty() == false) {
      oStr << " (corrected into `" << _correctedQueryString
           << "' with an edit distance/error of " << _editDistance << ")";
    }
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Document::toShortString() const {
    std::ostringstream oStr;
    oStr << describeShortKey();
    
    const Xapian::docid& lDocID = _document.get_docid();
    oStr << " => Document ID " << lDocID << " matching at " << _percentage
         << "% (edit distance of " << _editDistance << " over "
         << _allowableEditDistance << ") [" << _document.get_data() << "]";

    if (_documentList.empty() == false) {
      oStr << "  along with " << _documentList.size()
           << " other equivalent matching document(s) (";

      unsigned short idx = 0;
      for (XapianDocumentList_T::const_iterator itDoc = _documentList.begin();
           itDoc != _documentList.end(); ++itDoc, ++idx) {
        const Xapian::Document& lXapianDoc = *itDoc;
        const Xapian::docid& lDocID = lXapianDoc.get_docid();
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << lDocID;
      }
      oStr << ")";
    }
    
    if (_alternateDocumentList.empty() == false) {
      oStr << "  and with still " << _alternateDocumentList.size()
           << " other less matching document(s) (";

      unsigned short idx = 0;
      for (XapianAlternateDocumentList_T::const_iterator itDoc =
             _alternateDocumentList.begin();
           itDoc != _alternateDocumentList.end(); ++itDoc, ++idx) {
        const Xapian::percent& lPercentage = itDoc->first;
        const Xapian::Document& lXapianDoc = itDoc->second;
        const Xapian::docid& lDocID = lXapianDoc.get_docid();
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << lDocID << " / " << lPercentage << "%";
      }
      oStr << ")." << std::endl;
      
    } else {
      oStr << std::endl;
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  std::string Document::toString() const {
    std::ostringstream oStr;
    oStr << describeKey();
    
    const Xapian::docid& lDocID = _document.get_docid();
    oStr << " => Document ID " << lDocID << " matching at " << _percentage
         << "% [" << _document.get_data() << "]";

    if (_documentList.empty() == false) {
      oStr << "  along with " << _documentList.size()
           << " other equivalent matching document(s) { ";

      unsigned short idx = 0;
      for (XapianDocumentList_T::const_iterator itDoc = _documentList.begin();
           itDoc != _documentList.end(); ++itDoc, ++idx) {
        const Xapian::Document& lXapianDoc = *itDoc;
        const Xapian::docid& lDocID = lXapianDoc.get_docid();
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << "Doc ID " << lDocID << " [" << lXapianDoc.get_data() << "]";
      }
      oStr << " }";
    }
    
    if (_alternateDocumentList.empty() == false) {
      oStr << "  and with still " << _alternateDocumentList.size()
           << " other less matching document(s) { ";

      unsigned short idx = 0;
      for (XapianAlternateDocumentList_T::const_iterator itDoc =
             _alternateDocumentList.begin();
           itDoc != _alternateDocumentList.end(); ++itDoc, ++idx) {
        const Xapian::percent& lPercentage = itDoc->first;
        const Xapian::Document& lXapianDoc = itDoc->second;
        const Xapian::docid& lDocID = lXapianDoc.get_docid();
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << lDocID << " / " << lPercentage << "% ["
             << lXapianDoc.get_data() << "]";
      }
      oStr << " }." << std::endl;
      
    } else {
      oStr << std::endl;
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void Document::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void Document::fromStream (std::istream& ioIn) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T Document::notifyIfExtraMatch () const {
    NbOfMatches_T oNbOfMatches = _documentList.size();
    
    // DEBUG
    if (oNbOfMatches != 0) {
      OPENTREP_LOG_NOTIFICATION ("NOTE: the following document gets several "
                                 << "matches with the same matching percentage."
                                 << " You may want to alter the SQL database "
                                 << "and re-index the Xapian database, so as "
                                 << "to allow a more specific match:"
                                 << std::endl << toString());
    }

    // Return the total number of matches (main plus extras)
    return (1 + oNbOfMatches);
  }

}
