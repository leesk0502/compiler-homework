#ifndef __NFASTATE_H__
#define __NFASTATE_H__

#include <string>
#include <vector>
#include <stack>

using namespace std;

class NfaState{
  public:
    NfaState() : state_num(0), is_loof(false){}
    ~NfaState(){}

  public:
    int state_num;
    bool is_loof;
    vector<pair<NfaState *, string> > next;

    // his variable can be either number or regex
};
#endif
