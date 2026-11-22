use std::fmt::Display;

#[derive(Debug, Clone)]
pub struct Array<T> {
    pub data: Vec<T>,
}

impl<T> Default for Array<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> Array<T> {
    pub fn new() -> Self {
        Self {
            data: Vec::with_capacity(4),
        }
    }

    pub fn push_back(&mut self, val: T) {
        self.data.push(val);
    }

    pub fn insert(&mut self, index: usize, val: T) -> Result<(), &'static str> {
        if index > self.data.len() {
            return Err("Index out of range");
        }
        self.data.insert(index, val);
        Ok(())
    }

    pub fn get(&self, index: usize) -> Option<&T> {
        self.data.get(index)
    }

    pub fn pop_back(&mut self) -> Option<T> {
        self.data.pop()
    }

    pub fn erase(&mut self, index: usize) -> Result<(), &'static str> {
        if index >= self.data.len() {
            return Err("Index out of range");
        }
        self.data.remove(index);
        Ok(())
    }

    pub fn set(&mut self, index: usize, val: T) -> Result<(), &'static str> {
        if index >= self.data.len() {
            return Err("Index out of range");
        }
        self.data[index] = val;
        Ok(())
    }

    pub fn clear(&mut self) {
        self.data.clear();
    }

    pub fn len(&self) -> usize {
        self.data.len()
    }

    pub fn is_empty(&self) -> bool {
        self.data.is_empty()
    }
}

impl<T: Display> Array<T> {
    pub fn print(&self) {
        for (i, val) in self.data.iter().enumerate() {
            if i > 0 {
                print!(" ");
            }
            print!("{}", val);
        }
        println!();
    }
}
