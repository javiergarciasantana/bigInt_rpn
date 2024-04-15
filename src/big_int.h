// AUTOR: Javier Garcia Santana
// FECHA: 20/2/2023
// EMAIL: alu0101391663@ull.edu.es
// VERSION: 2.0
// ASIGNATURA: Algoritmos y Estructuras de Datos Avanzadas
// PRÁCTICA Nº: 2
// COMENTARIOS: Fichero .h en el que se instancia la
// clase BigInt y todos sus metodos para que posteriormente
// sea posible operar con ellos
//
// COMPILACIÓN: make

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <cmath>
#include <cassert>

#ifndef BIG_INT_H
#define BIG_INT_H

template <const size_t Base>
class BigInt;
//------------------------------------BigInt<2>--------------------------------------------//
template<>
class BigInt<2> {
 public: 
  BigInt(long n = 0) {}

  BigInt(std::vector<bool>& input) {
    binary_digits_ = input;
  }

  BigInt(std::string& input) {
    for (auto digit : input) {
      binary_digits_.push_back(digit - '0');
    }
    //reverse(binary_digits_.begin(), binary_digits_.end());
  }

  BigInt(const char* input) {
    std::string usable_input = input;
    for (auto digit : usable_input) {
      binary_digits_.push_back(digit - '0');
    }
  }

  void write(std::ostream& out) const {
    for (auto it : binary_digits_) {
      out << it;
    }
    out << std::endl;
  }

  std::ostream& operator<<(std::ostream& out) {
    this->write(out);
    return out;
  }

  std::string ComplemeterTwo() const {//metodo que complementa a 2
    std::string binary_number;
    for (auto digit : binary_digits_) { //invertimos digitos
      if (digit) {
        binary_number.push_back('0');
      } else {
        binary_number.push_back('1');
      }
    }
    BigInt<2> result = BigInt<2>(binary_number) + BigInt<2> ("01"); //le sumamos 1 al resultado
    return result.FromBoolVecToStr();
  }

  BigInt<2> operator=(const BigInt<2>& other_big_int) {
    binary_digits_ = other_big_int.binary_digits_;
    return *this;
  }
  bool operator==(const BigInt<2>& other_big_int) const {

    if (binary_digits_.size() == other_big_int.binary_digits_.size() && binary_digits_[0] == other_big_int.binary_digits_[0]) {
      for (int i = 0; i < binary_digits_.size(); ++i) {
        if (binary_digits_.at(i) != other_big_int.binary_digits_.at(i)) {
          return false;
        }
      }
      return true;
    } 
    return false;
  }

  bool operator>(const BigInt<2>& other_big_int) const {
    return binary_digits_.size() > other_big_int.binary_digits_.size();
  }

  bool operator>=(const BigInt<2>& other_big_int) const;

