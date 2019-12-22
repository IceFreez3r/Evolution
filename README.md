# Evolution
Test of a Evolution Algorithm

The program simulates entities so called "Dots", which walk around on the square Testground. Every Tick new Foodsources are generated at random spots. Dots can eat these to gain energy.  

Each Dot has multiple attributes:  
Constant (mutate between generations, see below):  
Speed - The distance a Dot can walk in one tick, but also increases the energy needed per distance unit  
Sight - The distance a Dot can see Foodsources, but also increases the energy needed each tick  
(Size - At the moment only increases the energy need per tick and doesn't mutate)

Dynamic:  
Energy - The life energy of Dot. If it falls below 1, the Dot dies.
Reproduction Cooldown - The amount of ticks until the dot can replicate again
Position - The position of the Dot on the Testground
Direction - The direction the Dot is facing

If a Dot sees food it turns in that direction and eats it once it reaches it. If a d
