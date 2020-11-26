#include "gtk.h"

//---- Main GTK

GtkWidget* window;
GtkWidget* image;
gchar *filename = "image/test_00.bmp";


GtkWidget* create_file_chooser_dialog(GtkFileChooserAction action)
{
    GtkWidget* dialog = NULL;
    switch(action)
    {
        case GTK_FILE_CHOOSER_ACTION_OPEN:
            dialog = gtk_file_chooser_dialog_new("Open File",
                    NULL, action,
                    "Cancel", GTK_RESPONSE_CANCEL,
                    "Open", GTK_RESPONSE_ACCEPT,
                    NULL); break;
        case GTK_FILE_CHOOSER_ACTION_SAVE:
            gtk_file_chooser_dialog_new ("Save File",
                    NULL, action,
                    "Cancel", GTK_RESPONSE_CANCEL,
                    "Save", GTK_RESPONSE_ACCEPT,
                    NULL); break;
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

void load_file(gchar *filename)
{
    if (filename[0] == 0)
        return;

    SDL_Surface* image_surface;
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
            gtk_image_set_from_file(GTK_IMAGE(image), filename);
            gtk_window_resize(GTK_WINDOW(window),
                image_surface->w, image_surface->h);
    }

    SDL_FreeSurface(image_surface);
}


void choose_file()
{
    GtkWidget* dialog;
    dialog = create_file_chooser_dialog(GTK_FILE_CHOOSER_ACTION_OPEN);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        load_file(filename);
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

void lunch_ocr()
{
    if (filename[0] == 0)
        return;

    init_sdl();

    SDL_Surface *image_surface;
    image_surface = IMG_Load(filename);

    GrayScale(image_surface);
    SDL_SaveBMP(image_surface, "image_grayscale.bmp");
    load_file("image_grayscale.bmp");

    Otsu(image_surface);
    SDL_SaveBMP(image_surface, "image_binarized.bmp");
    load_file("image_binarized.bmp");

    Contour(image_surface);
    SDL_SaveBMP(image_surface, "image_contour.bmp");
    load_file("image_contour.bmp");

    remove("image_grayscale.bmp");
    remove("image_binarized.bmp");
    remove("image_contour.bmp");
    SDL_FreeSurface(image_surface);
}

/*void lunch_neuralnetwork()
{
	//TODO
}*/


// Main

int gtk_init_window(int argc, char **argv)
{
    // Initializes GTK.
    gtk_init(&argc, &argv);

    // Gets the widgets.
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    image = gtk_image_new_from_file(filename);

    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget* box_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

    GtkWidget* openBMP_button = gtk_button_new_with_label("Open image");
    GtkWidget* lunchOCR_button = gtk_button_new_with_label("Lunch OCR");
//    GtkWidget* NN_button = gtk_button_new_with_label("Neural Network");

    // Window parameter
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "La compagnie des nez rouges <3");
    gtk_window_set_icon_from_file(GTK_WINDOW(window), "image/logo_OCR.png", NULL);

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(openBMP_button, "clicked", G_CALLBACK(choose_file), NULL);
    g_signal_connect(lunchOCR_button, "clicked", G_CALLBACK(lunch_ocr), NULL);
//    g_signal_connect(NN_button, "clicked", G_CALLBACK(lunch_neuralnetwork), NULL);

    // Box
    gtk_box_pack_start(GTK_BOX(main_box), box_1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), image, FALSE, TRUE, 0);
    /*A inserer un autre pour afficher le texte*/
    gtk_box_pack_start(GTK_BOX(box_1), openBMP_button, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box_1), lunchOCR_button, FALSE, TRUE, 0);
//    gtk_box_pack_start(GTK_BOX(box_1), NN_button, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    gtk_widget_show_all(window);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}