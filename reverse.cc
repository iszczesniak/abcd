#include <iostream>
#include <map>
#include <string>

using namespace std;

int
main()
{
  map<int, string> m;

  m[1] = "one";

  for(auto const &e: m)
    cout << e.first << " -> " << e.second << endl;
}
