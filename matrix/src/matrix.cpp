#include "matrix.h"
#include <cmath>
#include <stdexcept>

using namespace task;

Matrix::Matrix() : n_rows(1), n_cols(1), vals(new double[1]) {
  *vals = 1;
}

Matrix::Matrix(size_t row, size_t col) : n_rows(row), n_cols(col) {
  vals = new double[row * col];
  for (size_t i = 0; i < row; ++i) {
    for (size_t j = 0; j < col; ++j) {
      if (i == j) {
        vals[n_cols * i + j] = 1;
      } else {
        vals[n_cols * i + j] = 0;
      }
    }
  }
}

Matrix::~Matrix() {
  delete[] vals;
}

Matrix::Matrix(const Matrix &copy) : n_rows(copy.n_rows), n_cols(copy.n_cols) {
  vals = new double[n_rows * n_cols];
  for (size_t i = 0; i < n_rows * n_cols; ++i) {
    vals[i] = copy.vals[i];
  }
}

Matrix &Matrix::operator=(const Matrix &a) {
  if (this != &a) {
    (*this).resize(a.n_rows,a.n_cols);
    for (size_t i = 0; i < n_rows * n_cols; ++i) {
      vals[i] = a.vals[i];
    }
  }
  return *this;
}


double &Matrix::get(size_t row, size_t col) {
  if (row < 0 || col < 0 || row >= n_rows || col >= n_cols) {
    throw OutOfBoundsException();
  }
  return vals[n_cols * row + col];
}

const double &Matrix::get(size_t row, size_t col) const {
  if (row < 0 || col < 0 || row >= n_rows || col >= n_cols) {
    throw OutOfBoundsException();
  }
  return vals[n_cols * row + col];
}

void Matrix::set(size_t row, size_t col, const double &value) {
  if (row < 0 || col < 0 || row >= n_rows || col >= n_cols) {
    throw OutOfBoundsException();
  }
  vals[n_cols * row + col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {//проверить, работает неправильно
  double *new_vals;
  new_vals = new double[new_rows * new_cols];
  for (size_t i = 0; i < new_rows; ++i) {
    for (size_t j = 0; j < new_cols; ++j) {
      if (i < n_rows && j < n_cols) {
        new_vals[new_cols * i + j] = vals[n_cols * i + j];
      } else {
        new_vals[new_cols * i + j] = 0;
      }
    }
  }
  std::swap(vals, new_vals);
  n_rows = new_rows;
  n_cols = new_cols;
  delete[] new_vals;
}

double *Matrix::operator[](size_t row) {
  return &vals[row * n_cols];
}
const double *Matrix::operator[](size_t row) const {
  return &vals[row * n_cols];
}

Matrix &Matrix::operator+=(const Matrix &a) {
  if (n_rows != a.n_rows || n_cols != a.n_cols) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < n_rows * n_cols; ++i) {
    vals[i] += a.vals[i];
  }
  return *this;
}

Matrix &Matrix::operator-=(const Matrix &a) {
  if (n_rows != a.n_rows || n_cols != a.n_cols) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < n_rows * n_cols; ++i) {
    vals[i] -= a.vals[i];
  }
  return *this;
}

Matrix Matrix::operator*(const Matrix &a) const {
  if (n_cols != a.n_rows) {
    throw SizeMismatchException();
  }
  Matrix result(n_rows, a.n_cols);
  for (size_t i = 0; i < n_rows; ++i) {
    for (size_t j = 0; j < a.n_cols; ++j) {
      result.vals[a.n_cols * i + j] = 0.;
      for (size_t k = 0; k < n_cols; ++k) {
        result.vals[a.n_cols * i + j] += vals[n_cols * i + k] * a.vals[a.n_cols * k + j];
      }
    }
  }
  return result;
}
Matrix &Matrix::operator*=(const Matrix &a) {
  if (n_cols != a.n_rows) {
    throw SizeMismatchException();
  }
  *this = *this * a;
  return *this;
}

Matrix &Matrix::operator*=(const double &number) {
  for (size_t i = 0; i < n_rows * n_cols; ++i) {
    vals[i] *= number;
  }
  return *this;
}

