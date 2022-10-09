#include "bits/stdc++.h"
#include <algorithm>
#include <iterator>
#include <ratio>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

void split_Word(std::string const &str, std::vector<std::string> &result) {
  std::istringstream iss(str);
  for (std::string s; iss >> s;)
    result.push_back(s);
}

std::string get_Function_Name(std::string &str) {
  std::string delimiter = " ";
  std::string token = str.erase(0, str.find(delimiter) + delimiter.length());
  delimiter = "\(";
  token = token.substr(0, token.find(delimiter));
  std::string::iterator end_pos = std::remove(token.begin(), token.end(), ' ');
  token.erase(end_pos, token.end());
  return token;
}

std::string removeComments(std::string str, bool &multiline_Comment) {
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

int count_Commas(std::string const &str) {
  auto number_Commas = 0;
  const std::regex r("^(\\s*)(def)(\\s*).*");
  if (std::regex_match(str, r)) {
    number_Commas = std::count(str.begin(), str.end(), ',');
  }
  return number_Commas;
}

bool is_Function(const std::string &str) {

  int number_Commas = count_Commas(str);

  const std::regex function_Without_Parameters(
      "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s)*\\)\\:");
  std::string regular_Expression =
      "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s*)\\w+((\\s*)\\,(\\s*)\\w+){" +
      std::to_string(number_Commas) + "}(\\s*)\\)\\:";
  const std::regex function_With_Parameters(regular_Expression);

  if (std::regex_match(str, function_Without_Parameters) ||
      std::regex_match(str, function_With_Parameters))
    return true;

  return false;
}

bool is_Main(const std::string &str) {
  std::regex r("\\S.*");
  if (std::regex_match(str, r) || (str == "if __name__ == \"__main__\":"))
    return true;
  return false;
}

std::vector<std::string> read_Function(std::vector<std::string> &lines,
                                       const int start, const int end) {
  std::vector<std::string> v(&lines[start], &lines[end]);
  return v;
}

int main() {
  std::fstream my_File;
  my_File.open("Proyecto Final.txt",
               std::fstream::in | std::fstream::out | std::fstream::app);

  if (!my_File.is_open()) {
    std::cout << "ERROR al abrir el archivo\n";
    return -1;
  }

  std::map<int, std::string> modules{};
  bool found_Main = false;

  std::string line;
  std::vector<std::string> lines{};
  bool multiline_Comment = false;
  int number_Line = 0;
  while (std::getline(my_File, line)) {
    line = removeComments(line, multiline_Comment);
    if (line.empty()) {
      continue;
    }

    lines.push_back(line);
    auto it = modules.end();
    if (is_Function(line)) {
      modules.insert({number_Line, get_Function_Name(line)});
    } else if (number_Line >= it->first + 1 && is_Main(line) && !found_Main) {
      modules.insert({number_Line, "Main"});
      found_Main = true;
    }

    number_Line++;
  }

  for (auto x : modules) {
    std::cout << x.second << "\n";
  }

  /* auto it = modules.begin(); */

  /* std::cout << (++it)->first << "\n"; */

  return 0;
}
