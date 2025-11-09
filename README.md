# 🧮 Math-Information-Tools

<div align="center">

**A comprehensive collection of mathematical and computational algorithms implemented in C++**

[![Language](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Made with Love](https://img.shields.io/badge/Made%20with-❤️-red.svg)](https://github.com/capkimkhanh2k5)

</div>

---

## 📋 Table of Contents

- [Introduction](#-introduction)
- [Project Structure](#-project-structure)
- [Features & Algorithms](#-features--algorithms)
- [Technologies](#-technologies)
- [Installation & Usage](#-installation--usage)
- [Contributing](#-contributing)
- [Author](#-author)

---

## 🎯 Introduction

**Math-Information-Tools** is a comprehensive collection of mathematical and computational algorithms implemented in C++. This project serves as both a learning resource and a practical toolkit, covering topics ranging from basic number theory, linear algebra, computational geometry to numerical optimization methods.

Each subdirectory (`Chap1`, `Chap2`, etc.) focuses on a specific mathematical domain, providing clear and ready-to-compile implementations.

---

## 📁 Project Structure

```
Math-Information-Tools/
│
├── 📂 Assignment1/          # Assignment 1 - Numerical Optimization
├── 📂 Chap1/                # Number Theory
├── 📂 Chap2/                # Linear Algebra
├── 📂 Chap3/                # Computational Geometry - Convex Hull
├── 📂 Chap4/                # Numerical Optimization
└── 📄 README.md
```

---

## ✨ Features & Algorithms

### 🔢 Chapter 1: Number Theory

A comprehensive toolkit for fundamental number theory problems.

| Algorithm | Description |
|-----------|-------------|
| **Prime Factorization** | Decompose an integer `n` into prime factors |
| **Sieve of Eratosthenes** | Find all primes from 2 to `n` and sieve on interval [a, b] |
| **Divisors** | List all divisors, calculate sum and product of divisors |
| **Perfect Numbers** | Check if a number is perfect |
| **GCD & LCM** | Find greatest common divisor and least common multiple |
| **Chinese Remainder Theorem** | Solve system of congruence equations |

### 🔷 Chapter 2: Linear Algebra

Implementation of matrix decomposition methods.

#### Cholesky Decomposition
- Decompose a symmetric, positive-definite matrix `A` into `L * L^T`
- Includes functions to verify symmetry and positive-definiteness

#### Singular Value Decomposition (SVD)
- Decompose matrix `A` into `U * Σ * V^T`
- **Note**: Requires the Eigen library

### 📐 Chapter 3: Computational Geometry

Focus on **Convex Hull** problems for a set of points.

#### Convex Hull Algorithms
- ✅ **Jarvis March** (Gift Wrapping)
- ✅ **Monotone Chain** (Andrew's Algorithm)
- ✅ **Chan's Algorithm**
- ✅ **Divide and Conquer**

#### Utility Functions
- Calculate polygon area (convex hull)
- Find closest pair of points
- Find shortest edge of convex hull
- Check if a point is inside the convex hull

### 📊 Chapter 4 & Assignment 1: Numerical Optimization

Implementation of iterative algorithms to find function minima.

| Method | Description |
|--------|-------------|
| **Gradient Descent (GD)** | Basic gradient descent method |
| **GD with Momentum** | Enhanced GD with momentum for faster convergence |
| **Newton's Method** | Uses second derivatives (Hessian matrix), diagonal approximation version |
| **Augmented Lagrangian Method (ALM)** | Powerful method for constrained optimization (e.g., `min f(x)` subject to `h(x) = 0`) |

---

## 🛠️ Technologies

- ![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white) **C++**: Primary programming language
- **STL**: Extensive use of `vector`, `iostream`, `cmath`, etc.
- **Eigen**: External C++ library for linear algebra (specifically for SVD)

---

## 🚀 Installation & Usage

### Prerequisites

1. **C++ Compiler** (e.g., `g++`, `clang++`, `MSVC`)
2. **Eigen Library** (Required for `Chap2/SVD.cpp`)
   - Download from [Eigen official website](https://eigen.tuxfamily.org)
   - Eigen is header-only, no complex installation needed

### Compilation

#### Standard Compilation (Without Eigen)

```bash
# Navigate to project directory
cd Math-Information-Tools

# Compile a file (example: GCD_LCM.cpp)
g++ Chap1/GCD_LCM.cpp -o GCD_LCM

# Run the executable
./GCD_LCM
```

#### Compilation with Eigen

```bash
# Compile SVD.cpp with Eigen library
# Replace /path/to/eigen with your actual Eigen directory path
g++ -I /path/to/eigen Chap2/SVD.cpp -o SVD

# Run the executable
./SVD
```

**Example (Windows):**
```bash
g++ -I C:/Users/YourName/eigen Chap2/SVD.cpp -o SVD.exe
```

### Running Examples

```bash
# Number Theory Example
./GCD_LCM

# Linear Algebra Example
./SVD

# Computational Geometry Example
./ConvexHull

# Optimization Example
./GradientDescent
```

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 👨‍💻 Author

**capkimkhanh2k5**

- GitHub: [@capkimkhanh2k5](https://github.com/capkimkhanh2k5)

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

<div align="center">

**⭐ If you find this project helpful, please consider giving it a star! ⭐**

Made with ❤️ by capkimkhanh2k5

</div>
