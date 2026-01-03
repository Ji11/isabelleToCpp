#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

// auto-generated from theory data

template <typename T>
inline T isabelle_raise_undefined(const char *context)
{
    throw std::runtime_error(context ? context : "Isabelle undefined value");
}

// datatype nat
struct nat {
    enum class Tag {
        Zero,
        Suc
    };
    Tag tag;
    std::shared_ptr<nat> Suc_value0;
    bool is_Zero() const { return tag == Tag::Zero; }
    bool is_Suc() const { return tag == Tag::Suc; }

    static nat make_Zero() {
        nat value;
        value.tag = Tag::Zero;
        return value;
    }
    static nat make_Suc(const std::shared_ptr<nat> &value0) {
        nat value;
        value.tag = Tag::Suc;
        value.Suc_value0 = value0;
        return value;
    }

    // Equality operator
    friend bool operator==(const nat &lhs, const nat &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Suc) {
            if (!(*lhs.Suc_value0 == *rhs.Suc_value0)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const nat &lhs, const nat &rhs) {
        return !(lhs == rhs);
    }
};

inline nat make_nat_Zero() {
    return nat::make_Zero();
}

inline nat make_nat_Suc(const nat &value0) {
    return nat::make_Suc(std::make_shared<nat>(value0));
}

inline bool is_nat_Zero(const nat &value) {
    return value.tag == nat::Tag::Zero;
}

inline bool is_nat_Suc(const nat &value) {
    return value.tag == nat::Tag::Suc;
}

inline const nat &get_nat_Suc_value0(const nat &value) {
    return *(value.Suc_value0);
}

// datatype list
template <typename T0>
struct list {
    enum class Tag {
        Nul,
        Cons
    };
    Tag tag;
    T0 Cons_value0;
    std::shared_ptr<list<T0>> Cons_value1;
    bool is_Nul() const { return tag == Tag::Nul; }
    bool is_Cons() const { return tag == Tag::Cons; }

    static list make_Nul() {
        list value;
        value.tag = Tag::Nul;
        return value;
    }
    static list make_Cons(const T0 &value0, const std::shared_ptr<list<T0>> &value1) {
        list value;
        value.tag = Tag::Cons;
        value.Cons_value0 = value0;
        value.Cons_value1 = value1;
        return value;
    }

