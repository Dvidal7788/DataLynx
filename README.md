​​# Library in C for Data Wrangling, Data Cleaning & Analytics

Overview:
--------

---
>###### This is a library of functions written in C, for use in C programs, inspired by Python's standard CSV library as well as Pandas & SQLite3, giving the user an ease of use similar to Object-Oriented Programming, but with the low-level control that C offers. This library has functionality to read/write from/to CSV files as well as data cleaning and statistical analysis functionality. My goal is to bridge the gap between Python and C, and explore what it takes to write the underlying implementation of libraries that one might otherwise take for granted if accustomed to programming in higher-level languages like Python.

---
>###### NOTE: In order to acheive Object-Oriented style functionality, the first parameter for every user-facing function must be the address of the dataLynx struct (or 'object') you have declared / constructed using the constructor (e.g. `myData.function(&myData)`). Although this is of course not a true object in the OOP sense, moving forward I will be referring to dataLynx structs as objects since this is the intended use of the struct. All memory management is handled for you, as long as you run `myData.freeAll(&myData)` at the end of your program.

<hr>


<div align="center">
  
Statistics / Aggregate Data
=========

###### (This library provides you with the following statistical / aggregate data)
</div>


##### For numeric columns:
<ul>
  
###### <li>Min</li>
###### <li>Max</li>
###### <li>Sum</li>
###### <li>Mean</li>
###### <li>Standard Deviation</li>
###### <li>25th percentile (lower quartile)</li>
###### <li>Median (50th percentile)</li>
###### <li>75th percentile (upper quartile)</li>
###### <li>Is Null (number of empty field in a given column)</li>
###### <li>Not Null (number of non-empty field in a given column)</li>
</ul>

##### For non-numeric columns:
<ul>
  
###### <li>Value Counts for each unique value in a given column (i.e. how many times that specific value appears a column).</li>
###### <li>Is Null (number of empty field in a given column)</li>
###### <li>Not Null (number of non-empty field in a given column)</li>
</ul>
<div align="center">

###### (See [Functions for Statistical / Aggregate Data](#stats) below for more details.)
</div>
<hr>
<div align="center">
  
FUNCTIONS
=========
</div>
<div align="center">

###### The following is a complete list of *user-facing* functions (i.e. 'methods' as they exist in the dataLynx object).
###### (Assume that the dataLynx object that has been declared is named 'myData' for the purposes of these s.)
</div>
<hr>



<!-- CONSTRUCTOR -->
<h4 align="center">dataLynxConstructor()</h4>
<h6 align="center">dataLynx dataLynxConstructor(void)</h6>

##### PARAMETERS:
<ul>

###### <li>This function takes no parameters</li>
</ul>

##### Use:
<ul>

###### <li>Declare a dataLynx object and assign the return of this function to the object</li>
</ul>

##### RETURN:
<ul>

###### <li>Returns a dataLynx struct (i.e. 'object').</li>
</ul>


#####  Code:

```C
dataLynx myData = dataLynxConstructor();
```
<hr>




<!-- FUNCTIONS FOR READ/WRITE FROM/TO CSV -->
<div align="center">
  
Functions for Reading/Writing from/to a CSV file:
--------
###### (These functions are all available within the .csv namespace within your dataLynx object (i.e. `myData.csv.exampleFunction()`)
</div>
<hr>


<h4 align="center">csv.openFile()</h4>
<h6 align="center">bool openFile(dataLynx *self, char *filename)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>String of filename to open. (If the filename within your dataLynx object has already been set, you may pass NULL here.)</li>
</ul>

##### Use:
<ul>

###### <li>Before reading or writing from/to a CSV, you must open the file.</li>
###### <li>This function will attempt to open the file of the filename given.</li>
###### <li>This function will use the filename provided as input if one is given, however if you have already set the filename within the dataLynx object, you may pass NULL in place of a fileame, and the function will use the filename already stored in the dataLynx object.</li>
</ul>

