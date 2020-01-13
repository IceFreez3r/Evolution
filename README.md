# Evolution
Test of a Evolution Algorithm

The program simulates entities so called "Dots", which walk around on the square Testground. Every Tick new Foodsources are generated at random spots. Dots can eat these to gain energy.  

### Dots

Each Dot has multiple attributes:  
Constant (mutate between generations, see below):  
Speed - The distance a Dot can walk in one tick, but also increases the energy needed per distance unit  
Sight - The distance a Dot can see Foodsources, but also increases the energy needed each tick  
(Size - At the moment only increases the energy need per tick and doesn't mutate)

Dynamic:  
Energy - The life energy of Dot. If it falls below 1, the Dot dies (= SELECTION).
Reproduction Cooldown - The amount of ticks until the dot can replicate again
Position - The position of the Dot on the Testground
Direction - The direction the Dot is facing

If a Dot sees food it turns in that direction and eats it once it reaches it. If a Dot manages to get above a certain threshold of energy (5000 at the moment) and its Reproduction Cooldown is 0 or less, it can replicate (= REPRODUCTION).  
If this happens, a new Dot gets spawned in sight of the parent. All constant values get copied from the parent +/- a random summand (= VARIATION). The parent transfers half of its energy to its child.


### Testground/Environment

There are also some important attributes of the Testground:  
Testgroundsize - The length of one side of the square, can't be higher than 65535 (the maximum value of uint16_t)
min. and max. Food per Tick - The amount of new Foodsources to be generated each tick. If both are the same or if min is higher than  max , there will be always the same amount of food per tick  

size 65535, 0 to 200 food per tick and 1000 standard Dots for the start

### Start the Simulation
At the moment you need to edit the main.cpp. We will propably add other ways for this later.
To create the Testground you have multiple options:  
1. The standard option  
    ```C++
    Environment e();
    ```
    This will create a Testground with standard options.
2. The custom option
    ```C++
    Environment e(uint16_t testground_size, int dot_count, int min_food_count, int max_food_count)
    ```
    This lets you customize the options of the Testground on your own, but will still use the standard values for Dots.
3. The super custom option
    ```C++
    Dot start_dot(uint16_t testground_size, int energy, uint16_t speed, uint16_t sight, uint16_t size, std::pair<uint16_t, uint16_t> pos);
    Environment e(uint16_t testground_size, int dot_count, int min_food_count, int max_food_count, Dot start_dot)
    ```
    Basically the same as the second option, but if you create a Dot first and deliver it to the constructor, every generated Dot from the start will be a copy of it. If the `testground_size` is different in the Dot and the Environment, the one from the Environment will be copied into the Dot at first. If the `pos`of the Dot is outside the Testground, this may lead to some undefined behaviour (at the moment), but probably every Dot will be moved inside the Testground after one step.
    
To then run the simulation, use tick():  
```C++
e.tick(); // One tick
e.tick(amount); // As many ticks at once as you like
```
To print the Testground, use printTestground():  
```C++
e.printTestground();
```
To get a quick summary of the values of the living Dots, use printProperties():  
```C++
e.printProperties();
```
