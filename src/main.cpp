#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

#if defined(_WIN32) || defined(_WIN64)
constexpr char PATH_DELIMITER = ';';
#else
constexpr char PATH_DELIMITER = ':';
#endif

enum Command {
  CMD_UNKNOWN,
  CMD_EXIT,
  CMD_ECHO,
  CMD_TYPE
};

void evalBuiltIn(const std::string &command, const std::string &args) {
  std::string cmd = command + " " + args;

  if (resolveCommand(args) == CMD_ECHO || resolveCommand(args) == CMD_EXIT || resolveCommand(args) == CMD_TYPE) {
    FILE *pipe = popen(cmd.c_str(), "r");

    if (pipe) {
      char buffer[128];
      while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer;
      }
      pclose(pipe);
    }
    return;
  }

  const char *path = std::getenv("PATH");
  if (path != nullptr) {
    std::string paths(path);
    std::size_t start = 0;

    while (start <= paths.size()) {
      std::size_t end = paths.find(PATH_DELIMITER, start);
      std::string directory = paths.substr(start, end - start);

      std::string commandPath = directory + "/" + args;
      if (access(commandPath.c_str(), X_OK) == 0) {
        std::cout << args << " is " << commandPath << std::endl;
        return;
      }

      if (end == std::string::npos) {
        break;
      }

      start = end + 1;
    }
  }

  std::cout << args << ": not found" << std::endl;
}

Command resolveCommand(const std::string &input) {
  static const std::map<std::string, Command> commandMap = {
      {"exit", CMD_EXIT},
      {"echo", CMD_ECHO},
      {"type", CMD_TYPE}};

  auto it = commandMap.find(input);
  if (it != commandMap.end()) {
    return it->second;
  }

  return CMD_UNKNOWN;
}

void executeCommand(const std::string &command, const std::string &args) {
  Command cmd = resolveCommand(command);

  switch (cmd) {
    case CMD_EXIT:
      std::exit(0);
      break;
    case CMD_ECHO:
      std::cout << args << std::endl;
      break;
    case CMD_TYPE:
      evalBuiltIn(command, args);
      break;
    default:
      std::cerr << command << ": command not found" << std::endl;
      break;
  }
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";

    std::string fullCommand;
    std::getline(std::cin, fullCommand);

    std::string command = fullCommand.substr(0, fullCommand.find(' '));
    std::string args = fullCommand.substr(fullCommand.find(' ') + 1);

    executeCommand(command, args);
  }
}
