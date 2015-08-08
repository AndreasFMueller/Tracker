/*
 * spacerring.scad -- Spacer ring 
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
height = 5;
width = 2.5;
innerradius = 9.5 / 2;

difference() {
	cylinder(r = innerradius + 2.5, h = height, center = true, $fn = 60);
	cylinder(r = innerradius, h = height + 1, center = true, $fn = 60);
}
