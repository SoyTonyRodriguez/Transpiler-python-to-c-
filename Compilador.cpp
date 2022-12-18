#include "bits/stdc++.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <ratio>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> keywords = {"print",  "def",    "for",  "in", "range",
                                     "append", "return", "len(", "int"};

std::string get_Function_Name(std::string &str) {
  std::string delimiter = " ";
  std::string str2 = str;
  std::string token = str2.erase(0, str2.find(delimiter) + delimiter.length());
  delimiter = "\(";
  token = token.substr(0, token.find(delimiter));
  std::string::iterator end_pos = std::remove(token.begin(), token.end(), ' ');
  token.erase(end_pos, token.end());
  return token;
}

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

int count_Commas(std::string const &str) {
  auto number_Commas = 0;
  number_Commas = std::count(str.begin(), str.end(), ',');
  return number_Commas;
}

void message_Error(std::string error_Type, const std::string &line,
                   int &times) {
  std::cout << "Error of: " << error_Type << " line " << times + 1 << "\n\t"
            << line << "\n\t^\n";
  exit(-1);
}

std::vector<std::string> tokens_Functions(const std::string &str) {
  std::string str_Copy = str;
  std::vector<std::string> variables = {};
  std::vector<std::string> tokensF = {
      str_Copy.substr(0, 3).replace(0, 3, "_Funcion_")};

  /* std::cout << "\n" << new_Str << "\n"; */
  str_Copy.erase(0, 3);
  str_Copy.erase(std::remove_if(str_Copy.begin(), str_Copy.end(), ::isspace),
                 str_Copy.end());

  int position = str_Copy.find('(');
  tokensF.push_back(str_Copy.substr(0, position).replace(0, position, "_ID_"));
  str_Copy.erase(0, position);
  tokensF.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);

  int commas = count_Commas(str_Copy);
  if (commas != 0) {
    for (int i = 0; i < commas; i++) {
      position = str_Copy.find(',');
      tokensF.push_back(
          str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
      variables.push_back(str_Copy.substr(0, position));
      str_Copy.erase(0, position);
      tokensF.push_back(str_Copy.substr(0, 1));
      str_Copy.erase(0, 1);
    }
  }

  position = str_Copy.find(')');
  tokensF.push_back(
      str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
  variables.push_back(str_Copy.substr(0, position));
  str_Copy.erase(0, position);
  tokensF.push_back(str_Copy.substr(0, 2));

  for (auto x : tokensF) {
    std::cout << x << " ";
  }
  std::cout << "\n";
  return variables;
}

bool is_Function(const std::string &str, int &number_Line) {
  const std::regex begin_Def("(\\w)(\\w+)(\\s)*(\\w)\\w+(\\s)*\\((.*)\\)\\:");
  const std::regex begin2("(def)(\\s)+(.*)");
  if (std::regex_match(str, begin_Def) or std::regex_match(str, begin2)) {
    int numeber_Commas = count_Commas(str);

    const std::regex function_Without_Parameters(
        "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s)*\\)\\:");
    std::string regular_Expression =
        "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s*)\\w+((\\s*)\\,(\\s*)\\w+){" +
        std::to_string(numeber_Commas) + "}(\\s*)\\)(\\s*)\\:";
    const std::regex function_With_Parameters(regular_Expression);

    if (std::regex_match(str, function_Without_Parameters) ||
        std::regex_match(str, function_With_Parameters)) {
      return true;
    }

    else
      message_Error("Identacion o Sintactico", str, number_Line);
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
        message_Error("Indentation", line, times);
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
          message_Error("Indentation", line, times);

      } else if (level_Indentation > 1 && !first_Line_After_Colon &&
                 Is_function) {
        if (spaces_Default == spaces_At_The_Beggining) {
          level_Indentation--;
        } else if ((spaces_Default * level_Indentation) !=
                   spaces_At_The_Beggining) {
          message_Error("Indentation", line, times);
        }
      } else if (level_Indentation > 1 && first_Line_After_Colon &&
                 !Is_function) {
        if (spaces_At_The_Beggining == 0)
          message_Error("Indentation", line, times);

        first_Line_After_Colon = false;
        spaces_Before = spaces_At_The_Beggining;
      } else if (level_Indentation > 1 && !first_Line_After_Colon &&
                 !Is_function) {
        if (spaces_Default == spaces_At_The_Beggining) {
          level_Indentation--;
          continue;
        } else if (spaces_At_The_Beggining != spaces_Before) {
          message_Error("Indentation", line, times);
        }
      } else if ((spaces_Default * level_Indentation) ==
                 spaces_At_The_Beggining) {
        continue;
      } else {
        message_Error("Indentation", line, times);
      }
    } else {
      message_Error("Indentation", line, times);
    }

    times++;
  }
}

