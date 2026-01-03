#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

// auto-generated from theory test

template <typename T>
inline T isabelle_raise_undefined(const char *context)
{
    throw std::runtime_error(context ? context : "Isabelle undefined value");
}

// datatype mynat
struct mynat {
    enum class Tag {
        myzero,
        mysuc
    };
    Tag tag;
    std::shared_ptr<mynat> mysuc_value0;
    bool is_myzero() const { return tag == Tag::myzero; }
    bool is_mysuc() const { return tag == Tag::mysuc; }

    static mynat make_myzero() {
        mynat value;
        value.tag = Tag::myzero;
        return value;
    }
    static mynat make_mysuc(const std::shared_ptr<mynat> &value0) {
        mynat value;
        value.tag = Tag::mysuc;
        value.mysuc_value0 = value0;
        return value;
    }

    // Equality operator
    friend bool operator==(const mynat &lhs, const mynat &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::mysuc) {
            if (!(*lhs.mysuc_value0 == *rhs.mysuc_value0)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mynat &lhs, const mynat &rhs) {
        return !(lhs == rhs);
    }
};

inline mynat make_mynat_myzero() {
    return mynat::make_myzero();
}

inline mynat make_mynat_mysuc(const mynat &value0) {
    return mynat::make_mysuc(std::make_shared<mynat>(value0));
}

inline bool is_mynat_myzero(const mynat &value) {
    return value.tag == mynat::Tag::myzero;
}

inline bool is_mynat_mysuc(const mynat &value) {
    return value.tag == mynat::Tag::mysuc;
}

inline const mynat &get_mynat_mysuc_value0(const mynat &value) {
    return *(value.mysuc_value0);
}

// datatype mylist
template <typename T0>
struct mylist {
    enum class Tag {
        mynil,
        mycons
    };
    Tag tag;
    T0 mycons_value0;
    std::shared_ptr<mylist<T0>> mycons_value1;
    bool is_mynil() const { return tag == Tag::mynil; }
    bool is_mycons() const { return tag == Tag::mycons; }

    static mylist make_mynil() {
        mylist value;
        value.tag = Tag::mynil;
        return value;
    }
    static mylist make_mycons(const T0 &value0, const std::shared_ptr<mylist<T0>> &value1) {
        mylist value;
        value.tag = Tag::mycons;
        value.mycons_value0 = value0;
        value.mycons_value1 = value1;
        return value;
    }

