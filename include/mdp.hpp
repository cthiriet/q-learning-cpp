#ifndef MDP_HPP
#define MDP_HPP

template <class S, class A>
class MDP
{
protected:
  int numStates;
  int numActions;

  S *possibleStates;
  A *possibleActions;

  S(*reset)
  (int, S *);
  int (*possible_action)(S, A *);
  void (*environnement)(S, A, double &, S &);
  bool (*is_terminal)(S, int);

public:
  MDP();
  MDP(const MDP<S, A> &mdp);
  MDP &operator=(const MDP<S, A> &mdp);
  S operator[](int i);
  A operator()(int i);

  // getters
  int getNumStates();
  int getNumActions();
  S *getPossibleStates();
  A *getPossibleActions();

  // evals
  S evalReset(int n, S *state);
  int evalPossibleAction(S state, A *action);
  void evalEnvironement(S state, A action, double &reward, S &nextState);
  bool evalIsTerminal(S state, int n);

  // setters
  void setStates(int ns, S *ps);
  void setActions(int na, A *pa);
  void setReset(S (*reset)(int, S *));
  void setPossibleAction(int (*possible_action)(S, A *));
  void setEnvironnement(void (*environnement)(S, A, double &, S &));
  void setIsTerminal(bool (*is_terminal)(S, int));

  // destructor
  ~MDP();
};

template <class S, class A>
MDP<S, A>::MDP()
{
  numStates = 0;
  numActions = 0;
  possibleActions = nullptr;
  possibleStates = nullptr;
  reset = nullptr;
  possible_action = nullptr;
  environnement = nullptr;
  is_terminal = nullptr;
}

template <class S, class A>
MDP<S, A>::MDP(const MDP<S, A> &mdp)
{
  numStates = mdp.numStates;
  numActions = mdp.numActions;

  if (mdp.possibleActions != nullptr)
  {
    possibleActions = new A[numActions];
    for (int i = 0; i < numActions; i++)
      possibleActions[i] = mdp.possibleActions[i];
  }

  if (mdp.possibleStates != nullptr)
  {
    possibleStates = new S[numStates];
    for (int i = 0; i < numStates; i++)
      possibleStates[i] = mdp.possibleStates[i];
  }

  if (mdp.reset != nullptr)
    reset = mdp.reset;

  if (mdp.possible_action != nullptr)
    possible_action = mdp.possible_action;

  if (mdp.environnement != nullptr)
    environnement = mdp.environnement;

  if (mdp.is_terminal != nullptr)
    is_terminal = mdp.is_terminal;
}

template <class S, class A>
S MDP<S, A>::operator[](int i)
{
  return possibleStates[i];
}

template <class S, class A>
A MDP<S, A>::operator()(int i)
{
  return possibleActions[i];
}

template <class S, class A>
MDP<S, A> &MDP<S, A>::operator=(const MDP<S, A> &mdp)
{
  if (*this != &mdp)
  {
    if (possibleActions != nullptr)
      delete[] possibleActions;
    if (possibleStates != nullptr)
      delete[] possibleStates;

    numStates = mdp.numStates;
    numActions = mdp.numActions;

    if (mdp.possibleActions != nullptr)
    {
      possibleActions = new A[numActions];
      for (int i = 0; i < numActions; i++)
        possibleActions[i] = mdp.possibleActions[i];
    }

    if (mdp.possibleStates != nullptr)
    {
      possibleStates = new S[numStates];
      for (int i = 0; i < numStates; i++)
        possibleStates[i] = mdp.possibleStates[i];
    }

    if (mdp.reset != nullptr)
      reset = mdp.reset;

    if (mdp.possible_action != nullptr)
      possible_action = mdp.possible_action;

    if (mdp.environnement != nullptr)
      environnement = mdp.environnement;

    if (mdp.is_terminal != nullptr)
      is_terminal = mdp.is_terminal;
  }
  return *this;
}

template <class S, class A>
int MDP<S, A>::getNumStates()
{
  return numStates;
}

template <class S, class A>
int MDP<S, A>::getNumActions()
{
  return numActions;
}

template <class S, class A>
S *MDP<S, A>::getPossibleStates()
{
  return possibleStates;
}

template <class S, class A>
A *MDP<S, A>::getPossibleActions()
{
  return possibleActions;
}

template <class S, class A>
S MDP<S, A>::evalReset(int n, S *state)
{
  return reset(n, state);
}

template <class S, class A>
int MDP<S, A>::evalPossibleAction(S state, A *action)
{
  return possible_action(state, action);
}

template <class S, class A>
void MDP<S, A>::evalEnvironement(S state, A action, double &reward, S &nextState)
{
  return environnement(state, action, reward, nextState);
}

template <class S, class A>
bool MDP<S, A>::evalIsTerminal(S state, int n)
{
  return is_terminal(state, n);
}

template <class S, class A>
void MDP<S, A>::setStates(int ns, S *ps)
{
  numStates = ns;

  if (ps != nullptr)
  {
    possibleStates = new S[numStates];
    for (int i = 0; i < numStates; i++)
      possibleStates[i] = ps[i];
  }
}

template <class S, class A>
void MDP<S, A>::setActions(int na, A *pa)
{
  numActions = na;

  if (pa != nullptr)
  {
    possibleActions = new A[numActions];
    for (int i = 0; i < numActions; i++)
      possibleActions[i] = pa[i];
  }
}

template <class S, class A>
void MDP<S, A>::setReset(S (*re)(int, S *))
{
  reset = re;
}

template <class S, class A>
void MDP<S, A>::setPossibleAction(int (*p_a)(S, A *))
{
  possible_action = p_a;
}

template <class S, class A>
void MDP<S, A>::setEnvironnement(void (*env)(S, A, double &, S &))
{
  environnement = env;
}

template <class S, class A>
void MDP<S, A>::setIsTerminal(bool (*is_t)(S, int))
{
  is_terminal = is_t;
}

template <class S, class A>
MDP<S, A>::~MDP()
{
  if (possibleStates != nullptr)
    delete[] possibleStates;
  if (possibleActions != nullptr)
    delete[] possibleActions;
}

#endif