    // Equality operator
    friend bool operator==(const list &lhs, const list &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Cons) {
            if (!(lhs.Cons_value0 == rhs.Cons_value0)) return false;
            if (!(*lhs.Cons_value1 == *rhs.Cons_value1)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const list &lhs, const list &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline list<T0> make_list_Nul() {
    return list<T0>::make_Nul();
}

template <typename T0>
inline list<T0> make_list_Cons(const T0 &value0, const list<T0> &value1) {
    return list<T0>::make_Cons(value0, std::make_shared<list<T0>>(value1));
}

template <typename T0>
inline bool is_list_Nul(const list<T0> &value) {
    return value.tag == list<T0>::Tag::Nul;
}

template <typename T0>
inline bool is_list_Cons(const list<T0> &value) {
    return value.tag == list<T0>::Tag::Cons;
}

template <typename T0>
inline const T0 &get_list_Cons_value0(const list<T0> &value) {
    return value.Cons_value0;
}

template <typename T0>
inline const list<T0> &get_list_Cons_value1(const list<T0> &value) {
    return *(value.Cons_value1);
}

// datatype tree
template <typename T0>
struct tree {
    enum class Tag {
        Tip,
        TC
    };
    Tag tag;
    std::shared_ptr<tree<T0>> TC_value0;
    T0 TC_value1;
    std::shared_ptr<tree<T0>> TC_value2;
    bool is_Tip() const { return tag == Tag::Tip; }
    bool is_TC() const { return tag == Tag::TC; }

    static tree make_Tip() {
        tree value;
        value.tag = Tag::Tip;
        return value;
    }
    static tree make_TC(const std::shared_ptr<tree<T0>> &value0, const T0 &value1, const std::shared_ptr<tree<T0>> &value2) {
        tree value;
        value.tag = Tag::TC;
        value.TC_value0 = value0;
        value.TC_value1 = value1;
        value.TC_value2 = value2;
        return value;
    }

    // Equality operator
    friend bool operator==(const tree &lhs, const tree &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::TC) {
            if (!(*lhs.TC_value0 == *rhs.TC_value0)) return false;
            if (!(lhs.TC_value1 == rhs.TC_value1)) return false;
            if (!(*lhs.TC_value2 == *rhs.TC_value2)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const tree &lhs, const tree &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline tree<T0> make_tree_Tip() {
    return tree<T0>::make_Tip();
}

template <typename T0>
inline tree<T0> make_tree_TC(const tree<T0> &value0, const T0 &value1, const tree<T0> &value2) {
    return tree<T0>::make_TC(std::make_shared<tree<T0>>(value0), value1, std::make_shared<tree<T0>>(value2));
}

template <typename T0>
inline bool is_tree_Tip(const tree<T0> &value) {
    return value.tag == tree<T0>::Tag::Tip;
}

template <typename T0>
inline bool is_tree_TC(const tree<T0> &value) {
    return value.tag == tree<T0>::Tag::TC;
}

template <typename T0>
inline const tree<T0> &get_tree_TC_value0(const tree<T0> &value) {
    return *(value.TC_value0);
}

template <typename T0>
inline const T0 &get_tree_TC_value1(const tree<T0> &value) {
    return value.TC_value1;
}

template <typename T0>
inline const tree<T0> &get_tree_TC_value2(const tree<T0> &value) {
    return *(value.TC_value2);
}

// datatype option
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
            return true;
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

template <typename T0>
inline bool is_option_None(const option<T0> &value) {
    return value.tag == option<T0>::Tag::None;
}

template <typename T0>
inline bool is_option_Some(const option<T0> &value) {
    return value.tag == option<T0>::Tag::Some;
}

template <typename T0>
inline const T0 &get_option_Some_value0(const option<T0> &value) {
    return value.Some_value0;
}

// function declarations
// add :: "nat \<Rightarrow> nat \<Rightarrow> nat"
nat add(const nat &arg0, const nat &arg1);

// mult :: "nat \<Rightarrow> nat \<Rightarrow> nat"
nat mult(const nat &arg0, const nat &arg1);

// insert :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a list"
template <typename T0>
list<T0> insert(const T0 &arg0, const list<T0> &arg1);

// del :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a list"
template <typename T0>
list<T0> del(const T0 &arg0, const list<T0> &arg1);

// find :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a option"
template <typename T0>
option<T0> find_(const T0 &arg0, const list<T0> &arg1);

// inserttree :: "'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a tree"
template <typename T0>
tree<T0> inserttree(const T0 &arg0, const tree<T0> &arg1);

// findtree :: "'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a option"
template <typename T0>
option<T0> findtree(const T0 &arg0, const tree<T0> &arg1);

// mintree :: "'a::ord tree ⇒ 'a"
template <typename T0>
T0 mintree(const tree<T0> &arg0);

// insert_desc :: "'a::ord ⇒ 'a list ⇒ 'a list"
template <typename T0>
list<T0> insert_desc(const T0 &arg0, const list<T0> &arg1);

// sort :: "'a::ord list ⇒ 'a list"
template <typename T0>
list<T0> sort_(const list<T0> &arg0);

// append :: "'a list ⇒ 'a list ⇒ 'a list"
template <typename T0>
list<T0> append(const list<T0> &arg0, const list<T0> &arg1);

// traversaltree :: "'a tree ⇒ 'a list"
template <typename T0>
list<T0> traversaltree(const tree<T0> &arg0);

// function implementations converted from Isabelle/HOL

nat add(const nat &arg0, const nat &arg1) {
    if (((arg0).tag == nat::Tag::Zero)) {
        const nat &n = arg1;
        return n;
    }
    if (((arg0).tag == nat::Tag::Suc)) {
        const nat &m = (*(arg0.Suc_value0));
        const nat &n = arg1;
        return nat::make_Suc(std::make_shared<nat>(add(m, n)));
    }
    throw std::runtime_error("pattern match failure in add");
}

nat mult(const nat &arg0, const nat &arg1) {
    if (((arg0).tag == nat::Tag::Zero)) {
        const nat &n = arg1;
        return nat::make_Zero();
    }
    if (((arg0).tag == nat::Tag::Suc)) {
        const nat &m = (*(arg0.Suc_value0));
        const nat &n = arg1;
        return add(n, mult(m, n));
    }
    throw std::runtime_error("pattern match failure in mult");
}

template <typename T0>
list<T0> insert(const T0 &arg0, const list<T0> &arg1) {
    if (((arg1).tag == list<T0>::Tag::Nul)) {
        const T0 &x = arg0;
        return list<T0>::make_Cons(x, std::make_shared<list<T0>>(list<T0>::make_Nul()));
    }
    if (((arg1).tag == list<T0>::Tag::Cons)) {
        const T0 &x = arg0;
        const T0 &y = (arg1.Cons_value0);
        const list<T0> &ys = (*(arg1.Cons_value1));
        return list<T0>::make_Cons(y, std::make_shared<list<T0>>(insert(x, ys)));
    }
    throw std::runtime_error("pattern match failure in insert");
}

template <typename T0>
list<T0> del(const T0 &arg0, const list<T0> &arg1) {
    if (((arg1).tag == list<T0>::Tag::Nul)) {
        const T0 &x = arg0;
        return list<T0>::make_Nul();
    }
    if (((arg1).tag == list<T0>::Tag::Cons)) {
        const T0 &x = arg0;
        const T0 &y = (arg1.Cons_value0);
        const list<T0> &ys = (*(arg1.Cons_value1));
        return (((x == y)) ? (ys) : (list<T0>::make_Cons(y, std::make_shared<list<T0>>(del(x, ys)))));
    }
    throw std::runtime_error("pattern match failure in del");
}

template <typename T0>
option<T0> find_(const T0 &arg0, const list<T0> &arg1) {
    if (((arg1).tag == list<T0>::Tag::Nul)) {
        const T0 &x = arg0;
        return option<T0>::make_None();
    }
    if (((arg1).tag == list<T0>::Tag::Cons)) {
        const T0 &x = arg0;
        const T0 &y = (arg1.Cons_value0);
        const list<T0> &ys = (*(arg1.Cons_value1));
        return (((x == y)) ? (option<T0>::make_Some(y)) : (find_(x, ys)));
    }
    throw std::runtime_error("pattern match failure in find");
}

template <typename T0>
tree<T0> inserttree(const T0 &arg0, const tree<T0> &arg1) {
    if (((arg1).tag == tree<T0>::Tag::Tip)) {
        const T0 &x = arg0;
        return tree<T0>::make_TC(std::make_shared<tree<T0>>(tree<T0>::make_Tip()), x, std::make_shared<tree<T0>>(tree<T0>::make_Tip()));
    }
    if (((arg1).tag == tree<T0>::Tag::TC)) {
        const T0 &x = arg0;
        const tree<T0> &l = (*(arg1.TC_value0));
        const T0 &y = (arg1.TC_value1);
        const tree<T0> &r = (*(arg1.TC_value2));
        return (((x < y)) ? (tree<T0>::make_TC(std::make_shared<tree<T0>>(inserttree(x, l)), y, std::make_shared<tree<T0>>(r))) : (tree<T0>::make_TC(std::make_shared<tree<T0>>(l), y, std::make_shared<tree<T0>>(inserttree(x, r)))));
    }
    throw std::runtime_error("pattern match failure in inserttree");
}

template <typename T0>
option<T0> findtree(const T0 &arg0, const tree<T0> &arg1) {
    if (((arg1).tag == tree<T0>::Tag::Tip)) {
        const T0 &x = arg0;
        return option<T0>::make_None();
    }
    if (((arg1).tag == tree<T0>::Tag::TC)) {
        const T0 &x = arg0;
        const tree<T0> &l = (*(arg1.TC_value0));
        const T0 &y = (arg1.TC_value1);
        const tree<T0> &r = (*(arg1.TC_value2));
        return (((x == y)) ? (option<T0>::make_Some(y)) : ((((x < y)) ? (findtree(x, l)) : (findtree(x, r)))));
    }
    throw std::runtime_error("pattern match failure in findtree");
}

template <typename T0>
T0 mintree(const tree<T0> &arg0) {
    if (((arg0).tag == tree<T0>::Tag::Tip)) {
        return isabelle_raise_undefined<T0>("undefined");
    }
    if (((arg0).tag == tree<T0>::Tag::TC) && (((*(arg0.TC_value0))).tag == tree<T0>::Tag::Tip)) {
        const T0 &y = (arg0.TC_value1);
        return y;
    }
    if (((arg0).tag == tree<T0>::Tag::TC)) {
        const tree<T0> &l = (*(arg0.TC_value0));
        return mintree(l);
    }
    throw std::runtime_error("pattern match failure in mintree");
}

template <typename T0>
list<T0> insert_desc(const T0 &arg0, const list<T0> &arg1) {
    if (((arg1).tag == list<T0>::Tag::Nul)) {
        const T0 &x = arg0;
        return list<T0>::make_Cons(x, std::make_shared<list<T0>>(list<T0>::make_Nul()));
    }
    if (((arg1).tag == list<T0>::Tag::Cons)) {
        const T0 &x = arg0;
        const T0 &y = (arg1.Cons_value0);
        const list<T0> &ys = (*(arg1.Cons_value1));
        return (((x >= y)) ? (list<T0>::make_Cons(x, std::make_shared<list<T0>>(list<T0>::make_Cons(y, std::make_shared<list<T0>>(ys))))) : (list<T0>::make_Cons(y, std::make_shared<list<T0>>(insert_desc(x, ys)))));
    }
    throw std::runtime_error("pattern match failure in insert_desc");
}

template <typename T0>
list<T0> sort_(const list<T0> &arg0) {
    if (((arg0).tag == list<T0>::Tag::Nul)) {
        return list<T0>::make_Nul();
    }
    if (((arg0).tag == list<T0>::Tag::Cons)) {
        const T0 &x = (arg0.Cons_value0);
        const list<T0> &xs = (*(arg0.Cons_value1));
        return insert_desc(x, sort_(xs));
    }
    throw std::runtime_error("pattern match failure in sort");
}

template <typename T0>
list<T0> append(const list<T0> &arg0, const list<T0> &arg1) {
    if (((arg0).tag == list<T0>::Tag::Nul)) {
        const list<T0> &ys = arg1;
        return ys;
    }
    if (((arg0).tag == list<T0>::Tag::Cons)) {
        const T0 &x = (arg0.Cons_value0);
        const list<T0> &xs = (*(arg0.Cons_value1));
        const list<T0> &ys = arg1;
        return list<T0>::make_Cons(x, std::make_shared<list<T0>>(append(xs, ys)));
    }
    throw std::runtime_error("pattern match failure in append");
}

template <typename T0>
list<T0> traversaltree(const tree<T0> &arg0) {
    // patterns translated from Isabelle/HOL
    // clause: Tip => Nul
    // clause: TC(l, x, r) => append (traversaltree l) (Cons x (traversaltree r))
    throw std::runtime_error("function traversaltree not implemented yet");
}

// ======== 辅助打印函数 ========

// nat → int
int nat_to_int(const nat &n) {
    int x = 0;
    const nat *cur = &n;
    while (cur->tag == nat::Tag::Suc) {
        x++;
        cur = cur->Suc_value0.get();
    }
    return x;
}

// 打印 nat
void print_nat(const nat &n) {
    cout << nat_to_int(n);
}

// 打印 list<T>
template<typename T>
void print_list(const list<T> &xs) {
    cout << "[";
    const list<T> *cur = &xs;
    bool first = true;
    while (cur->tag == list<T>::Tag::Cons) {
        if (!first) cout << ", ";
        first = false;
        cout << cur->Cons_value0;
        cur = cur->Cons_value1.get();
    }
    cout << "]";
}

// 打印 tree<T> （中序遍历）
template<typename T>
void print_tree(const tree<T> &t) {
    if (t.tag == tree<T>::Tag::Tip) {
        cout << "Tip";
    } else {
        cout << "TC(";
        print_tree(*t.TC_value0);
        cout << ", " << t.TC_value1 << ", ";
        print_tree(*t.TC_value2);
        cout << ")";
    }
}


// ============= main 函数 =============
int main() {
    cout << "=== Test nat ===\n";
    nat n0 = make_nat_Zero();
    nat n1 = make_nat_Suc(n0);
    nat n2 = make_nat_Suc(n1);
    nat n3 = make_nat_Suc(n2);

    cout << "n2 = "; print_nat(n2); cout << "\n";
    cout << "add(n2, n3) = "; print_nat(add(n2, n3)); cout << "\n";
    cout << "mult(n2, n3) = "; print_nat(mult(n2, n3)); cout << "\n\n";


    cout << "=== Test list ===\n";
    list<int> l0 = make_list_Nul<int>();
    list<int> l1 = make_list_Cons(3, l0);
    list<int> l2 = make_list_Cons(1, l1);
    list<int> l3 = make_list_Cons(4, l2);

    cout << "Original list: ";
    print_list(l3);
    cout << "\n";

    cout << "After insert 2: ";
    print_list(insert(2, l3));
    cout << "\n";

    cout << "After delete 1: ";
    print_list(del(1, l3));
    cout << "\n";

    cout << "find 3 = ";
    auto fres = find_(3, l3);
    if (fres.is_Some()) cout << "Some(" << fres.Some_value0 << ")\n";
    else cout << "None\n";

    cout << "sort: ";
    print_list(sort_(l3));
    cout << "\n\n";


    cout << "=== Test tree ===\n";
    tree<int> t = make_tree_Tip<int>();
    t = inserttree(5, t);
    t = inserttree(2, t);
    t = inserttree(8, t);
    t = inserttree(1, t);
    t = inserttree(3, t);

    cout << "Tree = ";
    print_tree(t);
    cout << "\n";

    cout << "findtree(3) = ";
    auto ft = findtree(3, t);
    if (ft.is_Some()) cout << "Some(" << ft.Some_value0 << ")\n";
    else cout << "None\n";

    cout << "mintree = " << mintree(t) << "\n";

    return 0;
}
