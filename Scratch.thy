theory Scratch
imports Main 
begin  

datatype mynat = Zero | Suc mynat

fun add :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat" where
  "add Zero n = n"
| "add (Suc m) n = Suc (add m n)"

fun minus :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat" where
  "minus m Zero = m"
| "minus Zero m = Zero"
| "minus (Suc m) (Suc n) = minus m n"

fun mult :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat" where
  "mult Zero n = Zero"
| "mult (Suc m) n = add n (mult m n)"

datatype mynat_list = Nil | Cons mynat mynat_list

fun insert_nat_head :: "mynat \<Rightarrow> mynat_list \<Rightarrow> mynat_list" where
  "insert_nat_head x l = Cons x l"

fun delete_nat :: "mynat \<Rightarrow> mynat_list \<Rightarrow> mynat_list" where
  "delete_nat x Nil = Nil"
| "delete_nat x (Cons y l) = (if x = y then l else Cons y (delete_nat x l))"

fun update_nat :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat_list \<Rightarrow> mynat_list" where
  "update_nat old new Nil = Nil"
| "update_nat old new (Cons y l) = (if y = old then Cons new l else Cons y (update_nat old new l))"

fun search_nat :: "mynat \<Rightarrow> mynat_list \<Rightarrow> bool" where
  "search_nat x Nil = False"
| "search_nat x (Cons y l) = (x = y \<or> search_nat x l)"

fun less :: "mynat \<Rightarrow> mynat \<Rightarrow> bool" where
  "less Zero (Suc n) = True"
| "less Zero Zero = False"
| "less (Suc m) Zero = False"
| "less (Suc m) (Suc n) = less m n"

datatype mynat_tree = Leaf | Node mynat mynat_tree mynat_tree

fun insert_nat_tree :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> mynat_tree" where
  "insert_nat_tree x Leaf = Node x Leaf Leaf"
| "insert_nat_tree x (Node y l r) = 
    (if less x y then Node y (insert_nat_tree x l) r 
     else if less y x then Node y l (insert_nat_tree x r) 
     else Node y l r)"  

fun search_nat_tree :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> bool" where
  "search_nat_tree x Leaf = False"
| "search_nat_tree x (Node y l r) = 
    (x = y \<or> (if less x y then search_nat_tree x l else search_nat_tree x r))"


fun find_min_opt :: "mynat_tree \<Rightarrow> mynat option" where
  "find_min_opt Leaf = None"  
| "find_min_opt (Node x Leaf _) = Some x" 
| "find_min_opt (Node _ l _) = find_min_opt l"


fun find_nat_tree_opt :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> mynat option" where
  "find_nat_tree_opt x Leaf = None" 
| "find_nat_tree_opt x (Node y l r) = (
    if x = y then Some y 
    else if less x y then find_nat_tree_opt x l 
    else find_nat_tree_opt x r 
  )"


fun find_nat_list_opt :: "mynat \<Rightarrow> mynat_list \<Rightarrow> mynat option" where
  "find_nat_list_opt x Nil = None" 
| "find_nat_list_opt x (Cons y l) = (
    if x = y then Some y  
    else find_nat_list_opt x l 
  )"


fun find_min :: "mynat_tree \<Rightarrow> mynat" where
  "find_min Leaf = undefined"
| "find_min (Node x Leaf _) = x"
| "find_min (Node _ l _) = find_min l"

fun delete_min :: "mynat_tree \<Rightarrow> mynat_tree" where
  "delete_min Leaf = Leaf"
| "delete_min (Node x Leaf r) = r"
| "delete_min (Node x l r) = Node x (delete_min l) r"

fun delete_nat_tree :: "mynat \<Rightarrow> mynat_tree \<Rightarrow> mynat_tree" where
  "delete_nat_tree _ Leaf = Leaf"
| "delete_nat_tree x (Node y l r) = (
    if less x y then 
      Node y (delete_nat_tree x l) r
    else if less y x then 
      Node y l (delete_nat_tree x r)
    else 
      (case (l, r) of
        (Leaf, Leaf) \<Rightarrow> Leaf
      | (Leaf, _) \<Rightarrow> r
      | (_, Leaf) \<Rightarrow> l
      | _ \<Rightarrow> (let min_val = find_min r
             in Node min_val l (delete_min r)
            )
      )
  )"

end