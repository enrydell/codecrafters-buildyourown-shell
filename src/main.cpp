#include <iostream>
#include <string>
#include <map>

enum Command {
  CMD_UNKNOWN,
  CMD_EXIT,
  CMD_ECHO,
  CMD_TYPE
};

Command resolveCommand(const std::string& input) {
  static const std::map<std::string, Command> commandMap = {
    {"exit", CMD_EXIT},
    {"echo", CMD_ECHO},
    {"type", CMD_TYPE}
  };
  
  auto it = commandMap.find(input);
  if (it != commandMap.end()) {
    return it->second;
  }

  return CMD_UNKNOWN;
}

void evalBuiltIn(const std::string& command, const std::string& args) {
  std::string cmd = command + " " + args;

  if (resolveCommand(args) == CMD_UNKNOWN) {
    std::cout << args << ": command not found" << std::endl;
    return;
  }

  FILE* pipe = popen(cmd.c_str(), "r");

  if (pipe) {
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
      std::cout << buffer;
    }
    pclose(pipe);
  }
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true) {
    std::cout << "$ ";

    std::string fullCommand;
    std::getline(std::cin, fullCommand);

    std::string command = fullCommand.substr(0, fullCommand.find(' '));
    std::string args = fullCommand.substr(fullCommand.find(' ') + 1);

    switch (resolveCommand(command)) {
      case CMD_EXIT:
        exit(0);
        break;
      case CMD_ECHO:
        std::cout << fullCommand.substr(fullCommand.find(' ') + 1) << std::endl;
        break;
      case CMD_TYPE:
        evalBuiltIn(command, args);
        break;
      default:
        std::cerr << command << ": command not found" << std::endl;
    }
  }
}
