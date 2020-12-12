#include <gtk/gtk.h>
#include "sdl_base.h"

#include "segmentation.h"
#include "grayscale.h"
#include "otsu.h"
#include "hough.h"

#define IMG_W 1000
#define IMG_H 1000


// ---- GTK Interface

GtkWidget *window;
GtkWidget *image;
gchar *filename = "image/image_test0.jpg";


// ---- Tools dialog

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


// ---- Button function

// Load image

void load_file(gchar *file)
{
    SDL_Surface* image_surface;
    image_surface = IMG_Load(file);

    if (image_surface)
    {
        filename = file;

        size_t w = image_surface->w;
        size_t h = image_surface->h;

        if (w > IMG_W || h > IMG_H)
        {
            if (w > IMG_W)
                w = IMG_W;

            if (h > IMG_H)
                h = IMG_H;

            image_surface = resize(image_surface, w, h);
            SDL_SaveBMP(image_surface, "image/image_resized.bmp");
            gtk_image_set_from_file(GTK_IMAGE(image),
                "image/image_resized.bmp");
        }
        else
            gtk_image_set_from_file(GTK_IMAGE(image), filename);

        gtk_window_resize(GTK_WINDOW(window), w, h);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    }

    SDL_FreeSurface(image_surface);
}

// Choose file

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

// Save file

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

// Lunch OCR

void lunch_ocr()
{
    SDL_Surface *image_surface;
    image_surface = IMG_Load(filename);

    // Apply Grayscale
    GrayScale(image_surface);
    SDL_SaveBMP(image_surface, "image/seg_image-grayscale.bmp");
    load_file("image/seg_image-grayscale.bmp");

    // Apply Binarisation
    Otsu(image_surface);
    SDL_SaveBMP(image_surface, "image/seg_image-binarized.bmp");
    load_file("image/seg_image-binarized.bmp");

    // Apply Canny and find angle for rotate the image
    int angle = Hough_Transform(image_surface);
    image_surface = Rotate(image_surface, angle);
    SDL_SaveBMP(image_surface, "image/seg_image-canny.bmp");
    load_file("image/seg_image-binarized.bmp");

    // Apply Segmentation
    Segmentation(image_surface);
    SDL_SaveBMP(image_surface, "image/seg_image-contour.bmp");
    load_file("image/seg_image-contour.bmp");

    GtkWidget *text = gtk_message_dialog_new(GTK_WINDOW(window),
                        GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,
                        "TEXT : %s",
    "blalblalba\n blabla bla\n sqdhbfhqbdsfjbsdjf sjwbnlijefnk jSBFlkjBJQSBLfnSBlhjbfslfkNJLXlcb ljSB jkFBEk");
    gtk_window_set_title(GTK_WINDOW(text), "Text");
    gtk_dialog_run(GTK_DIALOG(text));
    gtk_widget_destroy(text);

    // Final image
    SDL_SaveBMP(image_surface, "image/seg_image_ocr.bmp");

    SDL_FreeSurface(image_surface);
}

void rotate90()
{
    SDL_Surface* image_surface;
    image_surface = IMG_Load(filename);

    image_surface = Rotate(image_surface, 90);
    SDL_SaveBMP(image_surface, "image/seg_image-rotate90.bmp");
    load_file("image/seg_image-rotate90.bmp");

    SDL_FreeSurface(image_surface);
}


// Main : GTK Interface

int gtk_init_window(int argc, char **argv)
{
    // Initializes GTK
    gtk_init(&argc, &argv);

    // Gets the widgets
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    image = gtk_image_new_from_file(filename);

    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget* box_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

    GtkWidget* openBMP_button = gtk_button_new_with_label("Open image");
    GtkWidget* lunchOCR_button = gtk_button_new_with_label("Lunch OCR");
    GtkWidget* rotate90_button = gtk_button_new_with_label("Rotate 90°");

    // Window parameter
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "La compagnie des nez rouges <3");
    gtk_window_set_icon_from_file(GTK_WINDOW(window), "image/logo_OCR.png", NULL);

    // Connects signal handlers
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(openBMP_button, "clicked", G_CALLBACK(choose_file), NULL);
    g_signal_connect(lunchOCR_button, "clicked", G_CALLBACK(lunch_ocr), NULL);
    g_signal_connect(rotate90_button, "clicked", G_CALLBACK(rotate90), NULL);

    // Box
    gtk_box_pack_start(GTK_BOX(main_box), box_1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), image, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box_1), openBMP_button, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box_1), lunchOCR_button, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box_1), rotate90_button, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    gtk_widget_show_all(window);

    // Runs the main loop
    gtk_main();

    // Exits
    return 0;
}