##### RETURN:
<ul>

###### <li>On sucess, returns true (file is successfully opened).</li>
###### <li>On failure, returns false (file is not successfully opened.)</li>
</ul>


#####  Code:

```C
myData.csv.openFile(&myData, "csv/staff.csv");
```
<hr>


<h4 align="center">csv.fileReader()</h4>
<h6 align="center">char *fileReader(dataLynx *self)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### Use:
<ul>

###### <li>This function reads the file that has already been opened using csv.openFile() into memory.</li>
###### <li>The header, as always, will be stored as an array of strings (i.e. one string per column name).</li>
###### <li>*The data will be stored in memory as one long string (i.e. raw data)*.</li>
###### <li> From this point the data can be parsed into various data structures. Typically, however, you will do this is in one fell swoop using csv.reader(), csv.dictReader or csv.reader_v3() (all of which internally use csv.fileReader() to first read the file into memory before parsing into various data structures. More on these data structures later.) I have provided the option to break this up into multiple steps for ultimate flexibility (e.g. if doing minimal or no data processing, you might want to read the file using fileReader(), because the string takes significantly less memory than the other data structures.</li>
###### <li> NOTE: *The data structure that this function creates is intended to only be used as a simpler, less memory-intensive data structure for certain instances. This data strucure does NOT offer the full capability of data processing that other data structures (like those created by csv.reader, csv.dictReader & csv.reader_v3) offer*.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns the string in which the data is stored. You do not need to assign the string to anything, as it is automatically stored in myData.raw. The string is only returned for convenience (e.g. instances such as: `printf("%s", myData.csv.fileReader(&myData)` require the string be returned.</li>
###### <li>On failure, returns NULL if any error occurs</li>
</ul>


##### Example Code:

```C
myData.csv.fileReader(&myData);
```
<hr>



<h4 align="center">csv.fileRowReader()</h4>
<h6 align="center">char **fileRowReader(dataLynx *self)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### Use:
<ul>

###### <li>This function reads the file that has already been opened using csv.openFile() into memory.</li>
###### <li>The header, as always, will be stored as an array of strings (i.e. one string per column name).</li>
###### <li>*The data will also be stored in memory as an array of strings (i.e. one row per string)*.</li>
###### <li> From this point it can be parsed into other various data structures.</li>
###### <li> NOTE: *The data structure that this function creates is intended only to be used as a simpler, less memory-intensive data structure for certain instances. This data strucure does NOT offer the full capability of data processing that other data structures (like those created by csv.reader, csv.dictReader & csv.reader_v3) offer*</li>

</ul>

##### RETURN:
<ul>

###### <li>On success, returns a pointer to an array of strings in which the data is stored. You do not need to assign the string to anything, as it is automatically stored in myData.rows. The string is only returned for convenience (e.g. instances such as: `printf("%s", myData.csv.fileReader(&myData)` require the string be returned.</li>
###### <li>On failure, returns NULL if any error occurs</li>
</ul>


##### Example Code:

```C
myData.csv.fileRowReader(&myData)
```
<hr>



<!-- CSV.READER_V3 -->
<h4 align="center">csv.reader_v3()</h4>
<h6 align="center">char ***reader_v3(dataLynx *self)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### Use:
<ul>

###### <li>This function reads the file that has already been opened using csv.openFile() into memory.</li>
###### <li>The header, as always, will be stored as an array of strings (i.e. one string per column name).</li>
###### <li>*The data will be stored in memory as a 3D array (i.e. an array of arrays, in which each secondary array is an array of strings). In other words, this is an array, which stores arrays (one for each row), each of which stores strings (one per field). See diagram below.*.</li>

</ul>

##### RETURN:
<ul>

###### <li>On success, returns a pointer to a 3D array.</li>
###### <li>Note: You do not need to assign this return value to anything. It is automatically assigned within the function to a pointer within your dataLynx object. The data structure only gets returned for  convenience.</li>
###### <li>On failure, returns NULL.</li>
</ul>


