#[derive(Debug, Clone)]
pub struct Array {
    pub data: Vec<f32>,
}

impl Array {
    pub fn new() -> Self {
        Self {
            data: Vec::with_capacity(4),
        }
    }

    pub fn push_back(&mut self, val: f32) {
        self.data.push(val);
    }

    pub fn insert(&mut self, index: usize, val: f32) -> Result<(), &'static str> {
        if index > self.data.len() {
            return Err("Index out of range");
        }
        self.data.insert(index, val);
        Ok(())
    }

    pub fn get(&self, index: usize) -> Option<f32> {
        self.data.get(index).copied()
    }

    pub fn pop_back(&mut self) -> Option<f32> {
        self.data.pop()
    }

    pub fn erase(&mut self, index: usize) -> Result<(), &'static str> {
        if index >= self.data.len() {
            return Err("Index out of range");
        }
        self.data.remove(index);
        Ok(())
    }

    pub fn set(&mut self, index: usize, val: f32) -> Result<(), &'static str> {
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
