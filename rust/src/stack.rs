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

impl<T: Display> Stack<T> {
    pub fn pop(&mut self) -> Option<T> {
        match self.data.pop() {
            Some(val) => {
                println!("-> {}", val);
                Some(val)
            }
            None => {
                println!("Stack is empty");
                None
            }
        }
    }

    pub fn print(&self) {
        if self.data.is_empty() {
            println!("Stack is empty");
            return;
        }
        for (i, val) in self.data.iter().rev().enumerate() {
            println!("{} : {}", i, val);
        }
    }
}

impl<T: Clone> Stack<T> {
    pub fn to_vec(&self) -> Vec<T> {
        self.data.clone()
    }
}