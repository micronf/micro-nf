#ifndef _MATCHABLE_H_
#define _MATCHABLE_H_

class Matchable {
  public:
    virtual bool Match(const Matchable* m) = 0;
}
#endif // _MATCHABLE_H_
