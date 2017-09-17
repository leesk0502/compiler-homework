#ifndef __REGEX_H__
#define __REGEX_H__

#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include "NfaState.h"
#include "NfaData.h"

using namespace std;

class Regex
{
  public:
    Regex() : lang("0123456789"), nfa_size(-1){}
    void regexToNfa(NfaState *&init_state, string regex);
    void print(NfaState *state);
    void makeNfaToTable(NfaState *state, vector<NfaData> *data);
    int getNfaSize();
    int getFinalStateNum();

  private:
    int priority(char c);
    string regexToPostfix(string regex);
    void postfixToNfa(string postfix);
    void makeSquareToOr(string numbers);
    void kleeneStar();
    void plusOr();
    void concat();

  private:
    const string lang;
    stack<NfaState *> st;
    int nfa_size;
    int final_state_num;
};

#endif