    // Equality operator
    friend bool operator==(const mylist &lhs, const mylist &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::mycons) {
            if (!(lhs.mycons_value0 == rhs.mycons_value0)) return false;
            if (!(*lhs.mycons_value1 == *rhs.mycons_value1)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mylist &lhs, const mylist &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline mylist<T0> make_mylist_mynil() {
    return mylist<T0>::make_mynil();
}

template <typename T0>
inline mylist<T0> make_mylist_mycons(const T0 &value0, const mylist<T0> &value1) {
    return mylist<T0>::make_mycons(value0, std::make_shared<mylist<T0>>(value1));
}

template <typename T0>
inline bool is_mylist_mynil(const mylist<T0> &value) {
    return value.tag == mylist<T0>::Tag::mynil;
}

template <typename T0>
inline bool is_mylist_mycons(const mylist<T0> &value) {
    return value.tag == mylist<T0>::Tag::mycons;
}

template <typename T0>
inline const T0 &get_mylist_mycons_value0(const mylist<T0> &value) {
    return value.mycons_value0;
}

template <typename T0>
inline const mylist<T0> &get_mylist_mycons_value1(const mylist<T0> &value) {
    return *(value.mycons_value1);
}

// datatype myoption
template <typename T0>
struct myoption {
    enum class Tag {
        mynone,
        mysome
    };
    Tag tag;
    T0 mysome_value0;
    bool is_mynone() const { return tag == Tag::mynone; }
    bool is_mysome() const { return tag == Tag::mysome; }

    static myoption make_mynone() {
        myoption value;
        value.tag = Tag::mynone;
        return value;
    }
    static myoption make_mysome(const T0 &value0) {
        myoption value;
        value.tag = Tag::mysome;
        value.mysome_value0 = value0;
        return value;
    }

    // Equality operator
    friend bool operator==(const myoption &lhs, const myoption &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::mysome) {
            if (!(lhs.mysome_value0 == rhs.mysome_value0)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const myoption &lhs, const myoption &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline myoption<T0> make_myoption_mynone() {
    return myoption<T0>::make_mynone();
}

template <typename T0>
inline myoption<T0> make_myoption_mysome(const T0 &value0) {
    return myoption<T0>::make_mysome(value0);
}

template <typename T0>
inline bool is_myoption_mynone(const myoption<T0> &value) {
    return value.tag == myoption<T0>::Tag::mynone;
}

template <typename T0>
inline bool is_myoption_mysome(const myoption<T0> &value) {
    return value.tag == myoption<T0>::Tag::mysome;
}

template <typename T0>
inline const T0 &get_myoption_mysome_value0(const myoption<T0> &value) {
    return value.mysome_value0;
}

// datatype mytree
template <typename T0>
struct mytree {
    enum class Tag {
        tip,
        treenode
    };
    Tag tag;
    T0 treenode_value0;
    std::shared_ptr<mytree<T0>> treenode_value1;
    std::shared_ptr<mytree<T0>> treenode_value2;
    bool is_tip() const { return tag == Tag::tip; }
    bool is_treenode() const { return tag == Tag::treenode; }

    static mytree make_tip() {
        mytree value;
        value.tag = Tag::tip;
        return value;
    }
    static mytree make_treenode(const T0 &value0, const std::shared_ptr<mytree<T0>> &value1, const std::shared_ptr<mytree<T0>> &value2) {
        mytree value;
        value.tag = Tag::treenode;
        value.treenode_value0 = value0;
        value.treenode_value1 = value1;
        value.treenode_value2 = value2;
        return value;
    }

    // Equality operator
    friend bool operator==(const mytree &lhs, const mytree &rhs) {
        if (lhs.tag != rhs.tag) return false;
        if (lhs.tag == Tag::treenode) {
            if (!(lhs.treenode_value0 == rhs.treenode_value0)) return false;
            if (!(*lhs.treenode_value1 == *rhs.treenode_value1)) return false;
            if (!(*lhs.treenode_value2 == *rhs.treenode_value2)) return false;
            return true;
        }
        return true;
    }
    friend bool operator!=(const mytree &lhs, const mytree &rhs) {
        return !(lhs == rhs);
    }
};

template <typename T0>
inline mytree<T0> make_mytree_tip() {
    return mytree<T0>::make_tip();
}

template <typename T0>
inline mytree<T0> make_mytree_treenode(const T0 &value0, const mytree<T0> &value1, const mytree<T0> &value2) {
    return mytree<T0>::make_treenode(value0, std::make_shared<mytree<T0>>(value1), std::make_shared<mytree<T0>>(value2));
}

template <typename T0>
inline bool is_mytree_tip(const mytree<T0> &value) {
    return value.tag == mytree<T0>::Tag::tip;
}

template <typename T0>
inline bool is_mytree_treenode(const mytree<T0> &value) {
    return value.tag == mytree<T0>::Tag::treenode;
}

template <typename T0>
inline const T0 &get_mytree_treenode_value0(const mytree<T0> &value) {
    return value.treenode_value0;
}

template <typename T0>
inline const mytree<T0> &get_mytree_treenode_value1(const mytree<T0> &value) {
    return *(value.treenode_value1);
}

template <typename T0>
inline const mytree<T0> &get_mytree_treenode_value2(const mytree<T0> &value) {
    return *(value.treenode_value2);
}

// function declarations
// myadd :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat"
mynat myadd(const mynat &arg0, const mynat &arg1);

// mymult :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat"
mynat mymult(const mynat &arg0, const mynat &arg1);

// insert_head :: "'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist"
template <typename T0>
mylist<T0> insert_head(const T0 &arg0, const mylist<T0> &arg1);

// insert_tail :: "'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist"
template <typename T0>
mylist<T0> insert_tail(const T0 &arg0, const mylist<T0> &arg1);

// mydelete :: "'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist"
template <typename T0>
mylist<T0> mydelete(const T0 &arg0, const mylist<T0> &arg1);

// myupdate :: "mynat \<Rightarrow> 'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist"
template <typename T0>
mylist<T0> myupdate(const mynat &arg0, const T0 &arg1, const mylist<T0> &arg2);

// myget :: "mynat \<Rightarrow> 'a mylist \<Rightarrow> 'a myoption"
template <typename T0>
myoption<T0> myget(const mynat &arg0, const mylist<T0> &arg1);

// myfind :: "'a  \<Rightarrow> 'a mylist \<Rightarrow> bool"
template <typename T0>
bool myfind(const T0 &arg0, const mylist<T0> &arg1);

// tree_insert :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> 'a mytree"
template <typename T0>
mytree<T0> tree_insert(const T0 &arg0, const mytree<T0> &arg1);

// tree_search :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> 'a myoption"
template <typename T0>
myoption<T0> tree_search(const T0 &arg0, const mytree<T0> &arg1);

// tree_min :: "'a::linorder mytree \<Rightarrow>'a"
template <typename T0>
T0 tree_min(const mytree<T0> &arg0);

// tree_delete :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> 'a mytree"
template <typename T0>
mytree<T0> tree_delete(const T0 &arg0, const mytree<T0> &arg1);

// function implementations converted from Isabelle/HOL

mynat myadd(const mynat &arg0, const mynat &arg1) {
    if (((arg1).tag == mynat::Tag::myzero)) {
        const mynat &n = arg0;
        return n;
    }
    if (((arg1).tag == mynat::Tag::mysuc)) {
        const mynat &n = arg0;
        const mynat &m = (*(arg1.mysuc_value0));
        return mynat::make_mysuc(std::make_shared<mynat>(myadd(n, m)));
    }
    throw std::runtime_error("pattern match failure in myadd");
}

mynat mymult(const mynat &arg0, const mynat &arg1) {
    if (((arg1).tag == mynat::Tag::myzero)) {
        const mynat &n = arg0;
        return mynat::make_myzero();
    }
    if (((arg1).tag == mynat::Tag::mysuc)) {
        const mynat &n = arg0;
        const mynat &m = (*(arg1.mysuc_value0));
        return myadd(mymult(n, m), n);
    }
    throw std::runtime_error("pattern match failure in mymult");
}

template <typename T0>
mylist<T0> insert_head(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::mynil)) {
        const T0 &x = arg0;
        return mylist<T0>::make_mycons(x, std::make_shared<mylist<T0>>(mylist<T0>::make_mynil()));
    }
    if (((arg1).tag == mylist<T0>::Tag::mycons)) {
        const T0 &x = arg0;
        const T0 &h = (arg1.mycons_value0);
        const mylist<T0> &list = (*(arg1.mycons_value1));
        return mylist<T0>::make_mycons(x, std::make_shared<mylist<T0>>(mylist<T0>::make_mycons(h, std::make_shared<mylist<T0>>(list))));
    }
    throw std::runtime_error("pattern match failure in insert_head");
}

template <typename T0>
mylist<T0> insert_tail(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::mynil)) {
        const T0 &x = arg0;
        return mylist<T0>::make_mycons(x, std::make_shared<mylist<T0>>(mylist<T0>::make_mynil()));
    }
    if (((arg1).tag == mylist<T0>::Tag::mycons)) {
        const T0 &x = arg0;
        const T0 &h = (arg1.mycons_value0);
        const mylist<T0> &list = (*(arg1.mycons_value1));
        return mylist<T0>::make_mycons(h, std::make_shared<mylist<T0>>(insert_tail(x, list)));
    }
    throw std::runtime_error("pattern match failure in insert_tail");
}

template <typename T0>
mylist<T0> mydelete(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::mynil)) {
        const T0 &x = arg0;
        return mylist<T0>::make_mynil();
    }
    if (((arg1).tag == mylist<T0>::Tag::mycons)) {
        const T0 &x = arg0;
        const T0 &h = (arg1.mycons_value0);
        const mylist<T0> &list = (*(arg1.mycons_value1));
        return (((x == h)) ? (list) : (mylist<T0>::make_mycons(h, std::make_shared<mylist<T0>>(mydelete(x, list)))));
    }
    throw std::runtime_error("pattern match failure in mydelete");
}

template <typename T0>
mylist<T0> myupdate(const mynat &arg0, const T0 &arg1, const mylist<T0> &arg2) {
    if (((arg2).tag == mylist<T0>::Tag::mynil)) {
        return mylist<T0>::make_mynil();
    }
    if (((arg0).tag == mynat::Tag::myzero) && ((arg2).tag == mylist<T0>::Tag::mycons)) {
        const T0 &new_value = arg1;
        const T0 &h = (arg2.mycons_value0);
        const mylist<T0> &list = (*(arg2.mycons_value1));
        return mylist<T0>::make_mycons(new_value, std::make_shared<mylist<T0>>(list));
    }
    if (((arg0).tag == mynat::Tag::mysuc) && ((arg2).tag == mylist<T0>::Tag::mycons)) {
        const mynat &n = (*(arg0.mysuc_value0));
        const T0 &new_value = arg1;
        const T0 &h = (arg2.mycons_value0);
        const mylist<T0> &list = (*(arg2.mycons_value1));
        return mylist<T0>::make_mycons(h, std::make_shared<mylist<T0>>(myupdate(n, new_value, list)));
    }
    throw std::runtime_error("pattern match failure in myupdate");
}

template <typename T0>
myoption<T0> myget(const mynat &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::mynil)) {
        return myoption<T0>::make_mynone();
    }
    if (((arg0).tag == mynat::Tag::myzero) && ((arg1).tag == mylist<T0>::Tag::mycons)) {
        const T0 &h = (arg1.mycons_value0);
        const mylist<T0> &list = (*(arg1.mycons_value1));
        return myoption<T0>::make_mysome(h);
    }
    if (((arg0).tag == mynat::Tag::mysuc) && ((arg1).tag == mylist<T0>::Tag::mycons)) {
        const mynat &n = (*(arg0.mysuc_value0));
        const T0 &h = (arg1.mycons_value0);
        const mylist<T0> &list = (*(arg1.mycons_value1));
        return myget(n, list);
    }
    throw std::runtime_error("pattern match failure in myget");
}

