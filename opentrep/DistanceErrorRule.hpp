#ifndef __OPENTREP_DISTANCEERRORRULE_HPP
#define __OPENTREP_DISTANCEERRORRULE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <map>
// Boost Array
#include <boost/array.hpp>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/OPENTREP_Abstract.hpp>

namespace OPENTREP {

  // ////////////////////////////////////////////////////////////////////
  /** 
   * Default distance error scale.
   *
   * Allowed error for a given number of letters:
   * <ul>
   *  <li><   4 => 0</li>
   *  <li><   7 => 1</li>
   *  <li><  10 => 2</li>
   *  <li><  15 => 3</li>
   *  <li><  20 => 4</li>
   *  <li>>= 20 => (nb of letters) / 5</li>
   *  </ul> 
   */
  static const DistanceErrorScaleArray_T K_DEFAULT_ERROR_SCALE =
    { {3, 6, 9, 14, 19} };


  // ///////////////////////////////////////////////////////////////
  /** 
   * Structure modelling a (geographical) distanceerrorrule. 
   */
  struct DistanceErrorRule : public OPENTREP_Abstract {
  public:
    // ///////// Getters ////////
    /**
     * Get the DistanceErrorRule code. 
     */
    const DistanceErrorScale_T& getDistanceErrorScale() const {
      return _scale;
    }
    
    
    // ///////// Setters //////////

    
  public:
    // ///////// Business methods ////////
    /**
     * Get, for a given number of letters, the corresponding allowed
     * (distance) error. 
     */
    NbOfErrors_T getAllowedDistanceError (const NbOfLetters_T& iNbOfLetters) const {
      NbOfErrors_T oNbOfErrors_T = 0;

      DistanceErrorScale_T::const_iterator itError =
        _scale.lower_bound (iNbOfLetters);

      if (itError != _scale.end()) {
        oNbOfErrors_T = itError->second;
        
      } else {
        oNbOfErrors_T = iNbOfLetters / 4;
      }
      
      return oNbOfErrors_T;
    }
    
    
  public:
    // ///////// Display methods ////////
    /**
     * Dump a structure into an output stream.
     *
     * @param ostream& the output stream. 
     */
    void toStream (std::ostream& ioOut) const {
      ioOut << toString();
    }

    /**
     * Read a structure from an input stream.
     *
     * @param istream& the input stream. 
     */
    void fromStream (std::istream&) {
    }

    /**
     * Get a short display of the DistanceErrorRule structure. 
     */
    std::string toShortString() const {
      std::ostringstream oStr;
      NbOfLetters_T idx = 0;
      for (DistanceErrorScale_T::const_iterator itError = _scale.begin();
           itError != _scale.end(); ++itError, ++idx) {
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << itError->second << ": " << itError->first;
      }
      return oStr.str();
    }
    
    /**
     * Get the serialised version of the DistanceErrorRule structure. 
     */
    std::string toString() const {
      std::ostringstream oStr;
      oStr << toShortString();
      return oStr.str();
    }

    
  public:
    /**
     * Main Constructor. 
     */
    DistanceErrorRule (const DistanceErrorScale_T& iScale)
      : _scale (iScale) {
    }

    /**
     * Constructor. 
     */
    DistanceErrorRule (const NbOfErrors_T iSize,
                       const NbOfLetters_T iScaleArray[]) {
      for (NbOfErrors_T idx = 0; idx != iSize; ++idx) {
        _scale.insert (DistanceErrorScale_T::value_type (iScaleArray[idx],
                                                         idx));
      }
    }

    /**
     * Constructor. 
     */
    DistanceErrorRule (const DistanceErrorScaleArray_T& iScaleArray) {
      for (NbOfErrors_T idx = 0; idx != iScaleArray.size(); ++idx) {
        _scale.insert (DistanceErrorScale_T::value_type (iScaleArray[idx],
                                                         idx));
      }
    }

  private:
    /**
     * Default Constructor. 
     */
    DistanceErrorRule ();

    /**
     * Default copy constructor. 
     */
    DistanceErrorRule (const DistanceErrorRule&);
    
    /**
     * Destructor. 
     */
    virtual ~DistanceErrorRule() {}

    
  private:
    // /////// Attributes /////////
    /** 
     * Scale of distance errors. 
     */
    DistanceErrorScale_T _scale;
  };

}
#endif // __OPENTREP_DISTANCEERRORRULE_HPP
