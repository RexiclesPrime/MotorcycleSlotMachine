// Motorcycle Slot Machine — garage wall case
// Units: millimetres
// OpenSCAD: set part = "front" or "back", press F6, File → Export → STL
// Print PETG (garage heat), 0.2 mm layers, 4 perimeters, 25% infill.

part = "preview"; // "front", "back", "preview"

$fn = 48;
clear = 0.35;

// --- 4.2" glass (WeAct / Good Display 400x300) ---
glass_w = 91.0;
glass_h = 77.0;
glass_t = 1.4;
aa_w = 84.8;
aa_h = 63.6;

// --- Waveshare ESP32 e-Paper Driver Board ---
esp_l = 48.5;
esp_w = 29.8;
esp_t = 8.5;

// --- 18650 + typical dual-slot holder / single keystone ---
cell_d = 19.2;
cell_l = 69.0;

// --- TP4056 Type-C charger (with protection) ---
chg_l = 27.0;
chg_w = 18.0;
chg_t = 6.0;

// --- 16 mm anti-vandal momentary ---
btn_hole = 16.2;
btn_boss_t = 2.6; // panel thickness at the button

wall = 2.6;
lip = 1.6;          // front window lip over the glass
bezel = 3.2;        // glass overlap around the active area
gap_g = 0.5;        // glass pocket clearance

inner_w = glass_w + 2 * gap_g;
inner_h = glass_h + 2 * gap_g;
win_w = aa_w - 1.2;
win_h = aa_h - 1.2;

btn_zone = 28;
case_w = inner_w + 2 * wall + 4;
case_h = inner_h + btn_zone + 2 * wall + 6;
case_d = wall + glass_t + 2.0 + esp_t + cell_d + wall; // ~40

front_t = wall + lip + glass_t + 1.2;
back_t = case_d - front_t + 1.2; // overlap seam

screw_r = 1.6;      // M3 clearance
insert_r = 2.05;    // M3 heat-set insert
boss_r = 4.2;

module rrect(s, r) {
  hull() {
    for (x = [r, s[0] - r], y = [r, s[1] - r])
      translate([x, y]) circle(r = r);
  }
}

module rbox(s, r) {
  linear_extrude(s[2]) rrect([s[0], s[1]], r);
}

module screw_xy() {
  m = wall + 3.5;
  for (x = [m, case_w - m], y = [m, case_h - m])
    translate([x, y]) children();
}

module keyhole() {
  // #8 / 4 mm screw, hang and drop
  hull() {
    circle(d = 8.5);
    translate([0, -7]) circle(d = 4.4);
  }
}

module front() {
  gx = (case_w - inner_w) / 2;
  gy = case_h - wall - 4 - inner_h;
  wx = gx + (inner_w - win_w) / 2;
  wy = gy + (inner_h - win_h) / 2;

  difference() {
    union() {
      rbox([case_w, case_h, front_t], 4);
      translate([wall - 0.8, wall - 0.8, front_t])
        rbox([case_w - 2 * wall + 1.6, case_h - 2 * wall + 1.6, 1.4], 3);
    }

    translate([wx, wy, -0.1])
      cube([win_w, win_h, front_t + 3]);

    translate([gx, gy, lip])
      cube([inner_w, inner_h, front_t]);

    // FPC folds down from the bottom edge of the glass
    translate([case_w / 2 - 9, gy - 3, lip + 0.3])
      cube([18, 6, front_t]);

    translate([case_w / 2, wall + 7 + btn_hole / 2, -0.1])
      cylinder(d = btn_hole, h = front_t + 4);

    screw_xy()
      translate([0, 0, -0.1]) cylinder(r = screw_r, h = front_t + 4);
  }

  translate([case_w / 2, wall + 7 + btn_hole + 5.2, front_t - 0.45])
    linear_extrude(0.55)
      text("SPIN", size = 4, halign = "center", valign = "center");
}

module back() {
  difference() {
    rbox([case_w, case_h, back_t], 4);

    // main cavity
    translate([wall, wall, wall])
      rbox([case_w - 2 * wall, case_h - 2 * wall, back_t], 3);

    // seam rebate for the front lip
    translate([wall - 1.0, wall - 1.0, back_t - 1.6])
      rbox([case_w - 2 * wall + 2.0, case_h - 2 * wall + 2.0, 2.2], 3);

    // M3 heat-set inserts
    screw_xy()
      translate([0, 0, -0.1]) cylinder(r = insert_r, h = 7);

    // USB-C charge (TP4056) on the bottom
    translate([case_w / 2 - 5, -0.1, wall + 4])
      cube([10, wall + 2, 6]);

    // ESP32 USB (programming) on the right side, case open not required if you
    // leave a 12 x 8 window — still easier to pop the back. Side window:
    translate([case_w - wall - 0.1, wall + 10, wall + 3])
      cube([wall + 1, 12, 8]);

    // keyholes (hang on two #8 pan heads)
    translate([case_w / 2 - 30, case_h - 14, -0.1])
      linear_extrude(wall + 0.3) keyhole();
    translate([case_w / 2 + 30, case_h - 14, -0.1])
      linear_extrude(wall + 0.3) keyhole();
  }

  // ESP32 standoffs (board sits USB toward the side window)
  translate([wall + 8, wall + 8, wall])
    for (x = [3, esp_l - 3], y = [3, esp_w - 3])
      translate([x, y, 0]) {
        difference() {
          cylinder(r = 3.2, h = 4);
          translate([0, 0, 1.2]) cylinder(r = 0.9, h = 4); // M2 self-tap
        }
      }

  // 18650 trough
  translate([wall + 8, wall + 8 + esp_w + 8, wall])
    difference() {
      cube([cell_l + 4, cell_d + 4, cell_d * 0.55]);
      translate([2, 2, 2]) cube([cell_l, cell_d, cell_d]);
    }

  // foam rib that presses the glass perimeter (not the active area)
  translate([(case_w - (inner_w - 6)) / 2,
             case_h - wall - 3 - inner_h + 3,
             back_t - 3.2])
    difference() {
      cube([inner_w - 6, inner_h - 6, 1.8]);
      translate([4, 4, -0.1])
        cube([inner_w - 14, inner_h - 14, 3]);
    }
}

if (part == "front") front();
else if (part == "back") back();
else {
  front();
  translate([case_w + 12, 0, 0]) back();
}