  BigInt<2> operator+(const BigInt<2>& other_big_int) const {
    BigInt<2> result; 
    bool aux, carry = 0;
    int limit, limit_2;
    std::vector<bool> sum, a = binary_digits_, b = other_big_int.binary_digits_;
    if (*this > other_big_int) { //igualamos el tamaño de los numeros
      limit = a.size();
      aux = b.at(0);
      limit_2 = a.size() - b.size();
      for (int i = 0; i < limit_2; ++i) {
        b.insert(b.begin(), aux);
      }
    } else {
      limit = b.size();
      aux = a.at(0);
      limit_2 = b.size() - a.size();
      for (int i = 0; i < limit_2; ++i) {
        a.insert(a.begin(), aux);
      }
    }
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());
    // std::cout << "op1+d: ";
    // for (auto it : a) {
    //   std::cout << it;
    // }
    // std::cout << std::endl << "op2+d: ";
    // for (auto it : b) {
    //   std::cout << it;
    // }
    //std::cout << std::endl;
    for(int i = 0; i < limit; ++i){ //realizamos la suma con un FA
      sum.push_back((a[i] ^ b[i]) ^ carry); 
      carry = ((a[i] & b[i]) | (a[i] & carry)) | (b[i] & carry); 
    }
    if (carry == 1 && a[a.size() - 1] == 1) { //Añadimos el bit de overflow
      sum.push_back(1);
    } else if (a[a.size() - 1] == 0 && b[b.size() - 1] == 0) {
      sum.push_back(0);
    }
    reverse(sum.begin(), sum.end());
    result.binary_digits_ = sum;
    //std::cout << "result: " << result.FromBoolVecToStr() << std::endl;
    return result;
  }

  BigInt<2> operator-(const BigInt<2>& other_big_int) const {
    std::string op2 = other_big_int.ComplemeterTwo(); 
    return *this + BigInt<2>(op2);
  }

  BigInt<2> operator*(const BigInt<2>& other_big_int) const { //multiplicacion por booth de 2 bits
    std::vector<bool> m = binary_digits_, q = other_big_int.binary_digits_, result, a;
    bool aux;
    int limit;
    if (*this > other_big_int) { //nos aseguramos que el numero de digitos de ambos
      aux = q.at(0);             //operandos sea el mismo
      for (int i = 0; i < m.size() - q.size(); ++i) {
        q.insert(q.begin(), aux);
      }
      m.insert(m.begin(), m[0]);
    } else {
      aux = m.at(0);
      for (int i = 0; i < q.size() - m.size(); ++i) {
        m.insert(m.begin(), aux);
      }
      m.insert(m.begin(), aux);
    }
    q.push_back(0); 
    for (int i = 0; i < q.size(); ++i) {//definimos a
      a.push_back(0);
    }
    for (auto j : q) {
      std::cout << j;
    }
    std::cout << " * ";
    for (auto j : m) {
      std::cout << j;
    }
    std::cout << std::endl;
    std::string last_2_bits;
    for (int i = 0; i < m.size() - 1; ++i) { //aplicamos el algoritmo de booth
      last_2_bits.push_back(q[(q.size() - 1) - 1] + '0');
      last_2_bits.push_back(q[(q.size() - 1)] + '0');
      if (last_2_bits == "01") {
        BigInt<2> sum = BigInt<2>(a) + BigInt<2>(m);
        //std::cout << "sum: " << sum.FromBoolVecToStr() << std::endl;
        if (sum.binary_digits_.size() > a.size()) { //si hay overflow lo suprimimos
          limit = sum.binary_digits_.size() - a.size();
          for (int i = 0; i < limit; ++i) {
            sum.binary_digits_.erase(sum.binary_digits_.begin());
          }
        }
        a = sum.binary_digits_;
      } 
      if (last_2_bits == "10") {
        BigInt<2> sum = BigInt<2>(a) - BigInt<2>(m);
        //std::cout << "subs: " << sum.FromBoolVecToStr() << std::endl;
        if (sum.binary_digits_.size() > a.size()) { //si hay overflow lo suprimimos
          limit = sum.binary_digits_.size() - a.size();
          for (int i = 0; i < limit; ++i) {
            sum.binary_digits_.erase(sum.binary_digits_.begin());
          }
        }
        a = sum.binary_digits_;
      }
      q.insert(q.begin(), a[a.size()- 1]); //desplazamiento hacia la derecha
      q.pop_back();
      a.pop_back();
      a.insert(a.begin(), a[0]);
      last_2_bits.clear();
    }
    for (int i = 1; i < a.size(); ++i) { //copiamos el resultado(a(n-1)->q)
      result.push_back(a[i]);
    }
    for (int i = 0; i < q.size() - 1; ++i) {
      result.push_back(q[i]);
    }
    aux = result[0];
    while (aux == result[0]) {
      result.erase(result.begin());
    }
    result.insert(result.begin(), aux); //quitamos los bits de signo extra
    // std::cout << "result: ";
    // for (auto it : result) {
    //   std::cout << it;
    // }
    // std::cout << std::endl;
    return BigInt<2>(result);
  }

  BigInt<2> operator/(const BigInt<2>& other_big_int) const { //algoritmo de división sin restauracion(funciona a veces(probar 4/2))
    std::vector<bool> q = binary_digits_, m = other_big_int.binary_digits_, a, c;
    bool aux;
    int limit, sign_1, sign_2;
    std::string c_2_1, c_2_2;
    if (this->binary_digits_[0] == 1) {
      sign_1 == -1;
      c_2_1 = ComplemeterTwo();
      BigInt<2> q_c2(c_2_1);
      q = q_c2.binary_digits_;
    }
    if (other_big_int.binary_digits_[0] == 1) {
      sign_2 == -1;
      c_2_2 = other_big_int.ComplemeterTwo();
      BigInt<2> m_c2(c_2_2);
      m = m_c2.binary_digits_;
    }
    if (*this == other_big_int) {
      return BigInt<2>("01");
    }
    if (*this > other_big_int) {
      aux = m.at(0);
      for (int i = 0; i < q.size() - m.size(); ++i) {
        m.insert(m.begin(), aux);
      }
      //m.insert(m.begin(), aux);
    } else {
      aux = q.at(0);
      for (int i = 0; i < (m.size() - q.size()) - 1; ++i) {
        q.insert(q.begin(), aux);
      }
      //m.insert(m.begin(), m[0]);
    }
    for (int i = 0; i < m.size(); ++i) {
      a.push_back(0);
    }
    for (int i = 0; i < m.size(); ++i) {
      c.push_back(0);
    }
    for (auto j : q) {
      std::cout << j;
    }
    std::cout << " / ";
    for (auto j : m) {
      std::cout << j;
    }
    std::cout << std::endl;
    for (int i = 0; i < q.size(); ++i) {
        //a.erase(a.begin());
        a.push_back(q[i]);
        //c.erase(c.begin());
        c.push_back(0);
        // for (auto j : a) {
        //   std::cout << j;
        // }
        // std::cout << std::endl;
      if (BigInt<2>(a) >= BigInt<2>(m)) {
        //std::cout << "hola" << std::endl;
        BigInt<2> sum = BigInt<2>(a) - BigInt<2>(m);
        //std::cout << "subs: " << sum.FromBoolVecToStr() << std::endl;
        a = sum.binary_digits_;
        c.at(c.size() - 1) = 1;
      }
    }
    if (sign_1 != sign_2) {
      BigInt<2> result(c);
      std::string aux = result.ComplemeterTwo();
      BigInt<2> result_c2(aux);
      return result_c2;
    }
    return BigInt<2> (c);
  }

  BigInt<2> power(const BigInt<2> &) const;//Por implementar
  BigInt<2> operator%(const BigInt<2>& other_big_int) const  {
   std::vector<bool> q = binary_digits_, m = other_big_int.binary_digits_, a, c;
    bool aux;
    int limit, sign_1, sign_2;
    std::string c_2_1, c_2_2;
    if (this->binary_digits_[0] == 1) {
      sign_1 == -1;
      c_2_1 = ComplemeterTwo();
      BigInt<2> q_c2(c_2_1);
      q = q_c2.binary_digits_;
    }
    if (other_big_int.binary_digits_[0] == 1) {
      sign_2 == -1;
      c_2_2 = other_big_int.ComplemeterTwo();
      BigInt<2> m_c2(c_2_2);
      m = m_c2.binary_digits_;
    }
    if (*this == other_big_int) {
      return BigInt<2>("01");
    }
    if (*this > other_big_int) {
      aux = m.at(0);
      for (int i = 0; i < q.size() - m.size(); ++i) {
        m.insert(m.begin(), aux);
      }
      //m.insert(m.begin(), aux);
    } else {
      aux = q.at(0);
      for (int i = 0; i < (m.size() - q.size()) - 1; ++i) {
        q.insert(q.begin(), aux);
      }
      //m.insert(m.begin(), m[0]);
    }
    for (int i = 0; i < m.size(); ++i) {
      a.push_back(0);
    }
    for (int i = 0; i < m.size(); ++i) {
      c.push_back(0);
    }
    for (auto j : q) {
      std::cout << j;
    }
    std::cout << " % ";
    for (auto j : m) {
      std::cout << j;
    }
    std::cout << std::endl;
    for (int i = 0; i < q.size(); ++i) {
        //a.erase(a.begin());
        a.push_back(q[i]);
        //c.erase(c.begin());
        c.push_back(0);
        // for (auto j : a) {
        //   std::cout << j;
        // }
        // std::cout << std::endl;
      if (BigInt<2>(a) >= BigInt<2>(m)) {
        BigInt<2> sum = BigInt<2>(a) - BigInt<2>(m);
        //std::cout << "subs: " << sum.FromBoolVecToStr() << std::endl;
        a = sum.binary_digits_;
        c.at(c.size() - 1) = 1;
      }
    }
    return BigInt<2> (a);
  }

  std::string FromBoolVecToStr() const {
    std::string binary_number;
    for (auto digit : binary_digits_) {
      if (digit) {
        binary_number.push_back('1');
      } else {
        binary_number.push_back('0');
      }
    }
    return binary_number;
  }

  operator BigInt<10>();
  operator BigInt<8>();
  operator BigInt<16>();  

 private:
   std::vector<bool> binary_digits_;
};

