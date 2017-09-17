#ifndef __DFA_H__
#define __DFA_H__

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "NfaData.h"

using namespace std;

class DFA
{
  public:
    DFA():lang("0123456789"), state_size(0), start_state(0), final_state(0){}
    DFA(int size, int start, int end) : lang("0123456789"), state_size(size), start_state(start), final_state(end){
      // Initialize with empty values
      for(int i=0; i<state_size; i++){
        vector<NfaData> a;
        closure.push_back(a);
        dfa_state_nums.push_back(-1);
      }
    }

    void buildNfaToDfa(vector<NfaData> table);
    bool test(string test);

  private:
    int findNfaData(vector<NfaData> table, int my_state, string value);
    bool epsilonClosure(vector<NfaData> table, NfaData curState, int my_state);
    void closerToTable(vector<NfaData> table);

  private:
    const string lang;
    const int state_size;
    const int start_state;
    const int final_state;
    vector<vector<NfaData> > closure;

    vector<vector<int> > dfa_table;
    vector<int> dfa_state_nums;
};
#endif
