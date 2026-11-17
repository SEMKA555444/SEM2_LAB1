#[derive(Debug, Clone)]
pub struct Node {
    pub data: f32,
    pub next: Option<Box<Node>>,
}

#[derive(Debug, Clone, Default)]
pub struct OneLinkedList {
    pub head: Option<Box<Node>>,
}

const EPS: f32 = 1e-6;

impl OneLinkedList {
    pub fn new() -> Self {
        Self { head: None }
    }

    pub fn push_front(&mut self, val: f32) {
        let new_node = Box::new(Node {
            data: val,
            next: self.head.take(),
        });
        self.head = Some(new_node);
    }

    pub fn push_back(&mut self, val: f32) {
        let new_node = Box::new(Node {
            data: val,
            next: None,
        });

        if self.head.is_none() {
            self.head = Some(new_node);
            return;
        }

        let mut curr = self.head.as_mut().unwrap();
        while curr.next.is_some() {
            curr = curr.next.as_mut().unwrap();
        }
        curr.next = Some(new_node);
    }

    pub fn pop_front(&mut self) -> Option<f32> {
        self.head.take().map(|node| {
            self.head = node.next;
            node.data
        })
    }

    pub fn pop_back(&mut self) -> Option<f32> {
        if self.head.is_none() {
            return None;
        }

        if self.head.as_ref().unwrap().next.is_none() {
            return self.pop_front();
        }

        let mut curr = self.head.as_mut().unwrap();
        while curr.next.as_ref().unwrap().next.is_some() {
            curr = curr.next.as_mut().unwrap();
        }

        curr.next.take().map(|n| n.data)
    }

    pub fn get_at(&self, index: usize) -> Option<f32> {
        let mut curr = self.head.as_ref();
        let mut i = 0;
        while let Some(node) = curr {
            if i == index {
                return Some(node.data);
            }
            curr = node.next.as_ref();
            i += 1;
        }
        None
    }

    // Insert before index
    pub fn insert_forward(&mut self, index: usize, val: f32) {
        if index == 0 {
            self.push_front(val);
            return;
        }

        let mut curr = self.head.as_mut();
        let mut i = 0;
        while let Some(node) = curr {
            if i == index - 1 {
                let new_node = Box::new(Node {
                    data: val,
                    next: node.next.take(),
                });
                node.next = Some(new_node);
                return;
            }
            curr = node.next.as_mut();
            i += 1;
        }
    }

    // Insert after index
    pub fn insert_backward(&mut self, index: usize, val: f32) {
        let mut curr = self.head.as_mut();
        let mut i = 0;
        while let Some(node) = curr {
            if i == index {
                let new_node = Box::new(Node {
                    data: val,
                    next: node.next.take(),
                });
                node.next = Some(new_node);
                return;
            }
            curr = node.next.as_mut();
            i += 1;
        }
    }

    // Remove before index
    pub fn remove_forward(&mut self, index: usize) {
        if index == 0 {
            return;
        }
        if index == 1 {
            self.pop_front();
            return;
        }

        let mut curr = self.head.as_mut();
        let mut i = 0;
        while let Some(node) = curr {
            if i == index - 2 {
                if let Some(target) = node.next.take() {
                    node.next = target.next;
                }
                return;
            }
            curr = node.next.as_mut();
            i += 1;
        }
    }

    // Remove after index
    pub fn remove_backward(&mut self, index: usize) {
        let mut curr = self.head.as_mut();
        let mut i = 0;
        while let Some(node) = curr {
            if i == index {
                if let Some(target) = node.next.take() {
                    node.next = target.next;
                }
                return;
            }
            curr = node.next.as_mut();
            i += 1;
        }
    }

    pub fn delete_by_value(&mut self, val: f32) {
        while let Some(node) = &self.head {
            if (node.data - val).abs() < EPS {
                self.pop_front();
            } else {
                break;
            }
        }

        let mut curr = self.head.as_mut();
        while let Some(node) = curr {
            while let Some(next_node) = &node.next {
                if (next_node.data - val).abs() < EPS {
                    let to_remove = node.next.take().unwrap();
                    node.next = to_remove.next;
                } else {
                    break;
                }
            }
            curr = node.next.as_mut();
        }
    }

    pub fn search_by_value(&self, val: f32) -> Option<usize> {
        let mut curr = self.head.as_ref();
        let mut index = 0;
        while let Some(node) = curr {
            if (node.data - val).abs() < EPS {
                return Some(index);
            }
            curr = node.next.as_ref();
            index += 1;
        }
        None
    }

    pub fn len(&self) -> usize {
        let mut count = 0;
        let mut curr = self.head.as_ref();
        while let Some(node) = curr {
            count += 1;
            curr = node.next.as_ref();
        }
        count
    }

    pub fn is_empty(&self) -> bool {
        self.head.is_none()
    }

    pub fn to_vec(&self) -> Vec<f32> {
        let mut v = Vec::new();
        let mut curr = self.head.as_ref();
        while let Some(node) = curr {
            v.push(node.data);
            curr = node.next.as_ref();
        }
        v
    }

    pub fn print(&self) {
        let mut curr = self.head.as_ref();
        let mut idx = 0;
        while let Some(node) = curr {
            println!("{} : {}", idx, node.data);
            curr = node.next.as_ref();
            idx += 1;
        }
    }

    pub fn print_reverse(&self) {
        let vals = self.to_vec();
        for (i, val) in vals.iter().enumerate().rev() {
            println!("{} : {}", i, val);
        }
    }

    pub fn print_ascending(&self) {
        let mut vals = self.to_vec();
        vals.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
        for (i, val) in vals.iter().enumerate() {
            println!("{} : {}", i, val);
        }
    }

    pub fn print_descending(&self) {
        let mut vals = self.to_vec();
        vals.sort_by(|a, b| b.partial_cmp(a).unwrap_or(std::cmp::Ordering::Equal));
        for (i, val) in vals.iter().enumerate() {
            println!("{} : {}", i, val);
        }
    }
}
