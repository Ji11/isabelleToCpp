theory data
  imports Main
begin
datatype nat = Zero | Suc nat
datatype 'a list = Nul | Cons 'a "'a list"
datatype 'a tree = Tip | TC "'a tree" 'a "'a tree"
datatype 'a option = None | Some 'a

fun add :: "nat \<Rightarrow> nat \<Rightarrow> nat" where
  "add Zero n = n"
| "add (Suc m) n = Suc (add m n)"

fun mult :: "nat \<Rightarrow> nat \<Rightarrow> nat" where
  "mult Zero n = Zero"
| "mult (Suc m) n = add n (mult m n)"

fun insert :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "insert x Nul = Cons x Nul"
| "insert x (Cons y ys) = Cons y (insert x ys)"

fun del :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a list" where
  "del x Nul = Nul"
| "del x (Cons y ys) = (if x = y then ys else Cons y (del x ys))"

fun find :: "'a \<Rightarrow> 'a list \<Rightarrow> 'a option" where
  "find x Nul = None"
| "find x (Cons y ys) = (if x = y then Some y else find x ys)"

fun inserttree :: "'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a tree" where
  "inserttree x Tip = TC Tip x Tip"
| "inserttree x (TC l y r) = (if x < y then TC (inserttree x l) y r else TC l y (inserttree x r))"

fun findtree :: "'a::ord \<Rightarrow> 'a tree \<Rightarrow> 'a option" where
  "findtree x Tip = None"
| "findtree x (TC l y r) = (if x = y then Some y else if x < y then findtree x l else findtree x r)"

fun mintree :: "'a::ord tree ⇒ 'a" where
  "mintree Tip = undefined"
| "mintree (TC Tip y _) = y"
| "mintree (TC l _ _) = mintree l"

fun insert_desc :: "'a::ord ⇒ 'a list ⇒ 'a list" where
  "insert_desc x Nul = Cons x Nul"
| "insert_desc x (Cons y ys) = (if x ≥ y then Cons x (Cons y ys) else Cons y (insert_desc x ys))"

fun sort :: "'a::ord list ⇒ 'a list" where
  "sort Nul = Nul"
| "sort (Cons x xs) = insert_desc x (sort xs)"
                                                                         
fun append :: "'a list ⇒ 'a list ⇒ 'a list" where
  "append Nul ys = ys"
| "append (Cons x xs) ys = Cons x (append xs ys)"

fun traversaltree :: "'a tree ⇒ 'a list" where
  "traversaltree Tip = Nul"
| "traversaltree (TC l x r) = append (traversaltree l) (Cons x (traversaltree r))"

end