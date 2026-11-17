#[derive(Debug, Clone, Default)]
pub struct Stack {
    data: Vec<f32>,
}

impl Stack {
    pub fn new() -> Self {
        Self { data: Vec::new() }
    }

    pub fn push(&mut self, val: f32) {
        self.data.push(val);
    }

    pub fn pop(&mut self) -> Option<f32> {
        self.data.pop()
    }

    pub fn top(&self) -> Option<f32> {
        self.data.last().copied()
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

    // Returns elements from bottom to top for serialization
    pub fn to_vec(&self) -> Vec<f32> {
        self.data.clone()
    }

    pub fn print(&self) {
        if self.data.is_empty() {
            println!("Stack is empty");
            return;
        }
        // Stack.cpp prints top-to-bottom: 0 is top
        for (i, val) in self.data.iter().rev().enumerate() {
            println!("{} : {}", i, val);
        }
    }
}
