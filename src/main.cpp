#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  std::cout << "$ ";

  std::string command;
  std::getline(std::cin, command);
  // if (command == "echo hello world") {
  //   std::cout << "hello world" << std::endl;
  // } else {
    std::string first_word = command.substr(0, command.find(' '));
    std::cerr << first_word << ": command not found" << std::endl;
  // }
}
