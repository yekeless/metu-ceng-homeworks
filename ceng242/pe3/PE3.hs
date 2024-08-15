{-# LANGUAGE FlexibleInstances #-}

module PE3 where

import Data.List (sort, sortBy)
import Text.Printf (printf)

data Term = Const Integer | Pw Integer Power | Trig Integer Power Trigonometric | Exp Integer Power Exponential

data Power = Power Integer
data Polynomial = Polynomial [(Integer, Power)]
data Exponential = Exponential Polynomial
data Trigonometric = Sin Polynomial | Cos Polynomial

class Evaluable a where
    function :: a -> (Integer -> Double)

class Differentiable a where
    derivative :: a -> [Term]

-- You can use this as is
getRounded :: Double -> Double 
getRounded x = read s :: Double
               where s = printf "%.2f" x

-- You don't have to follow the order the functions appear in the file
-- For example, you could first define all Show instances, then all Eq instances etc.
-- if that implementation order is more convenient for you.



-- INSTANCES FOR POWER

instance Show Power where
    show (Power 0) = "1"
    show (Power 1) = "x"
    show (Power p) = "x^" ++ show p
instance Eq Power where
    Power p1 == Power p2 = if p1==p2 then True else False 

instance Ord Power where
    Power p1 <= Power p2 = if p1<=p2 then True else False
    Power p1 < Power p2 = if p1<p2 then True else False
    Power p1 >= Power p2 = if p1>=p2 then True else False
    Power p1 > Power p2 = if p1>p2 then True else False
instance Evaluable Power where
    function (Power p) = \x ->  getRounded( (fromIntegral x) ** (fromIntegral p))

instance Differentiable Power where
    derivative (Power 0) = []
    derivative (Power c) = [Pw (fromIntegral c) (Power (c-1))]



-- INSTANCES FOR POLYNOMIAL

instance Show Polynomial where
    show (Polynomial [(c,Power p)]) = if c==(-1) then "-" ++ show (Power p)
                                else if c==1 then show (Power p)
                                else if p == 0 then show c
                                else show c ++ show (Power p)
    show (Polynomial ((c,Power p) : rest))=if c==(-1) then "-"++ show(Power p) ++ " + " ++ show(Polynomial rest)
                                    else if c==1 then show (Power p)++" + "++ show(Polynomial rest)
                                    else if p == 0 then show c ++" + "++ show (Polynomial rest)
                                    else show c ++ show (Power p)++" + "++show (Polynomial rest)                            

instance Evaluable Polynomial where
    function (Polynomial [(c,Power p)]) = \x -> getRounded((fromIntegral c) * ((fromIntegral x) ** (fromIntegral p)))
    function (Polynomial ((c,Power p):rest)) =
        let remain=function(Polynomial rest)
        in \x -> getRounded((fromIntegral c) * ((fromIntegral x) ** (fromIntegral p)) +  (remain x))

instance Differentiable Polynomial where
    derivative _ = []




-- INSTANCES FOR TRIGONOMETRIC

instance Show Trigonometric where
    show (Sin p) = if show p == "x" then "sinx" else "sin(" ++ show p ++ ")"
    show (Cos p) = if show p == "x" then "cosx" else "cos(" ++ show p ++ ")"

instance Evaluable Trigonometric where
    function (Sin p) = \x ->getRounded(sin(function(p) x))
    function (Cos p) = \x ->getRounded(cos(function(p) x))
instance Differentiable Trigonometric where
    derivative _ = []




-- INSTANCES FOR EXPONENTIAL

instance Show Exponential where
    show (Exponential p) = if show p ==""
                        then "1"
                        else "e^" ++"(" ++ (show p) ++")" 

instance Evaluable Exponential where
    function (Exponential p) = \x -> getRounded(exp(function(p) x))

instance Differentiable Exponential where
    derivative _ = []



-- INSTANCES FOR TERM

instance Show Term where
    show (Const c) = show c
    show (Pw c p)= if c==1 then show p
                    else show c ++ show p
    show (Exp c p e)= if c==1 then show p ++ show e
                    else show c ++ show p ++ show e
    show(Trig c p t)= if c==1 then show p ++ show t
                    else show c ++ show p ++ show t     
instance Evaluable Term where
    function (Const c) = \x -> getRounded(fromIntegral c)
    function (Pw c p) = \x -> getRounded( fromIntegral (c) * (function p x))
    function (Exp c p e)= \x -> getRounded(fromIntegral(c)*(function p x)*(function e x))
    function (Trig c p t)= \x -> getRounded(fromIntegral(c)*(function p x)*(function t x))
instance Differentiable Term where
    derivative _ = []




-- INSTANCES FOR [TERM]

instance Evaluable [Term] where
    function [] = \x -> getRounded(0.00)
    function (p:rest)= \x -> getRounded((function p x) +  (function rest x))

instance Differentiable [Term] where
    derivative _ = []
