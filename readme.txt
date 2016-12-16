=========================================================
DM2240 ADVANCED GAME DEVELOPMENT TECHNIQUES ASSIGNMENT 1
=========================================================

Name: Inflatable World Demo

DESCRIPTION

Inflatable, bouncy structures fill up an indoor hall. These structures are constructed from multiple balloon parts. Hop onto balloons to bounce, or shoot them with a needle gun to deflate them. Throw a grenade to destroy an entire nearby structure or enemy.
Enemies are made up of multiple balloon parts as well.
The goal of the game is to collect the diamonds. By destroying structures, they can be found more easily. There are ammo boxes for reloading as well.


CONTROLS

WSAD - Move the player
Spacebar - Jump
Shift - Sprint (hold down when moving)
CTRL - Toggle crouch
Left Mouse Button - Shoot
'1' - Change to needle gun
'2' - Change to grenade
'P' - Reset player position to origin

TECHNICAL FEATURES
Advanced Collision Detection - 3D AABB collision. Use of unique collision responses: deflating balloons when punctured, player bouncing on top of balloons.
Level of Detail - At different distances from the player, models are at lower resolution. At low resoltution, structures and enemies are rendered with fewer models.
Scene Graph - Each structure is made up of multiple balloon parts. It is possible to destroy a structure as a whole using a grenade.
Spatial Partitioning - Search only for structures or enemies in the same grid (as the player or projectile).


DONE BY: Foo Jing Ting (152856H)
         Donovan Sim Yew Wee (153492Y)