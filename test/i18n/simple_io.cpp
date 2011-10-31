// C
#include <iostream>
#include <string>


// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {

  std::cout << "What is your name? Как тебя завут?" << std::endl;
  std::string name;
  std::cin >> name;

  //
  std::cout << "Your name is: / Тебя завут: " << name << std::endl;
  
  return 0;
}
