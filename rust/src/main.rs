#![allow(dead_code)]

mod array;
mod one_linked_list;
mod doubly_linked_list;
mod stack;
mod queue;
mod complete_binary_tree;

use std::cmp::Ordering;
use std::collections::HashMap;
use std::env;
use std::fmt;
use std::fs::{File, OpenOptions};
use std::io::{self, BufRead, BufReader, Write};

use array::Array;
use one_linked_list::OneLinkedList;
use doubly_linked_list::DoublyLinkedList;
use stack::Stack;
use queue::Queue;
use complete_binary_tree::CompleteBinaryTree;

const EPS: f32 = 1e-6;

#[derive(Debug, Clone)]
pub enum Value {
    Number(f32),
    Text(String),
}

impl Value {
    pub fn parse(s: &str) -> Self {
        if let Ok(v) = s.parse::<f32>() {
            if v.is_finite() {
                return Value::Number(v);
            }
        }
        Value::Text(s.to_string())
    }

    pub fn format_for_file(&self) -> String {
        match self {
            Value::Number(v) => format!("{}", v),
            Value::Text(s) => format_token(s),
        }
    }
}

impl PartialEq for Value {
    fn eq(&self, other: &Self) -> bool {
        match (self, other) {
            (Value::Number(a), Value::Number(b)) => (a - b).abs() < EPS,
            (Value::Text(a), Value::Text(b)) => a == b,
            _ => false,
        }
    }
}

impl PartialOrd for Value {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        match (self, other) {
            (Value::Number(a), Value::Number(b)) => a.partial_cmp(b),
            (Value::Text(a), Value::Text(b)) => Some(a.cmp(b)),
            (Value::Number(_), Value::Text(_)) => Some(Ordering::Less),
            (Value::Text(_), Value::Number(_)) => Some(Ordering::Greater),
        }
    }
}

impl fmt::Display for Value {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Value::Number(v) => write!(f, "{}", v),
            Value::Text(s) => write!(f, "{}", s),
        }
    }
}

fn tokenize(s: &str) -> Vec<String> {
    let mut tokens = Vec::new();
    let mut current = String::new();
    let mut in_token = false;
    let mut in_quotes = false;
    let mut quote_char = '\0';

    let mut chars = s.chars();
    while let Some(c) = chars.next() {
        if in_quotes {
            if c == '\\' {
                if let Some(next_c) = chars.next() {
                    current.push(next_c);
                }
            } else if c == quote_char {
                in_quotes = false;
            } else {
                current.push(c);
            }
        } else if c.is_whitespace() {
            if in_token {
                tokens.push(current.clone());
                current.clear();
                in_token = false;
            }
        } else if c == '"' || c == '\'' {
            in_quotes = true;
            quote_char = c;
            in_token = true;
        } else {
            current.push(c);
            in_token = true;
        }
    }

    if in_token {
        tokens.push(current);
    }
    tokens
}

fn format_token(s: &str) -> String {
    let need_quotes = s.is_empty()
        || s.chars()
            .any(|c| c.is_whitespace() || c == '"' || c == '\'' || c == '\\');
    if !need_quotes {
        return s.to_string();
    }

    let mut res = String::from("\"");
    for c in s.chars() {
        if c == '"' || c == '\\' {
            res.push('\\');
        }
        res.push(c);
    }
    res.push('"');
    res
}

#[derive(Default)]
pub struct Database {
    arrays: HashMap<String, Array<Value>>,
    single_lists: HashMap<String, OneLinkedList<Value>>,
    double_lists: HashMap<String, DoublyLinkedList<Value>>,
    stacks: HashMap<String, Stack<Value>>,
    queues: HashMap<String, Queue<Value>>,
    trees: HashMap<String, CompleteBinaryTree<Value>>,
}

impl Database {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn load_file(&mut self, filename: &str) -> io::Result<()> {
        let file = match File::open(filename) {
            Ok(f) => f,
            Err(e) if e.kind() == io::ErrorKind::NotFound => return Ok(()),
            Err(e) => return Err(e),
        };

