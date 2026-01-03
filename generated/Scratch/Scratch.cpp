#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

// auto-generated from theory Scratch

template <typename T>
inline T isabelle_raise_undefined(const char *context)
{
    throw std::runtime_error(context ? context : "Isabelle undefined value");
}

// Built-in Isabelle option type
template <typename T0>
struct option {
    enum class Tag {
        None,
        Some
    };
    Tag tag;
    T0 Some_value0;
    bool is_None() const { return tag == Tag::None; }
    bool is_Some() const { return tag == Tag::Some; }

    static option make_None() {
        option value;
        value.tag = Tag::None;
        return value;
    }
    static option make_Some(const T0 &value0) {
        option value;
        value.tag = Tag::Some;
        value.Some_value0 = value0;
        return value;
    }

    // Equality operator
    friend bool operator==(const option &lhs, const option &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Some) {
            if (!(lhs.Some_value0 == rhs.Some_value0)) return false;
        }
        return true;
    }
    friend bool operator!=(const option &lhs, const option &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline option<T0> make_option_None() {
    return option<T0>::make_None();
}

template <typename T0>
inline option<T0> make_option_Some(const T0 &value0) {
    return option<T0>::make_Some(value0);
}

// datatype mynat
struct mynat {
    enum class Tag {
        Zero,
        Suc
    };
    Tag tag;
    std::shared_ptr<mynat> Suc_value0;
    bool is_Zero() const { return tag == Tag::Zero; }
    bool is_Suc() const { return tag == Tag::Suc; }

    static mynat make_Zero() {
        mynat value;
        value.tag = Tag::Zero;
        return value;
    }
    static mynat make_Suc(const std::shared_ptr<mynat> &value0) {
        mynat value;
        value.tag = Tag::Suc;
        value.Suc_value0 = value0;
        return value;
    }

    // Equality operator
    friend bool operator==(const mynat &lhs, const mynat &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Suc) {
            if (!(*lhs.Suc_value0 == *rhs.Suc_value0)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mynat &lhs, const mynat &rhs) {
        return !(lhs == rhs);
    }
};

inline mynat make_mynat_Zero() {
    return mynat::make_Zero();
}

inline mynat make_mynat_Suc(const mynat &value0) {
    return mynat::make_Suc(std::make_shared<mynat>(value0));
}

inline bool is_mynat_Zero(const mynat &value) {
    return value.tag == mynat::Tag::Zero;
}

inline bool is_mynat_Suc(const mynat &value) {
    return value.tag == mynat::Tag::Suc;
}

inline const mynat &get_mynat_Suc_value0(const mynat &value) {
    return *(value.Suc_value0);
}

// datatype mynat_list
struct mynat_list {
    enum class Tag {
        Nil,
        Cons
    };
    Tag tag;
    mynat Cons_value0;
    std::shared_ptr<mynat_list> Cons_value1;
    bool is_Nil() const { return tag == Tag::Nil; }
    bool is_Cons() const { return tag == Tag::Cons; }

    static mynat_list make_Nil() {
        mynat_list value;
        value.tag = Tag::Nil;
        return value;
    }
    static mynat_list make_Cons(const mynat &value0, const std::shared_ptr<mynat_list> &value1) {
        mynat_list value;
        value.tag = Tag::Cons;
        value.Cons_value0 = value0;
        value.Cons_value1 = value1;
        return value;
    }

    // Equality operator
    friend bool operator==(const mynat_list &lhs, const mynat_list &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Cons) {
            if (!(lhs.Cons_value0 == rhs.Cons_value0)) return false;
            if (!(*lhs.Cons_value1 == *rhs.Cons_value1)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mynat_list &lhs, const mynat_list &rhs) {
        return !(lhs == rhs);
    }
};

inline mynat_list make_mynat_list_Nil() {
    return mynat_list::make_Nil();
}

inline mynat_list make_mynat_list_Cons(const mynat &value0, const mynat_list &value1) {
    return mynat_list::make_Cons(value0, std::make_shared<mynat_list>(value1));
}

inline bool is_mynat_list_Nil(const mynat_list &value) {
    return value.tag == mynat_list::Tag::Nil;
}

inline bool is_mynat_list_Cons(const mynat_list &value) {
    return value.tag == mynat_list::Tag::Cons;
}

inline const mynat &get_mynat_list_Cons_value0(const mynat_list &value) {
    return value.Cons_value0;
}

inline const mynat_list &get_mynat_list_Cons_value1(const mynat_list &value) {
    return *(value.Cons_value1);
}

// datatype mynat_tree
struct mynat_tree {
    enum class Tag {
        Leaf,
        Node
    };
    Tag tag;
    mynat Node_value0;
    std::shared_ptr<mynat_tree> Node_value1;
    std::shared_ptr<mynat_tree> Node_value2;
    bool is_Leaf() const { return tag == Tag::Leaf; }
    bool is_Node() const { return tag == Tag::Node; }

    static mynat_tree make_Leaf() {
        mynat_tree value;
        value.tag = Tag::Leaf;
        return value;
    }
    static mynat_tree make_Node(const mynat &value0, const std::shared_ptr<mynat_tree> &value1, const std::shared_ptr<mynat_tree> &value2) {
        mynat_tree value;
        value.tag = Tag::Node;
        value.Node_value0 = value0;
        value.Node_value1 = value1;
        value.Node_value2 = value2;
        return value;
    }

    // Equality operator
    friend bool operator==(const mynat_tree &lhs, const mynat_tree &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Node) {
            if (!(lhs.Node_value0 == rhs.Node_value0)) return false;
            if (!(*lhs.Node_value1 == *rhs.Node_value1)) return false;
            if (!(*lhs.Node_value2 == *rhs.Node_value2)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mynat_tree &lhs, const mynat_tree &rhs) {
        return !(lhs == rhs);
    }
};

inline mynat_tree make_mynat_tree_Leaf() {
    return mynat_tree::make_Leaf();
}

inline mynat_tree make_mynat_tree_Node(const mynat &value0, const mynat_tree &value1, const mynat_tree &value2) {
    return mynat_tree::make_Node(value0, std::make_shared<mynat_tree>(value1), std::make_shared<mynat_tree>(value2));
}

inline bool is_mynat_tree_Leaf(const mynat_tree &value) {
    return value.tag == mynat_tree::Tag::Leaf;
}

inline bool is_mynat_tree_Node(const mynat_tree &value) {
    return value.tag == mynat_tree::Tag::Node;
}

inline const mynat &get_mynat_tree_Node_value0(const mynat_tree &value) {
    return value.Node_value0;
}

inline const mynat_tree &get_mynat_tree_Node_value1(const mynat_tree &value) {
    return *(value.Node_value1);
}

inline const mynat_tree &get_mynat_tree_Node_value2(const mynat_tree &value) {
    return *(value.Node_value2);
}

// function declarations
// add :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat"
mynat add(const mynat &arg0, const mynat &arg1);

// minus :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat"
mynat minus_(const mynat &arg0, const mynat &arg1);

// mult :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat"
mynat mult(const mynat &arg0, const mynat &arg1);

// insert_nat_head :: "mynat \<Rightarrow> mynat_list \<Rightarrow> mynat_list"
mynat_list insert_nat_head(const mynat &arg0, const mynat_list &arg1);

// delete_nat :: "mynat \<Rightarrow> mynat_list \<Rightarrow> mynat_list"
mynat_list delete_nat(const mynat &arg0, const mynat_list &arg1);

// update_nat :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat_list \<Rightarrow> mynat_list"
mynat_list update_nat(const mynat &arg0, const mynat &arg1, const mynat_list &arg2);

// search_nat :: "mynat \<Rightarrow> mynat_list \<Rightarrow> bool"
bool search_nat(const mynat &arg0, const mynat_list &arg1);

// less :: "mynat \<Rightarrow> mynat \<Rightarrow> bool"
bool less_(const mynat &arg0, const mynat &arg1);

// insert_nat_tree :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> mynat_tree"
mynat_tree insert_nat_tree(const mynat &arg0, const mynat_tree &arg1);

// search_nat_tree :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> bool"
bool search_nat_tree(const mynat &arg0, const mynat_tree &arg1);

// find_min_opt :: "mynat_tree \<Rightarrow> mynat option"
option<mynat> find_min_opt(const mynat_tree &arg0);

// find_nat_tree_opt :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> mynat option"
option<mynat> find_nat_tree_opt(const mynat &arg0, const mynat_tree &arg1);

// find_nat_list_opt :: "mynat \<Rightarrow> mynat_list \<Rightarrow> mynat option"
option<mynat> find_nat_list_opt(const mynat &arg0, const mynat_list &arg1);

// find_min :: "mynat_tree \<Rightarrow> mynat"
mynat find_min(const mynat_tree &arg0);

// delete_min :: "mynat_tree \<Rightarrow> mynat_tree"
mynat_tree delete_min(const mynat_tree &arg0);

// delete_nat_tree :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> mynat_tree"
mynat_tree delete_nat_tree(const mynat &arg0, const mynat_tree &arg1);

// function implementations converted from Isabelle/HOL

mynat add(const mynat &arg0, const mynat &arg1) {
    if (((arg0).tag == mynat::Tag::Zero)) {
        const mynat &n = arg1;
        return n;
    }
    if (((arg0).tag == mynat::Tag::Suc)) {
        const mynat &m = (*(arg0.Suc_value0));
        const mynat &n = arg1;
        return mynat::make_Suc(std::make_shared<mynat>(add(m, n)));
    }
    throw std::runtime_error("pattern match failure in add");
}

mynat minus_(const mynat &arg0, const mynat &arg1) {
    if (((arg1).tag == mynat::Tag::Zero)) {
        const mynat &m = arg0;
        return m;
    }
    if (((arg0).tag == mynat::Tag::Zero)) {
        const mynat &m = arg1;
        return mynat::make_Zero();
    }
    if (((arg0).tag == mynat::Tag::Suc) && ((arg1).tag == mynat::Tag::Suc)) {
        const mynat &m = (*(arg0.Suc_value0));
        const mynat &n = (*(arg1.Suc_value0));
        return minus_(m, n);
    }
    throw std::runtime_error("pattern match failure in minus");
}

mynat mult(const mynat &arg0, const mynat &arg1) {
    if (((arg0).tag == mynat::Tag::Zero)) {
        const mynat &n = arg1;
        return mynat::make_Zero();
    }
    if (((arg0).tag == mynat::Tag::Suc)) {
        const mynat &m = (*(arg0.Suc_value0));
        const mynat &n = arg1;
        return add(n, mult(m, n));
    }
    throw std::runtime_error("pattern match failure in mult");
}

mynat_list insert_nat_head(const mynat &arg0, const mynat_list &arg1) {
    {
        const mynat &x = arg0;
        const mynat_list &l = arg1;
        return mynat_list::make_Cons(x, std::make_shared<mynat_list>(l));
    }
    throw std::runtime_error("pattern match failure in insert_nat_head");
}

mynat_list delete_nat(const mynat &arg0, const mynat_list &arg1) {
    if (((arg1).tag == mynat_list::Tag::Nil)) {
        const mynat &x = arg0;
        return mynat_list::make_Nil();
    }
    if (((arg1).tag == mynat_list::Tag::Cons)) {
        const mynat &x = arg0;
        const mynat &y = (arg1.Cons_value0);
        const mynat_list &l = (*(arg1.Cons_value1));
        return (((x == y)) ? (l) : (mynat_list::make_Cons(y, std::make_shared<mynat_list>(delete_nat(x, l)))));
    }
    throw std::runtime_error("pattern match failure in delete_nat");
}

mynat_list update_nat(const mynat &arg0, const mynat &arg1, const mynat_list &arg2) {
    if (((arg2).tag == mynat_list::Tag::Nil)) {
        const mynat &old = arg0;
        const mynat &new_ = arg1;
        return mynat_list::make_Nil();
    }
    if (((arg2).tag == mynat_list::Tag::Cons)) {
        const mynat &old = arg0;
        const mynat &new_ = arg1;
        const mynat &y = (arg2.Cons_value0);
        const mynat_list &l = (*(arg2.Cons_value1));
        return (((y == old)) ? (mynat_list::make_Cons(new_, std::make_shared<mynat_list>(l))) : (mynat_list::make_Cons(y, std::make_shared<mynat_list>(update_nat(old, new_, l)))));
    }
    throw std::runtime_error("pattern match failure in update_nat");
}

bool search_nat(const mynat &arg0, const mynat_list &arg1) {
    if (((arg1).tag == mynat_list::Tag::Nil)) {
        const mynat &x = arg0;
        return false;
    }
    if (((arg1).tag == mynat_list::Tag::Cons)) {
        const mynat &x = arg0;
        const mynat &y = (arg1.Cons_value0);
        const mynat_list &l = (*(arg1.Cons_value1));
        return ((x == y) || search_nat(x, l));
    }
    throw std::runtime_error("pattern match failure in search_nat");
}

bool less_(const mynat &arg0, const mynat &arg1) {
    if (((arg0).tag == mynat::Tag::Zero) && ((arg1).tag == mynat::Tag::Suc)) {
        const mynat &n = (*(arg1.Suc_value0));
        return true;
    }
    if (((arg0).tag == mynat::Tag::Zero) && ((arg1).tag == mynat::Tag::Zero)) {
        return false;
    }
    if (((arg0).tag == mynat::Tag::Suc) && ((arg1).tag == mynat::Tag::Zero)) {
        const mynat &m = (*(arg0.Suc_value0));
        return false;
    }
    if (((arg0).tag == mynat::Tag::Suc) && ((arg1).tag == mynat::Tag::Suc)) {
        const mynat &m = (*(arg0.Suc_value0));
        const mynat &n = (*(arg1.Suc_value0));
        return less_(m, n);
    }
    throw std::runtime_error("pattern match failure in less");
}

mynat_tree insert_nat_tree(const mynat &arg0, const mynat_tree &arg1) {
    if (((arg1).tag == mynat_tree::Tag::Leaf)) {
        const mynat &x = arg0;
        return mynat_tree::make_Node(x, std::make_shared<mynat_tree>(mynat_tree::make_Leaf()), std::make_shared<mynat_tree>(mynat_tree::make_Leaf()));
    }
    if (((arg1).tag == mynat_tree::Tag::Node)) {
        const mynat &x = arg0;
        const mynat &y = (arg1.Node_value0);
        const mynat_tree &l = (*(arg1.Node_value1));
        const mynat_tree &r = (*(arg1.Node_value2));
        return ((less_(x, y)) ? (mynat_tree::make_Node(y, std::make_shared<mynat_tree>(insert_nat_tree(x, l)), std::make_shared<mynat_tree>(r))) : (((less_(y, x)) ? (mynat_tree::make_Node(y, std::make_shared<mynat_tree>(l), std::make_shared<mynat_tree>(insert_nat_tree(x, r)))) : (mynat_tree::make_Node(y, std::make_shared<mynat_tree>(l), std::make_shared<mynat_tree>(r))))));
    }
    throw std::runtime_error("pattern match failure in insert_nat_tree");
}

bool search_nat_tree(const mynat &arg0, const mynat_tree &arg1) {
    if (((arg1).tag == mynat_tree::Tag::Leaf)) {
        const mynat &x = arg0;
        return false;
    }
    if (((arg1).tag == mynat_tree::Tag::Node)) {
        const mynat &x = arg0;
        const mynat &y = (arg1.Node_value0);
        const mynat_tree &l = (*(arg1.Node_value1));
        const mynat_tree &r = (*(arg1.Node_value2));
        return ((x == y) || ((less_(x, y)) ? (search_nat_tree(x, l)) : (search_nat_tree(x, r))));
    }
    throw std::runtime_error("pattern match failure in search_nat_tree");
}

option<mynat> find_min_opt(const mynat_tree &arg0) {
    if (((arg0).tag == mynat_tree::Tag::Leaf)) {
        return option<mynat>::make_None();
    }
    if (((arg0).tag == mynat_tree::Tag::Node) && (((*(arg0.Node_value1))).tag == mynat_tree::Tag::Leaf)) {
        const mynat &x = (arg0.Node_value0);
        return option<mynat>::make_Some(x);
    }
    if (((arg0).tag == mynat_tree::Tag::Node)) {
        const mynat_tree &l = (*(arg0.Node_value1));
        return find_min_opt(l);
    }
    throw std::runtime_error("pattern match failure in find_min_opt");
}

option<mynat> find_nat_tree_opt(const mynat &arg0, const mynat_tree &arg1) {
    if (((arg1).tag == mynat_tree::Tag::Leaf)) {
        const mynat &x = arg0;
        return option<mynat>::make_None();
    }
    if (((arg1).tag == mynat_tree::Tag::Node)) {
        const mynat &x = arg0;
        const mynat &y = (arg1.Node_value0);
        const mynat_tree &l = (*(arg1.Node_value1));
        const mynat_tree &r = (*(arg1.Node_value2));
        return (((x == y)) ? (option<mynat>::make_Some(y)) : (((less_(x, y)) ? (find_nat_tree_opt(x, l)) : (find_nat_tree_opt(x, r)))));
    }
    throw std::runtime_error("pattern match failure in find_nat_tree_opt");
}

option<mynat> find_nat_list_opt(const mynat &arg0, const mynat_list &arg1) {
    if (((arg1).tag == mynat_list::Tag::Nil)) {
        const mynat &x = arg0;
        return option<mynat>::make_None();
    }
    if (((arg1).tag == mynat_list::Tag::Cons)) {
        const mynat &x = arg0;
        const mynat &y = (arg1.Cons_value0);
        const mynat_list &l = (*(arg1.Cons_value1));
        return (((x == y)) ? (option<mynat>::make_Some(y)) : (find_nat_list_opt(x, l)));
    }
    throw std::runtime_error("pattern match failure in find_nat_list_opt");
}

mynat find_min(const mynat_tree &arg0) {
    if (((arg0).tag == mynat_tree::Tag::Leaf)) {
        return isabelle_raise_undefined<mynat>("undefined");
    }
    if (((arg0).tag == mynat_tree::Tag::Node) && (((*(arg0.Node_value1))).tag == mynat_tree::Tag::Leaf)) {
        const mynat &x = (arg0.Node_value0);
        return x;
    }
    if (((arg0).tag == mynat_tree::Tag::Node)) {
        const mynat_tree &l = (*(arg0.Node_value1));
        return find_min(l);
    }
    throw std::runtime_error("pattern match failure in find_min");
}

mynat_tree delete_min(const mynat_tree &arg0) {
    if (((arg0).tag == mynat_tree::Tag::Leaf)) {
        return mynat_tree::make_Leaf();
    }
    if (((arg0).tag == mynat_tree::Tag::Node) && (((*(arg0.Node_value1))).tag == mynat_tree::Tag::Leaf)) {
        const mynat &x = (arg0.Node_value0);
        const mynat_tree &r = (*(arg0.Node_value2));
        return r;
    }
    if (((arg0).tag == mynat_tree::Tag::Node)) {
        const mynat &x = (arg0.Node_value0);
        const mynat_tree &l = (*(arg0.Node_value1));
        const mynat_tree &r = (*(arg0.Node_value2));
        return mynat_tree::make_Node(x, std::make_shared<mynat_tree>(delete_min(l)), std::make_shared<mynat_tree>(r));
    }
    throw std::runtime_error("pattern match failure in delete_min");
}

mynat_tree delete_nat_tree(const mynat &arg0, const mynat_tree &arg1) {
    // patterns translated from Isabelle/HOL
    // clause: _ Leaf => Leaf
    // clause: x Node(y, l, r) => ( if less x y then Node y (delete_nat_tree x l) r else if less y x then Node y l (delete_nat_tree x r) else (case (l, r) of (Leaf, Leaf) \<Rightarrow> Leaf | (Leaf, _) \<Rightarrow> r | (_, Leaf) \<Rightarrow> l | _ \<Rightarrow> (let min_val = find_min r in Node min_val l (delete_min r) ) ) )
    throw std::runtime_error("function delete_nat_tree not implemented yet");
}

// ======= 辅助函数：将 mynat 转为 int =======
int mynat_to_int(const mynat &n) {
    if (is_mynat_Zero(n)) return 0;
    return 1 + mynat_to_int(get_mynat_Suc_value0(n));
}

// ======= 打印 mynat =======
void print_mynat(const mynat &n) {
    cout << mynat_to_int(n);
}

// ======= 打印 mynat_list =======
void print_mynat_list(const mynat_list &lst) {
    cout << "[";
    const mynat_list *cur = &lst;
    bool first = true;
    while (!is_mynat_list_Nil(*cur)) {
        if (!first) cout << ", ";
        print_mynat(cur->Cons_value0);
        first = false;
        cur = cur->Cons_value1.get();
    }
    cout << "]";
}

// ======= 中序打印 mynat_tree =======
void print_mynat_tree(const mynat_tree &t) {
    if (is_mynat_tree_Leaf(t)) return;
    print_mynat_tree(*t.Node_value1);
    cout << mynat_to_int(t.Node_value0) << " ";
    print_mynat_tree(*t.Node_value2);
}

// ===========================================================
//                           MAIN
// ===========================================================

int main() {

    cout << "==================== NAT TEST ====================\n";
    mynat n0 = make_mynat_Zero();
    mynat n1 = make_mynat_Suc(n0);
    mynat n2 = make_mynat_Suc(n1);
    mynat n3 = make_mynat_Suc(n2);

    cout << "n3 = "; print_mynat(n3); cout << "\n";

    cout << "add(3, 2) = ";
    print_mynat(add(n3, n2));
    cout << "\n";

    cout << "minus(3,1) = ";
    print_mynat(minus_(n3, n1));
    cout << "\n";

    cout << "mult(3,2) = ";
    print_mynat(mult(n3, n2));
    cout << "\n\n";


    cout << "==================== LIST TEST ====================\n";

    mynat_list lst = make_mynat_list_Nil();

    lst = insert_nat_head(n1, lst);
    lst = insert_nat_head(n2, lst);
    lst = insert_nat_head(n3, lst);

    cout << "Initial list: ";
    print_mynat_list(lst); cout << "\n";

    auto lst2 = delete_nat(n2, lst);
    cout << "delete_nat(2): ";
    print_mynat_list(lst2); cout << "\n";

    auto lst3 = update_nat(n1, n3, lst);
    cout << "update_nat(1 -> 3): ";
    print_mynat_list(lst3); cout << "\n";

    cout << "search_nat(3): " << (search_nat(n3, lst) ? "true" : "false") << "\n";

    auto opt = find_nat_list_opt(n2, lst);
    cout << "find_nat_list_opt(2): ";
    if (opt.is_Some()) print_mynat(opt.Some_value0); else cout << "None";
    cout << "\n\n";


    cout << "==================== TREE TEST ====================\n";

    mynat_tree t = make_mynat_tree_Leaf();
    t = insert_nat_tree(n2, t);
    t = insert_nat_tree(n1, t);
    t = insert_nat_tree(n3, t);

    cout << "Tree inorder: ";
    print_mynat_tree(t); cout << "\n";

    cout << "search_nat_tree(3): " 
         << (search_nat_tree(n3, t) ? "true" : "false") << "\n";

    cout << "find_min_opt: ";
    auto opt_min = find_min_opt(t);
    if (opt_min.is_Some()) print_mynat(opt_min.Some_value0);
    else cout << "None";
    cout << "\n";

    cout << "find_min: ";
    print_mynat(find_min(t)); 
    cout << "\n";

    auto t2 = delete_min(t);
    cout << "Tree after delete_min: ";
    print_mynat_tree(t2);
    cout << "\n\n";

    cout << "==================== END TEST ====================\n";

    return 0;
}
