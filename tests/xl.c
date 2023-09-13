#include <xlsxwriter.h>

int main() {
    // Create a new Excel workbook and add a worksheet.
    lxw_workbook *workbook = workbook_new("excel_to_csv.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    // Write some data to the worksheet.
    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_string(worksheet, 0, 1, "World", NULL);

    // Close the Excel workbook.
    workbook_close(workbook);

    // Now, open the same Excel file and write its contents to a CSV file.
    workbook = workbook_new("excel_to_csv.xlsx");
    worksheet = workbook_add_worksheet(workbook, NULL);

    FILE *csv_file = fopen("output.csv", "w");

    if (csv_file == NULL) {
        fprintf(stderr, "Error opening CSV file for writing.\n");
        return 1;
    }

    // Loop through the rows and columns of the Excel worksheet.
    for (int row = 0; row < 1; row++) {
        for (int col = 0; col < 2; col++) {
            // Get the cell value from the Excel worksheet.
            lxw_cell *cell = worksheet->table[row][col];

            // Write the cell value to the CSV file.
            fprintf(csv_file, "%s,", lxw_get_cell_string(cell));
        }
        fprintf(csv_file, "\n"); // End the CSV row.
    }

    // Close the CSV file.
    fclose(csv_file);

    // Close the Excel workbook.
    workbook_close(workbook);

    return 0;
}
