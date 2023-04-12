#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void search(char *dir_name, char *file_name) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_name)) == NULL) {
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
            search(path, file_name);
        } else {
            if (strcmp(entry->d_name, file_name) == 0) {
                printf("File found: %s/%s\n", dir_name, entry->d_name);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    search("/", argv[1]);

    return 0;
}
