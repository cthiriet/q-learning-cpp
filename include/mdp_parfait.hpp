#ifndef MDP_PARFAIT_HPP
#define MDP_PARFAIT_HPP

#include <iostream>
#include "mdp.hpp"

template <class S, class A>
class MDP_parfait : public MDP<S, A>
{
private:
  double **P;
  double **R;
  int getIndexIsIa(int is, int ia);

public:
  MDP_parfait();
  MDP_parfait(const MDP_parfait<S, A> &mdpp);
  MDP_parfait &operator=(const MDP_parfait<S, A> &mdpp);
  void setP(int is, int ia, int isp, double proba);
  void setR(int is, int ia, int isp, double reward);
  double getP(int is, int ia, int isp);
  double getR(int is, int ia, int isp);
  ~MDP_parfait();
};

template <class S, class A>
MDP_parfait<S, A>::MDP_parfait() : MDP<S, A>()
{
  P = nullptr;
  R = nullptr;
}

template <class S, class A>
MDP_parfait<S, A>::MDP_parfait(const MDP_parfait<S, A> &mdpp) : MDP<S, A>(mdpp)
{
  int sizeLine = mdpp.getNumStates() * mdpp.getNumActions();
  int sizeCol = mdpp.getNumStates();

  if (mdpp.P != nullptr)
  {
    P = new double *[sizeLine];
    for (int i = 0; i < sizeLine; i++)
    {
      P[i] = new double[sizeCol];
      for (int k = 0; k < sizeCol; k++)
        P[i][k] = mdpp.P[i][k];
    }
  }

  if (mdpp.R != nullptr)
  {
    R = new double *[sizeLine];
    for (int i = 0; i < sizeLine; i++)
    {
      R[i] = new double[sizeCol];
      for (int k = 0; k < sizeCol; k++)
        R[i][k] = mdpp.R[i][k];
    }
  }
}

template <class S, class A>
MDP_parfait<S, A> &MDP_parfait<S, A>::operator=(const MDP_parfait<S, A> &mdpp)
{
  if (*this != &mdpp)
  {
    if (P != nullptr)
    {
      int Psize = sizeof(P) / sizeof(P[0]);
      for (int i = 0; i < Psize; i++)
        delete[] P[i];
      delete[] P;
      int Rsize = sizeof(R) / sizeof(R[0]);
      for (int i = 0; i < Rsize; i++)
        delete[] R[i];
      delete[] R;
    }

    int sizeLine = mdpp.getNumStates() * mdpp.getNumActions();
    int sizeCol = mdpp.getNumStates();

    if (mdpp.P != nullptr)
    {
      P = new double *[sizeLine];
      for (int i = 0; i < sizeLine; i++)
      {
        P[i] = new double[sizeCol];
        for (int k = 0; k < sizeCol; k++)
          P[i][k] = mdpp.P[i][k];
      }
    }

    if (mdpp.R != nullptr)
    {
      R = new double *[sizeLine];
      for (int i = 0; i < sizeLine; i++)
      {
        R[i] = new double[sizeCol];
        for (int k = 0; k < sizeCol; k++)
          R[i][k] = mdpp.R[i][k];
      }
    }
  }
  return *this;
}

template <class S, class A>
int MDP_parfait<S, A>::getIndexIsIa(int is, int ia)
{
  return this->getNumActions() * is + ia;
}

template <class S, class A>
void MDP_parfait<S, A>::setP(int is, int ia, int isp, double proba)
{
  // init P because it is not initialized in the constructor
  if (P == nullptr)
  {
    int sizeLine = this->numStates * this->numActions;
    int sizeCol = this->numStates;

    P = new double *[sizeLine];
    for (int i = 0; i < sizeLine; i++)
    {
      P[i] = new double[sizeCol];
      for (int k = 0; k < sizeCol; k++)
        P[i][k] = 0;
    }
  }

  int i = getIndexIsIa(is, ia);
  P[i][isp] = proba;
}

template <class S, class A>
void MDP_parfait<S, A>::setR(int is, int ia, int isp, double reward)
{
  // init R because it is not initialized in the constructor
  if (R == nullptr)
  {
    int sizeLine = this->numStates * this->numActions;
    int sizeCol = this->numStates;

    R = new double *[sizeLine];
    for (int i = 0; i < sizeLine; i++)
    {
      R[i] = new double[sizeCol];
      for (int k = 0; k < sizeCol; k++)
        R[i][k] = 0;
    }
  }

  int i = getIndexIsIa(is, ia);
  R[i][isp] = reward;
}

template <class S, class A>
double MDP_parfait<S, A>::getP(int is, int ia, int isp)
{
  int i = getIndexIsIa(is, ia);
  return P[i][isp];
}

template <class S, class A>
double MDP_parfait<S, A>::getR(int is, int ia, int isp)
{
  int i = getIndexIsIa(is, ia);
  return R[i][isp];
}

template <class S, class A>
MDP_parfait<S, A>::~MDP_parfait()
{
  if (P != nullptr)
  {
    int Psize = sizeof(P) / sizeof(P[0]);
    for (int i = 0; i < Psize; i++)
      delete[] P[i];
    delete[] P;
  }

  if (R != nullptr)
  {
    int Rsize = sizeof(R) / sizeof(R[0]);
    for (int i = 0; i < Rsize; i++)
      delete[] R[i];
    delete[] R;
  }
}

#endif