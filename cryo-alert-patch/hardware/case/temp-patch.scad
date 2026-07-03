// Cryo Alert Patch — Temp Patch TPU Enclosure (32mm)
// 3D-print in TPU 95A, 0.12 mm layer height, no supports
// Design version for 20x20mm PCB and CR1220 battery

/* [Dimensions] */
// Overall outer diameter of the circular case (mm)
outer_diameter   = 32.0;
// Overall height of the assembled case (mm)
total_height     = 4.0;
// Wall thickness of the circular enclosure (mm)
wall_thickness   = 1.2;

/* [Case Split] */
// Height of the bottom case (mm)
bottom_h = 2.2;
// Height of the top case (mm)
top_h    = 1.8;

/* [Cavity & Tolerances] */
// Material thickness under the MCU recess (mm)
floor_thick = 0.3;
// General clearance tolerance for fit (mm)
clearance   = 0.3;

/* [PCB Components] */
// PCB X dimension (mm)
pcb_x       = 20.0;
// PCB Y dimension (mm)
pcb_y       = 20.0;
// PCB thickness (mm)
pcb_z       = 0.8;
// nRF52 MCU package size (mm)
mcu_xy      = 6.0;
// nRF52 MCU package height (mm)
mcu_z       = 0.8;
// MAX30205 sensor window dimensions [X, Y] (mm)
sensor_win  = [5.0, 4.0];

/* [Battery Holder (CR1220 SMD)] */
// Battery holder overall diameter (fits solder tabs and clip, e.g. Keystone 1056) (mm)
battery_holder_d = 16.0;
// Battery holder overall height above PCB (mm)
battery_holder_h = 2.5;
// Battery holder offset in Y direction (mm)
battery_y_offset = 5.0;

/* [Alignment Lip & Snap-Fit] */
// Alignment lip height (mm)
lip_h       = 0.5;
// Alignment lip width (mm)
lip_w       = 0.6;
// Protrusion of the snap-fit ridge (mm)
snap_ridge_h = 0.2;
// Section radius of the snap-fit groove (mm)
snap_groove_r = 0.25;

/* [Render Selection] */
// Choose which part to render
part_to_render = "assembly"; // [bottom:Bottom Case, top:Top Case, assembly:Assembly, exploded:Exploded Assembly, print:Print Layout (Side-by-Side)]

/* [Hidden] */
eps      = 0.01;
$fn      = 96;

// Inner diameter of the case walls
inner_d  = outer_diameter - 2 * wall_thickness;

// ─── HELPERS ────────────────────────────────────────────────────────

module ring(od, id, h) {
  difference() {
    cylinder(d = od, h = h);
    cylinder(d = id, h = h + eps);
  }
}

module torus(r_torus, r_section) {
  rotate_extrude()
    translate([r_torus, 0, 0])
      circle(r = r_section);
}

// ─── MODULES ────────────────────────────────────────────────────────

module bottom_case() {
  difference() {
    // Outer body
    cylinder(d = outer_diameter, h = bottom_h);

    // PCB pocket (square, from top face down to z = 1.0)
    // The pocket depth is bottom_h - 1.0 = 1.2 mm.
    // The center in Z is at (1.0 + bottom_h)/2 = 1.6 mm.
    // The height of the pocket is 1.2 mm + eps.
    translate([0, 0, (1.0 + bottom_h) / 2])
      cube([pcb_x + clearance, pcb_y + clearance, bottom_h - 1.0 + eps], center = true);

    // MCU recess (centered, from z = 0.3 to z = 1.0)
    // The height of the recess is 0.7 mm.
    // The center in Z is at (0.3 + 1.0)/2 = 0.65 mm.
    translate([0, 0, (floor_thick + 1.0) / 2])
      cube([mcu_xy + clearance, mcu_xy + clearance, 1.0 - floor_thick + eps], center = true);

    // Sensor window — through to skin side
    // Positioned at bottom edge of PCB area (Y offset is -6.5 mm)
    // Through-hole from z = -eps to z = 1.0 + eps
    sensor_y = -(pcb_y / 2 - sensor_win.y / 2 - 1.5);
    translate([0, sensor_y, 0.5])
      cube([sensor_win.x, sensor_win.y, 1.0 + 2 * eps], center = true);

    // Alignment step groove on top face (cut from z = bottom_h - lip_h to z = bottom_h)
    translate([0, 0, bottom_h - lip_h])
      ring(inner_d + 2 * lip_w, inner_d, lip_h + eps);

    // Snap groove inside the step vertical wall (cut at z = bottom_h - lip_h / 2)
    translate([0, 0, bottom_h - lip_h / 2])
      torus(r_torus = inner_d/2 + lip_w, r_section = snap_groove_r);
  }
}

module top_case() {
  union() {
    difference() {
      // Outer body
      cylinder(d = outer_diameter, h = top_h);

      // Hollow interior (above wall height)
      // depth is 0.6 mm, leaving 1.2 mm ceiling
      translate([0, 0, -eps])
        cylinder(d = inner_d, h = top_h - wall_thickness + eps);

      // Battery holder pocket (fits the SMD holder tabs and clip)
      // depth is battery_holder_h - (bottom_h - 1.0) + clearance = 1.6 mm, leaving 0.2 mm ceiling
      bat_holder_depth = battery_holder_h - (bottom_h - 1.0) + clearance;
      translate([0, battery_y_offset, -eps])
        cylinder(d = battery_holder_d + clearance, h = bat_holder_depth + eps);
    }

    // Alignment step lip (protrudes downward)
    // fits into the bottom case step groove
    // Goes from z = 0 down to z = -lip_h
    // Outer diameter is inner_d + 2 * lip_w - clearance = 30.5 mm
    // Inner diameter is inner_d = 29.6 mm
    translate([0, 0, -lip_h])
      ring(inner_d + 2 * lip_w - clearance, inner_d, lip_h);

    // Snap-fit ridge on the step vertical face (at z = -lip_h / 2)
    translate([0, 0, -lip_h / 2])
      torus(r_torus = inner_d/2 + lip_w - clearance/2, r_section = snap_ridge_h);
  }
}

module assembly() {
  bottom_case();
  translate([0, 0, bottom_h])
    top_case();
}

// ─── RENDER SELECTION ───────────────────────────────────────────────

if (part_to_render == "bottom") {
  bottom_case();
} else if (part_to_render == "top") {
  top_case();
} else if (part_to_render == "exploded") {
  // Exploded assembly view for visualization
  bottom_case();
  translate([0, 0, bottom_h + 15]) // Float the top case with a 15mm gap
    top_case();
} else if (part_to_render == "print") {
  // Side-by-side print layout
  // Bottom case sits flat on Z=0 at X = -18
  translate([-18, 0, 0])
    bottom_case();
  // Top case sits flat on Z=0 at X = 18 (rotated 180 degrees to lay its flat face down)
  translate([18, 0, top_h])
    rotate([180, 0, 0])
      top_case();
} else {
  assembly();
}
