use std::fmt::Display;

#[derive(Debug, Clone)]
pub struct Stack<T> {
    data: Vec<T>,
}

impl<T> Default for Stack<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> Stack<T> {
    pub fn new() -> Self {
        Self { data: Vec::new() }
    }

    pub fn push(&mut self, val: T) {
        self.data.push(val);
    }

    pub fn pop(&mut self) -> Option<T> {
        self.data.pop()
    }

    pub fn top(&self) -> Option<&T> {
        self.data.last()
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

impl<T: Clone> Stack<T> {
    // Returns elements from bottom to top for serialization
    pub fn to_vec(&self) -> Vec<T> {
        self.data.clone()
    }
}

impl<T: Display> Stack<T> {
    pub fn print(&self) {
        if self.data.is_empty() {
            println!("Stack is empty");
            return;
        }
        // Stack prints top-to-bottom: 0 is top
        for (i, val) in self.data.iter().rev().enumerate() {
            println!("{} : {}", i, val);
        }
    }
}
