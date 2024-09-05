winwin_update();
if (!winwin_get_visible(extra)) {
	if (extra.show_in == 0) {
		extra.show_in = 30;
	} else if (--extra.show_in <= 0) {
		winwin_set_visible(extra, true);
		winwin_set_caption(extra, "I'm back :)");
	}
}
