// AUTOR: Javier Garcia Santana
// FECHA: 20/2/2023
// EMAIL: alu0101391663@ull.edu.es
// VERSION: 2.0
// ASIGNATURA: Algoritmos y Estructuras de Datos Avanzadas
// PRÁCTICA Nº: 2
// COMENTARIOS: Fichero main en el que 
//se recoge la informacion del fichero de entrada
//y se le pasa a la clase rpn_base para que instancie
//elementos de tipo BigInt y opere con ellos
//
// COMPILACIÓN: make

#include "big_int.h"
#include "rpn_base.h"

int main (int argc, char* argv[]) {

  std::ifstream input;
  std::vector<std::string> lines;
  input.open(argv[1], std::ios_base::in);
  if (input.is_open()) {
    std::string line;
   
    while (getline(input, line)) {
      if (line.empty() == false) {
        lines.push_back(line);
      }
    }
  } else {
    std::cerr << "An error was produced opening the file" << std::endl;
    exit(EXIT_SUCCESS);
  }
  lines[0].erase(lines[0].begin(), lines[0].begin() + 7);
  int base = stoi(lines[0]);
  lines.erase(lines.begin());

  switch (base) {
    case 2:
      {
      rpn_base<2> calculator(lines);
      }
      break;
    case 8:
      {
      rpn_base<8> calculator(lines);
      }
      break;
    case 10:
      {
      rpn_base<10> calculator(lines);
      }
      break;
    case 16:
      {
      rpn_base<16> calculator(lines);
      }
      break;
    default:
      break;
  }
  // BigInt<2> d = {"011010"};
  // BigInt<10> b = {"-2"};
  // std::cout << "d = " << (BigInt<8>)d;
  return 0;
} 