template <typename T0>
bool myfind(const T0 &arg0, const mylist<T0> &arg1) {
    if (((arg1).tag == mylist<T0>::Tag::mynil)) {
        const T0 &x = arg0;
        return false;
    }
    if (((arg1).tag == mylist<T0>::Tag::mycons)) {
        const T0 &x = arg0;
        const T0 &h = (arg1.mycons_value0);
        const mylist<T0> &list = (*(arg1.mycons_value1));
        return (((h == x)) ? (true) : (myfind(x, list)));
    }
    throw std::runtime_error("pattern match failure in myfind");
}

template <typename T0>
mytree<T0> tree_insert(const T0 &arg0, const mytree<T0> &arg1) {
    if (((arg1).tag == mytree<T0>::Tag::tip)) {
        const T0 &x = arg0;
        return mytree<T0>::make_treenode(x, std::make_shared<mytree<T0>>(mytree<T0>::make_tip()), std::make_shared<mytree<T0>>(mytree<T0>::make_tip()));
    }
    if (((arg1).tag == mytree<T0>::Tag::treenode)) {
        const T0 &x = arg0;
        const T0 &root = (arg1.treenode_value0);
        const mytree<T0> &left = (*(arg1.treenode_value1));
        const mytree<T0> &right = (*(arg1.treenode_value2));
        return (((x == root)) ? (mytree<T0>::make_treenode(root, std::make_shared<mytree<T0>>(left), std::make_shared<mytree<T0>>(right))) : ((((x < root)) ? (mytree<T0>::make_treenode(root, std::make_shared<mytree<T0>>(tree_insert(x, left)), std::make_shared<mytree<T0>>(right))) : (mytree<T0>::make_treenode(root, std::make_shared<mytree<T0>>(left), std::make_shared<mytree<T0>>(tree_insert(x, right)))))));
    }
    throw std::runtime_error("pattern match failure in tree_insert");
}