##### Example Code:

```C
myData.csv.reader_v3(&myData);
```
<hr>


<!-- CSV.READER -->
<h4 align="center">csv.reader()</h4>
<h6 align="center">node **reader(dataLynx *self);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### Use:
<ul>

###### <li>This function reads the file that has already been opened using csv.openFile() into memory.</li>
###### <li>The header, as always, will be stored as an array of strings (i.e. one string per column name).</li>
###### <li>*The data will be stored in memory as an array of linked lists. Each linked list represents a row. Each node in the linked list represents a value (i.e. field). See diagram below.*.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns a pointer to an array of linked lists.</li>
###### <li>On failure, returns NULL.</li>
</ul>


##### Example Code:

```C
myData.csv.reader(&myData);
```
<hr>



<!-- CSV.DICTREADER -->
<h4 align="center">csv.dictReader()</h4>
<h6 align="center">dict_node **dictReader(dataLynx *self);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### Use:
<ul>

###### <li>This function reads the file that has already been opened using csv.openFile() into memory.</li>
###### <li>The header, as always, will be stored as an array of strings (i.e. one string per column name).</li>
###### <li>*The data will be stored in memory as an array of dict-style linked lists. Each linked list represents a row. Each node in the linked list represents a value (i.e. field). In addition to storing a value, each node also stores the column name that corresponds with that particular value (e.g. {'Employee ID', '4511'}), in which 'Employee ID' is the column name and '4511' is the corresponding value for that row (i.e. linked list) that this particular node is in. See diagram below.*.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns a pointer to an array of dict-style linked lists.</li>
###### <li>On failure, returns NULL.</li>
</ul>


##### Example Code:

```C
myData.csv.dictReader(&myData);
```
<hr>


<!-- CSV.FIELDREADER() -->
<h4 align="center">csv.fieldReader()</h4>
<h6 align="center">char *fieldReader(dataLynx *self, uintmax_t desired_row, char *column_name)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>Integer row index location of field you wish to access.</li>
###### <li>String of column name of field you wish to access.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and (as long as a valid row integer/column name is provided), the function will read the deisred field directly from the opened CSV (must have already used csv.openFile()).This prevents the need for reading the entire CSV into memory, if you are only looking for one or a few fields.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns string containing the value at the provided row integer/column name. (Do not free. You only need to run freeAll() at the end of your program or when you are done with that specific dataLynx object.)</li>
###### <li>On failure, returns NULL.</li>
</ul>


##### Example Code:

```C
myData.fieldReader(&myData, 0, "First Name");
```

<ul>
 
###### <li>This will return a string with the value in row 0, column name "First Name".</li>
</ul>
<hr>



