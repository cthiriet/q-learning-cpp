#include <iostream>
#include "mdp.hpp"
#include "mdp_parfait.hpp"
#include "progdym.hpp"
#include "iteration_valeur.hpp"
#include "gen_rand.hpp"
#include "model_sto.hpp"
#include "q_learning.hpp"
using namespace std;

void buildActions(MDP<int, int> &ex)
{
  int actions[] = {0, 1, 2};
  ex.setActions(3, actions);
}

void buildStates(MDP<int, int> &ex)
{
  int states[] = {0, 1};
  ex.setStates(2, states);
}

void buildProbaReward(MDP_parfait<int, int> &ex)
{
  ex.setP(0, 0, 0, 0.1);
  ex.setP(1, 0, 0, 0.9);
  ex.setP(1, 1, 1, 1.);
  ex.setP(0, 0, 1, 0.9);
  ex.setP(1, 0, 1, 0.1);
  ex.setP(0, 1, 0, 1.);
  ex.setP(0, 2, 1, 1.);

  ex.setR(0, 0, 0, 2.);
  ex.setR(1, 0, 0, 2.);
  ex.setR(1, 1, 1, 1.);
  ex.setR(0, 0, 1, -3.);
  ex.setR(1, 0, 1, 2.);
  ex.setR(0, 1, 0, 1.);
  ex.setR(0, 2, 1, 0.);
}

int robot_reset(int nbState, int *state)
{
  // choose a number between 0 and nbState-1 (which corresponds to the number of states)
  int proba = randomNumber(nbState);
  return state[proba];
}

int robot_possible_action(int state, int *action)
{
  if (state == 0)
  {
    action = new int[3];
    action[0] = 0;
    action[1] = 1;
    action[2] = 2;
    return 3;
  }
  else
  {
    action = new int[2];
    action[0] = 0;
    action[1] = 1;
    return 2;
  }
}

void robot_environnement(int state, int action, double &reward, int &nextState)
{
  // get a random number between 0 and 1
  double proba = randomNumber();

  // bas
  if (state == 0)
  {
    switch (action)
    {
    // search
    case 0:
    {
      if (proba < 0.1)
      {
        reward = 2.;
        nextState = 0;
      }
      else
      {
        reward = -3.;
        nextState = 1;
      }
      break;
    }
    // wait
    case 1:
    {
      reward = 1.;
      nextState = 0;
      break;
    }
    // recharger
    case 2:
    {
      reward = 0.;
      nextState = 1;
      break;
    }
    default:
      break;
    }
  }
  else
  {
    switch (action)
    {
    // search
    case 0:
    {
      if (proba < 0.1)
      {
        reward = 2.;
        nextState = 1;
      }
      else
      {
        reward = 2.;
        nextState = 0;
      }
      break;
    }
    // wait
    case 1:
    {
      reward = 1.;
      nextState = 1;
      break;
    }
    default:
      break;
    }
  }
}

bool robot_is_terminal(int state, int n)
{
  return n > 30;
}

int main(int argc, char const *argv[])
{
  MDP_parfait<int, int> ex;
  buildActions(ex);
  buildStates(ex);
  buildProbaReward(ex);
  ex.setIsTerminal(robot_is_terminal);
  ex.setEnvironnement(robot_environnement);
  ex.setPossibleAction(robot_possible_action);
  ex.setReset(robot_reset);

  MDP<int, int> ex1;
  buildActions(ex1);
  buildStates(ex1);
  ex1.setIsTerminal(robot_is_terminal);
  ex1.setEnvironnement(robot_environnement);
  ex1.setPossibleAction(robot_possible_action);
  ex1.setReset(robot_reset);

  /****
   *
   *
   * With optimal policy
   *
   *
   ****/

  IterationValeur<int, int> iv(ex);
  iv.setImax(10000);
  iv.setEpsilon(0.0001);
  iv.setGamma(0.9);

  // compute the optimal value function
  iv.resolution();
  iv.displayVValue();

  // compute the optimal policy from the optimal value function
  iv.computePolicy();
  iv.displayPolicy();

  double reward = 0.;
  int t = 0;
  int currentState = ex.evalReset(2, ex.getPossibleStates());
  cout << "Init state: " << currentState << endl;

  while (t < 100)
  {
    int bestAction = iv.applyPolicy(currentState);

    int nextState;
    double r;
    ex.evalEnvironement(currentState, bestAction, r, nextState);
    reward += r;
    currentState = nextState;

    if (ex.evalIsTerminal(currentState, reward))
      break;

    ++t;
  }

  cout << "Reward with optimal policy: " << reward << endl;
  cout << "Time: " << t << endl;

  /****
   *
   *
   * With random policy
   *
   *
   ****/

  reward = 0.;
  t = 0;
  currentState = ex1.evalReset(2, ex1.getPossibleStates());
  cout << "Init state: " << currentState << endl;

  while (t < 100)
  {

    // Construction tu tableau actions ci-dessous car evalPossibleAction ne renvoie pas le tableau voulu (problème avec pointeur ?)
    int *actions;
    int numActions;
    if (currentState == 0)
    {
      numActions = 3;
      actions = new int[3];
      actions[0] = 0;
      actions[1] = 1;
      actions[2] = 2;
    }
    else
    {
      numActions = 2;
      actions = new int[2];
      actions[0] = 0;
      actions[1] = 1;
    }

    int probaAction = randomNumber(numActions);
    int bestAction = actions[probaAction];

    int nextState;
    double r;
    ex1.evalEnvironement(currentState, bestAction, r, nextState);
    reward += r;
    currentState = nextState;

    ++t;

    if (actions != nullptr)
      delete[] actions;

    if (ex1.evalIsTerminal(currentState, reward))
      break;
  }

  cout << "Reward with randomness: " << reward << endl;
  cout << "Time: " << t << endl;

  Q_learning<int, int> qlearning(ex);
  qlearning.setGamma(0.95);
  qlearning.setAlpha(0.5);
  qlearning.setEpsilon(1.0);
  qlearning.setD(0.98);

  qlearning.computeRandomPolicy();
  qlearning.apprentissage(10);

  return 0;
}
