#include "Regex.h"

using namespace std;

void Regex::regexToNfa(NfaState *&init_state, string regex)
{
  string postfix = regexToPostfix(regex);
  postfixToNfa(postfix);
  if( st.size() <= 0 ){
    cout << "Processing Error" << endl;
    exit(1);
  }
  init_state = st.top(); st.pop();
  while(!st.empty()){
    NfaState *a = st.top();
    st.pop();
    final_state_num = a->state_num;
  }
  nfa_size++;
}

void Regex::print(NfaState *state)
{
  if( state->next.size() == 0 ){
    return ;
  }
  for(int i=0; i<state->next.size(); i++){
    cout << state->state_num << " -> " << state->next[i].first->state_num << " on " << state->next[i].second << endl;
    if( state->is_loof == false )
      print(state->next[i].first);
  }
}

void Regex::makeNfaToTable(NfaState *state, vector<NfaData> *data)
{
  if( state->next.size() == 0 ){
    return ;
  }
  for(int i=0; i<state->next.size(); i++){
    NfaData d( state->state_num, state->next[i].first->state_num, state->next[i].second );
    if( find(data->begin(), data->end(), d) == data->end() ){
      data->push_back(d);
    }
    if( state->is_loof == false )
      makeNfaToTable(state->next[i].first, data);
  }
}

int Regex::getNfaSize(){
  return nfa_size;
}

int Regex::getFinalStateNum(){
  return final_state_num;
}

int Regex::priority(char c)
{
  switch(c){
    case '*': return 3;
    case '.': return 2;
    case '|': return 1;
    default: return 0;
  }
}

string Regex::regexToPostfix(string regex)
{
  string postfix;
  stack<char> op;
  for(int i=0; i < regex.length(); i++)
  {
    char c = regex[i];

    if( c == '[' || c == ']' || c == '^' || lang.find(c) != -1 ){
      postfix += c;
    } 
    else {
      switch(c){
        case '(':{
          op.push(c);
        }break;
        case ')':{
          while(op.top()!='('){
            postfix += op.top();
            op.pop();
          }
          op.pop();
        }break;
        default:
          while(!op.empty()){
            char a = op.top();
            if(priority(a) >= priority(c)){
              postfix += op.top();
              op.pop();
            }
            else {
              break;
            }
          }
          op.push(c);
      }
    }
  }
  while(!op.empty()){
    postfix += op.top();
    op.pop();
  }
  cout << "<Regex to Postfix>" << endl;
  cout << postfix << endl;
  return postfix;
}

void Regex::postfixToNfa(string postfix)
{
  for(int i=0; i<postfix.length(); i++){
    char c = postfix[i];
    switch(c){
      case '[' :{
        string temp=lang;

        // [a]
        if( postfix.length()-i <= 3 ){
          cout << "Processing Error1" << endl;
          exit(1);
        }

        // Check [^a]
        if( postfix[i+1] == '^'){
          if( lang.find(postfix[i+2]) != -1 && postfix[i+3] == ']'){ 
            temp.erase(temp.begin()+temp.find(postfix[i+2]));
            i += 2;
          }
          else{
            cout << "Processing Error2" << endl;
            exit(1);
          }
        } else {
          temp = "";
          if( i+1 >= postfix.length() ){
            cout << "Processing Error3" << endl;
          }
          for(int j=i+1; j < postfix.length(); j++){
            if( postfix[j] != ']' && lang.find(postfix[j]) != -1 ){
              temp += postfix[j];
            } else {
              if( postfix[j] == ']' ){
                i = j;
                break;
              }
              cout << "Processing Error4" << endl;
              exit(1);
            }
          }
        }
        makeSquareToOr(temp);
        NfaState *a = st.top();
      }break;
      case '*' : kleeneStar(); break;
      case '|' : plusOr(); break;
      case '.' : concat(); break;
    }
  }
}

void Regex::makeSquareToOr(string numbers)
{
  if( numbers.length() <= 0 ){
    cout << "Processing Error in makeSquareToOr()" << endl;
    exit(1);
  }
  NfaState *init_state = new NfaState();
  NfaState *final_state = new NfaState();;
  init_state->state_num = ++nfa_size;
  final_state->state_num = ++nfa_size;

  if( numbers.length() == 1 ){
    init_state->next.push_back(make_pair(final_state, numbers));
    st.push(final_state);
    st.push(init_state);
    return ;
  }
  const int TOTAL_STATE_SIZE = numbers.length()*2;
  NfaState *a[TOTAL_STATE_SIZE];
  for(int i=0; i<TOTAL_STATE_SIZE; i++){
    a[i] = new NfaState();
    a[i]->state_num = ++nfa_size;
  }

  //final_state->state_num = nfa_size;
  for(int i=0; i<TOTAL_STATE_SIZE; i+=2){
    string s(1, numbers[i/2]);
    a[i+1]->next.push_back(make_pair(final_state, "e"));
    a[i]->next.push_back(make_pair(a[i+1], s));
  }

  for(int i=0; i<TOTAL_STATE_SIZE; i+=2)
    init_state->next.push_back(make_pair(a[i], "e"));

  st.push(final_state);
  st.push(init_state);
}

void Regex::kleeneStar()
{
  if( st.size() < 2 ){
    cout << "Process error in kleeneStar()" << endl;
    exit(1);
  }
  NfaState *a = new NfaState();
  NfaState *d = new NfaState();
  a->state_num = ++nfa_size;
  d->state_num = ++nfa_size;
  NfaState *b = st.top(); st.pop();
  NfaState *c = st.top(); st.pop();

  c->next.push_back(make_pair(b, "e"));
  c->is_loof = true;
  c->next.push_back(make_pair(d, "e"));
  a->next.push_back(make_pair(b, "e"));
  a->next.push_back(make_pair(d, "e"));
  st.push(d);
  st.push(a);
}

void Regex::plusOr()
{
  if( st.size() < 4 ){
    cout << "Process error in plusOr()" << endl;
    exit(1);
  }
  NfaState *a = new NfaState();
  NfaState *f = new NfaState();
  a->state_num = ++nfa_size;
  f->state_num = ++nfa_size;

  NfaState *d = st.top(); st.pop();
  NfaState *e = st.top(); st.pop();

  NfaState *b = st.top(); st.pop();
  NfaState *c = st.top(); st.pop();

  c->next.push_back(make_pair(f, "e"));
  e->next.push_back(make_pair(f, "e"));
  a->next.push_back(make_pair(b, "e"));
  a->next.push_back(make_pair(d, "e"));
  
  st.push(f);
  st.push(a);
}

void Regex::concat()
{
  if( st.size() < 4 ){
    cout << "Process error in concat()" << endl;
    exit(1);
  }
  NfaState *c = st.top(); st.pop();
  NfaState *d = st.top(); st.pop();

  NfaState *a = st.top(); st.pop();
  NfaState *b = st.top(); st.pop();

  b->next.push_back(make_pair(c, "e"));
  st.push(d);
  st.push(a);
}



