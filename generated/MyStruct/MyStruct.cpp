#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

// auto-generated from theory MyStruct

template <typename T>
inline T isabelle_raise_undefined(const char *context)
{
    throw std::runtime_error(context ? context : "Isabelle undefined value");
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

// datatype myoption
template <typename T0>
struct myoption {
    enum class Tag {
        None,
        Op
    };
    Tag tag;
    T0 Op_value0;
    bool is_None() const { return tag == Tag::None; }
    bool is_Op() const { return tag == Tag::Op; }

    static myoption make_None() {
        myoption value;
        value.tag = Tag::None;
        return value;
    }
    static myoption make_Op(const T0 &value0) {
        myoption value;
        value.tag = Tag::Op;
        value.Op_value0 = value0;
        return value;
    }

    // Equality operator
    friend bool operator==(const myoption &lhs, const myoption &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Op) {
            if (!(lhs.Op_value0 == rhs.Op_value0)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const myoption &lhs, const myoption &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline myoption<T0> make_myoption_None() {
    return myoption<T0>::make_None();
}

template <typename T0>
inline myoption<T0> make_myoption_Op(const T0 &value0) {
    return myoption<T0>::make_Op(value0);
}

template <typename T0>
inline bool is_myoption_None(const myoption<T0> &value) {
    return value.tag == myoption<T0>::Tag::None;
}

template <typename T0>
inline bool is_myoption_Op(const myoption<T0> &value) {
    return value.tag == myoption<T0>::Tag::Op;
}

template <typename T0>
inline const T0 &get_myoption_Op_value0(const myoption<T0> &value) {
    return value.Op_value0;
}

// datatype mylist
template <typename T0>
struct mylist {
    enum class Tag {
        Nil,
        Cons
    };
    Tag tag;
    T0 Cons_value0;
    std::shared_ptr<mylist<T0>> Cons_value1;
    bool is_Nil() const { return tag == Tag::Nil; }
    bool is_Cons() const { return tag == Tag::Cons; }

    static mylist make_Nil() {
        mylist value;
        value.tag = Tag::Nil;
        return value;
    }
    static mylist make_Cons(const T0 &value0, const std::shared_ptr<mylist<T0>> &value1) {
        mylist value;
        value.tag = Tag::Cons;
        value.Cons_value0 = value0;
        value.Cons_value1 = value1;
        return value;
    }

    // Equality operator
    friend bool operator==(const mylist &lhs, const mylist &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::Cons) {
            if (!(lhs.Cons_value0 == rhs.Cons_value0)) return false;
            if (!(*lhs.Cons_value1 == *rhs.Cons_value1)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mylist &lhs, const mylist &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline mylist<T0> make_mylist_Nil() {
    return mylist<T0>::make_Nil();
}

template <typename T0>
inline mylist<T0> make_mylist_Cons(const T0 &value0, const mylist<T0> &value1) {
    return mylist<T0>::make_Cons(value0, std::make_shared<mylist<T0>>(value1));
}

template <typename T0>
inline bool is_mylist_Nil(const mylist<T0> &value) {
    return value.tag == mylist<T0>::Tag::Nil;
}

template <typename T0>
inline bool is_mylist_Cons(const mylist<T0> &value) {
    return value.tag == mylist<T0>::Tag::Cons;
}

template <typename T0>
inline const T0 &get_mylist_Cons_value0(const mylist<T0> &value) {
    return value.Cons_value0;
}

template <typename T0>
inline const mylist<T0> &get_mylist_Cons_value1(const mylist<T0> &value) {
    return *(value.Cons_value1);
}

// datatype mytree
template <typename T0>
struct mytree {
    enum class Tag {
        Tip,
        TC
    };
    Tag tag;
    T0 TC_value0;
    std::shared_ptr<mytree<T0>> TC_value1;
    std::shared_ptr<mytree<T0>> TC_value2;
    bool is_Tip() const { return tag == Tag::Tip; }
    bool is_TC() const { return tag == Tag::TC; }

    static mytree make_Tip() {
        mytree value;
        value.tag = Tag::Tip;
        return value;
    }
    static mytree make_TC(const T0 &value0, const std::shared_ptr<mytree<T0>> &value1, const std::shared_ptr<mytree<T0>> &value2) {
        mytree value;
        value.tag = Tag::TC;
        value.TC_value0 = value0;
        value.TC_value1 = value1;
        value.TC_value2 = value2;
        return value;
    }

    // Equality operator
    friend bool operator==(const mytree &lhs, const mytree &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::TC) {
            if (!(lhs.TC_value0 == rhs.TC_value0)) return false;
            if (!(*lhs.TC_value1 == *rhs.TC_value1)) return false;
            if (!(*lhs.TC_value2 == *rhs.TC_value2)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mytree &lhs, const mytree &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline mytree<T0> make_mytree_Tip() {
    return mytree<T0>::make_Tip();
}

template <typename T0>
inline mytree<T0> make_mytree_TC(const T0 &value0, const mytree<T0> &value1, const mytree<T0> &value2) {
    return mytree<T0>::make_TC(value0, std::make_shared<mytree<T0>>(value1), std::make_shared<mytree<T0>>(value2));
}

template <typename T0>
inline bool is_mytree_Tip(const mytree<T0> &value) {
    return value.tag == mytree<T0>::Tag::Tip;
}

template <typename T0>
inline bool is_mytree_TC(const mytree<T0> &value) {
    return value.tag == mytree<T0>::Tag::TC;
}

template <typename T0>
inline const T0 &get_mytree_TC_value0(const mytree<T0> &value) {
    return value.TC_value0;
}

template <typename T0>
inline const mytree<T0> &get_mytree_TC_value1(const mytree<T0> &value) {
    return *(value.TC_value1);
}

template <typename T0>
inline const mytree<T0> &get_mytree_TC_value2(const mytree<T0> &value) {
    return *(value.TC_value2);
}

// function declarations
// add :: "mynat\<Rightarrow>mynat\<Rightarrow>mynat"
mynat add(const mynat &arg0, const mynat &arg1);

// mul :: "mynat\<Rightarrow>mynat\<Rightarrow>mynat"
mynat mul(const mynat &arg0, const mynat &arg1);

// push_head :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> push_head(const T0 &arg0, const mylist<T0> &arg1);

// push_tail :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> push_tail(const T0 &arg0, const mylist<T0> &arg1);

// del_first :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> del_first(const T0 &arg0, const mylist<T0> &arg1);

// del_all :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> del_all(const T0 &arg0, const mylist<T0> &arg1);

// alter :: "mynat\<Rightarrow>'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> alter(const mynat &arg0, const T0 &arg1, const mylist<T0> &arg2);

// find_exist :: "'a\<Rightarrow>'a mylist\<Rightarrow>bool"
template <typename T0>
bool find_exist(const T0 &arg0, const mylist<T0> &arg1);

// find_index :: "mynat\<Rightarrow>'a mylist\<Rightarrow>'a myoption"
template <typename T0>
myoption<T0> find_index(const mynat &arg0, const mylist<T0> &arg1);

// mylist_connect :: "'a mylist \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist"
template <typename T0>
mylist<T0> mylist_connect(const mylist<T0> &arg0, const mylist<T0> &arg1);

// tree_insert :: "'a::linorder\<Rightarrow>'a mytree\<Rightarrow>'a mytree"
template <typename T0>
mytree<T0> tree_insert(const T0 &arg0, const mytree<T0> &arg1);

// find_max :: "'a mytree\<Rightarrow>'a myoption"
template <typename T0>
myoption<T0> find_max(const mytree<T0> &arg0);

// tree_del :: "'a::linorder\<Rightarrow>'a mytree\<Rightarrow>'a mytree"
template <typename T0>
mytree<T0> tree_del(const T0 &arg0, const mytree<T0> &arg1);

// tree_find :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> bool"
template <typename T0>
bool tree_find(const T0 &arg0, const mytree<T0> &arg1);

// tree_inorder_traverse :: "'a mytree\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> tree_inorder_traverse(const mytree<T0> &arg0);

// tree_preorder_traverse :: "'a mytree\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> tree_preorder_traverse(const mytree<T0> &arg0);

// tree_postorder_traverse :: "'a mytree\<Rightarrow>'a mylist"
template <typename T0>
mylist<T0> tree_postorder_traverse(const mytree<T0> &arg0);

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

mynat mul(const mynat &arg0, const mynat &arg1) {
    if (((arg0).tag == mynat::Tag::Zero)) {
        const mynat &n = arg1;
        return mynat::make_Zero();
    }
    if (((arg0).tag == mynat::Tag::Suc)) {
        const mynat &m = (*(arg0.Suc_value0));
        const mynat &n = arg1;
        return add(n, mul(m, n));
    }
    throw std::runtime_error("pattern match failure in mul");
}

template <typename T0>
mylist<T0> push_head(const T0 &arg0, const mylist<T0> &arg1) {
    {
        const T0 &elem = arg0;
        const mylist<T0> &ml = arg1;
        return mylist<T0>::make_Cons(elem, std::make_shared<mylist<T0>>(ml));
    }
    throw std::runtime_error("pattern match failure in push_head");
}

template <typename T0>
mylist<T0> push_tail(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::Nil)) {
        const T0 &elem = arg0;
        return mylist<T0>::make_Cons(elem, std::make_shared<mylist<T0>>(mylist<T0>::make_Nil()));
    }
    if (((arg1).tag == mylist<T0>::Tag::Cons)) {
        const T0 &elem = arg0;
        const T0 &head = (arg1.Cons_value0);
        const mylist<T0> &rest = (*(arg1.Cons_value1));
        return mylist<T0>::make_Cons(head, std::make_shared<mylist<T0>>(push_tail(elem, rest)));
    }
    throw std::runtime_error("pattern match failure in push_tail");
}

template <typename T0>
mylist<T0> del_first(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::Nil)) {
        const T0 &elem = arg0;
        return mylist<T0>::make_Nil();
    }
    if (((arg1).tag == mylist<T0>::Tag::Cons)) {
        const T0 &elem = arg0;
        const T0 &head = (arg1.Cons_value0);
        const mylist<T0> &rest = (*(arg1.Cons_value1));
        return (((elem == head)) ? (rest) : (mylist<T0>::make_Cons(head, std::make_shared<mylist<T0>>(del_first(elem, rest)))));
    }
    throw std::runtime_error("pattern match failure in del_first");
}

template <typename T0>
mylist<T0> del_all(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::Nil)) {
        const T0 &elem = arg0;
        return mylist<T0>::make_Nil();
    }
    if (((arg1).tag == mylist<T0>::Tag::Cons)) {
        const T0 &elem = arg0;
        const T0 &head = (arg1.Cons_value0);
        const mylist<T0> &rest = (*(arg1.Cons_value1));
        return (((elem == head)) ? (del_all(elem, rest)) : (mylist<T0>::make_Cons(head, std::make_shared<mylist<T0>>(del_all(elem, rest)))));
    }
    throw std::runtime_error("pattern match failure in del_all");
}

template <typename T0>
mylist<T0> alter(const mynat &arg0, const T0 &arg1, const mylist<T0> &arg2) {
    if (((arg2).tag == mylist<T0>::Tag::Nil)) {
        const mynat &n = arg0;
        const T0 &elem = arg1;
        return mylist<T0>::make_Nil();
    }
    if (((arg0).tag == mynat::Tag::Zero) && ((arg2).tag == mylist<T0>::Tag::Cons)) {
        const T0 &elem = arg1;
        const T0 &head = (arg2.Cons_value0);
        const mylist<T0> &rest = (*(arg2.Cons_value1));
        return mylist<T0>::make_Cons(elem, std::make_shared<mylist<T0>>(rest));
    }
    if (((arg0).tag == mynat::Tag::Suc) && ((arg2).tag == mylist<T0>::Tag::Cons)) {
        const mynat &n = (*(arg0.Suc_value0));
        const T0 &elem = arg1;
        const T0 &head = (arg2.Cons_value0);
        const mylist<T0> &rest = (*(arg2.Cons_value1));
        return mylist<T0>::make_Cons(head, std::make_shared<mylist<T0>>(alter(n, elem, rest)));
    }
    throw std::runtime_error("pattern match failure in alter");
}

template <typename T0>
bool find_exist(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::Nil)) {
        const T0 &elem = arg0;
        return false;
    }
    if (((arg1).tag == mylist<T0>::Tag::Cons)) {
        const T0 &elem = arg0;
        const T0 &head = (arg1.Cons_value0);
        const mylist<T0> &rest = (*(arg1.Cons_value1));
        return (((elem == head)) ? (true) : (find_exist(elem, rest)));
    }
    throw std::runtime_error("pattern match failure in find_exist");
}

template <typename T0>
myoption<T0> find_index(const mynat &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::Nil)) {
        const mynat &n = arg0;
        return myoption<T0>::make_None();
    }
    if (((arg0).tag == mynat::Tag::Zero) && ((arg1).tag == mylist<T0>::Tag::Cons)) {
        const T0 &head = (arg1.Cons_value0);
        const mylist<T0> &rest = (*(arg1.Cons_value1));
        return myoption<T0>::make_Op(head);
    }
    if (((arg0).tag == mynat::Tag::Suc) && ((arg1).tag == mylist<T0>::Tag::Cons)) {
        const mynat &n = (*(arg0.Suc_value0));
        const T0 &head = (arg1.Cons_value0);
        const mylist<T0> &rest = (*(arg1.Cons_value1));
        return find_index(n, rest);
    }
    throw std::runtime_error("pattern match failure in find_index");
}

