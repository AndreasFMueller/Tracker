/*---------------------------------------------------------\
|     From: Ekobots Innovation Ltda - www.ekobots.com      |
|       by: Juan Sirgado y Antico - www.jsya.com.br        |
|----------------------------------------------------------|
|         Program Camera Rotate & Tilt - 2014/04/10        |
|               All rights reserved 2014                   |
|---------------------------------------------------------*/
include <Gear_Double_Helical.scad>;
include <Gear_Grub.scad>;
gearwithgrub();
//---------------------------------------------------------|
module gearwithgrub() // motor gear
{
   union()
   {
      // Translate gear_dh Z = (gear_height / 2) Up
      translate([0,0,6]) gear_dh(number_of_teeth=15, 
                                 circular_pitch=180, 
                                 diametral_pitch=false, 
                                 gear_height=12,
                                 angle_twist=12, 
                                 central_hole=5);
      // Translate gear_grub Z = (grub_height / 2)*(-1) Down
      translate([0,0,-5]) gear_grub(grub_height=10,
                                     grub_diameter=20,
                                     grub_hole=1,
                                     hole_diameter=5,
                                     no_of_nuts=1,
                                     nut_angle=120,
                                     nut_elevation=0,
                                     nut_shaft_distance=1,    
                                     m3_diameter=3,
                                     m3_nut_hex=1,    
                                     m3_nut_size=6,
                                     m3_nut_height=3);
   }
}
//---------------------------------------------------------|
