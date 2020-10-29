#pragma once

#include <cmath>
#include <iostream>
#include <vector>

namespace task {
std::vector<double> operator+(const std::vector<double> &a, const std::vector<double> &b) {
  std::vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] + b[i];
  }
  return c;
}

std::vector<double> operator-(const std::vector<double> &a, const std::vector<double> &b) {
  std::vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] - b[i];
  }
  return c;
}

std::vector<double> operator+(const std::vector<double> &a) {
  return a;
}

std::vector<double> operator-(const std::vector<double> &a) {
  std::vector<double> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = -a[i];
  }
  return c;
}

double operator*(const std::vector<double> &a, const std::vector<double> &b) {
  double c = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    c += a[i] * b[i];
  }
  return c;
}

std::vector<double> operator%(const std::vector<double> &a, const std::vector<double> &b) {
  std::vector<double> c(3);
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
  return c;
}

bool operator||(const std::vector<double> &a, const std::vector<double> &b) {
  int i;
  int n = a.size();
  const double EPS = 1e-12;
  std::vector<double> c(n, 0.);
  if (a == c || b == c) {
    return true;
  }
  for (i = 0; i < n; ++i) {
    if (a[i] != 0 && b[i] != 0) {
      break;
    }
  }
  if (i == n) {
    return false;
  }
  double alpha = a[i] / b[i];
  for (size_t j = 0; j < n; ++j) {
    if ((b[j] == 0 && a[j] != 0) || (a[j] == 0 && b[j] != 0)) {
      return false;
    }
    if ((b[j] != 0) && (fabs(a[j] / b[j] - alpha) >= EPS)) {
      return false;
    }
  }
  return true;
}


bool operator&&(const std::vector<double> &a, const std::vector<double> &b) {
  int n = a.size();
  std::vector<double> c(n, 0.);
  int i;
  if (a == c || b == c) {
    return true;
  }
  for (i = 0; i < n; ++i) {
    if (a[i] != 0 && b[i] != 0) {
      break;
    }
  }
  if (i == n) {
    return false;
  }
  double alpha = a[i] / b[i];
  if ((a || b) && (alpha > 0)) {
    return true;
  } else {
    return false;
  }
}

void reverse(std::vector<double> &a) {
  int n = a.size();
  if ((n != 0) && (n != 1)) {
    for (ssize_t i = 0; i < n / 2; ++i) {
      std::swap(a[i], a[n - i - 1]);
    }
  }
}

std::vector<int> operator|(const std::vector<int> &a, const std::vector<int> &b) {
  std::vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] | b[i];
  }
  return c;
}

std::vector<int> operator&(const std::vector<int> &a, const std::vector<int> &b) {
  std::vector<int> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] & b[i];
  }
  return c;
}

std::istream &operator>>(std::istream &str, std::vector<double> &a) {
  int n;
  str >> n;
  a.clear();
  a = std::vector<double>(n);
  for (size_t i = 0; i < n; ++i) {
    double tmp;
    str >> tmp;
    a[i] = tmp;
  }
  return str;
}

std::ostream &operator<<(std::ostream &str, const std::vector<double> &a) {
  for (size_t i = 0; i < a.size(); ++i) {
    str << a[i] << ' ';
  }
  str << std::endl;
  return str;
}
}// namespace task