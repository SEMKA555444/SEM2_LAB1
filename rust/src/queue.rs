use std::collections::VecDeque;

#[derive(Debug, Clone, Default)]
pub struct Queue {
    data: VecDeque<f32>,
}

impl Queue {
    pub fn new() -> Self {
        Self {
            data: VecDeque::new(),
        }
    }

    pub fn push(&mut self, val: f32) {
        self.data.push_back(val);
    }

    pub fn pop(&mut self) -> Option<f32> {
        self.data.pop_front()
    }

    pub fn front(&self) -> Option<f32> {
        self.data.front().copied()
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

    pub fn to_vec(&self) -> Vec<f32> {
        self.data.iter().copied().collect()
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
