# g-lib

`g-lib` is a small C++ shared library to hold some of my shortcut logic intended for use on **macOS** and **Linux**.

It builds as:

- `g-lib.dylib` on macOS  
- `g-lib.so` on Linux  

and installs headers under:

- `/usr/local/include/g-lib/`

so you can easily link it from other projects.

---

## Project Layout

```text
g-lib/
├── include/g-lib
│          └── ... public namespaces/headers ...
├── src/
│   └── ... .cpp namespaces/impl files ...
├── tests/
│   └── ... integration test files ...
├── Makefile
└── README.md
```

## Build
```bash
make
```

## Install
```bash
make install
```

## Clean
```bash
make clean
```

## Run Test Suite
```bash
make test
```