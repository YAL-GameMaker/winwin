globalvar maze_dirx; maze_dirx = [1, 0, -1, 0];
globalvar maze_diry; maze_diry = [0, -1, 0, 1];

function scr_maze_gen(width, height, debug_window = undefined) {
	var right = width - 1;
	var bottom = height - 1;
	var grid = ds_grid_create(width, height);
	// current position:
	var cx = floor(random(width));
	var cy = floor(random(height));
	// delta/pointing direction:
	var dn = irandom(3);
	var dx = maze_dirx[dn];
	var dy = maze_diry[dn];
	//
	var ta = array_create(4, 0);
	static nonFullList = ds_list_create();
	//
	var debug_size = 1;
	if (!winwin_exists(debug_window)) debug_window = undefined;
	if (debug_window != undefined) {
		debug_size = min(
			winwin_get_width(debug_window) div width,
			winwin_get_height(debug_window) div height,
		);
	}
	//
	for (;;) { // maze generator loop!
		grid[#cx, cy] |= MazeFlags.Visited;
		if (debug_window != undefined) {
			winwin_draw_begin(debug_window);
			winwin_draw_clear(#889EC5);
			scr_maze_draw(grid, 0, 0, debug_size, cx, cy, dn);
			winwin_draw_end();
			winwin_sleep(16);
		}
		// check and mark adjacent cells
		if (cx > 0) scr_maze_check_full(grid, right, bottom, cx - 1, cy);
		if (cx < right) scr_maze_check_full(grid, right, bottom, cx + 1, cy);
		if (cy > 0) scr_maze_check_full(grid, right, bottom, cx, cy - 1);
		if (cy < bottom) scr_maze_check_full(grid, right, bottom, cx, cy + 1);
		// shall we just dig straight?
		if (random(2) < 1
			&& (dx == 0 || dx < 0 && cx > 0 || dx > 0 && cx < right)
			&& (dy == 0 || dy < 0 && cy > 0 || dy > 0 && cy < bottom)
			&& !(grid[#cx + dx, cy + dy] & MazeFlags.Visited)
		) {
			scr_maze_connect(grid, cx, cy, dn);
			cx += dx;
			cy += dy;
		} else {
			// can we just change direction while standing here..?
			var tn = 0;
			if (cx > 0 && !(grid[#cx - 1, cy] & MazeFlags.Visited)) ta[tn++] = MazeDir.Left;
			if (cx < right && !(grid[#cx + 1, cy] & MazeFlags.Visited)) ta[tn++] = MazeDir.Right;
			if (cy > 0 && !(grid[#cx, cy - 1] & MazeFlags.Visited)) ta[tn++] = MazeDir.Up;
			if (cy < bottom && !(grid[#cx, cy + 1] & MazeFlags.Visited)) ta[tn++] = MazeDir.Down;
			//
			if (tn > 0) {
				// we can turn:
				dn = ta[irandom(tn - 1)];
				dx = maze_dirx[dn];
				dy = maze_diry[dn];
				scr_maze_connect(grid, cx, cy, dn);
				if (tn == 1) { // this was the last empty side!
					grid[#cx, cy] |= MazeFlags.Full;
				}
				cx += dx;
				cy += dy;
			} else {
				grid[#cx, cy] |= MazeFlags.Full;
				if (ds_grid_get_min(grid, 0, 0, right, bottom) >= MazeFlags.Full) {
					// no cells without a Full flag left, the maze is complete!
					break;
				}
				// todo: could use ds_grid_get_min to look up the next suitable element,
				// though that's trickier with two conditions
				ds_list_clear(nonFullList);
				for (var i = 0; i < width; i++)
				for (var k = 0; k < height; k++) {
					if (grid[#i, k] & MazeFlags.Visited
						&& !(grid[#i, k] & MazeFlags.Full)
					) ds_list_add(nonFullList, i | (k << 16));
				}
				//
				var nonFullPick = irandom(ds_list_size(nonFullList) - 1);
				var nonFullPair = nonFullList[|nonFullPick];
				cx = nonFullPair & 0xFFFF;
				cy = nonFullPair >> 16;
				dn = irandom(3);
				dx = maze_dirx[dn];
				dy = maze_diry[dn];
			} // re-positioning
		} // doing a turn
	} // gen loop
	return grid;
}
function scr_maze_connect(grid, cx, cy, dn) {
	switch (dn) {
		case MazeDir.Right:
			grid[#cx, cy] |= MazeFlags.Right;
			grid[#cx + 1, cy] |= MazeFlags.Left;
			break;
		case MazeDir.Up:
			grid[#cx, cy] |= MazeFlags.Up;
			grid[#cx, cy - 1] |= MazeFlags.Down;
			break;
		case MazeDir.Left:
			grid[#cx, cy] |= MazeFlags.Left;
			grid[#cx - 1, cy] |= MazeFlags.Right;
			break;
		case MazeDir.Down:
			grid[#cx, cy] |= MazeFlags.Down;
			grid[#cx, cy + 1] |= MazeFlags.Up;
			break;
	}
}
function scr_maze_check_full(grid, right, bottom, cx, cy) {
	if !(grid[#cx, cy] & MazeFlags.Visited) exit;
	if (grid[#cx, cy] & MazeFlags.Full) exit;
	//
	if (cx > 0 && !(grid[#cx - 1, cy] & MazeFlags.Visited)) exit;
	if (cx < right && !(grid[#cx + 1, cy] & MazeFlags.Visited)) exit;
	if (cy > 0 && !(grid[#cx, cy - 1] & MazeFlags.Visited)) exit;
	if (cy < bottom && !(grid[#cx, cy + 1] & MazeFlags.Visited)) exit;
	//
	grid[#cx, cy] |= MazeFlags.Full;
}
function scr_maze_draw(grid, _x, _y, size, cx = -1, cy = -1, cd = -1) {
	var width = ds_grid_width(grid);
	var height = ds_grid_height(grid);
	for (var i = 0; i < width; i++)
	for (var k = 0; k < height; k++) {
		draw_sprite_stretched(spr_maze, grid[#i, k] & 15,
			_x + i * size,
			_y + k * size,
			size, size,
		);
	}
	if (cd >= 0) {
		draw_sprite_stretched(spr_maze_digger, cd,
			_x + cx * size,
			_y + cy * size,
			size, size,
		);
	}
}
enum MazeDir {
	Right,
	Up,
	Left,
	Down,
}
enum MazeFlags {
	Right = 1,
	Up = 2,
	Left = 4,
	Down = 8,
	Visited = 16,
	Full = 32,
}