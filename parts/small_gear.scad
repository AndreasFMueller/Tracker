/*
 * small_gear.scad -- small tracker gear, based on Gear_with_Grub_Sample.scad
 *                    by Juan Sirgado y Antico - www.jsya.com.br
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */

include <Gear_Double_Helical.scad>;
include <Gear_Grub.scad>;
include <nut.scad>;
//mirror([1,0,0])
gearwithgrub();

gap = 0.05;

module gearwithgrub() // motor gear
{
    difference() {
        union()
        {
            rotate([0, 0, 4])
            // Translate gear_dh Z = (gear_height / 2) Up
            translate([0,0,5]) gear_dh(number_of_teeth=54, 
                                 circular_pitch=180, 
                                 diametral_pitch=true, 
                                 gear_height=10,
                                 angle_twist=12, 
                                 central_hole=5.1);
            // Translate gear_grub Z = (grub_height / 2)*(-1) Down
		/*
            translate([0,0,-4+0.01]) gear_grub(grub_height=8,
                                     grub_diameter=20,
                                     grub_hole=1,
                                     hole_diameter=5 + gap,
                                     no_of_nuts=1,
                                     nut_angle=120,
                                     nut_elevation=0,
                                     nut_shaft_distance=1,    
                                     m3_diameter=3,
                                     m3_nut_hex=1,    
                                     m3_nut_size=6,
                                     m3_nut_height=3);
		*/
        }
	// recess in the wheel
        union() {
            translate([0, 0, 1.5])
            difference() {
                cylinder(r1 = 25, r2 = 21, h = 10, center = true, $fn = 90);
                cylinder(r1 = 10, r2 = 11, h = 10, center = true, $fn = 90);
            }
        }
	// holes / spikes
        for (alpha = [0 : 45 : 315]) {
            rotate([0, 0, alpha])
                translate([(21 + 11)/2, 0, 10])
                    cylinder(r = 5, h = 20, center = true, $fn = 30);
        }

        // opening for the nut
        translate([0, 0, 6])
            mirror([0, 0, 1])
                nut(5);
}
}
