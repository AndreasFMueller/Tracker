/*
 * electronics.scad 
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
board_width = 27.94 + 0.1;
board_height = 40.64 + 0.1;
board_thickness = 2.0;

gap = 0.1;

width = 44;
height = 44;
r = 4;
bottom_height = 6;
top_height = 18;

button1_x = -board_width / 2 + 1.8;
button1_y = -9;

button2_x = -board_width / 2 + 1.8;
button2_y = +14;

led1_x = 25.5 - board_width/2;
led1_y = 18;
led2_x = led1_x;
led2_y = 1.5;

switchwidth = 3.5;
switchheight = 4.5;

button_gap = 0.2;

module bottom_part() {
	difference() {
		union() {
			cube([width - 2 * r, height, bottom_height], center = true);
			cube([width, height - 2 * r, bottom_height], center = true);
			translate([-width/2 + r, -height/2 + r, 0])
				cylinder(r = r, h = bottom_height, center = true, $fn = 30);
			translate([-width/2 + r, +height/2 - r, 0])
				cylinder(r = r, h = bottom_height, center = true, $fn = 30);
			translate([+width/2 - r, -height/2 + r, 0])
				cylinder(r = r, h = bottom_height, center = true, $fn = 30);
			translate([+width/2 - r, +height/2 - r, 0])
				cylinder(r = r, h = bottom_height, center = true, $fn = 30);
		}
		union() {
			translate([0, 0, bottom_height - board_thickness])
				cube([board_width + gap, board_height + gap, bottom_height], center = true);
			translate([0, 0, 1])
				cube([board_width - 2, board_height - 2, bottom_height], center = true);
			translate([-width/2 + r, -height/2 + r, 0])
				cylinder(r = 1.6, h = bottom_height + 11, center = true, $fn = 20);
			translate([-width/2 + r, +height/2 - r, 0])
				cylinder(r = 1.6, h = bottom_height + 1, center = true, $fn = 20);
			translate([+width/2 - r, -height/2 + r, 0])
				cylinder(r = 1.6, h = bottom_height + 1, center = true, $fn = 20);
			translate([+width/2 - r, +height/2 - r, 0])
				cylinder(r = 1.6, h = bottom_height + 1, center = true, $fn = 20);
			translate([button1_x, -button1_y, 1])
				cylinder(r = 4 + button_gap, h = bottom_height,  center = true, $fn = 30);
			translate([button2_x, -button2_y, 1])
				cylinder(r = 4 + button_gap, h = bottom_height,  center = true, $fn = 30);
		}
	}
	translate([width/2 - 1.5, -9, 15/2 + bottom_height/2 - 0.01])
		cube([3, 10.5, 15], center = true);
}

module	screwhole() {
		cylinder(r = 1.6, h = top_height + 1, center = true, $fn = 20);
		translate([0, 0, -(top_height + 0.01) / 2])
			cylinder(r1 = 3, r2 = 0, h = 3, $fn = 20);
}

module button_cutout() {
	cylinder(r = 3 + button_gap, h = top_height + 0.1, center = true, $fn = 20);
	translate([0, 0, 1])
		cylinder(r = 4 + button_gap, h = top_height, center = true, $fn = 20);
}

module top_part() {
	difference() {
		union() {
			cube([width - 2 * r, height, top_height], center = true);
			cube([width, height - 2 * r, top_height], center = true);
			translate([-width/2 + r, -height/2 + r, 0])
				cylinder(r = r, h = top_height, center = true, $fn = 20);
			translate([-width/2 + r, +height/2 - r, 0])
				cylinder(r = r, h = top_height, center = true, $fn = 20);
			translate([+width/2 - r, -height/2 + r, 0])
				cylinder(r = r, h = top_height, center = true, $fn = 20);
			translate([+width/2 - r, +height/2 - r, 0])
				cylinder(r = r, h = top_height, center = true, $fn = 20);
		}
		union() {
			difference() {
				union() {
					translate([0, 0, 1])
						cube([board_width, board_height - 2, top_height], center = true);
					translate([0, 0, 1])
						cube([board_width - 2, board_height, top_height], center = true);
				}
				translate([led1_x + 1, led1_y, -2])
					cube([6, 5, top_height], center = true);
				translate([led2_x + 1, led2_y, -2])
					cube([6, 4, top_height], center = true);
			}
			translate([-width/2 + r, -height/2 + r, 0])
				screwhole();
			translate([-width/2 + r, +height/2 - r, 0])
				screwhole();
			translate([+width/2 - r, -height/2 + r, 0])
				screwhole();
			translate([+width/2 - r, +height/2 - r, 0])
				screwhole();
			// opening for the power connector
			translate([width/2, -10, top_height/2 - 4.99])
				cube([20, 10, 10], center = true);
			// opening for 
			translate([width/2,  9, 1])
				cube([20, 11, top_height], center = true);
			// openings for buttons
			translate([button1_x, button1_y, 0])
				button_cutout();
			translate([button2_x, button2_y, 0])
				button_cutout();
			// holes for fibre optics
			translate([led1_x, led1_y, 0])
				cylinder(r = 0.9, h = top_height + 1, center = true, $fn = 10);
			translate([led2_x, led2_y, 0])
				cylinder(r = 0.9, h = top_height + 1, center = true, $fn = 10);
		}
	}
//	translate([button1_x, button1_y, 0]) button();
//	translate([button2_x, button2_y, 0]) button();
}

module button() {
	difference() {
		union() {
			translate([0, 0, -1])
				cylinder(r = 3, h = top_height, center = true, $fn = 20);
			translate([0, 0, 1])
				cylinder(r = 4, h = top_height - 4, center = true, $fn = 20);
			translate([0, 0, -(top_height - 4) / 2])
				cylinder(r1 = 2, r2 = 4, h = 2, center = true, $fn = 20);
		}
		translate([-switchwidth/2 + 4, 0, top_height - switchheight])
			cube([10, 10, top_height], center = true);
	}
}