Matrix Matrix::operator+(const Matrix &a) const {
  Matrix result = Matrix(*this);
  result += a;
  return result;
}

Matrix Matrix::operator-(const Matrix &a) const {
  Matrix result = Matrix(*this);
  result -= a;
  return result;
}

Matrix Matrix::operator*(const double &number) const {
  Matrix result = Matrix(*this);
  result *= number;
  return result;
}

Matrix Matrix::operator+() const {
  Matrix result = Matrix(*this);
  return result;
}

Matrix Matrix::operator-() const {
  Matrix result = Matrix(*this);
  for (size_t i = 0; i < n_rows * n_cols; ++i) {
    result.vals[i] *= -1;
  }
  return result;
}

bool Matrix::operator==(const Matrix &a) const {
  if (n_rows != a.n_rows || n_cols != a.n_cols) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < n_rows * n_cols; ++i) {
    if (fabs(vals[i] - a.vals[i]) > EPS) {
      return false;
    }
  }
  return true;
}
bool Matrix::operator!=(const Matrix &a) const {
  if (n_rows != a.n_rows || n_cols != a.n_cols) {
    throw SizeMismatchException();
  }
  for (size_t i = 0; i < n_rows * n_cols; ++i) {
    if (fabs(vals[i] - a.vals[i]) > EPS) {
      return true;
    }
  }
  return false;
}

std::vector<double> Matrix::getRow(size_t row) {
  std::vector<double> result(n_rows);
  for (size_t i = 0; i < n_cols; ++i) {
    result.push_back(vals[n_cols * row + i]);
  }
  return result;
}

std::vector<double> Matrix::getColumn(size_t column) {
  std::vector<double> result(n_rows);
  for (size_t i = 0; i < n_rows; ++i) {
    result.push_back(vals[n_cols * i + column]);
  }
  return result;
}

double Matrix::det() const {
  if (n_rows != n_cols) {
    throw SizeMismatchException();
  }
  Matrix m = Matrix(*this);
  for (size_t k = 0; k < n_rows - 1; ++k) {
    for (size_t i = k + 1; i < n_rows; ++i) {
      double tmp = -m[i][k] / m[k][k];
      for (int j = 0; j < n_rows; ++j) {
        m[i][j] += m[k][j] * tmp;
      }
    }
  }
  double res = 1.;
  for (size_t i = 0; i < n_rows; ++i) {
    res *= m[i][i];
  }
  return res;
}

void Matrix::transpose() {
  double *new_vals = new double[n_rows * n_cols];
  for (size_t i = 0; i < n_rows; ++i) {
    for (size_t j = 0; j < n_cols; ++j) {
      new_vals[j * n_rows + i] = vals[i * n_cols + j];//set(j,i,tmp);
    }
  }
  std::swap(new_vals, vals);
  std::swap(n_rows, n_cols);
  delete[] new_vals;
}

Matrix Matrix::transposed() const {
  Matrix result = Matrix(n_cols, n_rows);
  for (size_t i = 0; i < n_rows; ++i) {
    for (size_t j = 0; j < n_cols; ++j) {
      double tmp = (*this).get(i, j);
      result.set(j, i, tmp);
    }
  }
  return result;
}

double Matrix::trace() const {
  if (n_rows != n_cols) {
    throw SizeMismatchException();
  }
  double result = 0.;
  for (size_t i = 0; i < n_rows; ++i) {
    result += (*this).get(i, i);
  }
  return result;
}
double Matrix::rows() const {
  return n_rows;
}
double Matrix::cols() const {
  return n_cols;
}

std::ostream &task::operator<<(std::ostream &output, const Matrix &matrix) {
  for (size_t i = 0; i < matrix.rows(); ++i) {
    for (size_t j = 0; j < matrix.cols(); ++j) {
      output << matrix[i][j] << " ";
    }
    output << std::endl;
  }
  return output;
}

std::istream &task::operator>>(std::istream &input, Matrix &matrix) {
  int n, m;
  input >> n >> m;
  matrix.resize(n, m);
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      double tmp;
      input >> tmp;
      matrix.set(i, j, tmp);
    }
  }
  return input;
}

Matrix task::operator*(const double &a, const Matrix &b) {
  return b.operator*(a);
}


// Your code goes here...