//------------------------------------BigInt<Base>--------------------------------------------//
template <const size_t Base>
class BigInt {

 public:
  BigInt(long n = 0);
  BigInt(std::string&);
  BigInt(const char*);
  BigInt(const BigInt<Base>&); // Constructor de copia

  BigInt<Base>& operator=(const BigInt<Base>&);
  
  void read(std::istream& = std::cin);
  void write(std::ostream& = std::cout) const;

  int sign() const; // Signo: 1 o -1
  long DecimalValue() const;
  int SumOfVectorElements() const;
  std::vector<char> OriginalBaseValue(long) const;
  char operator[](int) const; // Acceso al i-ésimo dígito

  bool operator!=(const BigInt<Base>&) const;
  bool operator>=(const BigInt<Base> &) const;
  bool operator<=(const BigInt<Base>&) const;

  BigInt<Base>& operator++(); 
  BigInt<Base> operator++(int); 
  BigInt<Base>& operator--(); 
  BigInt<Base> operator--(int);
  BigInt<Base> operator-(const BigInt<Base> &) const;
  BigInt<Base> operator-() const;
  BigInt<Base> operator*(const BigInt<Base>&) const;
  BigInt<Base> operator%(const BigInt<Base>&) const;

  friend bool operator==(const BigInt<Base>& big_int, const BigInt<Base>& other_big_int) {
    return !(big_int != other_big_int);
  }