template <typename T0>
mylist<T0> mylist_connect(const mylist<T0> &arg0, const mylist<T0> &arg1) {
    if (((arg0).tag == mylist<T0>::Tag::Nil)) {
        const mylist<T0> &anotherlist = arg1;
        return anotherlist;
    }
    if (((arg0).tag == mylist<T0>::Tag::Cons)) {
        const T0 &head = (arg0.Cons_value0);
        const mylist<T0> &rest = (*(arg0.Cons_value1));
        const mylist<T0> &anotherlist = arg1;
        return mylist<T0>::make_Cons(head, std::make_shared<mylist<T0>>(mylist_connect(rest, anotherlist)));
    }
    throw std::runtime_error("pattern match failure in mylist_connect");
}

template <typename T0>
mytree<T0> tree_insert(const T0 &arg0, const mytree<T0> &arg1) {
    if (((arg1).tag == mytree<T0>::Tag::Tip)) {
        const T0 &elem = arg0;
        return mytree<T0>::make_TC(elem, std::make_shared<mytree<T0>>(mytree<T0>::make_Tip()), std::make_shared<mytree<T0>>(mytree<T0>::make_Tip()));
    }
    if (((arg1).tag == mytree<T0>::Tag::TC)) {
        const T0 &elem = arg0;
        const T0 &parent = (arg1.TC_value0);
        const mytree<T0> &left = (*(arg1.TC_value1));
        const mytree<T0> &right = (*(arg1.TC_value2));
        return (((elem == parent)) ? (mytree<T0>::make_TC(parent, std::make_shared<mytree<T0>>(left), std::make_shared<mytree<T0>>(right))) : ((((elem < parent)) ? (mytree<T0>::make_TC(parent, std::make_shared<mytree<T0>>(tree_insert(elem, left)), std::make_shared<mytree<T0>>(right))) : (mytree<T0>::make_TC(parent, std::make_shared<mytree<T0>>(left), std::make_shared<mytree<T0>>(tree_insert(elem, right)))))));
    }
    throw std::runtime_error("pattern match failure in tree_insert");
}

