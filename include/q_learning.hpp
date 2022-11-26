#ifndef Q_LEARNING_HPP
#define Q_LEARNING_HPP

#include "mdp.hpp"
#include "mdp_parfait.hpp"
#include "gen_rand.hpp"
#include "model_sto.hpp"

using namespace std;

template <class S, class A>
class Q_learning : public ModelSto<S, A>
{
private:
  double alpha;

public:
  Q_learning();
  Q_learning(const Q_learning<S, A> &qlearning);
  Q_learning(MDP_parfait<S, A> &modeleptr);
  void calcul_episodes(S state) override;
  void setAlpha(double a);
  ~Q_learning();
};

template <class S, class A>
Q_learning<S, A>::Q_learning() : ModelSto<S, A>()
{
  alpha = 0;
}

template <class S, class A>
Q_learning<S, A>::Q_learning(const Q_learning<S, A> &qlearning) : ModelSto<S, A>(qlearning)
{
  alpha = qlearning.alpha;
}

template <class S, class A>
Q_learning<S, A>::Q_learning(MDP_parfait<S, A> &modeleptr) : ModelSto<S, A>(modeleptr)
{
  alpha = 0;
}

template <class S, class A>
void Q_learning<S, A>::calcul_episodes(S state)
{
  bool isTerminal = false;
  S st = state;
  double totalReward = 0;

  while (isTerminal == false)
  {
    isTerminal = this->modele->evalIsTerminal(st, totalReward);

    S stp;
    double rt;
    A at = this->getActionRandomPolicy(st);
    this->modele->evalEnvironement(st, at, rt, stp);
    totalReward += rt;
    A astar = this->getOptimalAction(stp);

    // Get indexes of actions and states
    int ist;
    int istp;
    int iat;
    int iastar;
    S *modeleStates = this->modele->getPossibleStates();
    A *modeleActions = this->modele->getPossibleActions();
    for (int i = 0; i < this->modele->getNumStates(); i++)
    {
      if (st == modeleStates[i])
        ist = i;
      if (stp == modeleStates[i])
        istp = i;
    }
    for (int i = 0; i < this->modele->getNumActions(); i++)
    {
      if (at == modeleActions[i])
        iat = i;
      if (astar == modeleActions[i])
        iastar = i;
    }

    if (isTerminal == true)
      this->Qfunction[ist][iat] = this->Qfunction[ist][iat] + alpha * rt;
    else
      this->Qfunction[ist][iat] = this->Qfunction[ist][iat] + alpha * (rt + this->gamma * this->Qfunction[istp][iastar]);

    this->updateOptimalPolicy();

    st = stp;
    this->epsilon = this->d * this->epsilon;
  }

  cout << "Reward with Q-learning: " << totalReward << endl;
}

template <class S, class A>
void Q_learning<S, A>::setAlpha(double a)
{
  alpha = a;
}

template <class S, class A>
Q_learning<S, A>::~Q_learning()
{
}

#endif