template <typename T0>
myoption<T0> tree_search(const T0 &arg0, const mytree<T0> &arg1) {
    if (((arg1).tag == mytree<T0>::Tag::tip)) {
        const T0 &x = arg0;
        return myoption<T0>::make_mynone();
    }
    if (((arg1).tag == mytree<T0>::Tag::treenode)) {
        const T0 &x = arg0;
        const T0 &root = (arg1.treenode_value0);
        const mytree<T0> &left = (*(arg1.treenode_value1));
        const mytree<T0> &right = (*(arg1.treenode_value2));
        return (((x == root)) ? (myoption<T0>::make_mysome(root)) : ((((x < root)) ? (tree_search(x, left)) : (tree_search(x, right)))));
    }
    throw std::runtime_error("pattern match failure in tree_search");
}

template <typename T0>
T0 tree_min(const mytree<T0> &arg0) {
    if (((arg0).tag == mytree<T0>::Tag::treenode) && (((*(arg0.treenode_value1))).tag == mytree<T0>::Tag::tip)) {
        const T0 &x = (arg0.treenode_value0);
        return x;
    }
    if (((arg0).tag == mytree<T0>::Tag::treenode)) {
        const T0 &x = (arg0.treenode_value0);
        const mytree<T0> &left = (*(arg0.treenode_value1));
        return tree_min(left);
    }
    if (((arg0).tag == mytree<T0>::Tag::tip)) {
        return isabelle_raise_undefined<T0>("undefined");
    }
    throw std::runtime_error("pattern match failure in tree_min");
}