template <typename T0>
myoption<T0> find_max(const mytree<T0> &arg0) {
    if (((arg0).tag == mytree<T0>::Tag::Tip)) {
        return myoption<T0>::make_None();
    }
    if (((arg0).tag == mytree<T0>::Tag::TC) && (((*(arg0.TC_value2))).tag == mytree<T0>::Tag::Tip)) {
        const T0 &parent = (arg0.TC_value0);
        const mytree<T0> &left = (*(arg0.TC_value1));
        return myoption<T0>::make_Op(parent);
    }
    if (((arg0).tag == mytree<T0>::Tag::TC)) {
        const T0 &parent = (arg0.TC_value0);
        const mytree<T0> &left = (*(arg0.TC_value1));
        const mytree<T0> &right = (*(arg0.TC_value2));
        return find_max(right);
    }
    throw std::runtime_error("pattern match failure in find_max");
}

template <typename T0>
mytree<T0> tree_del(const T0 &arg0, const mytree<T0> &arg1) {
    if (((arg1).tag == mytree<T0>::Tag::Tip)) {
        const T0 &elem = arg0;
        return mytree<T0>::make_Tip();
    }
    if (((arg1).tag == mytree<T0>::Tag::TC)) {
        const T0 &elem = arg0;
        const T0 &parent = (arg1.TC_value0);
        const mytree<T0> &left = (*(arg1.TC_value1));
        const mytree<T0> &right = (*(arg1.TC_value2));
        return (((elem < parent)) ? (mytree<T0>::make_TC(parent, std::make_shared<mytree<T0>>(tree_del(elem, left)), std::make_shared<mytree<T0>>(right))) : ((((elem > parent)) ? (mytree<T0>::make_TC(parent, std::make_shared<mytree<T0>>(left), std::make_shared<mytree<T0>>(tree_del(elem, right)))) : (([&]{ auto caseTemp0 = left; if (((caseTemp0).tag == std::remove_reference_t<decltype(caseTemp0)>::Tag::Tip)) {
    return right;
}
if (true) {
    return ([&]{ auto caseTemp1 = find_max(left); if (((caseTemp1).tag == std::remove_reference_t<decltype(caseTemp1)>::Tag::None)) {
    return right;
}
if (((caseTemp1).tag == std::remove_reference_t<decltype(caseTemp1)>::Tag::Op)) {
    const T0 &leftmax = (caseTemp1.Op_value0);
    return mytree<T0>::make_TC(leftmax, std::make_shared<mytree<T0>>(tree_del(leftmax, left)), std::make_shared<mytree<T0>>(right));
}
    throw std::runtime_error("case expression did not match");
}());
}
    throw std::runtime_error("case expression did not match");
}())))));
    }
    throw std::runtime_error("pattern match failure in tree_del");
}

