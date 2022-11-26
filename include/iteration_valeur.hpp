#ifndef ITERATION_VALEUR_HPP
#define ITERATION_VALEUR_HPP

#include "progdym.hpp"
#include "mdp_parfait.hpp"
#include <cmath>
using namespace std;

double norm2diff(double *X, double *Y, int size)
{
  double sum = 0.;
  for (int i = 0; i < size; i++)
  {
    sum += pow(X[i] - Y[i], 2);
  }
  return sqrt(sum);
}

template <class S, class A>
class IterationValeur : public ProgDym<S, A>
{
private:
  int imax;
  double epsilon;

public:
  IterationValeur();
  IterationValeur(const IterationValeur<S, A> &itv);
  IterationValeur(MDP_parfait<S, A> &modeleParfait);
  void setEpsilon(double eps);
  void setImax(int im);
  void resolution() override;
  ~IterationValeur();
};

template <class S, class A>
IterationValeur<S, A>::IterationValeur() : ProgDym<S, A>()
{
  imax = 0;
  epsilon = 0.;
}

template <class S, class A>
IterationValeur<S, A>::IterationValeur(const IterationValeur<S, A> &itv) : ProgDym<S, A>(itv)
{
  imax = itv.imax;
  epsilon = itv.epsilon;
}

template <class S, class A>
IterationValeur<S, A>::IterationValeur(MDP_parfait<S, A> &modeleParfait) : ProgDym<S, A>(modeleParfait)
{
  imax = 0;
  epsilon = 0.;
}

template <class S, class A>
void IterationValeur<S, A>::setEpsilon(double eps)
{
  epsilon = eps;
}

template <class S, class A>
void IterationValeur<S, A>::setImax(int im)
{
  imax = im;
}

template <class S, class A>
void IterationValeur<S, A>::resolution()
{
  int k = 0;
  while (k <= imax)
  {
    double vTemp[this->modele->getNumStates()];

    // Prend les etats possibles du modele
    const S *states = this->modele->getPossibleStates();
    for (int i = 0; i < this->modele->getNumStates(); i++)
    {
      vector<double> sumArray;

      A *actions = this->modele->getPossibleActions();

      for (int k = 0; k < this->modele->getNumActions(); k++)
      {
        double sum = 0;

        // Pour chaque état, calcule le terme de la somme associé
        for (int l = 0; l < this->modele->getNumStates(); l++)
        {
          double reward = this->modele->getR(i, k, l);
          double proba = this->modele->getP(i, k, l);
          double vNextState = this->vValue[l];
          sum += (reward + this->gamma * vNextState) * proba;
        }

        sumArray.push_back(sum);
      }

      // Renvoie le plus grand élément du tableau des sommes sumArray.
      const double maxSum = *max_element(sumArray.begin(), sumArray.end());

      vTemp[i] = maxSum;
    }

    // Copie vTemp dans vValue
    for (int i = 0; i < this->modele->getNumStates(); i++)
    {
      this->vValue[i] = vTemp[i];
    }

    if (norm2diff(vTemp, this->vValue, this->modele->getNumStates()) < epsilon)
    {
      break;
    }

    ++k;
  }
}

template <class S, class A>
IterationValeur<S, A>::~IterationValeur()
{
}

#endif