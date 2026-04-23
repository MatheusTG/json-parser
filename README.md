![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)
![Project](https://img.shields.io/badge/Project-JSON%20Parser-6f42c1?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-in%20development-darkblue?style=for-the-badge)

# json-parser

A lightweight JSON parser written in C, designed for educational purposes and focused on understanding low-level concepts such as memory management, pointers, and recursive data structures.

---

## 📁 Project Structure

```bash
json-parser/
│
├── Makefile
├── src/
│   ├── main.c
│   ├── json.c
│   ├── parser.c
│   ├── io.c
│   └── *.h
│
└── examples/
    ├── simple.json
    ├── nested.json
    └── array.json
```

---

## ⚙️ Build

This project uses a simple Makefile.

To compile:
```bash
make
```

This will generate the executable:
```bash
bin/json-parser
```

---

## ▶️ Run

You can run the program by passing a JSON file:

```bash
./bin/json-parser examples/simple.json
```

---

## 🧪 Example Files

The `examples/` directory contains sample JSON files used for testing:

- `simple.json` → basic key-value pair  
- `nested.json` → nested objects (tests recursion)  
- `array.json` → arrays and multiple values  

These files are useful to validate the parser behavior and ensure correctness across different JSON structures.

---

## Internal Representation

The JSON is converted into a tree structure using JsonValue as the main node.

Example for the JSON:

```json
{"name": "Matheus"}
```

Example for the JSON:

```bash
root (JsonValue)
 ├── type = JSON_OBJECT
 └── object
      ├── count = 1
      └── pairs[0]
           ├── key = "name"
           └── value (JsonValue)
                ├── type = JSON_STRING
                └── string = "Matheus"
```

---

## 🚀 Features

- Supports JSON objects  
- Supports strings  
- Dynamic memory allocation  
- Recursive parsing approach  

---

## 📌 Planned Improvements

- [ ] Full JSON specification support  
- [ ] Number parsing  
- [ ] Boolean and null support  
- [ ] Error handling and validation  
- [ ] `json_free` for memory cleanup  
- [ ] Pretty printing (`json_print`)  

---

## 🎯 Purpose

This project is intended for educational purposes, aiming to:

- Understand how parsers work internally  
- Practice manual memory management in C  
- Build complex data structures from scratch  

---

## ⚠️ Notes

- This is not a production-ready JSON parser  
- Memory management must be handled carefully (`malloc` / `free`)  
- Input JSON must be valid (no full validation implemented yet)

---

## 🤝 Contributing

Contributions are welcome and appreciated!

If you want to contribute to this project, please follow the steps below:

1. Fork the repository
2. Create a new branch (`git checkout -b feat/your-feature-name`)
3. Make your changes
4. Commit your changes following the Conventional Commits standard
5. Push your branch (`git push origin feat/your-feature-name`)
6. Open a Pull Request

---

### Commit Convention

This project follows the Conventional Commits specification:

- `feat`: A new feature
- `fix`: A bug fix
- `refactor`: Code refactoring without behavior change
- `test`: Adding or updating tests
- `chore`: Maintenance tasks and tooling changes
- `docs`: Documentation changes

Please make sure your code is well-tested and follows the existing project structure and linting rules.

---

## 📄 License

This project is licensed under the MIT License.

You are free to use, modify, and distribute this software, provided that the original copyright
and license notice are included in all copies or substantial portions of the software.

<img src="https://github.com/MatheusTG/MatheusTG/blob/main/images/banner.svg" />