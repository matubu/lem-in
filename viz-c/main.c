#include <gtk/gtk.h>

static void print_hello(GtkWidget *widget, gpointer data)
{
	g_print ("Hello World\n");
}

static void draw_function (GtkDrawingArea *area,
			cairo_t        *cr,
			int             width,
			int             height,
			gpointer        data)
{
	GdkRGBA color = {1.0, 0, 0, 1.0};
	GtkStyleContext *context;

	context = gtk_widget_get_style_context(GTK_WIDGET(area));

	cairo_arc(cr,
				width / 2.0, height / 2.0,
				MIN (width, height) / 2.0,
				0, 2 * G_PI);

	// gtk_style_context_get_color(context,
	// 							&color);
	printf("Draw");
	gdk_cairo_set_source_rgba(cr, &color);

	cairo_fill(cr);
}

static void
activate (GtkApplication *app,
		gpointer        user_data)
{
	GtkWidget	*window = gtk_application_window_new(app);
	gtk_window_set_title (GTK_WINDOW (window), "Lem-In");
	gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

	GtkWidget	*area = gtk_drawing_area_new();
	gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(area), 100);
	gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(area), 100);
	gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area),
									draw_function,
									NULL, NULL);
	gtk_window_set_child(GTK_WINDOW(window), area);

	GtkWidget	*box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

	gtk_window_set_child(GTK_WINDOW(window), box);

	GtkWidget	*button = gtk_button_new_with_label("Run");

	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

	gtk_box_append(GTK_BOX(box), button);

	gtk_widget_show(window);
}

int main(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