<!-- CSV.FIELDREADER2() -->
<h4 align="center">csv.fieldReader2()</h4>
<h6 align="center">char *fieldReader2(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>Integer row index location of field you wish to access.</li>
###### <li>Integer column index location of field you wish to access.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and (as long as a valid row integer/column integer is provided), the function will read the deisred field directly from the opened CSV (must have already used csv.openFile()).This prevents the need for reading the entire CSV into memory, if you are only looking for one or a few fields.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns string containing the value at the provided row integer/column integer. (Do not free. You only need to run freeAll() at the end of your program or when you are done with that specific dataLynx object.)</li>
###### <li>On failure, returns NULL.</li>
</ul>


##### Example Code:

```C
myData.fieldReader2(&myData, 0, 1);
```

<ul>
 
###### <li>This will return a string with the value in row 0, column 1.</li>
</ul>
<hr>

<!-- CSV.FIELDWRITER-->
<h4 align="center">csv.fieldWriter()</h4>
<h6 align="center">bool fieldWriter(dataLynx *self, uintmax_t row, char *column, char *new_field)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>Integer row index location of field you wish to update/write to.</li>
###### <li>String of column name of field you wish to update/write to.</li>
###### <li>String of new value you wish to update field to.</li>
</ul>

##### To Use:
<ul>

###### <li></li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.csv.fieldWriter(&myData, 3, "Last Name", "Smith");
```

<ul>
 
###### <li>This will update the value at row 3/column "Last Name" to "Smith". (The previous value in this field will be erased.)</li>
</ul>
<hr>







<!-- FUNCTIONS FOR DATA WRANGLING / DATA CLEANINING -->
<div align="center">
  
Functions for Data Wrangling / Data Cleaning:
--------
</div>

<!-- FORMAT HEADER-->
<h4 align="center">formatHeader()</h4>
<h6 align="center">bool formatHeader(dataLynx *self);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### Use:
<ul>

###### <li>Will format all column names in the header to be Title Case (i.e first letter of each word capitalized) and will replace '_' (underscore) with ' ' (space).</li>
###### <li>Ex: 'first_name' will be changed to 'First Name'.</li>
###### <li>Ex: 'employee_id' will be changed to 'Employee ID'. NOTE: It was NOT changed to 'Employee Id' in this example, because this function identifies the word 'id' and will capitalize 'D' as well.</li>
</ul>

##### RETURN:
<ul>

###### <li>Returns true if any change is made to any column name.</li>
###### <li>Returns false if no changes are made.</li>
</ul>


##### Example Code:

```C
myData.formatHeader(&myData);
```
<hr>


<!-- CHANGE COLUMN NAME-->
<h4 align="center">changeColumnName()</h4>
<h6 align="center">bool changeColumnName(dataLynx *self, char *old_column_name, char *new_column_name);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>String of the old column name that you want to change. (Case sensitive)</li>
###### <li>String of the new column name that you want it changed to.</li>
</ul>

##### Use:
<ul>

###### <li>Simply supply the function with the name of the column you wish to change (i.e. old_column_name) and the name you wish to change it to.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, will return true.</li>
###### <li>On failure, will return false. (This can include a malloc failure, invalid pointers etc, as well as an invalid old_column_name supplied as input.)</li>
</ul>


##### Example Code:

```C
myData.changeColumnName(&myData, "Dept", "Department");
```
<hr>



<!-- REPLACE -->
<h4 align="center">replaceAll()</h4>
<h6 align="center">bool replaceAll(dataLynx *self, char *to_replace, char *replace_with);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>String of the value you want to replaced. (Case sensitive)</li>
###### <li>String of the value you want to replace it with.</li>
</ul>

##### Use:
<ul>

###### <li>Supply the parameters and the function will replace all instances of the string supplied for to_replace with the string supplied for replace_with.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.replaceAll(&myData, 'Tom', 'Thomas');
```
<ul>
  
###### <li>Will replace *all* instances of 'Tom' in the data with 'Thomas'.</li>
</ul>

```C
myData.replaceAll(&myData, '?', '');
```
<ul>
  
###### <li>Will replace *all* instances of '?' in the data with an empty string. (This will make the Is NULL/Not NULL counts accurate, as '?' is not seen as a NULL value. More on this in later sections.) You may also pass NULL in place of an empty string to acheive the same result.</li>
</ul>
<hr>



<!--  REPLACE IN COLUMN -->
<h4 align="center">replaceInColumn()</h4>
<h6 align="center">bool replaceInColumn(dataLynx *self, char *column_name, char *to_replace, char *replace_with);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>String of the column name that you want values to be altered in. (Case sensitive)</li>
###### <li>String of the value that you want to replaced. (Case sensitive)</li>
###### <li>String of the value that you want to replace that value with.</li>
</ul>

##### Use:
<ul>

###### <li>Supply the parameters and the function will replace all instances of to_replace that appear *only* in the column who's name matches the column_name string supplied as input.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.replaceInColumn(&myData, 'First Name', 'Tom', 'Thomas');
```
<ul>
  
###### <li>Will replace *all* instances of 'Tom' in the data with 'Thomas'.</li>
</ul>

```C
myData.replaceAll(&myData, '?', '');
```
<ul>
  
###### <li>Will replace all instances of '?' (*only* in the column who's name matches with column_name) with an empty string. (This will make the Is NULL/Not NULL counts accurate, as '?' is not seen as a NULL value. More on this in later sections.) You may also pass NULL in place of an empty string to acheive the same result.</li>
</ul>
<hr>



<!-- SORT ROWS BY COLUMN-->
<h4 align="center">sortRowsByColumn()</h4>
<h6 align="center">bool sortRowsByColumn(dataLynx *self, const char *column_name, const char *asc_desc);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>String of the column name who want to sort the rows by. (Case sensitive)</li>
###### <li>String determining whether you want the columns sorted ascending or descending. Accepts abbreviated 'asc'/'desc' as well as 'ascending'/'descending'. (Case *in*sensitive). (Note: If anything other than 'asc', 'desc', 'ascending', or 'descending' is supplied here, the function will sort by ascending, as this is the default.</li>
</ul>

##### Use:
<ul>

###### <li>Supply the parameters and the function will sort the rows in the data by the column provided.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns true.</li>
###### <li>On failure, returns false (i.e. invalid dataLynx object address or invalid column name).</li>
</ul>


##### Example Code:

```C
myData.sortRowsByColumn(&myData, "Employee ID", "ascending");
```
<ul>

###### <li>This will sort the rows by 'Employee ID' in ascending order.</li>
</ul>

```C
myData.sortRowsByColumn(&myData, "First Name", "DESC");
```
<ul>

###### <li>This will sort the rows by 'First Name' in descending order. (Remember: 'asc', 'desc', 'ascending', *and* 'ascending' are all acceptable and are case *in*sensitive.)</li>
</ul>

```C
myData.sortRowsByColumn(&myData, "Department", "HELLO!");
```
<ul>

###### <li>This will sort the rows by 'Department' in ascending order. (Remember: ascending is default, if an invalid input is supplied for the ascending/descending parameter.)</li>
</ul>


<hr>





<!-- DROP ROW-->
<h4 align="center">dropRow()</h4>
<h6 align="center">bool dropRow(dataLynx *self, uintmax_t row_to_drop)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>Integer of the row index you wish to drop (i.e. remove) from the data.</li>
</ul>

##### Use:
<ul>

###### <li>Supply the integer index of the row you wish to drop and the function will remove it.</li>
</ul>

##### RETURN:
<ul>

###### <li></li>
</ul>


##### Example Code:

```C
myData.dropRow(&myData, 8);
```
<ul>

###### <li>This will drop (i.e. remove) the 9th row (i.e. index location 8, as everything is of course 0 indexed) from the data. The row count will be decremented by 1 and aggregate data (i.e. statistical data & value counts) will be updated accordingly.</li>
</ul>
<hr>








<!-- DROP ROWS FILTER-->
<h4 align="center">dropRowsFilter()</h4>
<h6 align="center">bool dropRowsFilter(dataLynx *self, char *column_name, char *condition_operator, char *condition_value);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>String of the column name the filter will apply to.</li>
###### <li>String of the condition operator (i.e. "<", ">", "<=", ">=", or "==").</li>
###### <li>String of the condition value (this can represent an integer, double or string, but must be passed in as a string.)</li>
</ul>

##### Use:
<ul>

###### <li>Supply the parameters and the function will drop (i.e. remove) all rows in the data that meet the provided condition.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.dropRowsFilter(&myData, "Profit", ">=", "800");
```
<ul>

###### <li>This will drop (i.e. remove) all rows in data in which the value in the 'Profit' column is greater than or equal to 800.</li>
</ul>
<hr>




<!-- FILTER-->
<h4 align="center">filter()</h4>
<h6 align="center">bool filter(dataLynx *self, dataLynx *filteredData, char *column_name, char *condition_operator, char *condition_value)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) original dataLynx object</li>
###### <li>Pointer to (i.e. address of) new dataLynx object</li>
###### <li>String of the column name the filter will apply to.</li>
###### <li>String of the condition operator (i.e. "<", ">", "<=", ">=", or "==").</li>
###### <li>String of the condition value (this can represent an integer, double or string, but must be passed in as a string.)</li>
</ul>

