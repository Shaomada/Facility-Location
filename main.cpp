#include "solver.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <cstring>

double max_dist(const std::vector<Solver::Point> &D) {
  double retval = 0;
  for (std::vector<Solver::Point>::const_iterator it = D.begin(); it != D.end(); ++it) {
    for (std::vector<Solver::Point>::const_iterator jt = it; ++jt != D.end();) {
      retval = std::max(retval, Solver::Point::dist(*it, *jt));
    }
  }
  return retval;
}

int main ( int argc, char **argv ) {
  if (argc  == 1) {
    std::cout << "Usage: " << argv[0] << " <Instanzdateiname> {-f <int>} {-u <int>} {-k <int>} ." << std::endl;
    return 1;
  }
  if (argc % 2) {
    std::cout << "Can't be correct Usage due to parity of number of arguments" << std::endl;
    std::cout << "Usage: " << argv[0] << " <Instanzdateiname> {-f <int>} {-u <int>} {-k <int>} ." << std::endl;
    return 1;
  }
  std::ifstream input(argv[1]);
  std::vector<Solver::Point> D;
  std::string word;
  do {
    if (!input) {
      std::cout << "Bad input file: ended before \"NODE_COORD_SECTION\"" << std::endl;
      return 1;
    }
    input >> word;
  } while (word != "NODE_COORD_SECTION");
  while (input >> word, word != "EOF") {
    int x, y;
    input >> x >> y;
    D.emplace_back((double) x, (double) y);
  }
  int r = 2;
  double f;
  if (r < argc && strcmp(argv[r], "-f") == 0) {
    f = atof(argv[++r,r++]);
  } else {
    f = 2*max_dist (D);
    std::cout << "f automatically chosen to be " << f << std::endl;    
  }
  unsigned u;
  if (r < argc && strcmp(argv[r], "-u") == 0) {
    u = atoi(argv[++r,r++]);
  } else {
    u = std::numeric_limits<unsigned>::max();
    std::cout << "u automatically chosen to be " << u << std::endl;
  }
  int k;
  if (r < argc && strcmp(argv[r], "-k") == 0) {
    k = atoi(argv[++r,r++]);
  } else {
    k = 1+(D.size()-1)/u;
    std::cout << "k automatically chosen to be " << k << std::endl;
  }
  if (u*k < D.size()) {
    std::cout << "Infeasible (u*k < D.size)" << std::endl;
    return 1;
  }
  if (r < argc) {
    std::cout << "Wrong Usage: Left over arguments:";
    while (r < argc) {
      std::cout << " " << argv[r++];
    }
    std::cout << std::endl;
    std::cout << "Usage: " << argv[0] << " <Instanzdateiname> {-f <int>} {-u <int>} {-k <int>} ." << std::endl;
    return 1;
  }
  std::vector<Solver::Point> I;
  while (k --> 0) {
    I.push_back(D.at(rand()%D.size()));
  }
  Solver s(u, D, I, f);
  s.solve();
  return 0;
}