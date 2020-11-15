#include <stdlib.h>
#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../segmentation/sdl_base.h"
#include "../segmentation/segmentation.h"


//---- Main GTK

GtkWidget *pImage;
gchar *filename = "image/test_00.bmp";


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

/*
SDL_Surface* Resize(SDL_Surface *image)
{
    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
            IMG_W, IMG_H,
            image->format->BitsPerPixel,0,0,0,0);
    SDL_BlitScaled(image, NULL, dest, NULL);
    return dest;
}
*/

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

    SDL_FreeSurface(image_surface);
}


void choose_file()
{
    GtkWidget *dialog;
    dialog = create_file_chooser_dialog(GTK_FILE_CHOOSER_ACTION_OPEN);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        open_file(filename);
    }

    gtk_widget_destroy(dialog);
}

/*
void save_file()
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
}
*/

void lunch_ocr()
{
    if (filename[0] == 0)
        return;

    init_sdl();

    SDL_Surface *image_surface;
    image_surface = IMG_Load(filename);

    GrayScale(image_surface);
    SDL_SaveBMP(image_surface, "image_grayscale.bmp");
    open_file("image_grayscale.bmp");

    Contour(image_surface);
    SDL_SaveBMP(image_surface, "image_contour.bmp");
    open_file("image_contour.bmp");

	remove("image_grayscale.bmp");
	remove("image_contour.bmp");
    SDL_FreeSurface(image_surface);
}

/*
void lunch_neuralnetwork()
{
	//TODO
}
*/

// Main

void gtk_init_window(int argc, char **argv)
{
    GtkWidget *pWindow;
    GtkWidget *pVBox, *pHBox;
    GtkWidget *pFile, *pLunchOCR, *pNeural;

    gtk_init(&argc, &argv);

    // Main window
    pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(pWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(pWindow),
		"La compagnie des nez rouges <3");
	gtk_window_set_icon_from_file(GTK_WINDOW(pWindow),
		"image/logo_OCR.png", NULL);
// A implementer: Resize la taille de la fenetre si changement de taille de l'image

    // Exit
    g_signal_connect(G_OBJECT(pWindow), "destroy",
		G_CALLBACK(gtk_main_quit), NULL);

    // Vertical Box
    pVBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Open image
    pFile = gtk_button_new_with_label("Open image");
    gtk_box_pack_start(GTK_BOX(pVBox), pFile, FALSE, TRUE, 0);
    g_signal_connect(G_OBJECT(pFile), "clicked",
		G_CALLBACK(choose_file), NULL);

    // Horizontal Box
    pHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, FALSE, TRUE, 0);

    // OCR
    pLunchOCR = gtk_button_new_with_label("Lunch OCR");
    gtk_box_pack_start(GTK_BOX(pHBox), pLunchOCR, TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(pLunchOCR), "clicked",
		G_CALLBACK(lunch_ocr), NULL);

    // NeuralNetwork
    pNeural = gtk_button_new_with_label("Neural Network");
    gtk_box_pack_start(GTK_BOX(pHBox), pNeural, TRUE, TRUE, 0);
//    g_signal_connect(G_OBJECT(pFile), "clicked", G_CALLBACK(lunch_neuralnetwork), NULL);

    // Image window
    pImage = gtk_image_new_from_file(filename);
    gtk_box_pack_start(GTK_BOX(pVBox), pImage, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(pWindow), pVBox);

    /*
    pSave = gtk_button_new_with_label("Save image");
    gtk_box_pack_start(GTK_BOX(pVBox), pSave, TRUE, FALSE, 5);
    g_signal_connect(pSave, "clicked",  G_CALLBACK(save_file), GTK_WINDOW(pWindow));
    */

    gtk_widget_show_all(pWindow);
    gtk_main();
}
