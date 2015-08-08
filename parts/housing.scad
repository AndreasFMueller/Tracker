//
// housing.scad -- ball bearing housing for the tracker
//
// (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
//
l = 35;
l1 = l * 2 / sqrt(3);
l2 = l * sqrt(2);
h = 12;
d = 1;
gap = 0.125;

with_spacer = 0;

difference() {
	intersection() {
		translate([-l/2, -l/2, 0]) cube([l, l, h]);
		rotate([0, 0, 45])
		cylinder(r1 = l2/2 + h - d, r2 = 0, h = l2/2 + h - d, $fn = 4); 
		cylinder(r = l1/2, h = h, $fn = 100);
		cylinder(r1 = l1/2 + h - d, r2 = 0, h = l1/2 + h - d, $fn = 100);
	}
	union() {
		translate([0, 0, 7.5 - h])
			cylinder(r = 12 + gap, h = h, $fn = 100);
		translate([0, 0, 8 - h])
			cylinder(r = 9, h = h);
		translate([0, 0, 10.5 - h])
			cylinder(r = 7, h = h, $fn = 100);
		translate([0, 0, 1])
			cylinder(r = 4, h = h, $fn = 100);
		for (alpha = [ 45 : 90 : 315 ]) {
			rotate([0, 0, alpha])
				translate([(12 + l1/2) / 2, 0, h + 0.01])
					rotate([180, 0, 0])
					union() {
						cylinder(r = 1.5, h = h + 1, $fn = 20);
						cylinder(r1 = 3, r2 = 0, h = 3, $fn = 20);
					}
		}
	}
}

if (with_spacer) {
	translate([30, 0, h - 0.5])
	difference() {
			cylinder(r = 11.5, h = 0.5);
			translate([0, 0, -0.25])
				cylinder(r = 9, h = 1);
	}
}
	
