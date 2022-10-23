#include "bits/stdc++.h"
#include <algorithm>
#include <cstdlib>
#include <ratio>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

/* void split_Word(std::string const &str, std::vector<std::string> &result) {
 */
/*   std::istringstream iss(str); */
/*   for (std::string s; iss >> s;) */
/*     result.push_back(s); */
/* } */

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

void message_Error_Indentation(std::string error_Type, const std::string &line, int &times) {
  std::cout << "Error of: " << error_Type << " line " << times + 1 << "\n\t" << line
            << "\n\t^\n";
  exit(-1);
}

bool is_Function(const std::string &str, int &number_Line) {
  const std::regex begin_Def("(\\s)*(def)(\\s)+(.*)");
  if (std::regex_match(str, begin_Def)) {
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
    else
      message_Error_Indentation("Indentation", str, number_Line);
  }
  return false;
}

bool is_Main(const std::string &str) {
  std::regex r("\\S.*");
  if (std::regex_match(str, r) || (str == "if __name__ == \"__main__\":"))
    return true;
  return false;
}

int count_Beggining_Spaces(const std::string &str) {
  int spaces_At_The_Beggining = 0;
  for (auto i = 0; i < int(str.length()); i++) {
    if (std::isspace(str[i]))
      spaces_At_The_Beggining++;
    else
      break;
  }
  return spaces_At_The_Beggining;
}

void check_Indentation(std::vector<std::string> &str) {
  int spaces_At_The_Beggining = 0;
  int spaces_Default = 0;
  int level_Indentation = 1;
  int times = 0;
  int spaces_Before = 0;

  bool first_Line_After_Colon = false;
  bool Is_function = false;

  for (auto line : str) {
    spaces_At_The_Beggining = count_Beggining_Spaces(line);

    if (times == 0) {
      if (line[line.size() - 1] == ':')
        Is_function = true;
    } else if (times == 1) {
      spaces_Default = spaces_At_The_Beggining;
      if (line[line.size() - 1] == ':' &&
          (spaces_Default * level_Indentation) == spaces_At_The_Beggining) {
        level_Indentation++;
        first_Line_After_Colon = true;
      } else if ((Is_function && spaces_At_The_Beggining == 0) ||
                 (!Is_function && spaces_At_The_Beggining != 0)) {
        message_Error_Indentation("Indentation", line, times);
      }
    } else if (times > 1) {

      if (line[line.size() - 1] == ':' &&
          (spaces_Default * level_Indentation) == spaces_At_The_Beggining) {
        level_Indentation++;
        first_Line_After_Colon = true;
      } else if (level_Indentation > 1 && first_Line_After_Colon &&
                 Is_function) {
        first_Line_After_Colon = false;
        if ((spaces_Default * level_Indentation) != spaces_At_The_Beggining)
          message_Error_Indentation("Indentation", line, times);

      } else if (level_Indentation > 1 && !first_Line_After_Colon &&
                 Is_function) {
        if (spaces_Default == spaces_At_The_Beggining) {
          level_Indentation--;
        } else if ((spaces_Default * level_Indentation) !=
                   spaces_At_The_Beggining) {
          message_Error_Indentation("Indentation", line, times);
        }
      } else if (level_Indentation > 1 && first_Line_After_Colon &&
                 !Is_function) {
        if (spaces_At_The_Beggining == 0)
          message_Error_Indentation("Indentation", line, times);

        first_Line_After_Colon = false;
        spaces_Before = spaces_At_The_Beggining;
      } else if (level_Indentation > 1 && !first_Line_After_Colon &&
                 !Is_function) {
        if (spaces_Default == spaces_At_The_Beggining) {
          level_Indentation--;
          continue;
        } else if (spaces_At_The_Beggining != spaces_Before) {
          message_Error_Indentation("Indentation", line, times);
        }
      } else if ((spaces_Default * level_Indentation) ==
                 spaces_At_The_Beggining) {
        continue;
      } else {
        message_Error_Indentation("Indentation", line, times);
      }
    } else {
      message_Error_Indentation("Indentation", line, times);
    }

    times++;
  }
}

void read_Functions(std::vector<std::string> &lines, const int start,
                    const int end) {
  /* std::cout << start << " - " << end << "\n"; */
  std::vector<std::string> v(&lines[start], &lines[end]);
  check_Indentation(v);

  /* for (auto &x : v) { */
  /*   std::cout << x << "\n"; */
  /* } */
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
    if (is_Function(line, number_Line)) {
      modules.insert({number_Line, get_Function_Name(line)});
    } else if (number_Line >= it->first + 1 && is_Main(line) && !found_Main) {
      modules.insert({number_Line, "Main"});
      found_Main = true;
    }

    number_Line++;
  }

  std::vector<std::string> keywords = {"def",   "print", "for",   "in",
                                       "range", "int",   "input", "return"};

  auto it = modules.begin();
  auto it2 = modules.begin();
  int size_Map = static_cast<int>(modules.size());

  for (int i = 0; i < size_Map; i++) {
    if (i == size_Map - 1) {
      read_Functions(lines, it->first, lines.size());
    } else {
      read_Functions(lines, it->first, (++it2)->first);
    }
    ++it;
  }

  return 0;
}
