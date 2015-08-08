/*
 * nut.scad
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */

nut_width = 2.8;
nut_radius = nut_width * 2 / sqrt(3);
nut_thickness = 2.4;
nut_offset = 6.5;

module nut(nut_offset) {
	// opening for the nut
	translate([0, -nut_offset, 5])
		cube([2 * nut_width, nut_thickness, 10], center=true);
	translate([0, -nut_offset, 1])
		rotate([90, 0, 0]) rotate([0, 0, 30])
			cylinder(r = nut_radius, h = 2.4, center = true, $fn = 6);
	// screw hole
	translate([0,-30,1])
		rotate([90,0,0])
			cylinder(r=1.0,h=60, center=true, $fn=20);
}
