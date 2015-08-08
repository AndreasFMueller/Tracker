/*
 * large_gear.scad
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */


include <Gear_Double_Helical.scad>;
include <Gear_Grub.scad>;
include <nut.scad>;
gearwithholes();

nut_offset = 6.5;

module gearwithholes() // base gear
{
   
   hole_size = 5;      // Diameter for hole 
   hole_angle = 45;    // Angle between holes
   hole_distance = 16; // Distance from gear center

   difference()
   {
      // Gear double helical
	rotate([0,0,4])
      gear_dh(number_of_teeth=54, 
              circular_pitch=180, 
              diametral_pitch=false, 
              gear_height=10,
              angle_twist=12, 
              central_hole=9);
	// diameter = 54 (distance between centers)

      // Gear side holes
      for (r = [0:hole_angle:360-hole_angle]) // From 0 to 360 angle between holes
         rotate(r,0,0) 
            translate([0,hole_distance,0]) // Distance from gear center   
               cylinder(10.2,hole_size,hole_size,center=true,$fn=60); // Diameter for hole 

      // Gear side groove 
      translate([0,0,3.5]) // Up Z (gear_height / 2)
         difference()
         {
             cylinder(10,21,25,center=true,$fn=60);
             cylinder(10,11,10,center=true,$fn=60);
         };

    translate([0, 0, -1])
        nut(6.5);
   }
}
