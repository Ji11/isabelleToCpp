theory test
  imports Main

begin

datatype mynat = myzero | mysuc mynat

datatype 'a mylist = mynil | mycons "'a" "'a mylist" 
datatype 'a myoption = mynone | mysome "'a"

datatype 'a mytree = tip | treenode "'a" "'a mytree" "'a mytree" 

fun myadd :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat" where
  "myadd n myzero = n" 
| "myadd n (mysuc m) = mysuc (myadd n m)"

fun mymult :: "mynat \<Rightarrow> mynat \<Rightarrow> mynat" where
  "mymult n myzero = myzero" 
| "mymult n (mysuc m) = myadd (mymult n m) n"



fun insert_head :: "'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist" where 
  "insert_head x mynil = mycons x mynil" 
| "insert_head x (mycons h list) = mycons x (mycons h list)"

fun insert_tail :: "'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist" where
  "insert_tail x mynil = mycons x mynil" 
| "insert_tail x (mycons h list) = mycons h (insert_tail x list)"

fun mydelete :: "'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist" where
  "mydelete x mynil = mynil"          
|  "mydelete x (mycons h list) = (if x = h then list else mycons h (mydelete x list))"

fun myupdate :: "mynat \<Rightarrow> 'a \<Rightarrow> 'a mylist \<Rightarrow> 'a mylist" where
  "myupdate _ _ mynil = mynil" 
| "myupdate myzero new_value (mycons h list) = mycons new_value list" 
| "myupdate (mysuc n) new_value (mycons h list) = mycons h (myupdate n new_value list)"

fun myget :: "mynat \<Rightarrow> 'a mylist \<Rightarrow> 'a myoption" where
  "myget _ mynil = mynone" 
| "myget myzero (mycons h list) = mysome h" 
| "myget (mysuc n) (mycons h list) = myget n list"

fun myfind :: "'a  \<Rightarrow> 'a mylist \<Rightarrow> bool" where
  "myfind x mynil = False" 
| "myfind x (mycons h list) = (if h = x then True else myfind x list)"


fun tree_insert :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> 'a mytree" where
  "tree_insert x tip = treenode x tip tip" 
| "tree_insert x (treenode root left right) = (if x = root then treenode root left right
    else if x < root then treenode root (tree_insert x left) right
    else treenode root left (tree_insert x right)
  )"

fun tree_search :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> 'a myoption" where
  "tree_search x tip = mynone" 
| "tree_search x (treenode root left right) = (
    if x = root then mysome root
    else if x < root then tree_search x left
    else tree_search x right
  )"

fun tree_min :: "'a::linorder mytree \<Rightarrow>'a" where
  "tree_min (treenode x tip _) = x" 
| "tree_min (treenode x left _) = tree_min left" 
| "tree_min tip = undefined"

fun tree_delete :: "'a::linorder \<Rightarrow> 'a mytree \<Rightarrow> 'a mytree" where
  "tree_delete _ tip = tip" 
| "tree_delete x (treenode root left right) = (
    if x < root then treenode root (tree_delete x left) right
    else if x > root then treenode root left (tree_delete x right)
    else (
      if left = tip \<and> (right = tip) then tip
      else if left = tip then right
      else if right = tip then left
      else let
            min_value = tree_min right
          in
            treenode min_value left (tree_delete min_value right)
    )
  )"

end