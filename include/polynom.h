#pragma once

#include <map>
#include <variant>
#include <vector>

class Polynom {
 private:
  std::map<int, double> coef;

 public:
  Polynom() { coef = std::map<int, double>(); }

  Polynom(std::map<int, double> &other) { coef = other; }

  Polynom(const Polynom &other) { this->coef = other.coef; }

  Polynom(Polynom &&other) {
    this->coef = other.coef;
    other.coef = std::map<int, double>();
  }

  Polynom &operator=(const Polynom &other) {
    if (this == &other) {
      return *this;
    }
    this->coef = other.coef;
    return *this;
  }

  Polynom &operator=(Polynom &&other) {
    if (this == &other) {
      return *this;
    }
    this->coef = other.coef;
    other.coef = std::map<int, double>();
    return *this;
  }

  Polynom operator+(const Polynom &other) {
    Polynom tmp = *this;
    for (auto iter = other.coef.begin(); iter != other.coef.end(); ++iter) {
      if (tmp.coef.contains(iter->first)) {
        tmp.coef[iter->first] += iter->second;
      } else {
        tmp.coef[iter->first] = iter->second;
      }
    }
    for (auto iter = tmp.coef.begin(); iter != tmp.coef.end();) {
      if (tmp.coef[iter->first] == 0) {
        iter = tmp.coef.erase(iter);
      } else {
        ++iter;
      }
    }
    return tmp;
  }

  Polynom operator-(const Polynom &other) {
    Polynom tmp = *this;
    for (auto iter = other.coef.begin(); iter != other.coef.end(); ++iter) {
      if (tmp.coef.contains(iter->first)) {
        tmp.coef[iter->first] -= iter->second;
      } else {
        tmp.coef[iter->first] = iter->second * (-1);
      }
    }
    for (auto iter = tmp.coef.begin(); iter != tmp.coef.end();) {
      if (tmp.coef[iter->first] == 0) {
        iter = tmp.coef.erase(iter);
      } else {
        ++iter;
      }
    }
    return tmp;
  }

  Polynom operator*(const Polynom &other) {
    Polynom tmp;
    for (auto iter1 = other.coef.begin(); iter1 != other.coef.end(); ++iter1) {
      for (auto iter2 = this->coef.begin(); iter2 != this->coef.end(); ++iter2) {
        tmp.coef[iter1->first + iter2->first] = iter1->second * iter2->second;
      }
    }
    for (auto iter = tmp.coef.begin(); iter != tmp.coef.end();) {
      if (tmp.coef[iter->first] == 0) {
        iter = tmp.coef.erase(iter);
      } else {
        ++iter;
      }
    }
    return tmp;
  }

  Polynom operator/(const Polynom &other) { return *this; }

  friend Polynom operator+(const long long &ll, const Polynom &p);
  friend Polynom operator-(const long long &ll, const Polynom &p);
  friend Polynom operator*(const long long &ll, const Polynom &p);
  friend Polynom operator/(const long long &ll, const Polynom &p);

  Polynom operator+(const long long &ll) {
    Polynom tmp = *this;
    if (tmp.coef.contains(0)) {
      tmp.coef[0] += ll;
    } else {
      tmp.coef[0] = ll;
    }
    if (tmp.coef[0] == 0) {
      tmp.coef.erase(0);
    }
    return tmp;
  }

  Polynom operator-(const long long &ll) {
    Polynom tmp = *this;
    if (tmp.coef.contains(0)) {
      tmp.coef[0] -= ll;
    } else {
      tmp.coef[0] = ll;
    }
    if (tmp.coef[0] == 0) {
      tmp.coef.erase(0);
    }
    return tmp;
  }

  Polynom operator*(const long long &ll) {
    Polynom tmp = *this;
    for (auto iter = tmp.coef.begin(); iter != tmp.coef.end(); ++iter) {
      int key = (*iter).first;
      tmp.coef[key] *= ll;
    }
    return tmp;
  }

  Polynom operator/(const long long &ll) {
    Polynom tmp = *this;
    for (auto iter = tmp.coef.begin(); iter != tmp.coef.end(); ++iter) {
      int key = (*iter).first;
      tmp.coef[key] /= ll;
    }
    return tmp;
  }

  friend Polynom operator+(const double &d, const Polynom &p);
  friend Polynom operator-(const double &d, const Polynom &p);
  friend Polynom operator*(const double &d, const Polynom &p);
  friend Polynom operator/(const double &d, const Polynom &p);