  friend bool operator>(const BigInt<Base>& big_int, const BigInt<Base>& other_big_int) {
    return big_int.DecimalValue() > other_big_int.DecimalValue();
  }

  friend bool operator<(const BigInt<Base>& big_int, const BigInt<Base>& other_big_int) {
    return !(big_int > other_big_int);
  } 

  friend BigInt<Base> operator+(const BigInt<Base>& big_int, const BigInt<Base>& other_big_int) {
    BigInt<Base> result;
    if ((big_int.DecimalValue() * big_int.sign_) + (other_big_int.DecimalValue() * other_big_int.sign_) < 0) {
      result.sign_ = -1;
    } else {
      result.sign_ = 1;
    }
    result.digits_ = result.OriginalBaseValue(((big_int.DecimalValue() * big_int.sign_) + (other_big_int.DecimalValue() * other_big_int.sign_)));
    return result;
  } 

  friend BigInt<Base> operator/(const BigInt<Base>& big_int, const BigInt<Base>& other_big_int) {
    BigInt<Base> result;
    assert(other_big_int.DecimalValue() != 0);
    if (big_int.sign_ == -1 || other_big_int.sign_ == -1) {
      result.sign_ = -1;
    }

    result.digits_ = result.OriginalBaseValue(big_int.DecimalValue() / other_big_int.DecimalValue());
    return result;
  }

  // Potencia a^b
  friend BigInt<Base> pow(const BigInt<Base>& big_int, const BigInt<Base>& other_big_int) {
    BigInt<Base> result;
    int multiplicator = 1;
    if (big_int.sign_ == -1 ) {
      result.sign_ = -1;
    }
    if (other_big_int.DecimalValue() < 0) {
      result.digits_.push_back(0);
      return result;
    }

    for (int i = 0; i < other_big_int.DecimalValue(); ++i) {
      multiplicator *= big_int.DecimalValue();
    }
    result.digits_ = result.OriginalBaseValue(multiplicator);
    return result;
  }

