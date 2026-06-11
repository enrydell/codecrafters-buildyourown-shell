#include <iostream>
#include <string>
#include <map>

enum Command {
  CMD_UNKNOWN,
  CMD_EXIT,
  CMD_ECHO
};

Command resolveCommand(const std::string& input) {
  static const std::map<std::string, Command> commandMap = {
    {"exit", CMD_EXIT},
    {"echo", CMD_ECHO}
  };
  
  auto it = commandMap.find(input);
  if (it != commandMap.end()) {
    return it->second;
  }

  return CMD_UNKNOWN;
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true) {
    std::cout << "$ ";

    std::string fullCommand;
    std::getline(std::cin, fullCommand);

    std::string command = fullCommand.substr(0, fullCommand.find(' '));

    switch (resolveCommand(command)) {
      case CMD_EXIT:
        exit(0);
        break;
      case CMD_ECHO:
        std::cout << fullCommand.substr(fullCommand.find(' ') + 1) << std::endl;
        break;
      default:
        std::cerr << command << ": command not found" << std::endl;
    }
  }
}
