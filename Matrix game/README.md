
# Falling Asteroids

It is an arcade game where the player has to save as many crews from the remaining damaged spaceships as possible before the get hit by the incoming asteroids.  

## How to play

### Gameplay
This is your spaceship represented by the two dots on the screen   
  
![image](https://github.com/istoriajocurilorvideo/robotics-lab/blob/master/Matrix%20game/Images/spaceship.png)

After a while random asteroids will start to appear on the screen and  move around. On top of that, the damaged  
spaceships will be floating around as well(they will be represented by a blicking dot on the screen). You can  
repair a spaceship by colliding with it.  
![image](https://github.com/istoriajocurilorvideo/robotics-lab/blob/master/Matrix%20game/Images/gameplay.png)  
The asteroids vary in size and speed, and this will change depending on your current level achieved in this game.    

Every repaired spaceship will add up to you final score, so get as many spaceships as possible.    

### Gamepad Buttons
On your gamepad you will have three buttons named A, B, X and a joystick.  
![image](https://github.com/istoriajocurilorvideo/robotics-lab/blob/master/Matrix%20game/Images/Gamepad.JPG)    
Using A and B you will move your spaceship forward or backward. The joystick will be used to change the direction of the player is space.

## Game System

### Main menu
Here we have five main tabs that you can select from (these tabs will be show up on the LCD):   
-> Play game   
-> Highscore   
-> Settings   
-> Info   

### Play game
When playing for the first time it will introduce you to a short story about the game. After that you will have  
a quick tutorial about how to play this game (you have also a skip button if you had already played this game before)  

Before you can actually play the game, you will have to enter your name ( You will have an additional option  
where you can choose whether to play as the last player or enter a new one)  

If you entered a name that appears on the score board, the new score that you will get with this player,  
if higher then its previous value will be updated on the highscore list.

### Highscore
This will display the five best scores (with their names on their left side on the screen  

### Settings
-> Starting level : to set a base level value from where the player can start off    
-> Contrast : Change the constrast of the lcd  
-> LCD Light : Change the light settings of led matrix  
-> Reset Scoreboard

## Hardware
8x8 LED matrix  
LCD  
Joystick  
MAX7219 Driver  
Buttons  
Buzzer  

## Demo Video
Currently working...
