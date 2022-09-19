#include <bits/stdc++.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

/* Function to remove comments */
std::string remove_Comments(std::string str, bool &multiline_Comment) {
  std::string result;

  bool simple_Comment = false;
  for (int i = 0; i < static_cast<int>(str.length()); i++) {
    /* Si estamos dentro de un comentario */
    if (multiline_Comment || simple_Comment) {
      if (str[i] == '\"' && str[i + 1] == '\"' && str[i + 2] == '\"') {
        multiline_Comment = false;
        i += 2;
      } else if (str[i] == '\n') {
        simple_Comment = false;
      } else
        continue;
    } else {
      if (str[i] == '\"' && str[i + 1] == '\"' && str[i + 2] == '\"') {
        multiline_Comment = true;
        i += 2;
      } else if (str[i] == '#') {
        simple_Comment = true;
        i++;
      } else
        result += str[i];
    }
  }

  return result;
}

bool is_keyword(const std::string &str) {
  const std::vector<std::string> keywords{"def",   "for",   "in",     "return",
                                          "print", "range", "append", "input"};

  for (const auto &keyword : keywords)
    if (keyword == str)
      return true;

  return false;
}

int is_Function(const std::string &str) {
  const std::regex function_Without_Parameters(
      "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s)*\\)\\:");
  const std::regex function_With_Parameter(
      "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s)*(\\w)(\\w*)(\\s)*\\)\\:");
  const std::regex function_With_Too_Parameters(
      "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s)*(\\w)(\\w*)(\\s)*\\,(\\s)*(\\w)(\\w*)"
      "(\\s)*\\)\\:");

  if (std::regex_match(str, function_Without_Parameters))
    return 1;
  else if (std::regex_match(str, function_With_Parameter))
    return 2;
  else if (std::regex_match(str, function_With_Too_Parameters))
    return 3;

  return 0;
}

void split_Word(std::string const &str, std::vector<std::string> &result) {
  std::istringstream iss(str);
  for (std::string s; iss >> s;)
    result.push_back(s);
}

int main() {
  std::fstream my_File;
  my_File.open("Proyecto Final.txt",
               std::fstream::in | std::fstream::out | std::fstream::app);

  if (!my_File.is_open()) {
    std::cout << "ERROR al abrir el archivo\n";
    return -1;
  }

  /* std::vector<std::string> lines; */
  std::string line;
  bool multiline_Comment = false;

  int number_Line = 0;
  while (std::getline(my_File, line)) {
    line = remove_Comments(line, multiline_Comment);

    switch (is_Function(line)) {
    case 1:
      std::cout << "FUNCTION WITHOUT PARAMETERS"
                << "\n";
      break;
    case 2:
      std::cout << "FUNCTION ONE PARAMETER"
                << "\n";
      break;
    case 3:
      std::cout << "FUNCTION TWO PARAMETERS"
                << "\n";
      break;
    default:
      std::cout << line << "\n";
    }

    number_Line++;
  }

  return 0;
}
