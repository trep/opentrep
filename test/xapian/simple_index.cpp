// STL
#include <iostream>
// Xapian
#include <xapian.h>

// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {
  
  // Simplest possible options parsing: we just require three or more
  // parameters.
  if (argc < 4) {
    std::cout << "Usage: " << argv[0]
              << " <path to database> <document data> <document terms>"
              << std::endl;
    return -1;
  }

  // Catch any Xapian::Error exceptions thrown
  try {
    
    // Make the database
    Xapian::WritableDatabase database (argv[1], Xapian::DB_CREATE_OR_OPEN);
    
    // Make the document
    Xapian::Document newdocument;
    
    // Put the data in the document
    newdocument.set_data (argv[2]);
    
    // Put the terms into the document
    for (int i = 3; i < argc; ++i) {
      newdocument.add_posting (argv[i], i - 2);
    }
    
    // Add the document to the database
    database.add_document (newdocument);
    
  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;
  }

  return 0;
}
