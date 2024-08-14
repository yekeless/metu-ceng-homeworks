module PE2 where

-- PE2: Dungeon Crawler
-- Dungeon map is :: Tree Chamber [Encounter]
-- Each encounter is either a fight or a treasure
-- Fights deal you damage (reduce HP) but enemies drop some gold (add
-- gold)
-- Tresures just give gold, or potions (which give hp)
-- Nodes hold encounters, when you visit a node you go through all of them in order
-- You start with a certain amount of HP and 0 gold.
-- You lose HP and accumulate gold as you descend the tree and go through encounters

-- Polymorphic tree structure
data Tree a b = EmptyTree | Leaf a b | Node a b [Tree a b] deriving (Show, Eq)

-- Every location in the tree is of some Chamber type.
data Chamber = Cavern |
               NarrowPassage |
               UndergroundRiver |
               SlipperyRocks deriving (Show, Eq)

-- An enemy has a name, an amount of damage that it deals
-- and an amount of gold that it drops (in that order).
data Enemy = Enemy String Integer Integer deriving (Show, Eq)

-- Gold n gives n amount of gold
-- Potion n heals n hp
data Loot = Gold Integer | Potion Integer deriving (Show, Eq)

-- An encounter is either a Fight with an Enemy, or a treasure where
-- you find Loot
data Encounter = Fight Enemy | Treasure Loot deriving (Show, Eq)

-- This is a type synonym for how we will represents our dungeons
type Dungeon = Tree Chamber [Encounter]

-- First argument is starting HP
-- Second argument is the dungeon map
-- Third argument is the path (each integer in the list shows what child
-- you descend into)
-- Calculate how much HP you have left and how much gold you've
-- accumulated after traversing the given path

helper_encounter_tp :: (Integer, Integer) -> [Encounter] -> (Integer, Integer)
helper_encounter_tp (hp,gold) [] = (hp,gold)
helper_encounter_tp (hp,gold) (first : remain)= case first of
    Fight (Enemy name damage dropped) ->
        let can=hp-damage
            para=gold+dropped
        in helper_encounter_tp (can,para) remain
    Treasure (Gold dropped) ->
        let para=gold+dropped
        in helper_encounter_tp (hp,para) remain
    Treasure (Potion heal) ->
        let can=hp+heal
        in helper_encounter_tp (can,gold) remain
helper_tp :: (Integer,Integer) -> Dungeon -> [Int]  -> (Integer,Integer)
helper_tp (hp,gold) (Node chamber encounters children) [] = (hp,gold)
helper_tp (hp,gold) (Leaf chamber encounters) _= helper_encounter_tp (hp,gold) encounters
helper_tp (hp,gold) (Node chamber encounters children) (first:remain)=
    let (new_hp,new_gold)=helper_encounter_tp (hp,gold) encounters
    in helper_tp (new_hp,new_gold) (children !! first) remain
--helper_tp (hp,gold) (Node chamber encounters children) [] = (hp,gold)
   

traversePath :: Integer -> Dungeon -> [Int] -> (Integer, Integer)
traversePath hp tree path = helper_tp (hp,0) tree path 


-- First argument is starting HP
-- Second argument is dungeon map
-- Find which path down the tree yields the most gold for you
-- You cannot turn back, i.e. you'll find a non-branching path
-- You do not need to reach the bottom of the tree
-- Return how much gold you've accumulated
helper_encounter :: (Integer, Integer) -> [Encounter] -> (Integer, Integer)
helper_encounter (hp,gold) [] = (hp,gold)
helper_encounter (hp,gold) (first : remain)= case first of
    Fight (Enemy name damage dropped) ->
        let can=if hp-damage>0 then hp-damage else 0
            para=if hp-damage>0 then gold+dropped else 0
        in helper_encounter (can,para) remain
    Treasure (Gold dropped) ->
        let para=gold+dropped
        in helper_encounter (hp,para) remain
    Treasure (Potion heal) ->
        let can=hp+heal
        in helper_encounter (can,gold) remain

helper_find_max:: Integer -> Integer -> Dungeon -> Integer
helper_find_max hp gold EmptyTree=gold
helper_find_max hp gold (Leaf chamber encounters)=  snd (helper_encounter (hp, gold) encounters) 
helper_find_max hp gold (Node chamber encounters childrens) = 
    let (new_hp,new_gold)=helper_encounter (hp,gold) encounters
    in maximum [helper_find_max new_hp new_gold x | x<-childrens]    
    
        
findMaximumGain :: Integer -> Dungeon -> Integer
findMaximumGain hp tree = helper_find_max hp 0 tree

