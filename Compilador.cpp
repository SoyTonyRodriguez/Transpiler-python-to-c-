#include <bits/stdc++.h>
#include <fstream>
#include <regex>
#include <vector>

void agregar_Texto(std::fstream *my_File) {
  *my_File << "#include <bits/stdc++.h>\n\n";
  *my_File << "int main() { \n\nreturn 0;\n}";
}

bool comparar(std::string cadena) {
  const std::regex expresion("(def)\\s[a-zA-Z]{1}[a-zA-Z]+\\(.*\\)\\:");

  return std::regex_match(cadena, expresion);
}

int main() {
  std::fstream my_File;
  my_File.open("Proyecto Final.txt",
               std::fstream::in | std::fstream::out | std::fstream::app);

  if (!my_File.is_open()) {
    std::cout << "ERROR al abrir el archivo\n";
    return -1;
  }

  std::vector<std::string> lines;
  std::string line;

  std::vector<std::string> tActual;

  int i = 0;
  while (std::getline(my_File, line)) {
    lines.push_back(line);
    std::cout << lines[i] << "\n";
    i++;
  }

  for (int j = 0; j < lines.size(); j++) {
    bool es_Funcion = comparar(lines[j]);

    if (es_Funcion) {
      tActual.push_back("FUNCION");
    } else {
      tActual.push_back(lines[j]);
    }
  }

  for (auto x : tActual) {
    std::cout << x << std::endl;
  }

  my_File.close();

  return 0;
}
