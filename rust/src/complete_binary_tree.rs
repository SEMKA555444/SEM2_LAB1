use std::cell::RefCell;
use std::collections::VecDeque;
use std::rc::Rc;

const EPS: f64 = 1e-9;

pub struct TreeNode {
    pub data: f64,
    pub left: Option<Rc<RefCell<TreeNode>>>,
    pub right: Option<Rc<RefCell<TreeNode>>>,
}

impl TreeNode {
    pub fn new(data: f64) -> Rc<RefCell<Self>> {
        Rc::new(RefCell::new(Self {
            data,
            left: None,
            right: None,
        }))
    }
}

pub struct CompleteBinaryTree {
    pub root: Option<Rc<RefCell<TreeNode>>>,
    pub size: usize,
}

impl Default for CompleteBinaryTree {
    fn default() -> Self {
        Self::new()
    }
}

impl CompleteBinaryTree {
    pub fn new() -> Self {
        Self {
            root: None,
            size: 0,
        }
    }

    pub fn is_empty(&self) -> bool {
        self.root.is_none()
    }

    pub fn size(&self) -> usize {
        self.size
    }

    pub fn clear(&mut self) {
        self.root = None;
        self.size = 0;
    }

    pub fn insert(&mut self, val: f64) {
        let new_node = TreeNode::new(val);

        if self.root.is_none() {
            self.root = Some(new_node);
            self.size = 1;
            return;
        }

        let mut queue = VecDeque::new();
        queue.push_back(Rc::clone(self.root.as_ref().unwrap()));

        while let Some(current) = queue.pop_front() {
            let mut curr = current.borrow_mut();

            if curr.left.is_none() {
                curr.left = Some(new_node);
                self.size += 1;
                return;
            } else {
                queue.push_back(Rc::clone(curr.left.as_ref().unwrap()));
            }

            if curr.right.is_none() {
                curr.right = Some(new_node);
                self.size += 1;
                return;
            } else {
                queue.push_back(Rc::clone(curr.right.as_ref().unwrap()));
            }
        }
    }

    pub fn is_complete(&self) -> bool {
        let root = match &self.root {
            Some(r) => Rc::clone(r),
            None => return true,
        };

        let mut queue = VecDeque::new();
        queue.push_back(root);
        let mut must_be_leaf = false;

        while let Some(current) = queue.pop_front() {
            let curr = current.borrow();

            if let Some(left) = &curr.left {
                if must_be_leaf {
                    return false;
                }
                queue.push_back(Rc::clone(left));
            } else {
                must_be_leaf = true;
            }

            if let Some(right) = &curr.right {
                if must_be_leaf {
                    return false;
                }
                queue.push_back(Rc::clone(right));
            } else {
                must_be_leaf = true;
            }
        }
        true
    }

    pub fn search(&self, val: f64) -> bool {
        let root = match &self.root {
            Some(r) => Rc::clone(r),
            None => return false,
        };

        let mut queue = VecDeque::new();
        queue.push_back(root);

        while let Some(current) = queue.pop_front() {
            let curr = current.borrow();
            if (curr.data - val).abs() < EPS {
                return true;
            }
            if let Some(left) = &curr.left {
                queue.push_back(Rc::clone(left));
            }
            if let Some(right) = &curr.right {
                queue.push_back(Rc::clone(right));
            }
        }
        false
    }

    pub fn to_bfs_vec(&self) -> Vec<f64> {
        let mut res = Vec::new();
        let root = match &self.root {
            Some(r) => Rc::clone(r),
            None => return res,
        };

        let mut queue = VecDeque::new();
        queue.push_back(root);

        while let Some(current) = queue.pop_front() {
            let curr = current.borrow();
            res.push(curr.data);
            if let Some(left) = &curr.left {
                queue.push_back(Rc::clone(left));
            }
            if let Some(right) = &curr.right {
                queue.push_back(Rc::clone(right));
            }
        }
        res
    }

    fn inorder_helper(node: &Option<Rc<RefCell<TreeNode>>>, res: &mut Vec<f64>) {
        if let Some(n) = node {
            let curr = n.borrow();
            Self::inorder_helper(&curr.left, res);
            res.push(curr.data);
            Self::inorder_helper(&curr.right, res);
        }
    }

