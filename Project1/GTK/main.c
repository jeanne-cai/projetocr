#include <stdlib.h>
#include <gtk/gtk.h>
#include <SDL/SDL_image.h>

#define IMG_W 700
#define IMG_H 700

GtkWidget *pImage;

GtkWidget* create_file_chooser_dialog(GtkFileChooserAction action)
{
	GtkWidget *dialog = NULL;

	switch(action)
	{
		case GTK_FILE_CHOOSER_ACTION_OPEN:
			dialog = gtk_file_chooser_dialog_new("Open File",
					NULL, action,
					"Cancel", GTK_RESPONSE_CANCEL,
					"Open", GTK_RESPONSE_ACCEPT,
					NULL);
			break;

		case GTK_FILE_CHOOSER_ACTION_SAVE:
			gtk_file_chooser_dialog_new ("Save File",
					NULL, action,
					"Cancel", GTK_RESPONSE_CANCEL,
					"Save", GTK_RESPONSE_ACCEPT,
					NULL);
			break;

		default:
			break;
	}

	return dialog;
}


/*SDL_Surface* Resize(SDL_Surface *image)
{
	SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
			IMG_W, IMG_H,
			image->format->BitsPerPixel,0,0,0,0);
	SDL_BlitScaled(image, NULL, dest, NULL);
	return dest;
}*/

void open_file(gchar *filename)
{
	if (filename[0] == 0)
		return;

	SDL_Surface *image_surface;
	image_surface = IMG_Load(filename);

	if (image_surface)
	{
		/*if (image_surface->w > IMG_W && image_surface->h > IMG_H)
		{
			SDL_Surface *new = Resize(image_surface);
			SDL_SaveBMP(new, "image_resize");
			gtk_image_set_from_file(GTK_IMAGE(pImage), "image_resize");
		}
		else*/
			gtk_image_set_from_file(GTK_IMAGE(pImage), filename);
	}
}


void choose_file()
{
	GtkWidget *dialog;
	dialog = create_file_chooser_dialog(GTK_FILE_CHOOSER_ACTION_OPEN);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename;
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		open_file(filename);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}


/*void save_file()
{
	GtkWidget *dialog;
	GtkFileChooser *chooser;

	dialog = create_file_chooser_dialog(GTK_FILE_CHOOSER_ACTION_SAVE);
	chooser = GTK_FILE_CHOOSER(dialog);

	if (gtk_dialog_run(GTK_DIALOG(dialog))  == GTK_RESPONSE_ACCEPT)
	{
		char *filename;
		filename = gtk_file_chooser_get_filename(chooser);
		//save_to_file (filename);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}*/


int main(int argc, char **argv)
{
	GtkWidget *pWindow;
	GtkWidget *pVBox;
	GtkWidget *pFile;

	gtk_init(&argc, &argv);

	// Main window
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(pWindow), "La compagnie des nez rouges <3");

	// Exit
	g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Box
	pVBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// Open image
	pFile = gtk_button_new_with_label("Open image");
	gtk_box_pack_start(GTK_BOX(pVBox), pFile, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(pFile), "clicked", G_CALLBACK(choose_file), NULL);

	// Image window
	pImage = gtk_image_new_from_file("my_image.jpg");
	gtk_box_pack_start(GTK_BOX(pVBox), pImage, TRUE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

	/*
	pSave = gtk_button_new_with_label("Save image");
	gtk_box_pack_start(GTK_BOX(pVBox), pSave, TRUE, FALSE, 5);
	g_signal_connect(pSave, "clicked",  G_CALLBACK(save_file), GTK_WINDOW(pWindow));
	*/

	gtk_widget_show_all(pWindow);

	gtk_main();

	return EXIT_SUCCESS; 
}