##### Use:
<ul>

###### <li>Supply the paramters and the function will make a copy of all the rows in the original dataLynx object that meet the condition.</li>
###### <li>The new dataLynx object will then be filled with these rows (i.e. any that met the provided condition).</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
dataLynx filteredData = dataLynxConstructor();
myData.filter(&myData, &filteredData, "Profit", ">=", "800");
```

<ul>

###### <li>This will fill the new dataLynx object (named 'filteredData' in this example) with the rows from the original dataLynx object (named 'myData' in this example) in which the value in the 'Profit' column is greater than or equal to 800 (i.e. all rows that meet the condition).</li>
###### <li>Note: The original dataLynx object remains unaffected.</li>
</ul>
<hr>





<!-- DROP COLUMN() -->
<h4 align="center">dropColumn()</h4>
<h6 align="center">bool dropColumn(dataLynx *self, char *column_name);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) original dataLynx object</li>
###### <li>String of column name you wish to remove from the data structure.</li>

</ul>

##### Use:
<ul>

###### <li>Supply the parameters and (as long as a valid column name is provided) the function will remove the desired column from the data.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.dropColumn(&myData, "Gross Profit");
```

<ul>

###### <li>This will remove the "Gross Profit" column from the data.</li>
</ul>
<hr>




<!-- GET FIELD() -->
<h4 align="center">getField()</h4>
<h6 align="center">char *getField(dataLynx *self, uintmax_t desired_row, char *desired_column);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) original dataLynx object.</li>
###### <li>Integer row index location of field you wish to access.</li>
###### <li>String of column name of field you wish to access.</li>

