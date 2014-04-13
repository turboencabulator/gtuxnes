/*
 * Copyright (C) 2000-2001  Scott Weber  <scott@scottweber.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.  Please see the file COPYING for the
 * full text of the License.
*/

#include "gtuxnes.h"

static const char *const renderer[] = {
	"auto",
	"x11",
	"diff",
	"old",
	"ggi",
	"w",
	"none",
	NULL
};

static const char *const renderer_user[] = {
	"Automatically Choose",
	"X11 Renderer",
	"Differential X11 Renderer",
	"Tile-based X11 Renderer",
	"GGI Renderer",
	"W Window System Renderer",
	"No Video Output",
	NULL
};

static const char *const mirror[] = {
	"v",
	"h",
	"s",
	"n",
	NULL
};

static const char *const mirror_user[] = {
	"Vertical",
	"Horizontal",
	"Single-Screen",
	"None",
	NULL
};

static const char *const bltinpal[] = {
	"loopy",
	"quor",
	"chris",
	"matt",
	"pasofami",
	"crashman",
	"mess",
	"zaphod-cv",
	"zaphod-smb",
	"vs-drmar",
	"vs-cv",
	"vs-smb",
	NULL
};

const char *renderer_from_index(int index)
{
	int last = (sizeof renderer / sizeof *renderer) - 1;

	if (index < 0 || index > last) {
		index = last;
	}
	return renderer[index];
}

const char *mirror_from_index(int index)
{
	int last = (sizeof mirror / sizeof *mirror) - 1;

	if (index < 0 || index > last) {
		index = last;
	}
	return mirror[index];
}

const char *bltinpal_from_index(int index)
{
	int last = (sizeof bltinpal / sizeof *bltinpal) - 1;

	if (index < 0 || index > last) {
		index = last;
	}
	return bltinpal[index];
}

void correct_ntsc_value(GtkWidget *entry, gpointer item)
{
	double entry_val;
	entry_val = atof(gtk_entry_get_text(GTK_ENTRY(entry)));

	if (GPOINTER_TO_INT(item) == NTSCHUE) {
		if (entry_val > 360.0)
			gtk_entry_set_text(GTK_ENTRY(entry), "360.0");
		if (entry_val < 0.0)
			gtk_entry_set_text(GTK_ENTRY(entry), "0.0");
	} else /* item == NTSCTINT */ {
		if (entry_val > 1.0)
			gtk_entry_set_text(GTK_ENTRY(entry), "1.0");
		if (entry_val < 0.0)
			gtk_entry_set_text(GTK_ENTRY(entry), "0.0");
	}
}

static void ntsc_toggle(GtkWidget *button, gpointer w)
{
	gboolean active;

	active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button));
	gtk_widget_set_sensitive(widgets[NTSCHUE], active);
	gtk_widget_set_sensitive(widgets[NTSCTINT], active);
}

GtkWidget *create_video_options_page(void)
{
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *button;
	GtkWidget *entry;
	GtkWidget *lbl;

	frame = gtk_frame_new(NULL);

	vbox = gtk_vbox_new(FALSE, 5);

	hbox = create_toggled_combo("Specify Renderer", RENDERER, 150, renderer_user);
	gtk_combo_box_set_active(GTK_COMBO_BOX(widgets[RENDERER]), 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = create_toggled_entry("Scanlines   Intensity:", SCANLINES, 30);
	gtk_entry_set_text(GTK_ENTRY(widgets[SCANLINES]), "0");
	lbl = gtk_label_new("%");
	gtk_box_pack_start(GTK_BOX(hbox), lbl, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = create_toggled_entry("Specify Window Geometry:", GEOMETRY, 70);
	gtk_entry_set_text(GTK_ENTRY(widgets[GEOMETRY]), "640x480");
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = create_toggled_entry("Enlarge:", ENLARGE, 30);
	gtk_entry_set_text(GTK_ENTRY(widgets[ENLARGE]), "2");
	lbl = gtk_label_new("x");
	gtk_box_pack_start(GTK_BOX(hbox), lbl, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = create_toggled_entry("Specify Display/Target ID:",
							DISPLAY, 125);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = create_toggled_combo("Specify Mirroring", MIRROR, 80, mirror_user);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = create_toggled_combo("Builtin Palette:", BLTINPAL, 90, bltinpal);
	gtk_combo_box_set_active(GTK_COMBO_BOX(widgets[BLTINPAL]), 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = create_toggled_file("Palette File:", PALFILE, 200, "Choose Palette File");
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 3);
	toggles[NTSC] = gtk_check_button_new_with_label("NTSC Palette:");
	toggles[NTSC+1] = toggles[NTSC];
	gtk_box_pack_start(GTK_BOX(hbox), toggles[NTSC], FALSE, FALSE, 0);
	g_signal_connect(toggles[NTSC], "clicked", G_CALLBACK(ntsc_toggle), NULL);
	lbl = gtk_label_new("Hue Angle:");
	gtk_box_pack_start(GTK_BOX(hbox), lbl, FALSE, FALSE, 0);
	entry = gtk_entry_new();
	gtk_widget_set_sensitive(entry, FALSE);
	gtk_widget_set_size_request(entry, 50, -1);
	gtk_entry_set_text(GTK_ENTRY(entry), "332.0");
	gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);
	widgets[NTSCHUE] = entry;
	g_signal_connect(entry, "activate", G_CALLBACK(correct_ntsc_value), GINT_TO_POINTER(NTSCHUE));
	lbl = gtk_label_new("Tint Level:");
	gtk_box_pack_start(GTK_BOX(hbox), lbl, FALSE, FALSE, 0);
	entry = gtk_entry_new();
	gtk_widget_set_sensitive(entry, FALSE);
	gtk_widget_set_size_request(entry, 30, -1);
	gtk_entry_set_text(GTK_ENTRY(entry), "0.5");
	gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 0);
	widgets[NTSCTINT] = entry;
	g_signal_connect(entry, "activate", G_CALLBACK(correct_ntsc_value), GINT_TO_POINTER(NTSCTINT));
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	button = create_toggle("Display In Root Window", DISPINROOT);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

	button = create_toggle("Static Color Allocation", STATCOLOR);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

	button = create_toggle("Grayscale", GRAYSCALE);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(frame), vbox);

	return frame;
}