template <typename T0>
mytree<T0> tree_delete(const T0 &arg0, const mytree<T0> &arg1) {
    if (((arg1).tag == mytree<T0>::Tag::tip)) {
        return mytree<T0>::make_tip();
    }
    if (((arg1).tag == mytree<T0>::Tag::treenode)) {
        const T0 &x = arg0;
        const T0 &root = (arg1.treenode_value0);
        const mytree<T0> &left = (*(arg1.treenode_value1));
        const mytree<T0> &right = (*(arg1.treenode_value2));
        return (((x < root)) ? (mytree<T0>::make_treenode(root, std::make_shared<mytree<T0>>(tree_delete(x, left)), std::make_shared<mytree<T0>>(right))) : ((((x > root)) ? (mytree<T0>::make_treenode(root, std::make_shared<mytree<T0>>(left), std::make_shared<mytree<T0>>(tree_delete(x, right)))) : ((((((left).tag == std::remove_reference_t<decltype((left))>::Tag::tip) && ((right).tag == std::remove_reference_t<decltype((right))>::Tag::tip))) ? (mytree<T0>::make_tip()) : (((((left).tag == std::remove_reference_t<decltype((left))>::Tag::tip)) ? (right) : (((((right).tag == std::remove_reference_t<decltype((right))>::Tag::tip)) ? (left) : (([&]{ auto min_value = tree_min(right); return mytree<T0>::make_treenode(min_value, std::make_shared<mytree<T0>>(left), std::make_shared<mytree<T0>>(tree_delete(min_value, right))); }())))))))))));
    }
    throw std::runtime_error("pattern match failure in tree_delete");
}

mynat wrapper_add(const mynat &a, const mynat &b) { return myadd(a, b); }

mynat wrapper_mul(const mynat &a, const mynat &b) { return mymult(a, b); }

template <typename T> mylist<T> wrapper_list_insert_head(const T &value, const mylist<T> &lst) { return insert_head(value, lst); }

template <typename T> mylist<T> wrapper_list_insert_tail(const T &value, const mylist<T> &lst) { return insert_tail(value, lst); }

template <typename T> mylist<T> wrapper_list_delete(const T &value, const mylist<T> &lst) { return mydelete(value, lst); }

template <typename T> mylist<T> wrapper_list_update(const mynat &index, const T &value, const mylist<T> &lst) { return myupdate(index, value, lst); }

