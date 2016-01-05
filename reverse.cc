#include <iostream>
#include <map>
#include <string>

using namespace std;

int
main()
{
  map<int, string> m;

  m[1] = "one";

  for(auto r = m.rbegin(); r != m.rend();)
    if ((r++)->first == 1)
      m.erase(r.base());
  
  for(auto const &e: m)
    cout << e.first << " -> " << e.second << endl;
}