template <typename T0>
bool tree_find(const T0 &arg0, const mytree<T0> &arg1) {
    if (((arg1).tag == mytree<T0>::Tag::Tip)) {
        const T0 &elem = arg0;
        return false;
    }
    if (((arg1).tag == mytree<T0>::Tag::TC)) {
        const T0 &elem = arg0;
        const T0 &parent = (arg1.TC_value0);
        const mytree<T0> &left = (*(arg1.TC_value1));
        const mytree<T0> &right = (*(arg1.TC_value2));
        return (((elem == parent)) ? (true) : ((((elem < parent)) ? (tree_find(elem, left)) : (tree_find(elem, right)))));
    }
    throw std::runtime_error("pattern match failure in tree_find");
}

template <typename T0>
mylist<T0> tree_inorder_traverse(const mytree<T0> &arg0) {
    if (((arg0).tag == mytree<T0>::Tag::Tip)) {
        return mylist<T0>::make_Nil();
    }
    if (((arg0).tag == mytree<T0>::Tag::TC)) {
        const T0 &parent = (arg0.TC_value0);
        const mytree<T0> &left = (*(arg0.TC_value1));
        const mytree<T0> &right = (*(arg0.TC_value2));
        return mylist_connect(tree_inorder_traverse(left), push_head(parent, tree_inorder_traverse(right)));
    }
    throw std::runtime_error("pattern match failure in tree_inorder_traverse");
}

