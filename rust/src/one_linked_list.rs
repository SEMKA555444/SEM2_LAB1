use std::cmp::Ordering;
use std::fmt::Display;

#[derive(Debug, Clone)]
pub struct Node<T> {
    pub data: T,
    pub next: Option<Box<Node<T>>>,
}

#[derive(Debug, Clone)]
pub struct OneLinkedList<T> {
    pub head: Option<Box<Node<T>>>,
}

impl<T> Default for OneLinkedList<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T> OneLinkedList<T> {
    pub fn new() -> Self {
        Self { head: None }
    }

    pub fn push_front(&mut self, val: T) {
        let new_node = Box::new(Node {
            data: val,
            next: self.head.take(),
        });
        self.head = Some(new_node);
    }

    pub fn push_back(&mut self, val: T) {
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

    pub fn pop_front(&mut self) -> Option<T> {
        self.head.take().map(|node| {
            self.head = node.next;
            node.data
        })
    }

    pub fn pop_back(&mut self) -> Option<T> {
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

    pub fn get_at(&self, index: usize) -> Option<&T> {
        let mut curr = self.head.as_ref();
        let mut i = 0;
        while let Some(node) = curr {
            if i == index {
                return Some(&node.data);
            }
            curr = node.next.as_ref();
            i += 1;
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
}

impl<T: PartialEq> OneLinkedList<T> {
    pub fn insert_forward(&mut self, target_val: &T, val: T) {
        if self.head.is_none() {
            return;
        }

        if &self.head.as_ref().unwrap().data == target_val {
            self.push_front(val);
            return;
        }

        let mut curr = self.head.as_mut().unwrap();
        while let Some(next_node) = &curr.next {
            if &next_node.data == target_val {
                let new_node = Box::new(Node {
                    data: val,
                    next: curr.next.take(),
                });
                curr.next = Some(new_node);
                return;
            }
            curr = curr.next.as_mut().unwrap();
        }
    }

    pub fn insert_backward(&mut self, target_val: &T, val: T) {
        let mut curr = self.head.as_mut();
        while let Some(node) = curr {
            if &node.data == target_val {
                let new_node = Box::new(Node {
                    data: val,
                    next: node.next.take(),
                });
                node.next = Some(new_node);
                return;
            }
            curr = node.next.as_mut();
        }
    }

    pub fn remove_forward(&mut self, target_val: &T) {
        if self.head.is_none() || self.head.as_ref().unwrap().next.is_none() {
            return;
        }

        if &self.head.as_ref().unwrap().data == target_val {
            return;
        }

        if &self.head.as_ref().unwrap().next.as_ref().unwrap().data == target_val {
            self.pop_front();
            return;
        }

        let mut curr = self.head.as_mut().unwrap();
        while let Some(next_node) = &curr.next {
            if let Some(after_next) = &next_node.next {
                if &after_next.data == target_val {
                    let to_remove = curr.next.take().unwrap();
                    curr.next = to_remove.next;
                    return;
                }
            } else {
                break;
            }
            curr = curr.next.as_mut().unwrap();
        }
    }

    pub fn remove_backward(&mut self, target_val: &T) {
        let mut curr = self.head.as_mut();
        while let Some(node) = curr {
            if &node.data == target_val {
                if let Some(target) = node.next.take() {
                    node.next = target.next;
                }
                return;
            }
            curr = node.next.as_mut();
        }
    }

    pub fn delete_by_value(&mut self, val: &T) {
        while let Some(node) = &self.head {
            if &node.data == val {
                self.pop_front();
            } else {
                break;
            }
        }

        let mut curr = self.head.as_mut();
        while let Some(node) = curr {
            while let Some(next_node) = &node.next {
                if &next_node.data == val {
                    let to_remove = node.next.take().unwrap();
                    node.next = to_remove.next;
                } else {
                    break;
                }
            }
            curr = node.next.as_mut();
        }
    }

    pub fn search_by_value(&self, val: &T) -> Option<usize> {
        let mut curr = self.head.as_ref();
        let mut index = 0;
        while let Some(node) = curr {
            if &node.data == val {
                return Some(index);
            }
            curr = node.next.as_ref();
            index += 1;
        }
        None
    }
}

impl<T: Clone> OneLinkedList<T> {
    pub fn to_vec(&self) -> Vec<T> {
        let mut v = Vec::new();
        let mut curr = self.head.as_ref();
        while let Some(node) = curr {
            v.push(node.data.clone());
            curr = node.next.as_ref();
        }
        v
    }
}

impl<T: Display> OneLinkedList<T> {
    pub fn print(&self) {
        let mut curr = self.head.as_ref();
        let mut idx = 0;
        while let Some(node) = curr {
            println!("{} : {}", idx, node.data);
            curr = node.next.as_ref();
            idx += 1;
        }
    }
}

impl<T: Clone + Display> OneLinkedList<T> {
    pub fn print_reverse(&self) {
        let vals = self.to_vec();
        for (i, val) in vals.iter().enumerate().rev() {
            println!("{} : {}", i, val);
        }
    }
}

impl<T: Clone + PartialOrd + Display> OneLinkedList<T> {
    pub fn print_ascending(&self) {
        let mut vals = self.to_vec();
        vals.sort_by(|a, b| a.partial_cmp(b).unwrap_or(Ordering::Equal));
        for (i, val) in vals.iter().enumerate() {
            println!("{} : {}", i, val);
        }
    }

    pub fn print_descending(&self) {
        let mut vals = self.to_vec();
        vals.sort_by(|a, b| b.partial_cmp(a).unwrap_or(Ordering::Equal));
        for (i, val) in vals.iter().enumerate() {
            println!("{} : {}", i, val);
        }
    }
}