# g-lib

`g-lib` is a small C++ shared library to hold some of my shortcut logic and brush up on data structures and algorithms intended for use on **macOS** and **Linux**.

If you use the library, just note the std equivalent are generally faster for most data structures, so it's not recommended to use this for production code that needs performance.

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
├── benchmarks/
│   └── ... per data-structure benchmarks vs std equivalents ...
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

## Run Benchmarks

Each data structure has a dedicated benchmark that compares it against its `std` equivalent. Run them all from the `benchmarks/` directory:

```bash
cd benchmarks && make bench
```

Or build and run a single benchmark:

```bash
cd benchmarks && make BenchHashMap && ./BenchHashMap
```

---

## Using g-lib in Your Project

After running `make install`, link against `g-lib` using `-lg-lib` and point your compiler at the installed headers if they're not in your system include path.

### Compiling directly with g++

```bash
g++ -std=c++17 main.cpp -o my_app \
    -I/usr/local/include \
    -L/usr/local/lib -lg-lib \
    -Wl,-rpath=/usr/local/lib
```

### Example Makefile

```makefile
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
PREFIX   ?= /usr/local

my_app: main.cpp
	$(CXX) $(CXXFLAGS) \
	    -I$(PREFIX)/include \
	    $< -o $@ \
	    -L$(PREFIX)/lib -lg-lib -Wl,-rpath=$(PREFIX)/lib
```

### Example source

```cpp
#include <g-lib/ds/SQueue.h>
#include <g-lib/ds/DQueue.h>
#include <g-lib/util/RNG.h>
#include <g-lib/util/Stopwatch.h>

int main() {
    GLib::DS::SQueue<int> queue;
    queue.Push(1);
    queue.Push(2);
    int val = queue.Pop();

    GLib::Util::RNG<int> rng(1, 100);
    rng.Fill(50);
    int r = rng.Next();

    GLib::Util::Stopwatch sw("timer");
    sw.Start();
    auto ns = sw.Current();
    auto ms = sw.Current<std::chrono::milliseconds>();
}
```

> **Custom install prefix** — if you installed to a non-default location, substitute that path for `/usr/local` everywhere above, or pass `PREFIX=/your/path` to the example Makefile.
