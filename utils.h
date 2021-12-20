//Daniel Thai
//151900198

#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

template<typename T>
class Stack
{
    struct Node
    {
        T _data;
        Node* _next;
        Node()
        {
            _next = nullptr;
        }
        Node(const T& data) : Node()
        {
            _data = T{data};
        }
        void setNext(Node* next)
        {
            _next = next;
        }
    };
    Node* _top;
    Node* _bottom;
    size_t _size;
public:
    Stack();
    ~Stack();
    void push(const T& data);
    void pop();
    const T& top() const;
    size_t size() const;
};


/*Implementations*/

template<typename T>
Stack<T>::Stack()
{
    _top = nullptr;
    _bottom = nullptr;
    _size = 0;
}


template<typename T>
Stack<T>::~Stack()
{
    if (_size > 0) {
        Node* nextToDel;
        Node* delNode = _top;

        while (delNode) {
            nextToDel = delNode->_next;
            delete delNode;
            delNode = nextToDel;
        }
    }
}


template<typename T>
void Stack<T>::push(const T& data)
{
    if (_size == 0) {
        _top = new Node(data);
        _bottom = _top;
        _size = 1;
    } else {
        Node* oldTop = _top;
        _top = new Node(data);
        _top->setNext(oldTop);
        _size++;
    }
}


template<typename T>
void Stack<T>::pop()
{
    if (_size == 0) {
        return;
    } else if (_size == 1) {
        delete _top;
        _top = nullptr;
        _bottom = nullptr;
        _size--;
    } else {
        Node* newTop = _top->_next;
        delete _top;
        _top = newTop;
        _size--;
    }
}


template<typename T>
const T& Stack<T>::top() const
{
    return _top->_data;
}


template<typename T>
size_t Stack<T>::size() const
{
    return _size;
}

template <typename T>
void filter(std::vector<T> &vec, std::function<bool(const T &)> f) {
  for (auto it = vec.begin(); it != vec.end();) {
    if (!f(*it)) {
      vec.erase();
    } else {
      ++it;
    }
  }
}

template <typename T> T &max(T &a, T &b) {
  if (a >= b)
    return a;
  else
    return b;
}

template <typename T> T min(T a, T b) {
  if (a <= b)
    return a;
  else
    return b;
}

///Deepcopies two vectors
template<typename T>
void deepCopy(std::vector<T*>& a, std::vector<T*>& b, size_t len)
{
    for (auto l : a) {
        b.push_back(new T[len]);
        for (auto j = 0u; j < len; j++) {
            b.back()[j] = l[j];
        }
    }
}

///Parses a number block
std::vector<short *> parseBlock(std::ifstream &f, size_t &len) {
  std::vector<short *> data;
  short *nline;
  std::string line;

  std::getline(f, line);
  size_t s = line.size();
  nline = new short[s];
  for (auto i = 0u; i < s; i++) {
    if (line[i] != '\0')
      nline[i] = line[i] - '0';
  }
  data.push_back(nline);

  while (std::getline(f, line)) {
    nline = new short[s];
    for (auto i = 0u; i < s; i++) {
      if (line[i] != '\0')
        nline[i] = line[i] - '0';
    }
    data.push_back(nline);
  }

  len = s;
  return data;
}


enum Rotation {
  ROT_0,
  ROT_90,
  ROT_180,
  ROT_270
};


struct Coord {
  int x;
  int y;
  int z;

  Coord(int a, int b, int c) {
    x = a;
    y = b;
    z = c;
  }

  void set(int a, int b, int c) {
    x = a;
    y = b;
    z = c;
  }

  void print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")";
  }

  bool operator==(const Coord& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }

  bool operator<(const Coord& rhs) const {
    if (x != rhs.x)
      return x < rhs.x;

    if (y != rhs.y)
      return y < rhs.y;

    return x < rhs.z;
    //return (abs(x) + abs(y) + abs(z)) < (abs(rhs.x) + abs(rhs.y) + abs(rhs.z));
  }
};


struct Matrix {
  int mat[3][3] = {0};
  bool operator==(const Matrix& rhs) {
    bool equal = true;
    for (int i = 0; i < 3 && equal; i++) {
      for (int j = 0; j < 3 && equal; j++) {
        equal = this->mat[i][j] == rhs.mat[i][j];
      }
    }

    return equal;
  }
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
  Matrix result;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result.mat[i][j] = lhs.mat[i][0]*rhs.mat[0][j] + lhs.mat[i][1]*rhs.mat[1][j] + lhs.mat[i][2]*rhs.mat[2][j];
    }
  }

  return result;
}


