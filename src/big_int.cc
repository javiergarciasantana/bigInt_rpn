// AUTOR: Javier Garcia Santana
// FECHA: 20/2/2023
// EMAIL: alu0101391663@ull.edu.es
// VERSION: 2.0
// ASIGNATURA: Algoritmos y Estructuras de Datos Avanzadas
// PRÁCTICA Nº: 2
// COMENTARIOS: Fichero .cc en el que definimos los operadores de conversion 
//              de tipo
// COMPILACIÓN: make

#include "big_int.h"

BigInt<2>::operator BigInt<10>() {
  std::string binary_number;
  if (binary_digits_[0] == 1) {
    binary_number = this->ComplemeterTwo();
  } else {
    binary_number = this->FromBoolVecToStr();
  }
  //std::cout << "from fnc to bn: " << binary_number << std::endl;
  long sum = 0;
  for (int i = 0; i < binary_number.size(); ++i) {
    sum += (binary_number.at((binary_number.size() - 1) - i) - '0') * (pow(2, i));
  }
  if (binary_digits_[0] == 1) {
    sum *= -1;
  }
  std::string result = std::to_string(sum);
  //std::cout << "result: " << result << std::endl;
  return BigInt<10>(result);
}

BigInt<2>::operator BigInt<8>() {
  std::string binary_number;
  if (binary_digits_[0] == 1) {
    binary_number = this->ComplemeterTwo();
  } else {
    binary_number = this->FromBoolVecToStr();
  }
  //std::cout << "from fnc to bn: " << binary_number << std::endl;
  BigInt<10> decimal_int = (BigInt<10>) *this;
  long sum = 0;
  int multiplier = 1;
  long decimal_number = decimal_int.DecimalValue();
  while (decimal_number != 0) {
    sum += (decimal_number % 8) * multiplier;
    decimal_number /= 8;
    multiplier *= 10;
  }
  
  if (binary_digits_[0] == 1) {
    sum *= -1;
  }
  std::string result = std::to_string(sum);
  //std::cout << "result: " << result << std::endl;
  return BigInt<8>(result);
}

BigInt<2>::operator BigInt<16>() {
  std::string binary_number, result;
  if (binary_digits_[0] == 1) {
    binary_number = this->ComplemeterTwo();
  } else {
    binary_number = this->FromBoolVecToStr();
  }
  //std::cout << "from fnc to bn: " << binary_number << std::endl;
  BigInt<10> decimal_int = (BigInt<10>) *this;
  std::vector<int> sum;
  long decimal_number = decimal_int.DecimalValue();
  while (decimal_number != 0) {
    
    int rem = 0;
    char each_character;
    rem = decimal_number % 16;
    if (rem < 10) {
      each_character = rem + 48;
    } else {
      each_character = rem + 55;
    } 
    result += each_character;
    decimal_number /= 16;
  }
  for (int i = 0, j = result.size() - 1; i <=j; ++i, --j) {
    std::swap(result[i], result[j]);
  }
  //std::cout << "result: " << result << std::endl;
  return BigInt<16>(result);
}

bool BigInt<2>::operator>=(const BigInt<2>& other_big_int) const {
  BigInt<2> a = *this;
  BigInt<2> b = other_big_int;
  return (BigInt<10>) a >= (BigInt<10>) b;
}