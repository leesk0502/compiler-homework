#ifndef __NFADATA_H__
#define __NFADATA_H__

#include <string>
#include <vector>
#include <stack>

using namespace std;

class NfaData{
  public:
    NfaData(int _my, int _next, string _value) : my_state(_my), next_state(_next), value(_value) {}

    bool operator ==(const NfaData &rhs) const{
      if( my_state == rhs.my_state && next_state == rhs.next_state && value == rhs.value)
        return true;
      return false;
    }

    bool operator <(const NfaData &rhs) const{
      if( this->my_state == rhs.my_state ){
        return this->next_state < rhs.next_state;
      }
      else{
        return this->my_state < rhs.my_state;
      }
    }

    int my_state, next_state;
    string value;
};
#endif
