/*
 * stencil.scad -- stencil for the mounting holes
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */

distance = 54;
l = 35;
l1 = l * 2 / sqrt(3);
radius = (12 + l1/2) / 2;

large_holes = 0;
motorholedistance = 31;
mho = motorholedistance/2;

difference() {
	cube([distance + 42, 42, 4], center = true);
	translate([-distance/2, 0, 0])
        union() {
            for (alpha = [ 45 : 90 : 315 ]) {
                rotate([0, 0, alpha])
                    translate([radius, 0, 0])
                        cylinder(r = 1.25, h = 6, $fn = 20, center = true);
            }
            cylinder(r = 1.5 + large_holes * 6, h = 6, center = true, $fn = 50);
        }
	translate([distance/2, 0, 0])
        union() {
            translate([0, 0, 2])
                cylinder(r = 11, h = 4, center = true, $fn = 60);
            cylinder(r = 1.5 + large_holes * 2, h = 6, center = true, $fn = 20);
            translate([-mho, mho, 0])
                cylinder(r = 1.5, h = 6, center = true, $fn = 20);
            translate([-mho, -mho, 0])
                cylinder(r = 1.5, h = 6, center = true, $fn = 20);
            translate([+mho, mho, 0])
                cylinder(r = 1.5, h = 6, center = true, $fn = 20);
            translate([+mho, -mho, 0])
                cylinder(r = 1.5, h = 6, center = true, $fn = 20);
        }
}
