use std::collections::VecDeque;
use std::fmt::Display;

#[derive(Debug, Clone)]
pub struct Queue<T> {
    data: VecDeque<T>,
}

impl<T> Default for Queue<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> Queue<T> {
    pub fn new() -> Self {
        Self {
            data: VecDeque::new(),
        }
    }

    pub fn push(&mut self, val: T) {
        self.data.push_back(val);
    }

    pub fn front(&self) -> Option<&T> {
        self.data.front()
    }

    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }

    pub fn size(&self) -> usize {
        self.data.len()
    }

    pub fn clear(&mut self) {
        self.data.clear();
    }
}

impl<T: Display> Queue<T> {
    pub fn pop(&mut self) -> Option<T> {
        match self.data.pop_front() {
            Some(val) => {
                println!("-> {}", val);
                Some(val)
            }
            None => {
                println!("Queue is empty");
                None
            }
        }
    }

    pub fn print(&self) {
        if self.data.is_empty() {
            eprintln!("Queue is empty");
            return;
        }
        for (i, val) in self.data.iter().enumerate() {
            println!("{} : {}", i, val);
        }
    }
}

impl<T: Clone> Queue<T> {
    pub fn to_vec(&self) -> Vec<T> {
        self.data.iter().cloned().collect()
    }
}