</ul>

##### Use:
<ul>

###### <li>Supply the paramters and (as long as a valid row integer/column name is provided) the function will return a string of the field you requested.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns a string (i.e. a pointer, which points to the requested field in the data structure).</li>
###### <li>On failure, returns NULL.</li>
</ul>


##### Example Code:

```C
char *field = myData.getField(&myData, 3, "Employee ID");
```

<ul>

###### <li>This will store the location of the field at row 3/column "Employee ID" in the pointer named field, for easy access. Do not free, as this points to the location in the data structure where the field exists.</li>
</ul>

```C
printf("%s", myData.getField(&myData, 3, "Employee ID"));
```

<ul>

###### <li>This will print the field at row 3/column "Employee ID".</li>
</ul>
<hr>



<!-- GET FIELD2() -->
<h4 align="center">getField2()</h4>
<h6 align="center">char *getField2(dataLynx *self, uintmax_t desired_row, uintmax_t desired_column);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) original dataLynx object.</li>
###### <li>Integer row index location of field you wish to access.</li>
###### <li>Integer of column index location of field you wish to access.</li>

</ul>

##### Use:
<ul>

###### <li>Supply the paramters and (as long as a valid row integer/column integer is provided) the function will return a string of the field you requested.</li>
</ul>

##### RETURN:
<ul>

###### <li>On success, returns a string (i.e. a pointer, which points to the requested field in the data structure).</li>
###### <li>On failure, returns NULL.</li>
</ul>


##### Example Code:

```C
char *field = myData.getField2(&myData, 3, 0);
```

<ul>

###### <li>This will store the location of the field at row 3/column 0 in the pointer named field, for easy access. Do not free, as this points to the location in the data structure where the field exists.</li>
</ul>

```C
printf("%s", myData.getField2(&myData, 3, 0));
```

<ul>

###### <li>This will print the field at row 3/column 0.</li>
</ul>
<hr>