-- First argument is starting HP
-- Second argument is the dungeon map
-- Remove paths that you cannot go thorugh with your starting HP. (By
-- removing nodes from tree).
-- Some internal nodes may become leafs during this process, make the
-- necessary changes in such a case.
helper_viable :: Integer -> Integer -> Dungeon -> Dungeon
helper_viable _ _ EmptyTree=EmptyTree
helper_viable hp gold (Leaf chamber encounters)=
   let (new_hp,new_gold)=helper_encounter (hp,gold) encounters
   in    if new_hp==0 then EmptyTree else (Leaf chamber encounters)
helper_viable hp gold (Node chamber encounters children)=
    let (new_hp,new_gold)=helper_encounter (hp,gold) encounters
        new_child=[helper_viable new_hp new_gold x | x <- children, helper_viable new_hp new_gold x /= EmptyTree ]
    in if new_hp==0 then EmptyTree else (if new_child==[] then (Leaf chamber encounters) else (Node chamber encounters new_child))

findViablePaths :: Integer -> Dungeon -> Dungeon
findViablePaths hp tree = helper_viable hp 0 tree 
-- First argument is starting HP
-- Second Argument is dungeon map
-- Find, among the viable paths in the tree (so the nodes you cannot
-- visit is already removed) the two most distant nodes, i.e. the two
-- nodes that are furthest awat from each other.
mostDistantPair :: Integer -> Dungeon -> (Integer, Dungeon)
mostDistantPair _ _ = (0, EmptyTree)

-- Find the subtree that has the highest total gold/damage ratio
-- Simply divide the total gold in the subtree by the total damage
-- in the subtree. You only take whole subtrees (i.e you can take a new
-- node as the root of your subtree, but you cannot remove nodes
-- below it). Note that the answer may be the whole tree.
helper_gain :: (Integer,Integer) ->  [Encounter] -> (Integer, Integer)
helper_gain (hp_gain,gold_gain)  [] = (hp_gain,gold_gain)
helper_gain (hp_gain,gold_gain) (first : remain)= case first of
    Fight (Enemy name damage dropped) ->
        let can_gain=hp_gain-damage
            para_gain=gold_gain+dropped
        in helper_gain (can_gain,para_gain) remain
    Treasure (Gold dropped) ->
        let para_gain=gold_gain+dropped
        in helper_gain (hp_gain,para_gain) remain
    Treasure (Potion heal) ->
        let can_gain=hp_gain+heal
        in helper_gain (can_gain,gold_gain) remain
take_hp:: [(Integer,Integer)]->Integer
take_hp list=sum[x | (x,y)<-list]
take_gold:: [(Integer,Integer)]->Integer
take_gold list=sum[y | (x,y)<-list]
helper_calculate :: Dungeon ->(Integer, Integer)
helper_calculate  EmptyTree =(0,0)
helper_calculate  (Leaf chambers encounter) = helper_gain (0,0) encounter
helper_calculate (Node chambers encounter children) =
    let (new_hp,new_gold)=helper_gain (0,0) encounter
        children_gain=[helper_calculate  x | x<- children]
        children_hp=take_hp children_gain
        children_gold=take_gold children_gain
    in (new_hp+children_hp,new_gold+children_gold)    

pick_max ::Double-> [Dungeon] -> Dungeon
pick_max efficiency list=head [x | x<-list , efficiency==helper_efficiency x]
helper_efficiency :: Dungeon -> Double
helper_efficiency EmptyTree=0
helper_efficiency (Leaf chamber encounters)=
    let (new_hp,new_gold) = helper_calculate (Leaf chamber encounters) 
    in    if new_hp>=0 then 99999999 else abs ((fromInteger new_gold) / (fromInteger new_hp))   
helper_efficiency (Node chamber encounters childrens)=
    let (new_hp,new_gold) = helper_calculate  (Node chamber encounters childrens) 
    in   if new_hp>=0 then 99999999 else abs((fromInteger new_gold) / (fromInteger new_hp))
mostEfficientSubtree :: Dungeon -> Dungeon
mostEfficientSubtree EmptyTree = EmptyTree
mostEfficientSubtree (Leaf chambers encounters)=(Leaf chambers encounters)
mostEfficientSubtree (Node chambers encounters children) =
    let node_efficiency = helper_efficiency (Node chambers encounters children)
        me_st=[mostEfficientSubtree x | x <- children]
        evalue=[helper_efficiency y |y <- me_st]
        most= if node_efficiency > maximum evalue then (Node chambers encounters children) else pick_max (maximum evalue) me_st
    in most
 