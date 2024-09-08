if (!winwin_exists(window)) exit;
//
if (check(vk_left) || check(ord("A"))) {
	motion_add(180, accel);
	if (image_xscale > 0) image_xscale *= -1;
}
if (check(vk_right) || check(ord("D"))) {
	motion_add(0, accel);
	if (image_xscale < 0) image_xscale *= -1;
}
if (check(vk_up) || check(ord("W"))) motion_add(90, accel);
if (check(vk_down) || check(ord("S"))) motion_add(270, accel);
//
speed = min(speed, maxspeed);

if (speed > 0.1) {
	sprite_index = spr_walk;
} else sprite_index = spr_idle;