  operator BigInt<2>() {
    long operand = DecimalValue();
    char aux;
    std::string result, c2_result;
    if (operand == 0) {
      result.push_back(0);
      return result;
    }
    while (operand > 0) {
      aux = (operand % 2) + '0';//char conversion
      result.push_back(aux);
      operand /= 2;
    }
    result.push_back('0');
    reverse(result.begin(), result.end());
  
    if (sign_ == -1) { //caso negativo
      for (auto digit : result) {
        if (digit == '0') {
          c2_result.push_back('1');
        } else if(digit == '1') {
          c2_result.push_back('0');
        }
      }
      //c2_result.insert(c2_result.begin(), '1');
      return BigInt<2>(c2_result) + BigInt<2>("01");
    }
    return BigInt<2>(result);
  }

 private:
  std::vector<char> digits_;
  int sign_;
};


template<size_t Base>
BigInt<Base>::BigInt(long n) {
  digits_.push_back(n);
}

template<size_t Base>
BigInt<Base>::BigInt(std::string& input) {

  if (input[0] == '-') {
    sign_ = -1;
    input.erase(input.begin());
  } else {
    sign_ = 1;
  }
  
  for (int i = 0; i < input.size(); ++i) {
    if ((input[i] - '0') < Base) {
      digits_.push_back(input[i] - '0');
    } else if (input[i] - 'A' + 10 < Base) {
      digits_.push_back(input[i] - 'A' + 10);
    } else {
      std::cerr << "El numero no se puede representar en la base indicada" << std::endl;
      exit(EXIT_FAILURE);
    }
  } 
}

template<size_t Base>
BigInt<Base>::BigInt(const char* input) {
  std::string usable_input = input;
  
  if (input[0] == '-') {
    sign_ = -1;
    usable_input.erase(usable_input.begin());
  } else {
    sign_ = 1;
  }

  for (int i = 0; i < usable_input.size(); ++i) {
    if ((usable_input[i] - '0') < Base) {
      digits_.push_back(usable_input[i] - '0');
    } else if (usable_input[i] - 'A' + 10 < Base) {
      digits_.push_back(usable_input[i] - 'A' + 10);
    } else {
      std::cerr << "El numero no se puede representar en la base indicada" << std::endl;
      exit(EXIT_FAILURE);
    }
  } 
}

template<size_t Base>
BigInt<Base>::BigInt(const BigInt<Base>& other_big_int) {
  digits_ = other_big_int.digits_;
  sign_ = other_big_int.sign_;
}

template<size_t Base>
BigInt<Base> &BigInt<Base>::operator=(const BigInt<Base>& other_big_int) {
  digits_ = other_big_int.digits_;
  sign_ = other_big_int.sign_;
  return *this;
}

template<size_t Base>
void BigInt<Base>::write(std::ostream& os) const {
  if (sign_ == -1) {
    os << "-";
  }
  for (int i = 0; i < digits_.size(); ++i) {
    if (digits_.at(i) < 10) {
      os << char(digits_.at(i) + '0');
    } else if (digits_.at(i) >= 10 && digits_.at(i) < 16) {
      os << char(digits_.at(i) + 'A' - 10);
    }
  }
  os << std::endl;
}

template<size_t Base> 
void BigInt<Base>::read(std::istream& in) {

  std::string input;
  in >> input;
  int size = input.size();
  for (int i = size - 1; i >= 0; --i) {
    digits_.push_back(input[i] - '0');
  }
}

template<size_t Base>
std::ostream& operator<<(std::ostream& out, const BigInt<Base>& big_int) {
  big_int.write(out);
  return out;
}

template<size_t Base>
std::istream& operator>>(std::istream& in, BigInt<Base>& big_int) {
  big_int.read(in);
  return in;
}

template<size_t Base>
int BigInt<Base>::sign() const { // Signo: 1 o -1
  return sign_;
}  