template <typename T0>
mylist<T0> tree_preorder_traverse(const mytree<T0> &arg0) {
    if (((arg0).tag == mytree<T0>::Tag::Tip)) {
        return mylist<T0>::make_Nil();
    }
    if (((arg0).tag == mytree<T0>::Tag::TC)) {
        const T0 &parent = (arg0.TC_value0);
        const mytree<T0> &left = (*(arg0.TC_value1));
        const mytree<T0> &right = (*(arg0.TC_value2));
        return push_head(parent, mylist_connect(tree_preorder_traverse(left), tree_preorder_traverse(right)));
    }
    throw std::runtime_error("pattern match failure in tree_preorder_traverse");
}

template <typename T0>
mylist<T0> tree_postorder_traverse(const mytree<T0> &arg0) {
    if (((arg0).tag == mytree<T0>::Tag::Tip)) {
        return mylist<T0>::make_Nil();
    }
    if (((arg0).tag == mytree<T0>::Tag::TC)) {
        const T0 &parent = (arg0.TC_value0);
        const mytree<T0> &left = (*(arg0.TC_value1));
        const mytree<T0> &right = (*(arg0.TC_value2));
        return push_tail(parent, mylist_connect(tree_postorder_traverse(left), tree_postorder_traverse(right)));
    }
    throw std::runtime_error("pattern match failure in tree_postorder_traverse");
}

