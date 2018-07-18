namespace app
{

void init() {
  int argc = 1;
  char* argv[] = {new char[6], nullptr};
  strcpy(argv[0], "asura");
  char** argvp = argv;

  gtk_init(&argc, &argvp);
  GtkSettings* gtkSettings = gtk_settings_get_default();

  // Show button images
  g_type_class_unref(g_type_class_ref(GTK_TYPE_BUTTON));
  g_object_set(gtkSettings, "gtk-button-images", true, nullptr);
}

void quit() {
    if (gtk_main_level())
    {
        gtk_main_quit();
    }
}

}
