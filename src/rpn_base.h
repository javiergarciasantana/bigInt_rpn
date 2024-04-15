// AUTOR: Javier Garcia Santana
// FECHA: 20/2/2023
// EMAIL: alu0101391663@ull.edu.es
// VERSION: 2.0
// ASIGNATURA: Algoritmos y Estructuras de Datos Avanzadas
// PRÁCTICA Nº: 2
// COMENTARIOS: Fichero .h en el que se instancia la clase Rpn_base,
//la cual crea objetos de tipo big int para operar con ellos mediante
//una calculadora RPN
//
// COMPILACIÓN: make

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <stack>

#include "big_int.h"

#ifndef RPN_BASE_H
#define RPN_BASE_H

template <size_t Base = 10>
class rpn_base {
 public:

 rpn_base(std::vector<std::string>&);
 void Operate(std::string& line, int identifier);


 private:
  std::vector<BigInt<2>> integers_;//aqui almacenamos los integers que se definen en el fichero de texto
  std::vector<BigInt<2>> results_;
  std::stack<BigInt<2>> stack_;//aqui metemos los integers operandos para trabajar con ellos
};

template <size_t Base>
rpn_base<Base>::rpn_base(std::vector<std::string>& input) {
  int counter = 0;

  for (int i = 0; i < input.size(); ++i) {
    if (input[i][0] == 'N') {
      input[i].erase(input[i].begin(), input[i].begin() + 5);
      BigInt<Base> aux(input[i]);
      integers_.push_back((BigInt<2>)aux);
    } else if (input[i][0] == 'E') {
      ++counter;
      input[i].erase(input[i].begin(), input[i].begin() + 5);
      std::cout << input[i] << std::endl;
      Operate(input[i], counter);
    }
  }
}


template <size_t Base>
void rpn_base<Base>::Operate(std::string& line, int identifier) {
  BigInt<2> result;
  const std::string kOperators = {"+-^*/%"};
  bool flag = false;
  for(int i = 0; i < line.size(); ++i) {
    for (int j = 0; j < kOperators.size(); ++j) {
      if (line[i] == kOperators[j]) {
        assert(!stack_.empty());                  
        BigInt<2> op1, op2;
        op2 = stack_.top();
        stack_.pop();
        op1 = stack_.top();
        stack_.pop();    
        switch(line[i]) {
          case '+':
            result = op1 + op2;
            break;
          case '*':
            result = op1 * op2;
            break;
          case '-':
            result = op1 - op2;
            break;
          case '/':
            result = op1 / op2;
            break;
          // case '^':
          //   result = power(op1, op2);
          //   break;
          case '%':
            result = op1 % op2;
            break;
          default:
            break;
        }
        stack_.push(result);
        results_.push_back(result);
        j = -1;
        ++i;
      } else if (line[i] == 'N' || line[i] == ' ' || line[i] == 'E') {
        j = -1;    
        ++i;
      } else if (int(line[i] - '0' > 0) && int(line[i] - '0' <= 10) && (line[i - 1] == 'N')){
        int aux = int(line[i] - '0');
        stack_.push(integers_.at(aux - 1));
        j = -1;
        ++i;
      } else if (int(line[i] - '0' > 0) && int(line[i] - '0' <= 10) && (line[i - 1] == 'E')){
        int aux = int(line[i] - '0');
        stack_.push(results_.at(aux - 1));
        j = -1;
        ++i;
      } 
    }
  }
  std::cout << "E" << identifier << " = " << (BigInt<Base>) result;
}

#endif