Coord operator*(const Matrix& matrix, const Coord& point) {
  int a = matrix.mat[0][0]*point.x + matrix.mat[0][1]*point.y + matrix.mat[0][2]*point.z;
  int b = matrix.mat[1][0]*point.x + matrix.mat[1][1]*point.y + matrix.mat[1][2]*point.z;
  int c = matrix.mat[2][0]*point.x + matrix.mat[2][1]*point.y + matrix.mat[2][2]*point.z;

  return Coord{a, b, c};
}


struct RotMatrix {
  Matrix matrix;
  Rotation rot_x;
  Rotation rot_y;
  Rotation rot_z;

  RotMatrix(Rotation r_x = ROT_0, Rotation r_y = ROT_0, Rotation r_z = ROT_0) {
    Matrix x, y, z;

    x.mat[0][0] = 1;
    y.mat[1][1] = 1;
    z.mat[2][2] = 1;

    //Generate x matrix
    if (r_x == ROT_0) {
      for (int i = 1; i < 3; i++) {
        x.mat[i][i] = 1;
      }
    } else if (r_x == ROT_90) {
      x.mat[2][1] = 1;
      x.mat[1][2] = -1;
    } else if (r_x == ROT_180) {
      for (int i = 1; i < 3; i++) {
        x.mat[i][i] = -1;
      }
    } else if (r_x == ROT_270) {
      x.mat[2][1] = -1;
      x.mat[1][2] = 1;
    }

    //Generate y matrix
    if (r_y == ROT_0) {
      y.mat[0][0] = 1;
      y.mat[2][2] = 1;
    } else if (r_y == ROT_90) {
      y.mat[0][2] = 1;
      y.mat[2][0] = -1;
    } else if (r_y == ROT_180) {
      y.mat[0][0] = -1;
      y.mat[2][2] = -1;
    } else if (r_y == ROT_270) {
      y.mat[0][2] = -1;
      y.mat[2][0] = 1;
    }

    //Generate z matrix
    if (r_z == ROT_0) {
      for (int i = 0; i < 2; i++) {
        z.mat[i][i] = 1;
      }
    } else if (r_z == ROT_90) {
      z.mat[0][1] = -1;
      z.mat[1][0] = 1;
    } else if (r_z == ROT_180) {
      for (int i = 0; i < 2; i++) {
        z.mat[i][i] = -1;
      }
    } else if (r_z == ROT_270) {
      z.mat[0][1] = 1;
      z.mat[1][0] = -1;
    }

    rot_x = r_x;
    rot_y = r_y;
    rot_z = r_z;

    matrix = x*y*z;
  }
  
  bool operator==(const RotMatrix& rhs) {
    return this->matrix == rhs.matrix;
  }

  bool operator!=(const RotMatrix& rhs) {
    return !(this->matrix == rhs.matrix);
  }
};

