module PE1 where

import Text.Printf

-- PE1: Recipe Calculator
-- The premise of this homework if to write a recipe calculator that
-- calculates: how much a recipe costs to make, what can be made with the
-- ingredients already available, and how much extra ingredients need to
-- be bought in order to make a recipe.

-- Recipe = Recipe Name [(Ingredient, Quantity)]
data Recipe = Recipe String [(String, Double)] deriving Show

-- Price = Price Ingredient Quantity Price
data Price = Price String Double Double deriving Show

-- You can use this as-is
getRounded :: Double -> Double 
getRounded x = read s :: Double
               where s = printf "%.2f" x
third :: (a, b, c) -> c  
third (_, _, z) = z
  

takestring :: Price -> String
takestring (Price name value1 value2) = name
takekg :: Price -> Double
takekg (Price name value1 value2) = value1
taketl :: Price -> Double
taketl (Price name value1 value2) = value2
-- Calculate how much the given amount of the given ingredient costs
getIngredientCost :: (String, Double) -> [Price] -> Double
--getIngredientCost (malzeme, miktar) (fiyat@(mal,kg,tl):rest) = if malzeme == mal then getRounded (miktar/kg*tl) else getIngredientCost (malzeme,miktar) rest   
--getIngredientCost (malzeme, miktar) fiyat = [fst(takestring x),snd(takestring x),takethird(takestring x)|x<-fiyat, fst (takestring x)==malzeme ] getRounded (miktar/kg*tl) 
getIngredientCost (malzeme, miktar) fiyatlar = getRounded ([miktar/takekg(x)*taketl(x) | x<-fiyatlar, (takestring x)==malzeme]!!0)

takemalzeme :: Recipe ->[(String,Double)]
takemalzeme (Recipe name list)=list
takemiktar :: (String,Double)-> Double 
takemiktar (str,dbl)= dbl
takeisim ::(String,Double)->String
takeisim (str,dbl)=str
-- Calculate how much it costs to buy all the ingredients of a recipe
recipeCost :: Recipe -> [Price] -> Double
recipeCost tarif fiyatlar = getRounded(sum([getIngredientCost(takeisim(x),takemiktar(x)) fiyatlar | x<-takemalzeme(tarif) ]))
--takelststr :: Recipe -> [(String, Double)] -> [String]
--takelststr tarif stok =[fst(x) |x<-(takemalzeme(tarif))]
takestokstr ::  [(String, Double)] -> [String]
takestokstr stok=[takeisim(x) |x<-(stok)]
taketarifstr :: Recipe ->[String]
taketarifstr tarif=[takeisim(x)|x<-(takemalzeme(tarif))]
isin:: Recipe -> [(String, Double)] -> [String]
isin tarif stok=filter (\x -> notElem x (takestokstr stok)) (taketarifstr tarif)
isinstok:: Recipe -> [(String, Double)] -> [String]
isinstok tarif stok=filter (\x -> notElem x (taketarifstr tarif)) (takestokstr stok)
-- Given a list of how much you already have of each ingredient,
-- calculate how much of which ingredients are missing for a recipe
missingIngredients :: Recipe -> [(String, Double)] -> [(String, Double)]
missingIngredients tarif stok  = [x | x<-[(takeisim(z),takemiktar(z)-takemiktar(t)) | z<-takemalzeme(tarif), t<-stok,  (takeisim(t)==takeisim(z))&&(takemiktar(t)<takemiktar(z))]] ++ [x | x<-[(takeisim(z),takemiktar(z)) | z<-takemalzeme(tarif), takeisim z `elem` isin tarif stok ]]
-- Given a list of ingredients in your kitchen, calculate what you would
-- have left after making the given recipe. If there isn't enough of an
-- ingredient, the recipe cannot be made! You shouldn't change the amount
-- of ingredient in that case.
makeRecipe :: [(String, Double)] -> Recipe -> [(String, Double)]
makeRecipe stok tarif = if missingIngredients tarif stok ==[] then [x | x<-[(takeisim(t),takemiktar(t)-takemiktar(z)) | z<-takemalzeme(tarif), t<-stok,  (takeisim(t)==takeisim(z))]]++ [x | x<-[(takeisim(z),takemiktar(z)) | z<-stok, takeisim z `elem` isinstok tarif stok ]] else stok
taketarifliststr :: [(String,Double)] ->[String]
taketarifliststr tarif=[takeisim(x)|x<-((tarif))]
modedisin:: [(String,Double)] -> [(String, Double)] -> [String]
modedisin tarif stok=filter (\x -> notElem x (takestokstr stok)) (taketarifliststr tarif)
-- Given a list of ingredients you already have, and a list of recipes,
-- make a shopping list showing how much of each ingredient you need
-- to buy, and its cost. Each ingredient mush appear in the shopping list
-- at most once (no duplicates!).
taketariflist :: [Recipe] -> [(String,Double)]
taketariflist tarifler=[x | x<-[(takeisim(z),takemiktar(z))|a<-tarifler,z<-takemalzeme a]]
needlist :: [(String, Double)] -> [(String, Double)]
needlist [] = []
needlist ((k, v):xs) = case lookup k xs of
  Just existingVal -> needlist $ (k, existingVal + v) : filter ((/= k) . fst) xs
  Nothing -> (k, v) : needlist xs
modifiedmi :: [(String, Double)] -> [(String, Double)] -> [(String,Double)]
modifiedmi need stok =[x | x<-[(takeisim(z),takemiktar(z)-takemiktar(t)) | z<-need, t<-stok,  (takeisim(t)==takeisim(z))&&(takemiktar(t)<takemiktar(z))]] ++ [x | x<-[(takeisim(z),takemiktar(z)) | z<-need, takeisim z `elem` modedisin need stok ]]
makeShoppingList :: [(String, Double)] -> [Recipe] -> [Price] -> [(String, Double, Double)]
makeShoppingList stok tarifler fiyatlar=[x |x<-[((takeisim y),takemiktar y,getIngredientCost y fiyatlar)|y<-modifiedmi (needlist (taketariflist tarifler)) stok]]