  Polynom operator+(const double &d) {
    Polynom tmp = *this;
    if (tmp.coef.contains(0)) {
      tmp.coef[0] += d;
    } else {
      tmp.coef[0] = d;
    }
    if (tmp.coef[0] == 0) {
      tmp.coef.erase(0);
    }
    return tmp;
  }

  Polynom operator-(const double &d) {
    Polynom tmp = *this;
    if (tmp.coef.contains(0)) {
      tmp.coef[0] -= d;
    } else {
      tmp.coef[0] = d;
    }
    if (tmp.coef[0] == 0) {
      tmp.coef.erase(0);
    }
    return tmp;
  }

  Polynom operator*(const double &d) {
    Polynom tmp = *this;
    for (auto iter = tmp.coef.begin(); iter != tmp.coef.end(); ++iter) {
      int key = (*iter).first;
      tmp.coef[key] *= d;
    }
    return tmp;
  }

  Polynom operator/(const double &d) {
    Polynom tmp = *this;
    for (auto iter = tmp.coef.begin(); iter != tmp.coef.end(); ++iter) {
      int key = (*iter).first;
      tmp.coef[key] /= d;
    }
    return tmp;
  }

  bool operator==(const Polynom &other) { return coef == other.coef; }

  bool operator==(const long long &ll) { return coef.size() == 1 && coef[0] == (double)ll; }

  bool operator==(const double &d) { return coef.size() == 1 && coef[0] == d; }

  void push(int pow, double value) { coef[pow] = value; }

  bool remove(int pow) {
    if (coef.contains(pow)) {
      coef.erase(pow);
      return true;
    }
    return false;
  }

  bool isDigit() { return coef.size() == 0 && coef.contains(0); }

  void print() {
    for (auto iter1 = coef.rbegin(); iter1 != coef.rend();) {
      if ((*iter1).first == 0) {
        std::cout << (*iter1).second;
      } else if ((*iter1).first == 1) {
        std::cout << (*iter1).second << "x";
      } else {
        std::cout << (*iter1).second << "x^" << (*iter1).first;
      }
      if (++iter1 != coef.rend()) {
        if ((*iter1).second > 0) {
          std::cout << "+";
        }
      } else {
        std::cout << " ";
      }
    }
  }
};

Polynom operator+(const long long &ll, const Polynom &p) {
  Polynom tmp = p;
  if (tmp.coef.contains(0)) {
    tmp.coef[0] += ll;
  } else {
    tmp.coef[0] = ll;
  }
  if (tmp.coef[0] == 0) {
    tmp.coef.erase(0);
  }
  return tmp;
}

Polynom operator-(const long long &ll, const Polynom &p) {
  Polynom tmp = p;
  if (tmp.coef.contains(0)) {
    tmp.coef[0] -= ll;
  } else {
    tmp.coef[0] = ll;
  }
  if (tmp.coef[0] == 0) {
    tmp.coef.erase(0);
  }
  return tmp;
}

Polynom operator*(const long long &ll, const Polynom &p) {
  Polynom tmp = p;
  for (auto iter = tmp.coef.begin(); iter != tmp.coef.end(); ++iter) {
    int key = (*iter).first;
    tmp.coef[key] *= ll;
  }
  return tmp;
}

Polynom operator/(const long long &ll, const Polynom &p) {
  Polynom tmp = p;
  tmp.push(0, (double)ll);
  return tmp;
}

Polynom operator+(const double &d, const Polynom &p) {
  Polynom tmp = p;
  if (tmp.coef.contains(0)) {
    tmp.coef[0] += d;
  } else {
    tmp.coef[0] = d;
  }
  if (tmp.coef[0] == 0) {
    tmp.coef.erase(0);
  }
  return tmp;
}

Polynom operator-(const double &d, const Polynom &p) {
  Polynom tmp = p;
  if (tmp.coef.contains(0)) {
    tmp.coef[0] -= d;
  } else {
    tmp.coef[0] = d;
  }
  if (tmp.coef[0] == 0) {
    tmp.coef.erase(0);
  }
  return tmp;
}

Polynom operator*(const double &d, const Polynom &p) {
  Polynom tmp = p;
  for (auto iter = tmp.coef.begin(); iter != tmp.coef.end(); ++iter) {
    int key = (*iter).first;
    tmp.coef[key] *= d;
  }
  return tmp;
}

Polynom operator/(const double &d, const Polynom &p) {
  Polynom tmp;
  tmp.push(0, d);
  return tmp;
}