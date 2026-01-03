theory MyStruct
  imports Main
begin

datatype mynat = Zero | Suc mynat

datatype 'a myoption = None | Op "'a"

fun add :: "mynat\<Rightarrow>mynat\<Rightarrow>mynat" where
  "add Zero n = n"
| "add (Suc m) n = Suc (add m n)"

fun mul :: "mynat\<Rightarrow>mynat\<Rightarrow>mynat" where
  "mul Zero n = Zero"
| "mul (Suc m) n = add n (mul m n)"

datatype 'a mylist = Nil | Cons "'a" "'a mylist"

fun push_head :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist" where
  "push_head elem ml = Cons elem ml"

fun push_tail :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist" where
  "push_tail elem Nil = Cons elem Nil"
| "push_tail elem (Cons head rest) = Cons head (push_tail elem rest)"

fun del_first :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist" where
  "del_first elem Nil = Nil"
| "del_first elem (Cons head rest) = (
    if elem = head then rest 
    else Cons head (del_first elem rest))"

fun del_all :: "'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist" where
  "del_all elem Nil = Nil"
| "del_all elem (Cons head rest) = (
    if elem = head then del_all elem rest 
    else Cons head (del_all elem rest))"

fun alter :: "mynat\<Rightarrow>'a\<Rightarrow>'a mylist\<Rightarrow>'a mylist" where
  "alter n elem Nil = Nil"
| "alter Zero elem (Cons head rest) = Cons elem rest"
| "alter (Suc n) elem (Cons head rest) = Cons head (alter n elem rest)"

fun find_exist :: "'a\<Rightarrow>'a mylist\<Rightarrow>bool" where
  "find_exist elem Nil = False"
| "find_exist elem (Cons head rest) = (if elem = head then True else find_exist elem rest)" 

fun find_index :: "mynat\<Rightarrow>'a mylist\<Rightarrow>'a myoption" where
  "find_index n Nil = None"
| "find_index Zero (Cons head rest) = Op head"
| "find_index (Suc n) (Cons head rest) = find_index n rest"

fun mylist_connect :: "'a mylist \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist" where
  "mylist_connect Nil anotherlist = anotherlist"
| "mylist_connect (Cons head rest) anotherlist = Cons head (mylist_connect rest anotherlist)"

datatype 'a mytree = Tip | TC "'a" "'a mytree" "'a mytree"

fun tree_insert :: "'a::linorder\<Rightarrow>'a mytree\<Rightarrow>'a mytree" where
  "tree_insert elem Tip = TC elem Tip Tip"
| "tree_insert elem (TC parent left right) = (
    if elem = parent then TC parent left right
    else if elem < parent then TC parent (tree_insert elem left) right
    else TC parent left (tree_insert elem right))"

fun find_max :: "'a mytree\<Rightarrow>'a myoption" where
  "find_max Tip = None"
| "find_max (TC parent left Tip) = Op parent"
| "find_max (TC parent left right) = find_max right"

fun tree_del :: "'a::linorder\<Rightarrow>'a mytree\<Rightarrow>'a mytree" where
  "tree_del elem Tip = Tip"
| "tree_del elem (TC parent left right) = (
    if elem < parent then TC parent (tree_del elem left) right
    else if elem > parent then TC parent left (tree_del elem right)
    else (
      case left of 
        Tip \<Rightarrow> right
      | _ \<Rightarrow> (
          case find_max left of
            None \<Rightarrow> right
          | Op leftmax \<Rightarrow> TC leftmax (tree_del leftmax left) right
        )
    )
  )"

fun tree_find :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> bool" where
  "tree_find elem Tip = False"
  | "tree_find elem (TC parent left right) = (
    if elem = parent then True 
    else if elem < parent then tree_find elem left 
    else tree_find elem right)"

fun tree_inorder_traverse :: "'a mytree\<Rightarrow>'a mylist" where
  "tree_inorder_traverse Tip = Nil"
| "tree_inorder_traverse (TC parent left right) = 
  mylist_connect (tree_inorder_traverse left) (push_head parent (tree_inorder_traverse right))"

fun tree_preorder_traverse :: "'a mytree\<Rightarrow>'a mylist" where
  "tree_preorder_traverse Tip = Nil"
| "tree_preorder_traverse (TC parent left right) = 
  push_head parent (mylist_connect (tree_preorder_traverse left) (tree_preorder_traverse right))"

fun tree_postorder_traverse :: "'a mytree\<Rightarrow>'a mylist" where
  "tree_postorder_traverse Tip = Nil"
| "tree_postorder_traverse (TC parent left right) = 
  push_tail parent (mylist_connect (tree_postorder_traverse left) (tree_postorder_traverse right))"

end