mytree<int> wrapper_tree_insert(int value, const mytree<int> &tree) { return tree_insert(value, tree); }

// ==================== 测试辅助函数 ====================

// 打印 mynat
int to_int(const mynat &n) {
    if (is_mynat_Zero(n)) return 0;
    return 1 + to_int(get_mynat_Suc_value0(n));
}

void print_mynat(const mynat &n) {
    cout << to_int(n);
}

// 打印 mylist
template<typename T>
void print_mylist(const mylist<T> &lst) {
    cout << "[";
    const mylist<T>* cur = &lst;
    bool first = true;
    while (!is_mylist_Nil(*cur)) {
        if (!first) cout << ", ";
        cout << cur->Cons_value0;
        first = false;
        cur = cur->Cons_value1.get();
    }
    cout << "]";
}

// 打印 mytree (中序遍历)
template<typename T>
void print_mytree(const mytree<T> &t) {
    if (is_mytree_Tip(t)) return;
    print_mytree(*t.TC_value1);
    cout << t.TC_value0 << " ";
    print_mytree(*t.TC_value2);
}


int main() {

    cout << "========= Test mynat =========\n";
    mynat n2 = make_mynat_Suc(make_mynat_Suc(make_mynat_Zero())); // 2
    mynat n3 = make_mynat_Suc(make_mynat_Suc(make_mynat_Suc(make_mynat_Zero()))); // 3

    cout << "2 + 3 = ";
    print_mynat(add(n2, n3));      // 应为 5
    cout << endl;

    cout << "2 * 3 = ";
    print_mynat(mul(n2, n3));      // 应为 6
    cout << endl;


    cout << "\n========= Test mylist =========\n";
    mylist<int> lst = make_mylist_Cons(1, make_mylist_Cons(2, make_mylist_Cons(3, make_mylist_Nil<int>())));

    cout << "Original list: ";
    print_mylist(lst); cout << endl;

    cout << "push_head(0): ";
    auto lst2 = push_head(0, lst);
    print_mylist(lst2); cout << endl;

    cout << "push_tail(4): ";
    auto lst3 = push_tail(4, lst);
    print_mylist(lst3); cout << endl;

    cout << "del_first(2): ";
    auto lst4 = del_first(2, lst);
    print_mylist(lst4); cout << endl;

    cout << "del_all(3): ";
    auto lst5 = del_all(3, lst);
    print_mylist(lst5); cout << endl;

    cout << "find_exist(2): ";
    cout << find_exist(2, lst) << endl;

    cout << "alter(1, 99): ";
    auto lst6 = alter(make_mynat_Suc(make_mynat_Zero()), 99, lst);  // index 1 替换为 99
    print_mylist(lst6); cout << endl;

    cout << "find_index(2): ";
    auto res = find_index(make_mynat_Suc(make_mynat_Suc(make_mynat_Zero())), lst);
    if (is_myoption_Op(res)) cout << get_myoption_Op_value0(res);
    else cout << "None";
    cout << endl;

    cout << "connect lst ++ lst: ";
    auto lst7 = mylist_connect(lst, lst);
    print_mylist(lst7); cout << endl;


    cout << "\n========= Test mytree =========\n";
    mytree<int> t = make_mytree_Tip<int>();
    t = tree_insert(3, t);
    t = tree_insert(1, t);
    t = tree_insert(4, t);
    t = tree_insert(2, t);

    cout << "In-order traversal: ";
    print_mytree(t); cout << endl;

    cout << "tree_find(3): " << tree_find(3, t) << endl;
    cout << "tree_find(5): " << tree_find(5, t) << endl;

    cout << "delete 3, inorder: ";
    t = tree_del(3, t);
    print_mytree(t); cout << endl;

    cout << "Finished tests.\n";
    return 0;
}

