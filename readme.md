# 🔴 Mini-Redis  
> A lightweight in-memory key-value store based on **LRU(Least Recently Used) Cache implementation** built with **C++ (Crow)** and a simple **Web UI**.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Crow](https://img.shields.io/badge/Framework-Crow-green)
![Frontend](https://img.shields.io/badge/Frontend-HTML%20%7C%20CSS%20%7C%20JS-orange)

---

## 📌 Overview

Mini-Redis is a simplified Redis-like key-value store implemented in C++ using the Crow web framework.  
It supports basic CRUD operations and optional TTL (Time-To-Live) for automatic key expiration.
It is implemented based on the LRU cache system(Data Strucuture Used: Doubly linked list) where the most commonly accessed key is stored first and the least recently used is stored at the end or removed from the system depending on the total allowed keys.

---

## ✨ Features

- ✅ Set key-value pairs
- ✅ Optional TTL support
- ✅ Retrieve value by key
- ✅ List all keys
- ✅ Lightweight and fast
- ✅ Clean Web UI

---

## 🏗️ Architecture

Frontend (HTML/JS)  
        ↓ Fetch API  
Crow REST Server (C++)  
        ↓  
In-memory unordered_map  

---

## 📂 Basic Project Structure

```
mini-redis/
│
├── src/
│   ├── lru_cache.cpp
│   ├── main.cpp
│   └── test_lru.cpp
├── frontend/
│   ├── index.html
│   ├── script.js
│   └── styles.css
└── README.md
```

---
## Prerequisites

- CMake ≥ 3.10
- vcpkg with crow and asio installed
- A C++17-capable compiler (MSVC, GCC, Clang)

- Install dependencies via vcpkg if not: vcpkg install crow asio

---

## 🚀 Getting Started

### 1️⃣ Clone Repository

```bash
git clone https://github.com/KarnaPratik/mini-redis
cd mini-redis
```

---

### 2️⃣ Run Backend Server

Compile:

```bash
g++ main.cpp lru_cache.cpp -I../include -I"path_to/mini-redis/asio-1.36.0/include" -o server.exe -std=c++17 -pthread
```

Run:

```bash
./server
```

Server runs at:

```
http://localhost:18080
```

---

### 3️⃣ Run Frontend

Start a local HTTP server:

```bash
python -m http.server 5500
```

Open in browser:

```
http://localhost:5500/frontend/
```

⚠️ Do NOT open `index.html` directly using `file://`.

---

## 📡 API Documentation

### 🔹 Set Key

**POST** `/set`

```json
{
  "key": "name",
  "value": "Pratik",
  "ttl": 5
}
```

Response:
```
OK
```

---

### 🔹 Get Key

**GET** `/get?key=name`

Response:
```
Pratik
```

Returns `404` if key not found.

---

### 🔹 Get All Keys

**GET** `/keys`

Response:
```json
["name", "age"]
```

---

## ⏳ TTL Mechanism

If a TTL value is provided:
- Key expires after given seconds
- Expired keys are removed during access

---

## 🛠 Technologies Used

- C++17
- Crow Web Framework
- STL (unordered_map, chrono)
- HTML5
- CSS3
- JavaScript (Fetch API)

---



