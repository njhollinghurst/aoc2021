#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <vector>

using namespace std;

unsigned nbits;
vector<uint8_t> data;

void rd()
{
  unsigned u;

  nbits = 0;
  while (scanf("%1x", &u) >= 1) {
    if (!(nbits & 4)) data.push_back(u<<4);
    else data.back() |= u;
    nbits += 4;
  }
}

unsigned get_bits(unsigned *pos, unsigned num)
{
  unsigned u = 0;
  unsigned p = *pos;
  assert(p + num <= nbits);
  for(; num > 0; num--,p++) {
    u = 2*u + !!(data[p>>3] & (0x80 >> (p & 7)));
  }
  *pos = p;
  return u;
}

unsigned task1(unsigned *pos, unsigned maxpos)
{
  unsigned v, t;

  v = get_bits(pos, 3);
  t = get_bits(pos, 3);
  if (t==0x4) { // literal
    unsigned nybble;
    do {
      nybble = get_bits(pos, 5);
    } while (nybble & 0x10);
    return v;
  }
  else if (get_bits(pos, 1) == 0) { // operator with total length
    unsigned len, sum = v;
    len = get_bits(pos, 15);
    unsigned end = *pos + len;
    assert(end <= maxpos);
    while (*pos < end) sum += task1(pos, end);
    return sum;
  }
  else { // operator with count
    unsigned count, sum = v;
    count = get_bits(pos, 11);
    while (count-- > 0) sum += task1(pos, maxpos);
    assert(*pos <= maxpos);
    return sum;
  }
}

long long task2(unsigned *pos, unsigned maxpos)
{
  unsigned ver, t, count, end;
  long long val = 0;

  ver = get_bits(pos, 3);
  (void)ver;
  t = get_bits(pos, 3);
  if (t==0x4) { // literal
    unsigned nybble;
    do {
      nybble = get_bits(pos, 5);
      val = (val<<4) + (nybble & 0xF);
    } while (nybble & 0x10);
    return val;
  }
  else if (get_bits(pos, 1) == 0) { // operator with total length
    unsigned len;
    len = get_bits(pos, 15);
    end = *pos + len;
    assert(end <= maxpos);
    count = 0x7FFFFFFF;
  }
  else { // operator with count
    count = get_bits(pos, 11);
    end = maxpos;
  }

  std::vector<long long> values;
  while (*pos < end && count-- > 0) {
    values.push_back(task2(pos, end));
  }
  assert(*pos <= maxpos);
 
  switch (t) {
  case 0:
    for(auto v : values) val += v;
    break;
  case 1:
    val = 1;
    for(auto v : values) val *= v;
    break;
  case 2:
    val = values[0];
    for(auto v : values) if (v < val) val = v;
    break;
  case 3:
    val = values[0];
    for(auto v : values) if (v > val) val = v;
    break;
  case 5:
    assert(values.size() == 2);
    val = values[0] > values[1];
    break;
  case 6:
    assert(values.size() == 2);
    val = values[0] < values[1];
    break;
  case 7:
    assert(values.size() == 2);
    val = values[0] == values[1];
    break;
  }

  return val;
}


int main()
{
  rd();
  unsigned pos = 0;
  unsigned total_version = task1(&pos, nbits);
  assert(pos <= nbits && pos+8 >= nbits);
  printf("Total versions: %u\n", total_version);

  pos = 0;
  long long result = task2(&pos, nbits);
  assert(pos <= nbits && pos+8 >= nbits);
  printf("Result: %lld\n", result);

  return 0;
}