    pub fn remove(&mut self, val: f64) -> bool {
        let root = match &self.root {
            Some(r) => Rc::clone(r),
            None => return false,
        };

        if root.borrow().left.is_none() && root.borrow().right.is_none() {
            if (root.borrow().data - val).abs() < EPS {
                self.root = None;
                self.size = 0;
                return true;
            }
            return false;
        }

        let mut queue = VecDeque::new();
        queue.push_back((root, None::<Rc<RefCell<TreeNode>>>));

        let mut target_node: Option<Rc<RefCell<TreeNode>>> = None;
        let mut last_node: Option<Rc<RefCell<TreeNode>>> = None;
        let mut last_parent: Option<Rc<RefCell<TreeNode>>> = None;

        while let Some((curr, parent)) = queue.pop_front() {
            if (curr.borrow().data - val).abs() < EPS && target_node.is_none() {
                target_node = Some(Rc::clone(&curr));
            }

            last_node = Some(Rc::clone(&curr));
            last_parent = parent;

            let c = curr.borrow();
            if let Some(left) = &c.left {
                queue.push_back((Rc::clone(left), Some(Rc::clone(&curr))));
            }
            if let Some(right) = &c.right {
                queue.push_back((Rc::clone(right), Some(Rc::clone(&curr))));
            }
        }

        let target = match target_node {
            Some(t) => t,
            None => return false,
        };

        let last = last_node.unwrap();
        let last_val = last.borrow().data;
        target.borrow_mut().data = last_val;

        if let Some(parent) = last_parent {
            let mut p = parent.borrow_mut();
            if let Some(right) = &p.right {
                if Rc::ptr_eq(right, &last) {
                    p.right = None;
                } else {
                    p.left = None;
                }
            } else {
                p.left = None;
            }
        }

        self.size -= 1;
        true
    }

    pub fn find_successor(&self, val: f64) -> Option<f64> {
        let mut elements = Vec::new();
        Self::inorder_helper(&self.root, &mut elements);
        for (i, &v) in elements.iter().enumerate() {
            if (v - val).abs() < EPS {
                if i + 1 < elements.len() {
                    return Some(elements[i + 1]);
                }
                return None;
            }
        }
        None
    }

    pub fn find_predecessor(&self, val: f64) -> Option<f64> {
        let mut elements = Vec::new();
        Self::inorder_helper(&self.root, &mut elements);
        for (i, &v) in elements.iter().enumerate() {
            if (v - val).abs() < EPS {
                if i > 0 {
                    return Some(elements[i - 1]);
                }
                return None;
            }
        }
        None
    }

    pub fn print_bfs(&self) {
        let vals = self.to_bfs_vec();
        if vals.is_empty() {
            println!("(empty tree)");
            return;
        }
        for (i, val) in vals.iter().enumerate() {
            if i > 0 {
                print!(" ");
            }
            print!("{}", val);
        }
        println!();
    }

    pub fn print_inorder(&self) {
        if self.root.is_none() {
            println!("(empty tree)");
            return;
        }
        let mut res = Vec::new();
        Self::inorder_helper(&self.root, &mut res);
        for (i, val) in res.iter().enumerate() {
            if i > 0 {
                print!(" ");
            }
            print!("{}", val);
        }
        println!();
    }

    fn preorder_helper(node: &Option<Rc<RefCell<TreeNode>>>) {
        if let Some(n) = node {
            let curr = n.borrow();
            print!("{} ", curr.data);
            Self::preorder_helper(&curr.left);
            Self::preorder_helper(&curr.right);
        }
    }

    pub fn print_preorder(&self) {
        if self.root.is_none() {
            println!("(empty tree)");
            return;
        }
        Self::preorder_helper(&self.root);
        println!();
    }

    fn postorder_helper(node: &Option<Rc<RefCell<TreeNode>>>) {
        if let Some(n) = node {
            let curr = n.borrow();
            Self::postorder_helper(&curr.left);
            Self::postorder_helper(&curr.right);
            print!("{} ", curr.data);
        }
    }

    pub fn print_postorder(&self) {
        if self.root.is_none() {
            println!("(empty tree)");
            return;
        }
        Self::postorder_helper(&self.root);
        println!();
    }

    fn print_tree_helper(node: &Rc<RefCell<TreeNode>>, prefix: &str, is_left: bool) {
        let curr = node.borrow();
        print!("{}", prefix);
        print!("{}", if is_left { "├── " } else { "└── " });
        println!("{}", curr.data);

        let next_prefix = format!("{}{}", prefix, if is_left { "│   " } else { "    " });
        if curr.left.is_some() && curr.right.is_some() {
            Self::print_tree_helper(curr.left.as_ref().unwrap(), &next_prefix, true);
            Self::print_tree_helper(curr.right.as_ref().unwrap(), &next_prefix, false);
        } else if let Some(left) = &curr.left {
            Self::print_tree_helper(left, &next_prefix, false);
        } else if let Some(right) = &curr.right {
            Self::print_tree_helper(right, &next_prefix, false);
        }
    }

    pub fn print_tree(&self) {
        let root = match &self.root {
            Some(r) => r,
            None => {
                println!("(empty tree)");
                return;
            }
        };

        println!("{}", root.borrow().data);
        let r = root.borrow();
        if r.left.is_some() && r.right.is_some() {
            Self::print_tree_helper(r.left.as_ref().unwrap(), "", true);
            Self::print_tree_helper(r.right.as_ref().unwrap(), "", false);
        } else if let Some(left) = &r.left {
            Self::print_tree_helper(left, "", false);
        } else if let Some(right) = &r.right {
            Self::print_tree_helper(right, "", false);
        }
    }
}