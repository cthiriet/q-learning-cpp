#ifndef MODEL_STO_HPP
#define MODEL_STO_HPP

#include "mdp.hpp"
#include "mdp_parfait.hpp"
#include "gen_rand.hpp"

template <class S, class A>
class ModelSto
{
protected:
  double gamma;
  double epsilon;
  double d;
  double **policy;
  double **Qfunction;
  int *muOpt;
  MDP<S, A> *modele;

public:
  ModelSto();
  ModelSto(const ModelSto<S, A> &modsto);
  ModelSto(MDP_parfait<S, A> &modeleptr);
  A getOptimalAction(S state);
  A getActionRandomPolicy(S state);
  void updateOptimalPolicy();
  void computeRandomPolicy();
  virtual void calcul_episodes(S state) = 0;
  void apprentissage(int n);
  void setGamma(double g);
  void setEpsilon(double e);
  void setD(double d2);
  ~ModelSto();
};

template <class S, class A>
ModelSto<S, A>::ModelSto()
{
  gamma = 0;
  epsilon = 0;
  d = 0;
  policy = nullptr;
  muOpt = nullptr;
  modele = nullptr;
  Qfunction = nullptr;
}

template <class S, class A>
ModelSto<S, A>::ModelSto(const ModelSto<S, A> &modsto)
{
  gamma = modsto.gamma;
  epsilon = modsto.epsilon;
  d = modsto.d;

  if (modsto.modele != nullptr)
    modele = modsto.modele;

  if (modsto.policy != nullptr)
  {
    int sizeLine = sizeof(modsto.policy) / sizeof(modsto.policy[0]);
    int sizeCol = sizeof(modsto.policy[0]);
    policy = new double *[sizeLine];
    for (int i = 0; i < sizeLine; i++)
    {
      policy[i] = new double[sizeCol];
      for (int k = 0; k < sizeCol; k++)
        policy[i][k] = modsto.policy[i][k];
    }
  }

  if (modsto.Qfunction != nullptr)
  {
    int sizeLine = sizeof(modsto.Qfunction) / sizeof(modsto.Qfunction[0]);
    int sizeCol = sizeof(modsto.Qfunction[0]);
    Qfunction = new double *[sizeLine];
    for (int i = 0; i < sizeLine; i++)
    {
      Qfunction[i] = new double[sizeCol];
      for (int k = 0; k < sizeCol; k++)
        Qfunction[i][k] = modsto.Qfunction[i][k];
    }
  }

  if (modsto.muOpt != nullptr)
  {
    int sizeLine = sizeof(modsto.muOpt) / sizeof(modsto.muOpt[0]);
    muOpt = new int[sizeLine];
    for (int i = 0; i < sizeLine; i++)
    {
      muOpt[i] = modsto.muOpt[i];
    }
  }
}

template <class S, class A>
ModelSto<S, A>::ModelSto(MDP_parfait<S, A> &modeleptr)
{
  int numStates = modeleptr.getNumStates();
  int numActions = modeleptr.getNumActions();

  gamma = 0;
  epsilon = 0;
  d = 0;

  policy = new double *[numStates];
  for (int i = 0; i < numStates; i++)
  {
    policy[i] = new double[numActions];
    for (int k = 0; k < numActions; k++)
    {
      policy[i][k] = 0;
    }
  }

  Qfunction = new double *[numStates];
  for (int i = 0; i < numStates; i++)
  {
    Qfunction[i] = new double[numActions];
    for (int k = 0; k < numActions; k++)
    {
      Qfunction[i][k] = 0;
    }
  }

  muOpt = new int[numStates];
  for (int k = 0; k < numStates; k++)
  {
    muOpt[k] = 0;
  }

  modele = &modeleptr;
}

template <class S, class A>
A ModelSto<S, A>::getOptimalAction(S state)
{
  // Compute state's index
  int indexState;
  S *modeleStates = modele->getPossibleStates();

  for (int i = 0; i < modele->getNumStates(); i++)
  {
    if (state == modeleStates[i])
      indexState = i;
  }

  int bestActionIndex = muOpt[indexState];

  A *modeleActions = modele->getPossibleActions();
  return modeleActions[bestActionIndex];
}

template <class S, class A>
A ModelSto<S, A>::getActionRandomPolicy(S state)
{
  // Compute state's index
  int indexState;
  S *modeleStates = modele->getPossibleStates();

  for (int i = 0; i < modele->getNumStates(); i++)
  {
    if (state == modeleStates[i])
      indexState = i;
  }

  A *possibleActions;
  int nA = modele->evalPossibleAction(state, possibleActions);

  double foncRepartition[nA + 1];
  foncRepartition[0] = 0;
  for (int i = 0; i < nA; i++)
  {
    foncRepartition[i + 1] = foncRepartition[i] + policy[indexState][i];
  }

  double p = randomNumber();

  int indexAction;
  for (int i = 0; i < nA; i++)
  {
    if (foncRepartition[i] <= p && foncRepartition[i + 1] > p)
      indexAction = i;
  }

  if (possibleActions != nullptr)
    delete[] possibleActions;

  A *modeleActions = modele->getPossibleActions();
  return modeleActions[indexAction];
}

template <class S, class A>
void ModelSto<S, A>::updateOptimalPolicy()
{
  for (int i = 0; i < modele->getNumStates(); i++)
  {
    int argmaxAction = distance(Qfunction[i], max_element(Qfunction[i], Qfunction[i] + modele->getNumActions()));
    muOpt[i] = argmaxAction;
  }
}

template <class S, class A>
void ModelSto<S, A>::computeRandomPolicy()
{
  S *modeleStates = modele->getPossibleStates();
  for (int i = 0; i < modele->getNumStates(); i++)
  {
    A *possibleActions;
    int na = modele->evalPossibleAction(modeleStates[i], possibleActions);

    int indexOptimalAction = muOpt[i];

    for (int a = 0; a < modele->getNumActions(); a++)
    {
      if (indexOptimalAction == a)
        policy[i][a] = 1. - epsilon + (epsilon / na);
      else
        policy[i][a] = epsilon / na;
    }

    if (possibleActions != nullptr)
      delete[] possibleActions;
  }
}

template <class S, class A>
void ModelSto<S, A>::apprentissage(int n)
{
  S *modeleStates = modele->getPossibleStates();
  for (int i = 0; i < n; i++)
  {
    int newState = modele->evalReset(modele->getNumStates(), modeleStates);
    calcul_episodes(newState);
  }
}

template <class S, class A>
void ModelSto<S, A>::setGamma(double g)
{
  gamma = g;
}

template <class S, class A>
void ModelSto<S, A>::setEpsilon(double e)
{
  epsilon = e;
}

template <class S, class A>
void ModelSto<S, A>::setD(double d2)
{
  d = d2;
}

template <class S, class A>
ModelSto<S, A>::~ModelSto()
{
  if (muOpt != nullptr)
    delete[] muOpt;
  if (policy != nullptr)
  {
    int size = sizeof(policy) / sizeof(policy[0]);
    for (int i = 0; i < size; i++)
      delete[] policy[i];
    delete[] policy;
  }
  if (Qfunction != nullptr)
  {
    int size = sizeof(Qfunction) / sizeof(Qfunction[0]);
    for (int i = 0; i < size; i++)
      delete[] Qfunction[i];
    delete[] Qfunction;
  }
}

#endif