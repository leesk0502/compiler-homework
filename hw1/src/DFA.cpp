#include "DFA.h"

using namespace std;

void DFA::buildNfaToDfa(vector<NfaData> table)
{
  for(int i=0; i<table.size(); i++){
    bool is_end = epsilonClosure(table, table[i], table[i].my_state);
    if( table[i].value == "e" && find( closure[table[i].my_state].begin(), closure[table[i].my_state].end(), table[i] ) == closure[table[i].my_state].end() ){
      closure[table[i].my_state].push_back(table[i]);
    }
  }
  cout << endl << "<epsilon closure>" << endl;
  for(int i=0; i<state_size; i++){
    cout << i << " -> {";
    for(int j=0; j<closure[i].size(); j++){
      cout << closure[i][j].next_state << ", ";
    }
    cout << "} " << endl;
  }
  closerToTable(table);
}

bool DFA::test(string test)
{
  int next_num = dfa_state_nums[start_state];
  for(int i=0; i<test.length(); i++){
    int lang_index = lang.find(test[i]);
    if( lang_index == -1 ){
      cout << "The charater " << test[i] << " is not allow" << endl;
      return false;
    }

    if( dfa_table[next_num][lang_index] == -1 )
      return false;
    
    next_num = dfa_table[next_num][lang_index];
  }


  // Check final state
  int idx = -1;
  for(int k=0; k<state_size; k++){
    if( next_num == dfa_state_nums[k] )
      idx = k;
  }
  for(int k=0; k<closure[idx].size(); k++){
    if( closure[idx][k].next_state == final_state ){
      return true;
    }
  }

  return false;
}

int DFA::findNfaData(vector<NfaData> table, int my_state, string value)
{
  for(int i=0; i<table.size(); i++){
    if( table[i].my_state == my_state && table[i].value == value ){
      return i;
    }
  }
  return -1;
}

bool DFA::epsilonClosure(vector<NfaData> table, NfaData curState, int my_state)
{
  if( curState.value != "e" || curState.next_state == final_state)
    return true;

  for(int i=0; i<table.size(); i++){
    if( table[i].my_state == curState.next_state ){
      if( table[i].value == "e" ){
        bool is_end = epsilonClosure(table, table[i], my_state);

        if( is_end && find( closure[curState.my_state].begin(), closure[curState.my_state].end(), table[i] ) == closure[curState.my_state].end() ){
          closure[my_state].push_back(table[i]);
        }
      }
      else{
        return true;
      }
    } 
  }
  return false;
}

void DFA::closerToTable(vector<NfaData> table)
{
  int dfa_state_size = 0;

  // Only closure have more than one, can be dfa state
  for(int i=0, j=0; i<state_size; i++){
    dfa_state_nums[i] = -1;
    if( closure[i].size() > 0 ){
      dfa_state_size++;
      dfa_state_nums[i] = j++;
    }
  }

  //int dfa_table[dfa_state_size][lang.length()];
  
  // initialize dfa_table with -1, -1 means empty set
  for(int i=0; i<dfa_state_size; i++){
    vector<int> temp;
    for(int j=0; j<lang.length(); j++){
      temp.push_back(-1);
    }
    dfa_table.push_back(temp);
  }

  for(int i=0; i<dfa_state_size; i++){
    for(int j=0; j<lang.length(); j++){
      string value(1, lang[j]);
    
      int idx = -1;
      for(int k=0; k<state_size; k++){
        if( i == dfa_state_nums[k] )
          idx = k;
      }
      if( idx == -1 ) continue;
          

      for(int k=0; k<closure[idx].size(); k++){
        // If nfa state in closure have same with value,
        // dfa_table of i, j has next dfa state.
        int d = findNfaData(table, closure[idx][k].next_state, value);
        if( d != -1 ){
          dfa_table[i][j] = dfa_state_nums[ table[d].next_state ];
        }
        
      }
    }
  }

  cout << endl << "<DFA Table>" << endl;
  for(int i=0; i<dfa_state_size; i++){
    cout << i << " | ";
    for(int j=0; j<lang.length(); j++){
      cout << dfa_table[i][j] << " ";    
    }
    cout << endl;
  }

}

