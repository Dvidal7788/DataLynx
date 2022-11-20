

    char *file_path = inf_buffer("Name of list: ");

    // Add Prepend "csv/" and append ".csv" to file_name (prepending requires char by char so it does not add \0)
    uint16_t l = strlen(file_path);
    file_path = realloc(file_path, (l+1+strlen("csv/.csv"))*sizeof(char));
    if (file_path == NULL) {if_error("Failed to reallocated buffer for file name");}

    // Move user input over 4 spaces (make room for "csv/") (must use tmp[] bc strcpy(&file_name[4], file_name) causes issues since it copies over in loop)
    char list_name[l+1];
    strcpy(list_name, file_path);
    strcpy(&file_path[4], list_name);
    const char *pre = "csv/";
    for (uint8_t i = 0; i < strlen(pre); i++) {file_path[i] = pre[i];}
    strcpy(&file_path[l+4], ".csv");
