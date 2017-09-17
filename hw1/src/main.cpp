#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "Regex.h"
#include "DFA.h"

using namespace std;


int main(int argc, char **argv){
  
  if(argc < 2){
    printf("Usage: ./regex \"<regexp>\"\n");
    printf("If you want Quit enter q or Q or [Ctrl + c]\n");
    return -1;
  }

  string reg_string = argv[1];

  cout << "---Regex to Nfa---" << endl;

  Regex regex;
  NfaState *init = new NfaState();

  regex.regexToNfa(*&init, reg_string);  

  vector<NfaData> table;
  regex.makeNfaToTable(init, &table);
  sort(table.begin(), table.end());

  for(int i=0; i<table.size(); i++){
    cout << table[i].my_state << " -> " << table[i].next_state << " on " << table[i].value << endl;

  }

  cout << endl<< "---Nfa to Dfa---" << endl;
  
  DFA dfa(regex.getNfaSize(), init->state_num, regex.getFinalStateNum());
  dfa.buildNfaToDfa(table);

  while(1){
    string input;
    cin >> input;

    if( input == "q" ) break;

    if( dfa.test(input) ){
      cout << "Accepted" << endl;
    }
    else{
      cout << "Not Accepted" << endl;
    }
  }


  return 0;
}
