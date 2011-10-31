// STL
#include <iostream>
// ICU
#include <unicode/choicfmt.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

// //////////// M A I N /////////////
int main (int argc, char *argv[]) {
  double limits[] = {1,2,3,4,5,6,7};

  UnicodeString weekDayNames[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  ChoiceFormat fmt (limits, weekDayNames, 7);

  UnicodeString str;
  for (double x = 1.0; x != 8.0; x += 1.0) {
    fmt.format(x, str);
    std::cout << x << " -> " << str << std::endl;
  }
  
  std::cout << std::endl;
  
  return 0;
}

