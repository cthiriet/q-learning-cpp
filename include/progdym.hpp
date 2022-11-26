#ifndef PROGDYM_HPP
#define PROGDYM_HPP

#include "mdp_parfait.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

template <class S, class A>
class ProgDym
{
protected:
  double gamma;
  int *policy;
  double *vValue;
  MDP_parfait<S, A> *modele;

public:
  ProgDym();
  ProgDym(const ProgDym<S, A> &prd);
  ProgDym(MDP_parfait<S, A> &modeleParfait);
  A applyPolicy(S state);
  void computePolicy();
  virtual void resolution() = 0;
  void setGamma(double g);
  void displayPolicy();
  void displayVValue();
  ~ProgDym();
};

template <class S, class A>
ProgDym<S, A>::ProgDym()
{
  gamma = 0;
  policy = nullptr;
  vValue = nullptr;
  modele = nullptr;
}

template <class S, class A>
ProgDym<S, A>::ProgDym(const ProgDym<S, A> &prd)
{
  gamma = prd.gamma;
  if (prd.policy != nullptr)
  {
    int size = sizeof(prd.policy) / sizeof(prd.policy[0]);
    policy = new int[size];
    for (int i = 0; i < size; i++)
      policy[i] = prd.policy[i];
  }
  if (prd.vValue != nullptr)
  {
    int size = sizeof(prd.vValue) / sizeof(prd.vValue[0]);
    vValue = new double[size];
    for (int i = 0; i < size; i++)
      vValue[i] = prd.vValue[i];
  }
  if (prd.modele != nullptr)
    modele = prd.modele;
}

template <class S, class A>
ProgDym<S, A>::ProgDym(MDP_parfait<S, A> &modeleParfait)
{
  int nbStates = modeleParfait.getNumStates();
  gamma = 0;

  policy = new int[nbStates];
  for (int k = 0; k < nbStates; k++)
  {
    policy[k] = 0;
  }

  vValue = new double[nbStates];
  for (int k = 0; k < nbStates; k++)
  {
    vValue[k] = 0;
  }

  modele = &modeleParfait;
}

template <class S, class A>
void ProgDym<S, A>::displayPolicy()
{
  cout << "Policy: [";
  for (int i = 0; i < this->modele->getNumStates(); i++)
  {
    cout << " " << policy[i];
  }
  cout << " ]" << endl;
}

template <class S, class A>
void ProgDym<S, A>::displayVValue()
{
  cout << "V Value: [";
  for (int i = 0; i < this->modele->getNumStates(); i++)
  {
    cout << " " << vValue[i];
  }
  cout << " ]" << endl;
}

template <class S, class A>
A ProgDym<S, A>::applyPolicy(S state)
{
  const S *states = modele->getPossibleStates();
  int indexS;
  for (int i = 0; i < modele->getNumStates(); i++)
    if (states[i] == state)
      indexS = i;
  int indexAction = policy[indexS];

  A *actions = modele->getPossibleActions();

  return actions[indexAction];
}

template <class S, class A>
void ProgDym<S, A>::computePolicy()
{
  // Prend les etats possibles du modele
  const S *states = modele->getPossibleStates();
  for (int i = 0; i < modele->getNumStates(); i++)
  {
    vector<double> sumArray;

    A *actions = this->modele->getPossibleActions();

    for (int k = 0; k < this->modele->getNumActions(); k++)
    {
      double sum = 0;

      // Pour chaque état, calcule le terme de la somme associé
      for (int l = 0; l < modele->getNumStates(); l++)
      {
        double reward = modele->getR(i, k, l);
        double proba = modele->getP(i, k, l);
        double vNextState = vValue[l];
        sum += (reward + gamma * vNextState) * proba;
      }

      sumArray.push_back(sum);
    }

    // Renvoie un pointeur du plus grand élément du tableau des sommes sumArray.
    const vector<double>::iterator result = max_element(sumArray.begin(), sumArray.end());

    // Stock l'index de ce plus grand élément dans policy
    policy[i] = distance(sumArray.begin(), result);
  }
}

template <class S, class A>
void ProgDym<S, A>::setGamma(double g)
{
  gamma = g;
}

template <class S, class A>
ProgDym<S, A>::~ProgDym()
{
  if (policy != nullptr)
    delete[] policy;
  if (vValue != nullptr)
    delete[] vValue;
}

#endif