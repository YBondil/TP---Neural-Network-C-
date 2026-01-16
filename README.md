# End-term Project: Neural Network in C++

This project implements a simple Neural Network from scratch in C++, including a custom linear algebra library and a CSV data loader.

## How to Run the Project

* **Compile:** Run `make` in the terminal to compile the source files using `g++`.
* **Execute:** Run `./NN` to start the program.
* **Clean:** Run `make clean` to remove compiled objects and the executable.

## Libraries and Components

### 1. Maths Library (`maths.h`, `maths.cpp`)

This library provides a `Matrix` class to handle the linear algebra required for neural network operations.

**Key Matrix Methods:**

* **`Matrix(rows, cols)`**: Constructor that allocates memory for a matrix of a specific size.
* **`operator()(i, j)`**: Accesses elements with safety checks. It throws an `out_of_range` error if indices are invalid.
* **`operator*`**: Implements the dot product between two matrices. It checks if dimensions are compatible before calculation.
* **`hadamard(other)`**: Performs element-wise multiplication, which is essential for backpropagation.
* **`apply(std::function)`**: A high-level method that applies any mathematical function (like Sigmoid) to every element in the matrix.
* **`transpose()` / `transposed()`**: Methods to flip the matrix dimensions.
* **`randomize(min, max)`**: Uses `<random>` to fill the matrix with values, used to initialize weights and biases.

**Maths Namespace:**

* **`sigmoid(x)`**: Implements the standard activation function.
* **`sigmoid_prime(x)`**: The derivative of the sigmoid function, used during the training phase.

### 2. CSVReader Library (`csv_reader.h`, `csv_reader.cpp`)

This tool handles data ingestion, specifically for datasets like MNIST.

**Main Methods:**

* **`read(filename)`**: Parses a CSV file and returns a 2D vector of floats.
* **`readAsMatrix(filename)`**: A helper method that converts the CSV data directly into a `Matrix` object for immediate use in the network.

### 3. Neural Network

This component (currently in development) uses the `Maths` and `CSVReader` libraries to:

* Define the architecture (Layers, Neurons).
* Perform **Forward Propagation** to make predictions.
* Perform **Backpropagation** to update weights and reduce error.
