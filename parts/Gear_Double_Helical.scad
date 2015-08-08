/*---------------------------------------------------------\
|     From: Ekobots Innovation Ltda - www.ekobots.com      |
|       by: Juan Sirgado y Antico - www.jsya.com.br        |
|----------------------------------------------------------|
|         Program Gears Double Helical - 2013/08/13        |
|                All rights reserved 2013                  |
|---------------------------------------------------------*/
// Derivation of:
// Parametric Gear by D1plo1d Copyright 2010 LGPL 2.1
//---------------------------------------------------------|
//gear_dh();
//---------------------------------------------------------|
module gear_dh(number_of_teeth=50, 
               circular_pitch=180, 
               diametral_pitch=false, 
               gear_height=10,
               angle_twist=12, 
               central_hole=5)
{
   difference()
   {
      make_gear(number_of_teeth, 
                circular_pitch,
                diametral_pitch, 
                gear_height,
                angle_twist);
      cylinder(gear_height+0.2, central_hole/2, 
               central_hole/2, center = true, $fn=30);
   }
}
//---------------------------------------------------------|
module make_gear(number_of_teeth, 
                 circular_pitch=false,
                 diametral_pitch=false, 
                 gear_height=5,
                 angle_twist=12)
{
   // Tooth pressure angle
   pressure_angle = 86;
   // Tooth between gears 
   clearance = .5;
   // Tooth attack angle
   attack_angle = 1;
   // Tooth twist angle
   base_twist = (angle_twist / number_of_teeth) * angle_twist; 

// Make 3d gear: 2 parts top and bottom
   translate([0,0,-gear_height / 4]) 
      linear_extrude(gear_height / 2, center = true, 
                     convexity = 3, twist = -base_twist)
         half_gear(number_of_teeth,
                   circular_pitch, 
                   diametral_pitch,
                   pressure_angle, 
                   clearance,
                   attack_angle);
// Rotation to correct tooth join angle
   rotate([0,0,base_twist])
      translate([0,0,gear_height / 4]) 
         linear_extrude(gear_height / 2, center = true, 
                        convexity = 3, twist = base_twist)
            half_gear(number_of_teeth,
                      circular_pitch, 
                      diametral_pitch,
                      pressure_angle, 
                      clearance,
                      attack_angle);
}
//---------------------------------------------------------|
module half_gear(number_of_teeth,
                 circular_pitch=false, 
                 diametral_pitch=false,
                 pressure_angle=20, 
                 clearance=0,
                 attack_angle=1)
{
   if (circular_pitch==false && diametral_pitch==false) 
      echo("Error: Gear module needs a diametral_pitch or circular_pitch");

   //Convert diametrial pitch to our native circular pitch
   circular_pitch = (circular_pitch != false ? 
                     circular_pitch : 180 / diametral_pitch);

   // Pitch diameter: Diameter of pitch circle. 
   pitch_diameter = number_of_teeth * circular_pitch / 180;
   pitch_radius = pitch_diameter / 2;

   // Base Circle
   base_diameter = pitch_diameter * cos(pressure_angle);
   base_radius = base_diameter / 2;

   // Diametrial pitch: Number of teeth per unit length.
   pitch_diametrial = number_of_teeth / pitch_diameter;

   // Addendum: Radial distance from pitch circle to outside circle.
   addendum = 1 / pitch_diametrial;

   //Outer Circle
   outer_radius = pitch_radius + addendum;
   outer_diameter = outer_radius * 2;

   // Dedendum: Radial distance from pitch circle to root diameter
   dedendum = addendum + clearance;

   // Root diameter: Diameter of bottom of tooth spaces.
   root_radius = pitch_radius - dedendum;
   root_diameter = root_radius * 2;

   thick_angle = 360 / (4 * number_of_teeth);

   echo("pitch_radius = ", pitch_radius,
        "root_radius = ", root_radius,
        "base_radius = ", base_radius,
        "outer_radius = ", outer_radius,
        "thick_angle = ", thick_angle,
        "attack_angle = ", attack_angle);

   union()
   {
      rotate(thick_angle) 
         circle(r=root_radius * 1.001, $fn=number_of_teeth * 2);

      for (i= [1:number_of_teeth])
      {
         rotate([0,0,i * 360 / number_of_teeth]) 
         {
            involute_gear_tooth(pitch_radius = pitch_radius,
                                root_radius = root_radius,
                                base_radius = base_radius,
                                outer_radius = outer_radius,
                                thick_angle = thick_angle,
                                attack_angle = attack_angle);
         }
      }
   }
}
//---------------------------------------------------------|
module involute_gear_tooth(pitch_radius,
                           root_radius,
                           base_radius,
                           outer_radius,
                           thick_angle,
                           attack_angle)
{
// Tooth size and base/top angles 
   pitch_to_base_angle = involute_intersect_angle( base_radius, pitch_radius );
   outer_to_base_angle = involute_intersect_angle( base_radius, outer_radius );

// Base angle: Tooth thick angle ( defalt = 1 )
   thick1 = thick_angle / 1.2;
// Top angle: Tooth attack angle ( defalt = 1 )
   attack1 = attack_angle * 1.5;

   base1 = 0 - pitch_to_base_angle - thick1;
   pitch1 = 0 - thick1;
   outer1 = (outer_to_base_angle - pitch_to_base_angle - thick1) / attack1;

   b1 = polar_to_cartesian([ base1, base_radius ]);
   p1 = polar_to_cartesian([ pitch1, pitch_radius ]);
   o1 = polar_to_cartesian([ outer1, outer_radius ]);

   b2 = polar_to_cartesian([ -base1, base_radius ]);
   p2 = polar_to_cartesian([ -pitch1, pitch_radius ]);
   o2 = polar_to_cartesian([ -outer1, outer_radius ]);

   pitch_to_root_angle = pitch_to_base_angle - 
                         involute_intersect_angle(base_radius, root_radius );
   root1 = pitch1 - pitch_to_root_angle;
   root2 = -pitch1 + pitch_to_root_angle;

// if ( root_radius > base_radius variables )
   r1_t = polar_to_cartesian([ root1, root_radius ]);
   r2_t = polar_to_cartesian([ -root1, root_radius ]);
// ( else )
   r1_f = polar_to_cartesian([ base1, root_radius ]);
   r2_f = polar_to_cartesian([ -base1, root_radius ]);
// ( end if )

   if (root_radius > base_radius)
   {
      polygon( points = [r1_t,p1,o1,o2,p2,r2_t], convexity = 3);
   }
   else
   {
      polygon( points = [r1_f, b1,p1,o1,o2,p2,b2,r2_f], convexity = 3);
   }

}
//---------------------------------------------------------|
// Finds the angle of the involute about the base 
// radius at the given distance (radius) from it's center.
function involute_intersect_angle(base_radius, radius) = 
   sqrt( pow(radius/base_radius,2) - 1);
//---------------------------------------------------------|
// Polar coord [angle,radius] to cartesian coord [x,y]
function polar_to_cartesian(polar) = 
   [polar[1] * cos(polar[0]), polar[1] * sin(polar[0])];
//---------------------------------------------------------|