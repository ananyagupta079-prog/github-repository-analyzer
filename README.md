# GitHub Repository Analyzer

A C++ project that analyzes and ranks GitHub repositories based on their details, activity, and relevance to a selected role.

## Project Files

- [Main Program](main.cpp)
- [JSON Library](json.hpp)

## Features

- Analyze multiple GitHub repositories
- Fetch repository information using the GitHub API
- Support for C++, Web, and Data roles
- Check README and `.gitignore` files
- Analyze programming languages, topics, stars, and forks
- Count commits, branches, pull requests, and contributions
- Analyze source files and folder structure
- Calculate scores and rank repositories
- Display reasons for each repository's score

## Requirements

- C++ compiler
- `curl.exe`
- `json.hpp` (nlohmann JSON library)
- Internet connection

## How to Run

Compile the program:

```bash
g++ -std=c++11 main.cpp -o analyzer.exe
```

Run the program:

```bash
./analyzer.exe
```

Enter the number of repositories, their public GitHub URLs, and your target role (`cpp`, `web`, or `data`).

## Development Progress

This project is being developed step-by-step, with major development stages recorded through separate Git commits.
