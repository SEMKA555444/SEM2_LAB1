#[derive(Debug, Clone)]
struct DNode {
    data: f32,
    prev: Option<usize>,
    next: Option<usize>,
}

#[derive(Debug, Clone, Default)]
pub struct DoublyLinkedList {
    nodes: Vec<Option<DNode>>,
    head: Option<usize>,
    tail: Option<usize>,
    len: usize,
    free_list: Vec<usize>,
}

const EPS: f32 = 1e-6;

impl DoublyLinkedList {
    pub fn new() -> Self {
        Self {
            nodes: Vec::new(),
            head: None,
            tail: None,
            len: 0,
            free_list: Vec::new(),
        }
    }

    fn alloc_node(&mut self, data: f32, prev: Option<usize>, next: Option<usize>) -> usize {
        let node = DNode { data, prev, next };
        if let Some(idx) = self.free_list.pop() {
            self.nodes[idx] = Some(node);
            idx
        } else {
            let idx = self.nodes.len();
            self.nodes.push(Some(node));
            idx
        }
    }

    fn free_node(&mut self, idx: usize) -> f32 {
        let node = self.nodes[idx].take().expect("invalid node index");
        self.free_list.push(idx);
        node.data
    }

    pub fn push_front(&mut self, val: f32) {
        let new_idx = self.alloc_node(val, None, self.head);
        if let Some(old_head) = self.head {
            if let Some(ref mut node) = self.nodes[old_head] {
                node.prev = Some(new_idx);
            }
        } else {
            self.tail = Some(new_idx);
        }
        self.head = Some(new_idx);
        self.len += 1;
    }

    pub fn push_back(&mut self, val: f32) {
        let new_idx = self.alloc_node(val, self.tail, None);
        if let Some(old_tail) = self.tail {
            if let Some(ref mut node) = self.nodes[old_tail] {
                node.next = Some(new_idx);
            }
        } else {
            self.head = Some(new_idx);
        }
        self.tail = Some(new_idx);
        self.len += 1;
    }

    pub fn pop_front(&mut self) -> Option<f32> {
        let head_idx = self.head?;
        let next_idx = self.nodes[head_idx].as_ref().unwrap().next;

        let val = self.free_node(head_idx);
        self.head = next_idx;
        if let Some(new_head) = self.head {
            if let Some(ref mut node) = self.nodes[new_head] {
                node.prev = None;
            }
        } else {
            self.tail = None;
        }
        self.len -= 1;
        Some(val)
    }

    pub fn pop_back(&mut self) -> Option<f32> {
        let tail_idx = self.tail?;
        let prev_idx = self.nodes[tail_idx].as_ref().unwrap().prev;

        let val = self.free_node(tail_idx);
        self.tail = prev_idx;
        if let Some(new_tail) = self.tail {
            if let Some(ref mut node) = self.nodes[new_tail] {
                node.next = None;
            }
        } else {
            self.head = None;
        }
        self.len -= 1;
        Some(val)
    }

    fn get_node_idx_at(&self, index: usize) -> Option<usize> {
        let mut curr = self.head;
        let mut i = 0;
        while let Some(idx) = curr {
            if i == index {
                return Some(idx);
            }
            curr = self.nodes[idx].as_ref().unwrap().next;
            i += 1;
        }
        None
    }

    pub fn get_at(&self, index: usize) -> Option<f32> {
        self.get_node_idx_at(index)
            .map(|idx| self.nodes[idx].as_ref().unwrap().data)
    }

    // Insert before index
    pub fn insert_forward(&mut self, index: usize, val: f32) {
        if index == 0 {
            self.push_front(val);
            return;
        }
        if index >= self.len {
            return;
        }

        let target_idx = match self.get_node_idx_at(index) {
            Some(idx) => idx,
            None => return,
        };

        let prev_idx = self.nodes[target_idx].as_ref().unwrap().prev.unwrap();
        let new_idx = self.alloc_node(val, Some(prev_idx), Some(target_idx));

        self.nodes[prev_idx].as_mut().unwrap().next = Some(new_idx);
        self.nodes[target_idx].as_mut().unwrap().prev = Some(new_idx);
        self.len += 1;
    }

