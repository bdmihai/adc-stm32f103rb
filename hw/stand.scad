$fn = 64;

difference() {
    union() {
        cube([50,100,2]);
        translate([4.5, 4.5, 0]) difference() {
            cylinder(h=7, d=7);
            translate([0, 0, 2]) cylinder(h=7, d=2.3);
        }
        
        translate([4.5+41, 4.5, 0]) difference() {
            cylinder(h=7, d=7);
            translate([0, 0, 2]) cylinder(h=7, d=2.3);
        }
        
        translate([4.5+41, 4.5+33, 0]) difference() {
            cylinder(h=7, d=7);
            translate([0, 0, 2]) cylinder(h=7, d=2.3);
        }
        
        translate([4.5, 4.5+33, 0]) difference() {
            cylinder(h=7, d=7);
            translate([0, 0, 2]) cylinder(h=7, d=2.3);
        }
        
        
        translate([9+32, 4.5+82, 0]) difference() {
            cylinder(h=7, d=7);
            translate([0, 0, 2]) cylinder(h=7, d=2.3);
        }
        
        translate([9, 4.5+82, 0]) difference() {
            cylinder(h=7, d=7);
            translate([0, 0, 2]) cylinder(h=7, d=2.3);
        }
    }
    for (i=[19:5:79]) {
        translate([15,i,-1])
        cube([20, 2, 4]);
    }
}