        let reader = BufReader::new(file);
        for line_res in reader.lines() {
            let line = line_res?;
            let trimmed = line.trim();
            if trimmed.is_empty() || trimmed.starts_with('#') {
                continue;
            }

            let tokens = tokenize(trimmed);
            if tokens.len() < 2 {
                continue;
            }

            let kind = tokens[0].to_uppercase();
            let name = tokens[1].clone();

            match kind.as_str() {
                "M" | "ARRAY" => {
                    let arr = self.arrays.entry(name).or_insert_with(Array::new);
                    for tok in &tokens[2..] {
                        arr.push_back(Value::parse(tok));
                    }
                }
                "F" | "SLIST" => {
                    let list = self.single_lists.entry(name).or_insert_with(OneLinkedList::new);
                    for tok in &tokens[2..] {
                        list.push_back(Value::parse(tok));
                    }
                }
                "L" | "DLIST" => {
                    let list = self.double_lists.entry(name).or_insert_with(DoublyLinkedList::new);
                    for tok in &tokens[2..] {
                        list.push_back(Value::parse(tok));
                    }
                }
                "S" | "STACK" => {
                    let st = self.stacks.entry(name).or_insert_with(Stack::new);
                    for tok in &tokens[2..] {
                        st.push(Value::parse(tok));
                    }
                }
                "Q" | "QUEUE" => {
                    let q = self.queues.entry(name).or_insert_with(Queue::new);
                    for tok in &tokens[2..] {
                        q.push(Value::parse(tok));
                    }
                }
                "T" | "TREE" => {
                    let tree = self.trees.entry(name).or_insert_with(CompleteBinaryTree::new);
                    for tok in &tokens[2..] {
                        tree.insert(Value::parse(tok));
                    }
                }
                _ => {}
            }
        }
        Ok(())
    }

    pub fn save_file(&self, filename: &str) -> io::Result<()> {
        let mut file = OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(filename)?;

        writeln!(file, "# DBMS Data File")?;

        for (name, arr) in &self.arrays {
            if arr.is_empty() { continue; }
            write!(file, "M {}", format_token(name))?;
            for val in &arr.data {
                write!(file, " {}", val.format_for_file())?;
            }
            writeln!(file)?;
        }

        for (name, list) in &self.single_lists {
            if list.is_empty() { continue; }
            write!(file, "F {}", format_token(name))?;
            for val in list.to_vec() {
                write!(file, " {}", val.format_for_file())?;
            }
            writeln!(file)?;
        }

        for (name, list) in &self.double_lists {
            if list.is_empty() { continue; }
            write!(file, "L {}", format_token(name))?;
            for val in list.to_vec() {
                write!(file, " {}", val.format_for_file())?;
            }
            writeln!(file)?;
        }

        for (name, st) in &self.stacks {
            if st.is_empty() { continue; }
            write!(file, "S {}", format_token(name))?;
            for val in st.to_vec() {
                write!(file, " {}", val.format_for_file())?;
            }
            writeln!(file)?;
        }

        for (name, q) in &self.queues {
            if q.is_empty() { continue; }
            write!(file, "Q {}", format_token(name))?;
            for val in q.to_vec() {
                write!(file, " {}", val.format_for_file())?;
            }
            writeln!(file)?;
        }

        for (name, tree) in &self.trees {
            if tree.is_empty() { continue; }
            write!(file, "T {}", format_token(name))?;
            for val in tree.to_bfs_vec() {
                write!(file, " {}", val.format_for_file())?;
            }
            writeln!(file)?;
        }

        Ok(())
    }

