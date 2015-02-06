#fake
###Current Version: 0.1 alpha

#### *Probably not better than 'make' but it's okay!*

Another small project that was incepted in my Systems class, and I decided to keep adding onto it. Why? Curiosity, maybe. Or boredom? Or because it was *fun*? Anyway, it's got basic functionality that you see in *make* and I will keep adding on to it to make it just as good. Maybe even *better*.

##Usage
####Create a 'Fakefile' in your working directory to get started
    ~$ echo "New Fakefile" > Fakefile

You can fill it up with different **targets** that *fake* can look for and correspondingly run commands for you, including compilation (soon with flags!) and other things like echo, cat -- basic shell commands!
  
####**For example:**
     1 # Fakefile
     2 # For test.cpp
     3 test:
     4     echo "Building test..."
     5     clang test.cpp -o test
     6     echo "Finished building test!"
     7 
     8 clean:
     9     rm test
    10    rm log.txt
    11    ls

dfklfd
    kdfkdf
