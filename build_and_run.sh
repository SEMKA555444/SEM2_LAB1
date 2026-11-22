#!/usr/bin/env bash
set -e

echo "=== Сборка проекта (C++ & Rust) на Linux ==="

# 1. Сборка C++ СУБД
echo "[1/3] Компиляция C++ dbms..."
if command -v make >/dev/null 2>&1; then
    make dbms
else
    g++ -std=c++17 -Wall -Wextra -O2 main.cpp -o dbms
fi

# 2. Сборка Rust СУБД
echo "[2/3] Компиляция Rust dbms_rust..."
if command -v cargo >/dev/null 2>&1; then
    (cd rust && cargo build --release)
    cp rust/target/release/dbms_rust ./dbms_rust 2>/dev/null || true
else
    echo "Предупреждение: cargo не найден в PATH. Пропуск сборки Rust."
fi

# 3. Демонстрационный запуск
echo "[3/3] Тестирование C++ версии..."
./dbms --file demo.data --query "SPUSH mystack 10"
./dbms --file demo.data --query "SPUSH mystack hello"
./dbms --file demo.data --query "SPOP mystack"
./dbms --file demo.data --query "QPUSH myqueue 100"
./dbms --file demo.data --query "QPUSH myqueue element"
./dbms --file demo.data --query "QPOP myqueue"
./dbms --file demo.data --query "TINSERT mytree 1"
./dbms --file demo.data --query "TINSERT mytree alpha"
./dbms --file demo.data --query "TINSERT mytree 3.5"
./dbms --file demo.data --query "TINSERT mytree beta"
./dbms --file demo.data --query "TINSERT mytree 5"
./dbms --file demo.data --query "TISCOMPLETE mytree"
./dbms --file demo.data --query "ISMEMBER mytree alpha"
./dbms --file demo.data --query "ISMEMBER mytree 3.5"
./dbms --file demo.data --query "PRINT mytree"

if [ -f "./dbms_rust" ]; then
    echo "=== Тестирование Rust версии (чтение того же файла demo.data) ==="
    ./dbms_rust --file demo.data --query "PRINT mytree"
    ./dbms_rust --file demo.data --query "TGET mytree BFS"
fi

rm -f demo.data
echo "=== Все проверки успешно пройдены! ==="
