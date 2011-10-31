// C
#include <sstream>
#include <string>
#include <list>
#include <vector>
// Boost String
#include <boost/python.hpp>

// //////////////////////////////////////////////
char const* simpleGreet() {
  return "hello, world";
}

// //////////////////////////////////////////////
namespace OPENTREP {

  /** */
  typedef unsigned int OpenTrepID_T;
  
  /** */
  typedef std::vector<std::string> MsgList_T;

  /** */
  typedef std::vector<OpenTrepID_T> IDList_T;

  /** */
  typedef std::pair<MsgList_T, IDList_T> OpenTrepList_T;

  // //////////////////////////////////////////////
  struct World {
  public:

    /** */
    MsgList_T getMsgList () const {
      return _msgList;
    }
    
    /** */
    IDList_T getIDList () const {
      return _idList;
    }
    
    /** */
    OpenTrepList_T getListPair () const {
      return OpenTrepList_T (_msgList, _idList);
    }
    
    /** */
    void add (const std::string& iMessage, const OpenTrepID_T& iID) {
      _msgList.push_back (iMessage);
      _idList.push_back (iID);
    }
    

    /** */
    std::string toString() const {
      std::ostringstream oStr;
      IDList_T::const_iterator itID = _idList.begin();
      OpenTrepID_T idx = 0;
      for (MsgList_T::const_iterator itMsg = _msgList.begin();
           itMsg != _msgList.end() && itID != _idList.end();
           ++itMsg, ++itID, ++idx) {
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << *itID << ": " << *itMsg;
      }
      return oStr.str();
    }
    
    /** */
    std::string toSimpleString() const {
      std::ostringstream oStr;
      IDList_T::const_iterator itID = _idList.begin();
      OpenTrepID_T idx = 0;
      for (MsgList_T::const_iterator itMsg = _msgList.begin();
           itMsg != _msgList.end() && itID != _idList.end();
           ++itMsg, ++itID, ++idx) {
        if (idx != 0) {
          oStr << ",";
        }
        oStr << *itMsg;
      }
      return oStr.str();
    }
    
  private:
    /** */
    MsgList_T _msgList;
    IDList_T _idList;
  };
  
}

// //////////////////////////////////////////////
// using namespace boost::python;
BOOST_PYTHON_MODULE(hello_ext) {
  boost::python::def("greet", simpleGreet);
}


// //////////////////////////////////////////////
BOOST_PYTHON_MODULE(libpyboost) {
  boost::python::class_<OPENTREP::World> ("World")
    .def ("getMsgList", &OPENTREP::World::getMsgList)
    .def ("getIDList", &OPENTREP::World::getIDList)
    .def ("getListPair", &OPENTREP::World::getListPair)
    .def ("add", &OPENTREP::World::add)
    .def ("toSimpleString", &OPENTREP::World::toSimpleString)
    .def ("toString", &OPENTREP::World::toString);
}
