# Text Conversion Builder Pattern Implementation

This project demonstrates the Builder Design Pattern through a text conversion system implemented in both C++ and Java. The system processes RTF-like input and converts it into different output formats.

## 🏗️ Builder Pattern Overview
<div align="center">
    <img src="./assets/Builder.png" alt="Builder Pattern UML">
</div>

The Builder Pattern is used to construct complex objects step by step. In this implementation:
- `TextConverter` serves as the abstract builder
- Concrete builders (`ASCIIConverter`, `TeXConverter`, `TextWidgetConverter`) create specific representations
- `RTFReader` acts as the director that constructs the object using the builder interface

## 📚 Visual Example

<div align="center">
    <img src="./assets/Robots.jpg" alt="Robot Builder Example">
</div>

Just like building a robot piece by piece, our text converter builds the final output format step by step!


## 🚀 Features

- Converts RTF-like text to multiple formats:
    - ASCII (plain text)
    - TeX (with formatting commands)
    - Text Widget (custom format)
- Supports text styling:
    - Bold
    - Italic
    - Underline
- Handles paragraph breaks
- Implemented in both C++ and Java

## 📁 Project Structure

```plaintext
├── Python/
│   └── app.py
├── C++/
│   └── main.cpp
└── Java/
    ├── Main.java
    └── rtf/
        ├── ASCIIConverter.java
        ├── FontStyle.java
        ├── RTFReader.java
        ├── TeXConverter.java
        ├── TextConverter.java
        └── TextWidgetConverter.java
```

## 🔍 Implementation Details

### Python Implementation [`Source`](./Python)
- Single file script for simplicity
- Uses classes to define `TextConverter` and concrete converters
- Easy-to-read and maintainable code

### C++ Implementation [`Source`](./C++)
- Single file modular design with smart pointers
- Implements `TextConverter`, `RTFReader`, and concrete converters
- Memory-safe RAII approach

### Java Implementation [`Source`](./Java)
- Multi-file OOP design with `rtf` package structure
- Uses `StringBuilder` for string operations
- Interfaces for `TextConverter` with three concrete implementations

## 🛠️ How to Run

### Python Version
1. Navigate to the `Python` directory.
2. Run the script using Python, e.g., `python app.py`.

### C++ Version
1. Navigate to the `C++` directory.
2. Compile the code using a C++ compiler, e.g., `g++ main.cpp -o TextConverter`.
3. Run the executable, e.g., `./TextConverter`.

### Java Version
1. Navigate to the `Java` directory.
2. Compile the code using `javac`, e.g., `javac Main.java`.
3. Run the program, e.g., `java Main`.

## 📚 Design Pattern Benefits

- **Separation of Concerns**: Construction logic is separated from representation.
- **Fine-grained Control**: Step-by-step construction of text formatting.
- **Multiple Representations**: Same construction process creates different text formats.
- **Single Responsibility**: Each converter handles one specific format.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