template <typename T> myoption<T> wrapper_list_get(const mynat &index, const mylist<T> &lst) { return myget(index, lst); }

template <typename T> bool wrapper_list_find(const T &value, const mylist<T> &lst) { return myfind(value, lst); }

mytree<int> wrapper_tree_insert(int value, const mytree<int> &tree) { return tree_insert(value, tree); }

myoption<int> wrapper_tree_search(int value, const mytree<int> &tree) { return tree_search(value, tree); }

int wrapper_tree_min(const mytree<int> &tree) { return tree_min(tree); }

mytree<int> wrapper_tree_delete(int value, const mytree<int> &tree) { return tree_delete(value, tree); }

// ======== 辅助打印函数 ========

// mynat 转 int
int to_int(const mynat &n) {
    if (is_mynat_myzero(n)) return 0;
    return 1 + to_int(get_mynat_mysuc_value0(n));
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
    while (!is_mylist_mynil(*cur)) {
        if (!first) cout << ", ";
        cout << cur->mycons_value0;
        first = false;
        cur = cur->mycons_value1.get();
    }
    cout << "]";
}

// 中序打印 mytree
template<typename T>
void print_mytree(const mytree<T> &t) {
    if (is_mytree_tip(t)) return;
    print_mytree(*t.treenode_value1);
    cout << t.treenode_value0 << " ";
    print_mytree(*t.treenode_value2);
}



// ========== MAIN ==========

int main() {

    cout << "==================== MYNAT TEST ====================\n";
    mynat n0 = make_mynat_myzero();
    mynat n1 = make_mynat_mysuc(n0);
    mynat n2 = make_mynat_mysuc(n1);
    mynat n3 = make_mynat_mysuc(n2);

    cout << "n2 = "; print_mynat(n2); cout << "\n";
    cout << "n3 = "; print_mynat(n3); cout << "\n";

    cout << "myadd(2, 3) = ";
    print_mynat(myadd(n2, n3));
    cout << "\n";

    cout << "mymult(2, 3) = ";
    print_mynat(mymult(n2, n3));
    cout << "\n\n";



    cout << "==================== MYLIST TEST ====================\n";

    mylist<int> lst = make_mylist_mynil<int>();
    lst = insert_head(3, lst);
    lst = insert_head(2, lst);
    lst = insert_head(1, lst);

    cout << "Initial list: ";
    print_mylist(lst); cout << "\n";

    auto lst2 = insert_tail(4, lst);
    cout << "After insert_tail(4): ";
    print_mylist(lst2); cout << "\n";

    auto lst3 = mydelete(2, lst2);
    cout << "After mydelete(2): ";
    print_mylist(lst3); cout << "\n";

    auto lst4 = myupdate(n1, 99, lst);  // index 1 替换为 99
    cout << "After myupdate(1, 99): ";
    print_mylist(lst4); cout << "\n";

    auto opt = myget(n2, lst);  // index 2
    cout << "myget(2, lst) = ";
    if (is_myoption_mysome(opt)) cout << get_myoption_mysome_value0(opt);
    else cout << "None";
    cout << "\n";

    cout << "myfind(3, lst) = " << (myfind(3, lst) ? "true" : "false") << "\n\n";



    cout << "==================== MYTREE TEST ====================\n";

    mytree<int> t = make_mytree_tip<int>();
    t = tree_insert(5, t);
    t = tree_insert(2, t);
    t = tree_insert(8, t);
    t = tree_insert(1, t);
    t = tree_insert(3, t);

    cout << "Tree inorder: ";
    print_mytree(t);
    cout << "\n";

    cout << "tree_search(3) = ";
    auto r = tree_search(3, t);
    if (is_myoption_mysome(r)) cout << get_myoption_mysome_value0(r);
    else cout << "None";
    cout << "\n";

    cout << "tree_min = " << tree_min(t) << "\n";

    t = tree_delete(2, t);
    cout << "Tree after delete(2): ";
    print_mytree(t);
    cout << "\n";

    cout << "==================== END ====================\n";

    return 0;
}
