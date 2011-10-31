// STL
#include <iostream>
#include <sstream>
// Xapian
#include <xapian.h>

// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {

  // Simplest possible options parsing: we just require two or more
    // parameters.
    if (argc < 3) {
      std::cout << "Usage: " << argv[0]
                << " <path to database> <search terms>" << std::endl;
      return -1;
    }

    // Catch any Xapian::Error exceptions thrown
    try {
      
      // Make the database
      Xapian::Database db (argv[1]);

      // Start an enquire session
      Xapian::Enquire enquire (db);

      std::ostringstream oStr;
      for (int idx=2; idx != argc; ++idx) {
        if (idx != 2) {
          oStr << " ";
// oStr << " AND ";
        }
        const std::string lWord (argv[idx]);
        const std::string lSuggestedWord =
          db.get_spelling_suggestion (lWord, 3);
        std::cout << "Word `" << lWord << "' ==> Suggested word `"
                  << lSuggestedWord << "'" << std::endl;
        oStr << lWord;
      }
      const std::string lQueryString = oStr.str();

      // Build the query object
      Xapian::QueryParser lQueryParser;
      lQueryParser.set_database (db);
      
      std::cout << "Query parser `" << lQueryParser.get_description() << "'"
                << std::endl;

      Xapian::Query lQuery =
        lQueryParser.parse_query (lQueryString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE
                                  | Xapian::QueryParser::FLAG_SPELLING_CORRECTION);
      Xapian::Query lCorrectedQuery = lQueryParser.get_corrected_query_string();
      
      std::cout << "Query `" << lQuery.get_description()
                << "', Corrected query `" << lCorrectedQuery.get_description()
                << "'" << std::endl;
      
      // Give the query object to the enquire session
      enquire.set_query (lQuery);

      // Get the top 10 results of the query
      Xapian::MSet matches = enquire.get_mset (0, 10);

      // Display the results
      int nbMatches = matches.size();
      std::cout << nbMatches << " results found" << std::endl;

      if (nbMatches == 0) {
        enquire.set_query (lCorrectedQuery);
        matches = enquire.get_mset (0, 10);
      }

      const Xapian::Query& lActualQuery = enquire.get_query();
      std::cout << "Actual query `" << lActualQuery.get_description()
                << "'" << std::endl;
      
      for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
        Xapian::Document doc = i.get_document();
        std::cout << "Document ID " << *i << "\t" <<
          i.get_percent() << "% [" <<
          doc.get_data() << "]" << std::endl;
      }
      
    } catch (const Xapian::Error& error) {
      std::cerr << "Exception: "  << error.get_msg() << std::endl;
    }

    return 0;
}
