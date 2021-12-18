#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>

class SFNum {
public:
  bool literally;
  long long val;  // let's not use a union, thanks
  SFNum * p[2];   // first, second elements, which we now own
  SFNum(long long l) : literally(true), val(l), p{0,0} {}
  SFNum(SFNum * head, SFNum * tail) : literally(false), val(0), p{head,tail} { }
  ~SFNum() {
    delete p[1];
    delete p[0];
  }
  bool isLiteral() const { return literally; }
  void print(bool lf = true) const;
  void reduce();
  long long magnitude() const;
  SFNum * clone() const;
  static SFNum * read(FILE * fp);
private:
  bool splitFirst();
  SFNum * findFirstLiteral(unsigned direction = 0);
  bool explodeFirst(unsigned depth = 0, SFNum * llb = 0, SFNum * fla = 0);
};

void SFNum::print(bool lf) const
{
  if (isLiteral()) {
    printf("%llu", val);
  }
  else {
    putchar('[');
    p[0]->print(false);
    putchar(',');
    p[1]->print(false);
    putchar(']');
  }
  if (lf) putchar('\n');
}

bool SFNum::splitFirst()
{
  if (isLiteral()) {
    if (val >= 10) {
      p[0] = new SFNum(val >> 1);
      p[1] = new SFNum((val+1) >> 1);
      literally = false;
      val = 0;
      return true;
    }
    return false;
  }
  else {
    return p[0]->splitFirst() || p[1]->splitFirst();
  }
}

SFNum * SFNum::findFirstLiteral(unsigned direction)
{
  if (isLiteral()) return this;
  return p[!!direction]->findFirstLiteral(direction);
}

bool SFNum::explodeFirst(unsigned depth, SFNum * llb, SFNum * fla)
{
  if (isLiteral()) return false;

  if (p[0]->isLiteral() && p[1]->isLiteral() && depth == 4) {
    if (llb) llb->val += p[0]->val;
    if (fla) fla->val += p[1]->val;
    delete p[0];
    delete p[1];
    p[0] = 0;
    p[1] = 0;
    literally = true;
    val = 0;
    return true;
  }
  else {
    return p[0]->explodeFirst(depth+1, llb, p[1]->findFirstLiteral(0))
      ||   p[1]->explodeFirst(depth+1, p[0]->findFirstLiteral(1), fla);
  }
}

void SFNum::reduce()
{
  do {
    while (explodeFirst()) ;
  } while (splitFirst());
}

long long SFNum::magnitude() const
{
  if (isLiteral()) return val;
  return 3*p[0]->magnitude() + 2*p[1]->magnitude();
}

SFNum * SFNum::clone() const
{
  if (isLiteral()) return new SFNum(val);
  return new SFNum(p[0]->clone(), p[1]->clone());
}

SFNum * SFNum::read(FILE * fp)
{
  int c;
  do  c = fgetc(fp); while (c > 0 && isspace(c));
  if (c < 0) return 0; // EOF
  else if (c=='[') {
    SFNum *a = read(fp);
    do  c = fgetc(fp); while (c > 0 && isspace(c));
    assert(c==',');
    SFNum *b = read(fp);
    do  c = fgetc(fp); while (c > 0 && isspace(c));
    assert(c==']');
    return new SFNum(a,b);
  }
  else {
    assert(isdigit(c));
    ungetc(c, fp);
    long long l;
    fscanf(fp, "%lld", &l);
    return new SFNum(l);
  }
}

int main()
{
  std::vector<SFNum *> inputs;

  SFNum * theNumber = SFNum::read(stdin);
  assert(theNumber);
  inputs.push_back(theNumber->clone());

  for (;;) {
    SFNum * nextNumber = SFNum::read(stdin);
    if (!nextNumber) break;
    inputs.push_back(nextNumber->clone());
    theNumber = new SFNum(theNumber, nextNumber); // NB takes ownership!
    theNumber->reduce();
  }
  theNumber->print();
  printf("Final magnitude: %lld\n", theNumber->magnitude());
  delete theNumber;

  unsigned i, j;
  long long maxmag = 0;

  for(i = 0; i < inputs.size(); ++i) {
    for(j = 0; j < inputs.size(); ++j) {
      if (i != j) {
	SFNum sum(inputs[i]->clone(), inputs[j]->clone());
	sum.reduce();
	long long mag = sum.magnitude();
	if (mag > maxmag) maxmag = mag;
      }
    }
  }
  printf("Max   magnitude: %lld\n", maxmag);
  while (inputs.size()) {
    delete inputs.back();
    inputs.pop_back();
  }

  return 0;
}
