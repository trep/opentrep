#ifndef __OPENTREP_DBPARAMS_HPP
#define __OPENTREP_DBPARAMS_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/OPENTREP_Abstract.hpp>

namespace OPENTREP {

  /** List of names for a given (geographical) dbparams. */
  typedef std::list<std::string> DBParamsNameList_T;

  
  /** Structure modelling a (geographical) dbparams. */
  struct DBParams : public OPENTREP_Abstract {
  public:
    // ///////// Getters ////////
    /** Get the database user name. */
    std::string getUser() const {
      return _user;
    }
    
    /** Get the database user password. */
    std::string getPassword() const {
      return _passwd;
    }
    
    /** Get the database host name. */
    std::string getHost() const {
      return _host;
    }
    
    /** Get the database port number. */
    std::string getPort() const {
      return _port;
    }
    
    /** Get the database name. */
    std::string getDBName() const {
      return _dbname;
    }
    
    
    // ///////// Setters //////////
    /** Set the database user name. */
    void setUser (const std::string& iUser) {
      _user = iUser;
    }

    /** Set the database password. */
    void setPassword (const std::string& iPasswd) {
      _passwd = iPasswd;
    }

    /** Set the database host name. */
    void setHost (const std::string& iHost) {
      _host = iHost;
    }

    /** Set the database port number. */
    void setPort (const std::string& iPort) {
      _port = iPort;
    }

    /** Set the database name. */
    void setDBName (const std::string& iDBName) {
      _dbname = iDBName;
    }

    
  public:
    // ///////// Busines methods ////////
    /** Check that all the parameters are fine. */
    bool check () const {
      if (_user.empty() == true || _passwd.empty() == true
          || _host.empty() == true || _port.empty()
          || _dbname.empty() == true) {
        return false;
      }
      return true;
    }
    
  public:
    // ///////// Display methods ////////
    /** Dump a structure into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const {
      ioOut << toString();
    }

    /** Read a structure from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream&) {
    }

    /** Get a short display of the DBParams structure. */
    std::string toShortString() const {
      std::ostringstream oStr;
      oStr << _dbname << "." << _user << "@" << _host << ":" << _port;
      return oStr.str();
    }
    
    /** Get the serialised version of the DBParams structure. */
    std::string toString() const {
      std::ostringstream oStr;
      oStr << _dbname << "." << _user << "@" << _host << ":" << _port;
      return oStr.str();
    }

    
  public:
    /** Main Constructor. */
    DBParams (const std::string& iDBUser, const std::string& iDBPasswd,
              const std::string& iDBHost, const std::string& iDBPort,
              const std::string& iDBName)
      : _user (iDBUser), _passwd (iDBPasswd), _host (iDBHost), _port (iDBPort),
        _dbname (iDBName) {
    }

    /** Default Constructor. */
    // DBParams ();
    /** Default copy constructor. */
    // DBParams (const DBParams&);
    
    /** Destructor. */
    virtual ~DBParams() {}

    
  private:
    // /////// Attributes /////////
    /** Database user name. */
    std::string _user;
    /** Database user password. */
    std::string _passwd;
    /** Database host name. */
    std::string _host;
    /** Database port number. */
    std::string _port;
    /** Database name. */
    std::string _dbname;
  };

}
#endif // __OPENTREP_DBPARAMS_HPP
