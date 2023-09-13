#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to read a cell from the CSV file
int readCell(FILE* file, char* buffer, int bufferSize) {
    int i = 0;
    char c;
    while ((c = fgetc(file)) != EOF && c != ',' && c != '\n') {
        if (i < bufferSize - 1) {
            buffer[i++] = c;
        } else {
            // Handle buffer overflow
            fprintf(stderr, "Buffer overflow while reading cell\n");
            return -1;
        }
    }
    buffer[i] = '\0';  // Null-terminate the string
    return i;
}

int main() {
    FILE* excelFile = fopen("input.xlsx", "r");  // Open the Excel file for reading
    FILE* csvFile = fopen("output.csv", "w");    // Open a CSV file for writing

    if (excelFile == NULL || csvFile == NULL) {
        perror("Error opening files");
        return 1;
    }

    char cell[256];  // Assuming a maximum cell size of 255 characters
    int cellsRead = 0;
    while (readCell(excelFile, cell, sizeof(cell)) >= 0) {
        if (cellsRead > 0) {
            fprintf(csvFile, ",");
        }
        fprintf(csvFile, "%s", cell);
        cellsRead++;
    }

    fclose(excelFile);
    fclose(csvFile);

    printf("Converted %d cells from Excel to CSV.\n", cellsRead);

    return 0;
}
