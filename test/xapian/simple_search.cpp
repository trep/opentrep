// STL
#include <iostream>
#include <string>
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
      
    // Open the database for searching.
    Xapian::Database db (argv[1]);

    // Start an enquire session
    Xapian::Enquire enquire (db);

    // Combine the rest of the command line arguments with spaces between
    // them, so that simple queries don't have to be quoted at the shell
    // level.
    std::string query_string (argv[2]);
    argv += 3;
    while (*argv) {
      query_string += ' ';
      query_string += *argv++;
    }

    // Parse the query string to produce a Xapian::Query object.
    Xapian::QueryParser qp;
    Xapian::Stem stemmer ("english");
    qp.set_stemmer (stemmer);
    qp.set_database (db);
    qp.set_stemming_strategy (Xapian::QueryParser::STEM_SOME);
    Xapian::Query query = qp.parse_query (query_string);
    std::cout << "Parsed query is: " << query.get_description() << std::endl;

    // Find the top 10 results for the query.
    enquire.set_query (query);
    Xapian::MSet matches = enquire.get_mset(0, 10);

    // Display the results.
    std::cout << matches.get_matches_estimated() << " results found."
              << std::endl;
    std::cout << "Matches 1-" << matches.size() << ":" << std::endl << std::endl;

    for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
      std::cout << i.get_rank() + 1 << ": " << i.get_percent() << "% docid="
                << *i << " [" << i.get_document().get_data() << "]"
                << std::endl << std::endl;
    }
      
  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;
  }

  return 0;
}
