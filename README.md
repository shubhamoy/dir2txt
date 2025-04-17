# dir2txt
![logo](logo.jpg)
A blazing-fast CLI tool to export a directory's structure and contents into a neatly formatted `.txt` or `.json` file.

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

---

## 🔍 Features

- 📁 Tree view of folders and files
- 📄 Dump file contents with optional comment stripping
- ⚙️ Honors `.gitignore`, `.dockerignore`, `.npmignore`, `.dir2txtignore`
- 🧠 Smart binary file detection (skips them)
- 🧾 Outputs to `.txt` or `.json` with timestamp
- 💬 Emoji-rich formatting, helpful error messages

---

## 🖼️ Screenshot

![screenshot](screenshot_dir2txt.png)

---

## 🛠️ Usage

```bash
dir2txt ./my-project \
  --ignore node_modules,build \
  --strip-comments \
  --include-dotfiles \
  --json
```
---

## 🛠️ Build Instructions

✅ Prerequisites
```
A C++20 compatible compiler:
- macOS: Apple Clang 14+ or g++-13 via Homebrew
- Linux: g++ 10 or newer
CMake 3.16 or higher
git (to clone the repo)
```
### 🔧 Build (macOS)

```bash
git clone https://github.com/shubhamoy/dir2txt.git
cd dir2txt
mkdir build && cd build
cmake ..
cmake --build .
./dir2txt
```