// AUTOR: Samuel Martín Morales	
// FECHA: 29/03/2023
// EMAIL: alu0101359526@ull.edu.es
// VERSION: 3.0
// ASIGNATURA: Algoritmos y Estructuras de Datos
// PRÁCTICA Nº: 4
// ESTILO: Google C++ Style Guide
// COMENTARIOS:

#ifndef SLLPOLYNOMIAL_H_
#define SLLPOLYNOMIAL_H_

// Inclusión de las librerías necesarias
#include <iostream>
#include <math.h>  // fabs, pow

#include "pair_t.h"
#include "sll_t.h"
#include "vector_t.h"

#define EPS 1.0e-6

typedef pair_t<double> pair_double_t;  // Campo data_ de SllPolynomial
typedef sll_node_t<pair_double_t> SllPolyNode;  // Nodos de SllPolynomial

// Clase para polinomios basados en listas simples de pares
class SllPolynomial : public sll_t<pair_double_t> {
 public:
  // constructores
  SllPolynomial(void) : sll_t() {};
  SllPolynomial(const vector_t<double>&, const double = EPS);

  // destructor
  ~SllPolynomial() {};

  // E/S
  void Write(std::ostream& = std::cout) const;
  
  // operaciones
  double Eval(const double) const;
  bool IsEqual(const SllPolynomial&, const double = EPS) const;
  void Sum(const SllPolynomial&, SllPolynomial&, const double = EPS);
  void EliminateDegree(const int degree, const SllPolynomial&);
};


bool IsNotZero(const double val, const double eps = EPS) {
  return fabs(val) > eps;
}

// FASE II
// constructor
SllPolynomial::SllPolynomial(const vector_t<double>& v, const double eps) {
  // NOTA: Debido a que únicamente existe el método de push_front() y no de push_back(),
  //       se recorre el vector de forma inversa para que el polinomio se construya
  //       correctamente.
  for (int i = v.get_size() - 1; i >= 0; i--) {
    if (IsNotZero(v[i], eps)) {
      // Creamos un par de valores (valor, índice)
      pair_double_t pair;
      pair.set(v[i], i);
      // Creamos un nodo con el par
      SllPolyNode* aux = new SllPolyNode(pair);
      // Insertamos el nodo en la lista
      push_front(aux);
    }
  }
}


// E/S
void SllPolynomial::Write(std::ostream& os) const {
  os << "[ ";
  bool first{true};
  SllPolyNode* aux{get_head()};
  while (aux != NULL) {
    int inx{aux->get_data().get_inx()};
    double val{aux->get_data().get_val()};
    if (val > 0)
      os << (!first ? " + " : "") << val;
    else
      os << (!first ? " - " : "-") << fabs(val);
    os << (inx > 1 ? " x^" : (inx == 1) ? " x" : "");
    if (inx > 1)
      os << inx;
    first = false;
    aux = aux->get_next();
  }
  os << " ]" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const SllPolynomial& p) {
  p.Write(os);
  return os;
}


// Operaciones con polinomios

// FASE III
// Evaluación de un polinomio representado por lista simple
double SllPolynomial::Eval(const double x) const {
  double result{0.0};
  for (SllPolyNode* aux{get_head()}; aux != NULL; aux = aux->get_next()) {
    result += aux->get_data().get_val() * pow(x, aux->get_data().get_inx());
  }
  return result;
}

// Comparación si son iguales dos polinomios representados por listas simples
bool SllPolynomial::IsEqual(const SllPolynomial& sllpol,
			    const double eps) const {
  bool differents = false;
  SllPolyNode* aux1{get_head()};
  SllPolyNode* aux2{sllpol.get_head()};
  while (aux1 != NULL && aux2 != NULL && !differents) {
    differents = (aux1->get_data().get_inx() != aux2->get_data().get_inx()) ||
     (fabs(aux1->get_data().get_val() - aux2->get_data().get_val()) > eps);
    aux1 = aux1->get_next();
    aux2 = aux2->get_next();
  }
  return !differents;
}

// FASE IV
// Generar nuevo polinomio suma del polinomio invocante mas otro polinomio
void SllPolynomial::Sum(const SllPolynomial& sllpol,
			SllPolynomial& sllpolsum,
			const double eps) {
  SllPolyNode* aux1{get_head()};
  SllPolyNode* aux2{sllpol.get_head()};

  while (aux1 != NULL && aux2 != NULL) {
    if (aux1->get_data().get_inx() == aux2->get_data().get_inx()) {
      if (IsNotZero(aux1->get_data().get_val() + aux2->get_data().get_val(),
                    eps)) {
        pair_double_t pair;
        pair.set(aux1->get_data().get_val() + aux2->get_data().get_val(),
                 aux1->get_data().get_inx());
        SllPolyNode *aux = new SllPolyNode(pair);
        sllpolsum.push_front(aux);
      }
    }
    if (aux1->get_data().get_inx() < aux2->get_data().get_inx()) {
      pair_double_t pair;
      pair.set(aux1->get_data().get_val(), aux1->get_data().get_inx());
      SllPolyNode *aux = new SllPolyNode(pair);
      sllpolsum.push_front(aux);
    }
    if (aux2->get_data().get_inx() < aux1->get_data().get_inx()) {
      pair_double_t pair;
      pair.set(aux2->get_data().get_val(), aux2->get_data().get_inx());
      SllPolyNode *aux = new SllPolyNode(pair);
      sllpolsum.push_front(aux);
    }

    if (aux1->get_data().get_inx() == aux2->get_data().get_inx()) {
      aux1 = aux1->get_next();
      aux2 = aux2->get_next();
    } else if (aux1->get_data().get_inx() < aux2->get_data().get_inx()) {
      aux1 = aux1->get_next();
    } else if (aux2->get_data().get_inx() < aux1->get_data().get_inx()) {
      aux2 = aux2->get_next();
    }

    if (aux1 == NULL && aux2 != NULL) {
      pair_double_t pair;
      pair.set(aux2->get_data().get_val(), aux2->get_data().get_inx());
      SllPolyNode *aux = new SllPolyNode(pair);
      sllpolsum.push_front(aux);
      aux2 = aux2->get_next();
    } else if (aux2 == NULL && aux1 != NULL) {
      pair_double_t pair;
      pair.set(aux1->get_data().get_val(), aux1->get_data().get_inx());
      SllPolyNode *aux = new SllPolyNode(pair);
      sllpolsum.push_front(aux);
      aux1 = aux1->get_next();
    }
  }
}

// MODIFICACIÓN
// Generar un polinomio producto que elimina los monomios que tengan el grado el cual se le pasa como parámetro
void SllPolynomial::EliminateDegree(const int degree, const SllPolynomial& sllpol) {
  SllPolyNode* aux{sllpol.get_head()};
  while (aux != NULL) {
    if (aux->get_data().get_inx() != degree) {
      pair_double_t pair;
      pair.set(aux->get_data().get_val(), aux->get_data().get_inx());
      SllPolyNode *aux2 = new SllPolyNode(pair);
      push_front(aux2);
    }
    aux = aux->get_next();
  }
}


#endif  // SLLPOLYNOMIAL_H_