template<size_t Base>
std::vector<char> BigInt<Base>::OriginalBaseValue(long input) const {

  std::vector<char> result;
  if (input < 0) {
    input *= -1;
  }
  if (input == 0) {
    result.push_back(0);
    return result;
  }
  while (input > 0) {
    result.push_back(input % Base);
    input /= Base;
  }
  reverse(result.begin(), result.end());
  return result;
}

template<size_t Base>
char BigInt<Base>::operator[](int i) const{ // Acceso al i-ésimo dígito
  return digits_.at(i);
}

template<size_t Base>
bool BigInt<Base>::operator!=(const BigInt<Base>& other_big_int) const {
  bool flag = false;
  if (digits_.size() == other_big_int.digits_.size() && sign_ == other_big_int.sign_) {
    for (int i = 0; i < digits_.size(); ++i) {
      if (digits_.at(i) != other_big_int.digits_.at(i)) {
        flag = true;
      }
    }
    return flag;
  } 
  return true;
}

template<size_t Base>
bool BigInt<Base>::operator>=(const BigInt<Base> & other_big_int) const {

  if (*this > other_big_int || *this == other_big_int) {
    return true;
  }
  return false;
}

template<size_t Base>
bool BigInt<Base>::operator<=(const BigInt<Base> & other_big_int) const {

  if (*this < other_big_int || *this == other_big_int) {
    return true;
  }
  return false;
}

template<size_t Base>
BigInt<Base> &BigInt<Base>::operator++(){
  int i, n = digits_.size();
  for (i = 0; i < n && digits_[i] == 9;i++)
      digits_[i] = 0;
  if(i == n)
      digits_.push_back(1);
  else
      digits_[i]++;
  return *this;
}

template<size_t Base>
BigInt<Base> BigInt<Base>::operator++(int temp){
  BigInt<Base> aux;
  aux = *this;
  ++(*this);
  return aux;
}

template<size_t Base>
BigInt<Base> &BigInt<Base>::operator--(){
  if(digits_[0] == 0 && digits_.size() == 1)
      throw("UNDERFLOW");
  int i, n = digits_.size();
  for (i = 0; digits_[i] == 0 && i < n;i++)
      digits_[i] = 9;
  digits_[i]--;
  if(n > 1 && digits_[n - 1] == 0)
      digits_.pop_back();
  return *this;
}

template<size_t Base>
BigInt<Base> BigInt<Base>::operator--(int temp){
  BigInt aux;
  aux = *this;
  --(*this);
  return aux;
}

template<size_t Base>
BigInt<Base> BigInt<Base>::operator-(const BigInt<Base>& other_big_int) const {
  BigInt<Base> result;
  if ((DecimalValue() * sign_) - (other_big_int.DecimalValue() * other_big_int.sign_) < 0) {
    result.sign_ = -1;
  } else {
    result.sign_ = 1;
  }
  result.digits_ = OriginalBaseValue(((DecimalValue() * sign_) - (other_big_int.DecimalValue() * other_big_int.sign_)));
  return result;
}

template<size_t Base>
long BigInt<Base>::DecimalValue() const {

  long sum = 0;
  for (int i = 0; i < digits_.size(); ++i) {
    sum += (digits_.at(i) * pow(Base, (digits_.size() - 1) - i));
  }
  return sum;
}

template<size_t Base>
BigInt<Base> BigInt<Base>::operator*(const BigInt<Base>& other_big_int) const {
  BigInt<Base> result;
  if (sign_ == -1 || other_big_int.sign_ == -1) {
    result.sign_ = -1;
  }
  result.digits_ = OriginalBaseValue(DecimalValue() * other_big_int.DecimalValue());
  return result;
}

template<size_t Base>
BigInt<Base> BigInt<Base>::operator%(const BigInt<Base>& other_big_int) const {
  BigInt<Base> result;
  if (sign_ == -1 || other_big_int.sign_ == -1) {
    result.sign_ = -1;
  }
  result.digits_ = OriginalBaseValue(DecimalValue() % other_big_int.DecimalValue());
  return result;
}


#endif