<!-- IS IN COLUMN()-->
<h4 align="center">isInColumn()</h4>
<h6 align="center">bool isInColumn(dataLynx *self, char *value, char *column_name)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) original dataLynx object</li>
###### <li>String of value you want to know is in a the data or not.</li>
###### <li>String of column name you want to search in.</li>

</ul>

##### Use:
<ul>

###### <li>Supply the parameters and the function will search for any instances of the provided value in the column name provided.</li>
</ul>

##### RETURN:
<ul>

###### <li>If the value provided is in the column provided, returns true. (Will return true regardless of how many times the value appears. This function only checks *that* it appears in the column.)</li>
###### <li>If the value provided is *not* in the column provided, returns false.</li>
</ul>


##### Example Code:

```C
bool value_exists = myData.inInColumn(&myData, "David", "First Name");
```

<ul>

###### <li>This will store true or false in the bool declared as 'value_exists', based on whether or not the value "David" is in the column named "First Name" at all.</li>
</ul>
<hr>


<!-- IS IN DATA()-->
<h4 align="center">isInData()</h4>
<h6 align="center">bool isInData(dataLynx *self, char *value)</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) original dataLynx object</li>
###### <li>String of value you want to know is in a the data or not.</li>

</ul>

##### Use:
<ul>

###### <li>Supply the parameters and the function will search for any instances of the provided value in the data.</li>
</ul>

##### RETURN:
<ul>

###### <li>If the value provided is in the data, returns true. (Will return true regardless of how many times the value appears. This function only checks *that* it appears in the data.)</li>
###### <li>If the value provided is *not* in the column provided, returns false.</li>
</ul>

##### Example Code:

```C
bool value_exists = myData.isInData(&myData, "David");
```

<ul>

###### <li>This will store true or false in the bool declared as 'value_exists', based on whether or not the value "David" appears in the data at all.</li>
</ul>
<hr>




<!-- STATISTICS / AGGREGATE DATA -->
<a id="stats"></a>
<div align="center">
  
Functions for Statistics / Aggregate Data:
--------
</div

<!-- GETSTAT() -->
<h4 align="center">getStat()</h4>
<h6 align="center">double getStat(dataLynx *self, char *column_name, char *operation)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>String of column name you wish to get the stat from.</li>
###### <li>String of the stat you wish to retrieve (i.e. min, max, sum, mean, std, 25th percentile, median, 75th percentile, is null, not null).</li>
</ul>

##### To Use:
<ul>

###### <li>Note: min, max, sum, mean, std, 25th percentile, median & 75th percentile are only available for numeric columns. If you try to access these stats from a non-numeric column, the function will return 0.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns the statistic requested.</li>
###### <li>On failure (i.e. invalid column name, either that does not exist at all or is a non-numeric column), returns 0. Do NOT mistake this for a valid statistic (as of course 0 could be a vaid statistic of a numeric column).</li>
</ul>


##### Example Code:

```C

```

<ul>
 
###### <li>.</li>
</ul>
<hr>





<h4 align="center"></h4>
<h6 align="center"></h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### To Use:
<ul>

###### <li></li>
</ul>

##### RETURN:
<ul>
    
###### <li></li>
</ul>


##### Example Code:

```C

```

<ul>
 
###### <li>.</li>
</ul>
<hr>





###### WOW, you made it to the end of the README file! I'll let you in on a secret: you do not need to access the functions through the dataLynx object. For example, instead of `myData.dropRow(&myData, 7);` to drop the row at index location 7, you could simply write `dropRow(&myData, 7);`. This project was started with the intention of emulating features of Object-Oriented Programming, which is why the former syntax has been used in all examples. However, I have made the functions publicly accessible for ultimate flexibility, so feel free to use the shorter syntax instead... One more secret: you can use the internal functions too! They are *not* private! But be careful, because the internal functions have less error checking, as that is done mainly in the client facing functions. Use at your own risk!