void tokens_Print(const std::string &str, int begin_Spaces) {
  std::string str_Copy = str;

  str_Copy.erase(std::remove_if(str_Copy.begin(), str_Copy.end(), ::isspace),
                 str_Copy.end());
  std::vector<std::string> tokensP = {
      str_Copy.substr(0, 5).replace(0, 5, "_Keyword(print)_")};
  str_Copy.erase(0, 5);
  tokensP.push_back(str_Copy.substr(0, 1));

  int position;
  int comas = count_Commas(str_Copy);
  if (comas != 0) {
    for (int i = 0; i < comas; i++) {
      position = str_Copy.find(',');
      tokensP.push_back(
          str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
      str_Copy.erase(0, position);
      tokensP.push_back(str_Copy.substr(0, 1));
    }
  } else if (comas == 0 && str_Copy.find('\"') != std::string::npos) {
    str_Copy.erase(0, 2);
    position = str_Copy.find("\"");
    /* std::cout << str_Copia << "\n"; */
    tokensP.push_back(str.substr(0, position).replace(0, position, "_String_"));
    str_Copy.erase(0, position + 1);
    tokensP.push_back(str_Copy.substr(0, 1));
  }

  if (comas != 0) {
    position = str_Copy.find(')');
    tokensP.push_back(
        str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
    str_Copy.erase(0, position);
    tokensP.push_back(str_Copy.substr(0, 1));
  }

  for (auto x : tokensP) {
    std::cout << std::string(begin_Spaces, ' ') << x << " ";
  }
  std::cout << "\n";
}

void tokens_Assignment(const std::string &str, int spaces_At_The_Beggining) {
  std::string str_Copy = str;
  std::vector<std::string> tokensA = {};
  str_Copy.erase(std::remove_if(str_Copy.begin(), str_Copy.end(), ::isspace),
                 str_Copy.end());
  int position = str_Copy.find('=');
  tokensA.push_back(
      str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
  str_Copy.erase(0, position + 1);

  tokensA.push_back("_ASSIGNMENT_");

  tokensA.push_back(str_Copy.substr(0, str_Copy.length())
                        .replace(0, str_Copy.length(), "_ASSIGNED_VALUE_"));

  for (auto x : tokensA) {
    std::cout << std::string(spaces_At_The_Beggining, ' ') << x << " ";
  }
  std::cout << "\n";
}

void tokens_Return(const std::string &str, int spaces_At_The_Beggining) {
  std::string str_Copy = str;
  std::vector<std::string> tokensR = {};
  str_Copy.erase(std::remove_if(str_Copy.begin(), str_Copy.end(), ::isspace),
                 str_Copy.end());

  int position = str_Copy.find('n');
  tokensR.push_back(
      str_Copy.substr(0, position).replace(0, position, "_KEYWORD(return)_"));
  str_Copy.erase(0, position + 1);

  tokensR.push_back(str_Copy.substr(0, str_Copy.length())
                        .replace(0, str_Copy.length(), "_VALOR RETORNADO_"));

  for (auto x : tokensR) {
    std::cout << std::string(spaces_At_The_Beggining, ' ') << x << " ";
  }
  std::cout << "\n";
}

void tokens_For(const std::string &str, int spaces_At_The_Beggining) {
  std::string str_Copy = str;
  std::vector<std::string> tokensF = {};
  str_Copy.erase(std::remove_if(str_Copy.begin(), str_Copy.end(), ::isspace),
                 str_Copy.end());

  tokensF.push_back(str_Copy.substr(0, 3).replace(0, 3, "_KEYWORD(for)_"));
  str_Copy.erase(0, 3);

  int position = str_Copy.find("in");
  tokensF.push_back(
      str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
  str_Copy.erase(0, position);
  tokensF.push_back(str_Copy.substr(0, 2).replace(0, 2, "_KEYWORD(in)_"));
  str_Copy.erase(0, 2);
  tokensF.push_back(str_Copy.substr(0, 5).replace(0, 5, "_KEYWORD(range)_"));
  str_Copy.erase(0, 5);
  tokensF.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);

  position = str_Copy.find(",");
  tokensF.push_back(
      str_Copy.substr(0, position).replace(0, position, "_VALUE_"));
  str_Copy.erase(0, position);
  tokensF.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);

  if (str_Copy.find('(') != std::string::npos) {
    tokensF.push_back(str_Copy.substr(0, 3).replace(0, 3, "_KEYWORD(len)_"));
    str_Copy.erase(0, 3);
    tokensF.push_back(str_Copy.substr(0, 1));
    str_Copy.erase(0, 1);
    position = str_Copy.find(')');
    tokensF.push_back(
        str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
    str_Copy.erase(0, position);
    tokensF.push_back(str_Copy.substr(0, 1));
    str_Copy.erase(0, 1);
  } else {
    position = str_Copy.find(')');
    tokensF.push_back(
        str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
    str_Copy.erase(0, position);
  }
  tokensF.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);
  tokensF.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);

  for (auto x : tokensF) {
    std::cout << std::string(spaces_At_The_Beggining, ' ') << x << " ";
  }
  std::cout << "\n";
}

void tokens_Method(const std::string &str, int spaces_At_The_Beggining) {
  std::string str_Copy = str;
  str_Copy.erase(std::remove_if(str_Copy.begin(), str_Copy.end(), ::isspace),
                 str_Copy.end());
  std::vector<std::string> tokensM = {};

  int position = str_Copy.find('.');
  tokensM.push_back(
      str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
  str_Copy.erase(0, position);
  tokensM.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);

  position = str_Copy.find('(');
  tokensM.push_back(
      str_Copy.substr(0, position).replace(0, position, "_METHOD_"));
  str_Copy.erase(0, position);
  tokensM.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);
  position = str_Copy.find(')');
  tokensM.push_back(
      str_Copy.substr(0, position).replace(0, position, "_VARIABLE_"));
  str_Copy.erase(0, position);
  tokensM.push_back(str_Copy.substr(0, 1));
  str_Copy.erase(0, 1);

  for (auto x : tokensM) {
    std::cout << std::string(spaces_At_The_Beggining, ' ') << x << " ";
  }
  std::cout << "\n";
}

void check_Syntax(std::vector<std::string> &str) {
  int spaces_At_The_Beggining = 0;
  int commas;
  int times = 0;

  std::map<std::string, std::string> variables{};
  std::vector<std::string> saved_Variables{};

  for (auto line : str) {
    spaces_At_The_Beggining = count_Beggining_Spaces(line);
    commas = count_Commas(line);

    std::regex regex_Print("((\\s){" + std::to_string(spaces_At_The_Beggining) +
                           "}(print)(\\()(\\w)+((\\s*)\\,(\\s*)\\w+){" +
                           std::to_string(commas) + "}(\\s*)\\))|(\\s){" +
                           std::to_string(spaces_At_The_Beggining) +
                           "}(print)(\\()(\")(.*)(\")(\\))(\\s*)(\\s*)");

    std::regex regex_Return("(\\s*){" +
                            std::to_string(spaces_At_The_Beggining) +
                            "}(return)(\\s*)(\\w+)(\\s*)");

    std::regex regex_Input("(\\s){" + std::to_string(spaces_At_The_Beggining) +
                           "}(\\w)+(\\s*)(\\=)(\\s*)((\\w)(\\w+)(\\()(\\s*)("
                           "input)(\\s*)(\\()(\\s*)(\")(.*)(\")(\\))(\\))|(("
                           "input)(\\s*)(\\()(\\s*)(\")(.*)(\")(\\))))(\\s*)");

    std::regex function_Without_Parameters(
        "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s)*\\)\\:(\\s*)");
    std::regex function_With_Parameters(
        "(def)(\\s)*(\\w)\\w+(\\s)*\\((\\s*)\\w+((\\s*)\\,(\\s*)\\w+){" +
        std::to_string(commas) + "}(\\s*)\\)(\\s*)\\:(\\s*)");

    std::regex regex_Assignment("(\\s*){" +
                                std::to_string(spaces_At_The_Beggining) +
                                "}(\\w*)(\\s*)(\\=)(\\s*)(.*)");

    std::regex regex_For(
        "(\\s*){" + std::to_string(spaces_At_The_Beggining) +
        "}(for)(\\s+)(\\w+)(\\s+)(in)(\\s+)(range)(\\s*)(\\()(\\s*)([0-9])(\\s*"
        ")(\\,)(\\s*)((\\w+)|(len)\\((\\w+)\\))(\\s*)(\\))(\\:)(\\s*)");

    std::regex regex_Method(
        "(\\s*){" + std::to_string(spaces_At_The_Beggining) +
        "}(\\w+)(\\.)(\\w+)(\\s*)(\\()(\\s*)(\\w+)(\\s*)(\\))(\\s*)");

    if (std::regex_match(line, function_With_Parameters) ||
        std::regex_match(line, function_Without_Parameters)) {
      saved_Variables = tokens_Functions(line);
      std::cout << "Saved variables --> ";
      for (auto x : saved_Variables) {
        std::cout << x << ",";
      }
      std::cout << "\n";
    } else if (std::regex_match(line, regex_Print)) {
      tokens_Print(line, spaces_At_The_Beggining);
    } else if (std::regex_match(line, regex_For)) {
      tokens_For(line, spaces_At_The_Beggining);
    } else if (std::regex_match(line, regex_Assignment)) {
      tokens_Assignment(line, spaces_At_The_Beggining);
    } else if (std::regex_match(line, regex_Method)) {
      tokens_Method(line, spaces_At_The_Beggining);
    } else if (std::regex_match(line, regex_Return)) {
      tokens_Return(line, spaces_At_The_Beggining);
    } else {
      std::cout << line << " --- No se que es esto xd ---\n";
      /* message_Error("Sintaxis", line, times); */
    }
    times++;
  }
}

void read_Functions(std::vector<std::string> &lines, const int start,
                    const int end) {

  std::vector<std::string> v(&lines[start], &lines[end]);
  check_Indentation(v);
  check_Syntax(v);
  std::cout << "\n\n";

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
    line = remove_Comments(line, multiline_Comment);
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