    pub fn execute_query(&mut self, query_str: &str, modified: &mut bool) -> bool {
        let tokens = tokenize(query_str);
        if tokens.is_empty() {
            return true;
        }

        let cmd = tokens[0].to_uppercase();

        // Universal PRINT command
        if cmd == "PRINT" {
            if tokens.len() < 2 {
                eprintln!("Usage: PRINT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(tree) = self.trees.get(name) {
                println!("Tree [{}]:", name);
                tree.print_tree();
                return true;
            }
            if let Some(arr) = self.arrays.get(name) {
                print!("Array [{}]: ", name);
                arr.print();
                return true;
            }
            if let Some(list) = self.single_lists.get(name) {
                println!("SinglyLinkedList [{}]:", name);
                list.print();
                return true;
            }
            if let Some(list) = self.double_lists.get(name) {
                println!("DoublyLinkedList [{}]:", name);
                list.print();
                return true;
            }
            if let Some(st) = self.stacks.get(name) {
                println!("Stack [{}]:", name);
                st.print();
                return true;
            }
            if let Some(q) = self.queues.get(name) {
                println!("Queue [{}]:", name);
                q.print();
                return true;
            }
            println!("Structure '{}' not found", name);
            return false;
        }

        // Stack commands (S)
        if cmd == "SPUSH" {
            if tokens.len() < 3 {
                eprintln!("Usage: SPUSH <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let st = self.stacks.entry(name).or_insert_with(Stack::new);
            st.push(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "SPOP" {
            if tokens.len() < 2 {
                eprintln!("Usage: SPOP <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(st) = self.stacks.get_mut(name) {
                if let Some(val) = st.pop() {
                    *modified = true;
                    println!("-> {}", val);
                    return true;
                }
            }
            println!("Stack is empty");
            return false;
        }
        if cmd == "STOP" || cmd == "SGET" {
            if tokens.len() < 2 {
                eprintln!("Usage: STOP <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(st) = self.stacks.get(name) {
                if let Some(val) = st.top() {
                    println!("-> {}", val);
                    return true;
                }
            }
            println!("Stack is empty");
            return false;
        }
        if cmd == "SSIZE" {
            if tokens.len() < 2 {
                eprintln!("Usage: SSIZE <name>");
                return false;
            }
            let name = tokens[1].as_str();
            let sz = self.stacks.get(name).map(|s| s.size()).unwrap_or(0);
            println!("-> {}", sz);
            return true;
        }
        if cmd == "SPRINT" {
            if tokens.len() < 2 {
                eprintln!("Usage: SPRINT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(st) = self.stacks.get(name) {
                st.print();
            } else {
                println!("Stack is empty");
            }
            return true;
        }

        // Queue commands (Q)
        if cmd == "QPUSH" {
            if tokens.len() < 3 {
                eprintln!("Usage: QPUSH <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let q = self.queues.entry(name).or_insert_with(Queue::new);
            q.push(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "QPOP" {
            if tokens.len() < 2 {
                eprintln!("Usage: QPOP <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(q) = self.queues.get_mut(name) {
                if let Some(val) = q.pop() {
                    *modified = true;
                    println!("-> {}", val);
                    return true;
                }
            }
            println!("Queue is empty");
            return false;
        }
        if cmd == "QFRONT" || cmd == "QGET" {
            if tokens.len() < 2 {
                eprintln!("Usage: QFRONT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(q) = self.queues.get(name) {
                if let Some(val) = q.front() {
                    println!("-> {}", val);
                    return true;
                }
            }
            println!("Queue is empty");
            return false;
        }
        if cmd == "QSIZE" {
            if tokens.len() < 2 {
                eprintln!("Usage: QSIZE <name>");
                return false;
            }
            let name = tokens[1].as_str();
            let sz = self.queues.get(name).map(|q| q.size()).unwrap_or(0);
            println!("-> {}", sz);
            return true;
        }
        if cmd == "QPRINT" {
            if tokens.len() < 2 {
                eprintln!("Usage: QPRINT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(q) = self.queues.get(name) {
                q.print();
            } else {
                println!("Queue is empty");
            }
            return true;
        }

        // Array commands (M)
        if cmd == "MPUSH" || cmd == "MPUSH_BACK" {
            if tokens.len() < 3 {
                eprintln!("Usage: MPUSH <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let arr = self.arrays.entry(name).or_insert_with(Array::new);
            arr.push_back(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "MINSERT" {
            if tokens.len() < 4 {
                eprintln!("Usage: MINSERT <name> <index> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            let val = Value::parse(&tokens[3]);
            let arr = self.arrays.entry(name).or_insert_with(Array::new);
            if arr.insert(idx, val.clone()).is_ok() {
                *modified = true;
                println!("-> {}", val);
                return true;
            } else {
                eprintln!("Index out of range");
                return false;
            }
        }
        if cmd == "MGET" {
            if tokens.len() < 3 {
                eprintln!("Usage: MGET <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(arr) = self.arrays.get(name) {
                if let Some(val) = arr.get(idx) {
                    println!("-> {}", val);
                    return true;
                }
            }
            eprintln!("Index out of range");
            return false;
        }
        if cmd == "MDEL" {
            if tokens.len() < 3 {
                eprintln!("Usage: MDEL <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(arr) = self.arrays.get_mut(name) {
                if arr.erase(idx).is_ok() {
                    *modified = true;
                    println!("-> OK");
                    return true;
                }
            }
            eprintln!("Index out of range");
            return false;
        }
        if cmd == "MSET" {
            if tokens.len() < 4 {
                eprintln!("Usage: MSET <name> <index> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            let val = Value::parse(&tokens[3]);
            if let Some(arr) = self.arrays.get_mut(name) {
                if arr.set(idx, val.clone()).is_ok() {
                    *modified = true;
                    println!("-> {}", val);
                    return true;
                }
            }
            eprintln!("Index out of range");
            return false;
        }
        if cmd == "MLEN" || cmd == "MSIZE" {
            if tokens.len() < 2 {
                eprintln!("Usage: MLEN <name>");
                return false;
            }
            let name = tokens[1].as_str();
            let len = self.arrays.get(name).map(|a| a.len()).unwrap_or(0);
            println!("-> {}", len);
            return true;
        }
        if cmd == "MPRINT" {
            if tokens.len() < 2 {
                eprintln!("Usage: MPRINT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(arr) = self.arrays.get(name) {
                arr.print();
            }
            return true;
        }

        // Singly Linked List (F)
        if cmd == "FPUSH" || cmd == "FPUSH_BACK" {
            if tokens.len() < 3 {
                eprintln!("Usage: FPUSH <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let list = self.single_lists.entry(name).or_insert_with(OneLinkedList::new);
            list.push_back(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "FPUSH_FRONT" {
            if tokens.len() < 3 {
                eprintln!("Usage: FPUSH_FRONT <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let list = self.single_lists.entry(name).or_insert_with(OneLinkedList::new);
            list.push_front(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "FINSERT_BEFORE" || cmd == "FINSERT_FW" {
            if tokens.len() < 4 {
                eprintln!("Usage: FINSERT_BEFORE <name> <index> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            let val = Value::parse(&tokens[3]);
            let list = self.single_lists.entry(name).or_insert_with(OneLinkedList::new);
            list.insert_forward(idx, val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "FINSERT_AFTER" || cmd == "FINSERT_BW" {
            if tokens.len() < 4 {
                eprintln!("Usage: FINSERT_AFTER <name> <index> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            let val = Value::parse(&tokens[3]);
            let list = self.single_lists.entry(name).or_insert_with(OneLinkedList::new);
            list.insert_backward(idx, val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "FPOP_FRONT" {
            if tokens.len() < 2 {
                eprintln!("Usage: FPOP_FRONT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.single_lists.get_mut(name) {
                if list.pop_front().is_some() {
                    *modified = true;
                    println!("-> OK");
                    return true;
                }
            }
            return false;
        }
        if cmd == "FPOP_BACK" {
            if tokens.len() < 2 {
                eprintln!("Usage: FPOP_BACK <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.single_lists.get_mut(name) {
                if list.pop_back().is_some() {
                    *modified = true;
                    println!("-> OK");
                    return true;
                }
            }
            return false;
        }
        if cmd == "FDEL" || cmd == "FDEL_VAL" {
            if tokens.len() < 3 {
                eprintln!("Usage: FDEL <name> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let val = Value::parse(&tokens[2]);
            if let Some(list) = self.single_lists.get_mut(name) {
                list.delete_by_value(&val);
                *modified = true;
                println!("-> OK");
                return true;
            }
            return false;
        }
        if cmd == "FDEL_FW" || cmd == "FDEL_BEFORE" {
            if tokens.len() < 3 {
                eprintln!("Usage: FDEL_BEFORE <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(list) = self.single_lists.get_mut(name) {
                list.remove_forward(idx);
                *modified = true;
                println!("-> OK");
                return true;
            }
            return false;
        }
        if cmd == "FDEL_BW" || cmd == "FDEL_AFTER" {
            if tokens.len() < 3 {
                eprintln!("Usage: FDEL_AFTER <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(list) = self.single_lists.get_mut(name) {
                list.remove_backward(idx);
                *modified = true;
                println!("-> OK");
                return true;
            }
            return false;
        }
        if cmd == "FGET" {
            if tokens.len() < 3 {
                eprintln!("Usage: FGET <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(list) = self.single_lists.get(name) {
                if let Some(val) = list.get_at(idx) {
                    println!("-> {}", val);
                    return true;
                }
            }
            eprintln!("Element not found");
            return false;
        }
        if cmd == "FSEARCH" {
            if tokens.len() < 3 {
                eprintln!("Usage: FSEARCH <name> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let val = Value::parse(&tokens[2]);
            if let Some(list) = self.single_lists.get(name) {
                if let Some(idx) = list.search_by_value(&val) {
                    println!("Value {} found at index: {}", val, idx);
                } else {
                    println!("Value {} not found in the list.", val);
                }
                return true;
            }
            return false;
        }
        if cmd == "FLEN" || cmd == "FSIZE" {
            if tokens.len() < 2 {
                eprintln!("Usage: FLEN <name>");
                return false;
            }
            let name = tokens[1].as_str();
            let len = self.single_lists.get(name).map(|l| l.len()).unwrap_or(0);
            println!("-> {}", len);
            return true;
        }
        if cmd == "FPRINT" {
            if tokens.len() < 2 {
                eprintln!("Usage: FPRINT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.single_lists.get(name) {
                list.print();
            }
            return true;
        }
        if cmd == "FPRINT_REV" {
            if tokens.len() < 2 {
                eprintln!("Usage: FPRINT_REV <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.single_lists.get(name) {
                list.print_reverse();
            }
            return true;
        }
        if cmd == "FPRINT_ASC" {
            if tokens.len() < 2 {
                eprintln!("Usage: FPRINT_ASC <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.single_lists.get(name) {
                list.print_ascending();
            }
            return true;
        }
        if cmd == "FPRINT_DESC" {
            if tokens.len() < 2 {
                eprintln!("Usage: FPRINT_DESC <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.single_lists.get(name) {
                list.print_descending();
            }
            return true;
        }

        // Doubly Linked List (L)
        if cmd == "LPUSH" || cmd == "LPUSH_BACK" {
            if tokens.len() < 3 {
                eprintln!("Usage: LPUSH <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let list = self.double_lists.entry(name).or_insert_with(DoublyLinkedList::new);
            list.push_back(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "LPUSH_FRONT" {
            if tokens.len() < 3 {
                eprintln!("Usage: LPUSH_FRONT <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let list = self.double_lists.entry(name).or_insert_with(DoublyLinkedList::new);
            list.push_front(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "LINSERT_BEFORE" || cmd == "LINSERT_FW" {
            if tokens.len() < 4 {
                eprintln!("Usage: LINSERT_BEFORE <name> <index> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            let val = Value::parse(&tokens[3]);
            let list = self.double_lists.entry(name).or_insert_with(DoublyLinkedList::new);
            list.insert_forward(idx, val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "LINSERT_AFTER" || cmd == "LINSERT_BW" {
            if tokens.len() < 4 {
                eprintln!("Usage: LINSERT_AFTER <name> <index> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            let val = Value::parse(&tokens[3]);
            let list = self.double_lists.entry(name).or_insert_with(DoublyLinkedList::new);
            list.insert_backward(idx, val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "LPOP_FRONT" {
            if tokens.len() < 2 {
                eprintln!("Usage: LPOP_FRONT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.double_lists.get_mut(name) {
                if list.pop_front().is_some() {
                    *modified = true;
                    println!("-> OK");
                    return true;
                }
            }
            return false;
        }
        if cmd == "LPOP_BACK" {
            if tokens.len() < 2 {
                eprintln!("Usage: LPOP_BACK <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.double_lists.get_mut(name) {
                if list.pop_back().is_some() {
                    *modified = true;
                    println!("-> OK");
                    return true;
                }
            }
            return false;
        }
        if cmd == "LDEL" || cmd == "LDEL_VAL" {
            if tokens.len() < 3 {
                eprintln!("Usage: LDEL <name> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let val = Value::parse(&tokens[2]);
            if let Some(list) = self.double_lists.get_mut(name) {
                list.delete_by_value(&val);
                *modified = true;
                println!("-> OK");
                return true;
            }
            return false;
        }
        if cmd == "LDEL_FW" || cmd == "LDEL_BEFORE" {
            if tokens.len() < 3 {
                eprintln!("Usage: LDEL_BEFORE <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(list) = self.double_lists.get_mut(name) {
                list.remove_forward(idx);
                *modified = true;
                println!("-> OK");
                return true;
            }
            return false;
        }
        if cmd == "LDEL_BW" || cmd == "LDEL_AFTER" {
            if tokens.len() < 3 {
                eprintln!("Usage: LDEL_AFTER <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(list) = self.double_lists.get_mut(name) {
                list.remove_backward(idx);
                *modified = true;
                println!("-> OK");
                return true;
            }
            return false;
        }
        if cmd == "LGET" {
            if tokens.len() < 3 {
                eprintln!("Usage: LGET <name> <index>");
                return false;
            }
            let name = tokens[1].as_str();
            let idx = match tokens[2].parse::<usize>() {
                Ok(i) => i,
                Err(_) => { eprintln!("Invalid index: {}", tokens[2]); return false; }
            };
            if let Some(list) = self.double_lists.get(name) {
                if let Some(val) = list.get_at(idx) {
                    println!("-> {}", val);
                    return true;
                }
            }
            eprintln!("Element not found");
            return false;
        }
        if cmd == "LSEARCH" {
            if tokens.len() < 3 {
                eprintln!("Usage: LSEARCH <name> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let val = Value::parse(&tokens[2]);
            if let Some(list) = self.double_lists.get(name) {
                if let Some(idx) = list.search_by_value(&val) {
                    println!("Value {} found at index: {}", val, idx);
                } else {
                    println!("Value {} not found in the list.", val);
                }
                return true;
            }
            return false;
        }
        if cmd == "LLEN" || cmd == "LSIZE" {
            if tokens.len() < 2 {
                eprintln!("Usage: LLEN <name>");
                return false;
            }
            let name = tokens[1].as_str();
            let len = self.double_lists.get(name).map(|l| l.len()).unwrap_or(0);
            println!("-> {}", len);
            return true;
        }
        if cmd == "LPRINT" {
            if tokens.len() < 2 {
                eprintln!("Usage: LPRINT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.double_lists.get(name) {
                list.print();
            }
            return true;
        }
        if cmd == "LPRINT_REV" {
            if tokens.len() < 2 {
                eprintln!("Usage: LPRINT_REV <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.double_lists.get(name) {
                list.print_reverse();
            }
            return true;
        }
        if cmd == "LPRINT_ASC" {
            if tokens.len() < 2 {
                eprintln!("Usage: LPRINT_ASC <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.double_lists.get(name) {
                list.print_ascending();
            }
            return true;
        }
        if cmd == "LPRINT_DESC" {
            if tokens.len() < 2 {
                eprintln!("Usage: LPRINT_DESC <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(list) = self.double_lists.get(name) {
                list.print_descending();
            }
            return true;
        }

        // Complete Binary Tree commands (T)
        if cmd == "TINSERT" {
            if tokens.len() < 3 {
                eprintln!("Usage: TINSERT <name> <value>");
                return false;
            }
            let name = tokens[1].clone();
            let val = Value::parse(&tokens[2]);
            let tree = self.trees.entry(name).or_insert_with(CompleteBinaryTree::new);
            tree.insert(val.clone());
            *modified = true;
            println!("-> {}", val);
            return true;
        }
        if cmd == "TSEARCH" || cmd == "ISMEMBER" {
            if tokens.len() < 3 {
                eprintln!("Usage: ISMEMBER <name> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let val = Value::parse(&tokens[2]);
            let found = self.trees.get(name).map(|t| t.search(&val)).unwrap_or(false);
            println!("{}", if found { "-> TRUE" } else { "-> FALSE" });
            return true;
        }
        if cmd == "TISCOMPLETE" || cmd == "TCHECK" {
            if tokens.len() < 2 {
                eprintln!("Usage: TISCOMPLETE <name>");
                return false;
            }
            let name = tokens[1].as_str();
            let ok = self.trees.get(name).map(|t| t.is_complete()).unwrap_or(true);
            println!("{}", if ok { "-> TRUE" } else { "-> FALSE" });
            return true;
        }
        if cmd == "TDEL" {
            if tokens.len() < 2 {
                eprintln!("Usage: TDEL <name> [value]");
                return false;
            }
            let name = tokens[1].as_str();
            if tokens.len() == 2 {
                // Per variant requirement: TDEL checks complete tree
                let ok = self.trees.get(name).map(|t| t.is_complete()).unwrap_or(true);
                println!("{}", if ok { "-> TRUE" } else { "-> FALSE" });
                return true;
            } else {
                let val = Value::parse(&tokens[2]);
                if let Some(tree) = self.trees.get_mut(name) {
                    if tree.remove(&val) {
                        *modified = true;
                        println!("-> OK");
                        return true;
                    }
                }
                println!("-> NOT_FOUND");
                return false;
            }
        }
        if cmd == "TGET" {
            if tokens.len() < 2 {
                eprintln!("Usage: TGET <name> [inorder|preorder|postorder|bfs]");
                return false;
            }
            let name = tokens[1].as_str();
            let tree = match self.trees.get(name) {
                Some(t) => t,
                None => {
                    println!("(empty tree)");
                    return true;
                }
            };
            let order = if tokens.len() >= 3 { tokens[2].to_uppercase() } else { "INORDER".to_string() };
            match order.as_str() {
                "PREORDER" | "PRE" => tree.print_preorder(),
                "POSTORDER" | "POST" => tree.print_postorder(),
                "BFS" | "LEVEL" => tree.print_bfs(),
                _ => tree.print_inorder(),
            }
            return true;
        }
        if cmd == "TPRINT" {
            if tokens.len() < 2 {
                eprintln!("Usage: TPRINT <name>");
                return false;
            }
            let name = tokens[1].as_str();
            if let Some(tree) = self.trees.get(name) {
                tree.print_tree();
            } else {
                println!("(empty tree)");
            }
            return true;
        }
        if cmd == "TSIZE" {
            if tokens.len() < 2 {
                eprintln!("Usage: TSIZE <name>");
                return false;
            }
            let name = tokens[1].as_str();
            let sz = self.trees.get(name).map(|t| t.size()).unwrap_or(0);
            println!("-> {}", sz);
            return true;
        }
        if cmd == "TSUCC" {
            if tokens.len() < 3 {
                eprintln!("Usage: TSUCC <name> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let val = Value::parse(&tokens[2]);
            if let Some(tree) = self.trees.get(name) {
                if let Some(succ) = tree.find_successor(&val) {
                    println!("-> {}", succ);
                    return true;
                }
            }
            println!("No successor found");
            return false;
        }
        if cmd == "TPRED" {
            if tokens.len() < 3 {
                eprintln!("Usage: TPRED <name> <value>");
                return false;
            }
            let name = tokens[1].as_str();
            let val = Value::parse(&tokens[2]);
            if let Some(tree) = self.trees.get(name) {
                if let Some(pred) = tree.find_predecessor(&val) {
                    println!("-> {}", pred);
                    return true;
                }
            }
            println!("No predecessor found");
            return false;
        }

        eprintln!("Unknown command: {}", tokens[0]);
        false
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut filename = String::new();
    let mut query = String::new();

    let mut i = 1;
    while i < args.len() {
        if args[i] == "--file" && i + 1 < args.len() {
            filename = args[i + 1].clone();
            i += 2;
        } else if args[i] == "--query" && i + 1 < args.len() {
            query = args[i + 1].clone();
            i += 2;
        } else {
            i += 1;
        }
    }

    if filename.is_empty() {
        eprintln!("Usage: {} --file <filename> [--query '<command>']", args[0]);
        std::process::exit(1);
    }

    let mut db = Database::new();
    let _ = db.load_file(&filename);

    if !query.is_empty() {
        let mut modified = false;
        let ok = db.execute_query(&query, &mut modified);
        if modified {
            let _ = db.save_file(&filename);
        }
        std::process::exit(if ok { 0 } else { 1 });
    }

    // Interactive mode
    println!("=== Simple DBMS CLI (Rust) ===");
    println!("Loaded database file: {}", filename);
    println!("Type queries or 'EXIT' / 'QUIT' to leave.");

    let stdin = io::stdin();
    loop {
        print!("dbms> ");
        io::stdout().flush().unwrap();

        let mut line = String::new();
        if stdin.read_line(&mut line).unwrap_or(0) == 0 {
            break;
        }

        let trimmed = line.trim();
        if trimmed.is_empty() {
            continue;
        }

        let upper = trimmed.to_uppercase();
        if upper == "EXIT" || upper == "QUIT" {
            break;
        }

        let mut modified = false;
        db.execute_query(trimmed, &mut modified);
        if modified {
            let _ = db.save_file(&filename);
        }
    }
}