    // Insert after index
    pub fn insert_backward(&mut self, index: usize, val: f32) {
        if index == self.len.saturating_sub(1) {
            self.push_back(val);
            return;
        }
        if index >= self.len {
            return;
        }

        let target_idx = match self.get_node_idx_at(index) {
            Some(idx) => idx,
            None => return,
        };

        let next_idx = self.nodes[target_idx].as_ref().unwrap().next.unwrap();
        let new_idx = self.alloc_node(val, Some(target_idx), Some(next_idx));

        self.nodes[target_idx].as_mut().unwrap().next = Some(new_idx);
        self.nodes[next_idx].as_mut().unwrap().prev = Some(new_idx);
        self.len += 1;
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
        if index >= self.len {
            return;
        }

        if let Some(target_idx) = self.get_node_idx_at(index - 1) {
            self.unlink_and_free(target_idx);
        }
    }

    // Remove after index
    pub fn remove_backward(&mut self, index: usize) {
        if index + 1 >= self.len {
            return;
        }
        if index + 1 == self.len - 1 {
            self.pop_back();
            return;
        }

        if let Some(target_idx) = self.get_node_idx_at(index + 1) {
            self.unlink_and_free(target_idx);
        }
    }

    fn unlink_and_free(&mut self, idx: usize) {
        let prev = self.nodes[idx].as_ref().unwrap().prev;
        let next = self.nodes[idx].as_ref().unwrap().next;

        if let Some(p) = prev {
            self.nodes[p].as_mut().unwrap().next = next;
        } else {
            self.head = next;
        }

        if let Some(n) = next {
            self.nodes[n].as_mut().unwrap().prev = prev;
        } else {
            self.tail = prev;
        }

        self.free_node(idx);
        self.len -= 1;
    }

    pub fn delete_by_value(&mut self, val: f32) {
        let mut curr = self.head;
        while let Some(idx) = curr {
            let next = self.nodes[idx].as_ref().unwrap().next;
            if (self.nodes[idx].as_ref().unwrap().data - val).abs() < EPS {
                self.unlink_and_free(idx);
            }
            curr = next;
        }
    }

    pub fn search_by_value(&self, val: f32) -> Option<usize> {
        let mut curr = self.head;
        let mut idx = 0;
        while let Some(node_idx) = curr {
            if (self.nodes[node_idx].as_ref().unwrap().data - val).abs() < EPS {
                return Some(idx);
            }
            curr = self.nodes[node_idx].as_ref().unwrap().next;
            idx += 1;
        }
        None
    }

    pub fn len(&self) -> usize {
        self.len
    }

    pub fn is_empty(&self) -> bool {
        self.len == 0
    }

    pub fn to_vec(&self) -> Vec<f32> {
        let mut v = Vec::with_capacity(self.len);
        let mut curr = self.head;
        while let Some(idx) = curr {
            v.push(self.nodes[idx].as_ref().unwrap().data);
            curr = self.nodes[idx].as_ref().unwrap().next;
        }
        v
    }

    pub fn print(&self) {
        let mut curr = self.head;
        let mut idx = 0;
        while let Some(node_idx) = curr {
            println!("{} : {}", idx, self.nodes[node_idx].as_ref().unwrap().data);
            curr = self.nodes[node_idx].as_ref().unwrap().next;
            idx += 1;
        }
    }

    pub fn print_reverse(&self) {
        let mut curr = self.tail;
        let mut idx = self.len.saturating_sub(1);
        while let Some(node_idx) = curr {
            println!("{} : {}", idx, self.nodes[node_idx].as_ref().unwrap().data);
            curr = self.nodes[node_idx].as_ref().unwrap().prev;
            if idx > 0 {
                idx -= 1;
            }
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