std::vector<RotMatrix> generateRotations() {
  std::vector<RotMatrix> rotations;

  rotations.push_back(RotMatrix(ROT_0, ROT_0, ROT_0));
  rotations.push_back(RotMatrix(ROT_0, ROT_90, ROT_0));
  rotations.push_back(RotMatrix(ROT_0, ROT_0, ROT_90));
  rotations.push_back(RotMatrix(ROT_0, ROT_180, ROT_0));
  rotations.push_back(RotMatrix(ROT_0, ROT_0, ROT_180));
  rotations.push_back(RotMatrix(ROT_0, ROT_270, ROT_0));
  rotations.push_back(RotMatrix(ROT_0, ROT_0, ROT_270));
  rotations.push_back(RotMatrix(ROT_0, ROT_90, ROT_90));
  rotations.push_back(RotMatrix(ROT_0, ROT_90, ROT_180));
  rotations.push_back(RotMatrix(ROT_0, ROT_90, ROT_270));
  rotations.push_back(RotMatrix(ROT_0, ROT_180, ROT_90));
  rotations.push_back(RotMatrix(ROT_0, ROT_180, ROT_180));
  rotations.push_back(RotMatrix(ROT_0, ROT_180, ROT_270));
  rotations.push_back(RotMatrix(ROT_0, ROT_270, ROT_90));
  rotations.push_back(RotMatrix(ROT_0, ROT_270, ROT_180));
  rotations.push_back(RotMatrix(ROT_0, ROT_270, ROT_270));
  rotations.push_back(RotMatrix(ROT_90, ROT_0, ROT_0));
  rotations.push_back(RotMatrix(ROT_90, ROT_90, ROT_0));
  rotations.push_back(RotMatrix(ROT_90, ROT_0, ROT_90));
  rotations.push_back(RotMatrix(ROT_90, ROT_180, ROT_0));
  rotations.push_back(RotMatrix(ROT_90, ROT_0, ROT_180));
  rotations.push_back(RotMatrix(ROT_90, ROT_270, ROT_0));
  rotations.push_back(RotMatrix(ROT_90, ROT_0, ROT_270));
  rotations.push_back(RotMatrix(ROT_90, ROT_90, ROT_90));
  rotations.push_back(RotMatrix(ROT_90, ROT_90, ROT_180));
  rotations.push_back(RotMatrix(ROT_90, ROT_90, ROT_270));
  rotations.push_back(RotMatrix(ROT_90, ROT_180, ROT_90));
  rotations.push_back(RotMatrix(ROT_90, ROT_180, ROT_180));
  rotations.push_back(RotMatrix(ROT_90, ROT_180, ROT_270));
  rotations.push_back(RotMatrix(ROT_90, ROT_270, ROT_90));
  rotations.push_back(RotMatrix(ROT_90, ROT_270, ROT_180));
  rotations.push_back(RotMatrix(ROT_90, ROT_270, ROT_270));
  rotations.push_back(RotMatrix(ROT_180, ROT_0, ROT_0));
  rotations.push_back(RotMatrix(ROT_180, ROT_90, ROT_0));
  rotations.push_back(RotMatrix(ROT_180, ROT_0, ROT_90));
  rotations.push_back(RotMatrix(ROT_180, ROT_180, ROT_0));
  rotations.push_back(RotMatrix(ROT_180, ROT_0, ROT_180));
  rotations.push_back(RotMatrix(ROT_180, ROT_270, ROT_0));
  rotations.push_back(RotMatrix(ROT_180, ROT_0, ROT_270));
  rotations.push_back(RotMatrix(ROT_180, ROT_90, ROT_90));
  rotations.push_back(RotMatrix(ROT_180, ROT_90, ROT_180));
  rotations.push_back(RotMatrix(ROT_180, ROT_90, ROT_270));
  rotations.push_back(RotMatrix(ROT_180, ROT_180, ROT_90));
  rotations.push_back(RotMatrix(ROT_180, ROT_180, ROT_180));
  rotations.push_back(RotMatrix(ROT_180, ROT_180, ROT_270));
  rotations.push_back(RotMatrix(ROT_180, ROT_270, ROT_90));
  rotations.push_back(RotMatrix(ROT_180, ROT_270, ROT_180));
  rotations.push_back(RotMatrix(ROT_180, ROT_270, ROT_270));
  rotations.push_back(RotMatrix(ROT_270, ROT_0, ROT_0));
  rotations.push_back(RotMatrix(ROT_270, ROT_90, ROT_0));
  rotations.push_back(RotMatrix(ROT_270, ROT_0, ROT_90));
  rotations.push_back(RotMatrix(ROT_270, ROT_180, ROT_0));
  rotations.push_back(RotMatrix(ROT_270, ROT_0, ROT_180));
  rotations.push_back(RotMatrix(ROT_270, ROT_270, ROT_0));
  rotations.push_back(RotMatrix(ROT_270, ROT_0, ROT_270));
  rotations.push_back(RotMatrix(ROT_270, ROT_90, ROT_90));
  rotations.push_back(RotMatrix(ROT_270, ROT_90, ROT_180));
  rotations.push_back(RotMatrix(ROT_270, ROT_90, ROT_270));
  rotations.push_back(RotMatrix(ROT_270, ROT_180, ROT_90));
  rotations.push_back(RotMatrix(ROT_270, ROT_180, ROT_180));
  rotations.push_back(RotMatrix(ROT_270, ROT_180, ROT_270));
  rotations.push_back(RotMatrix(ROT_270, ROT_270, ROT_90));
  rotations.push_back(RotMatrix(ROT_270, ROT_270, ROT_180));
  rotations.push_back(RotMatrix(ROT_270, ROT_270, ROT_270));

  RotMatrix temp;
  for (auto i = 0u; i < rotations.size(); i++) {
    temp = rotations[i];
    rotations.erase(std::remove_if(rotations.begin()+i+1, rotations.end(), [&](RotMatrix& rot){ return temp == rot ;}), rotations.end());
  }

  std::cout << rotations.size() << std::endl;
  return rotations;
}
#endif