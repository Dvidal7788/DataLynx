<div align="center">
  
# Data Lynx
### Data Wrangling, Cleaning & Analytics Library in C
<kbd>
<img src="https://github.com/Dvidal7788/dataLynx/assets/91298183/7b6a07be-0669-4d03-ad7e-e98b9840e6c1" alt="DataLynx" width="200"/>
</kbd>
</div>

Overview:
--------

---
>###### This is a library of functions written in C, for use in C programs, inspired by Python's standard CSV library as well as Pandas & SQLite3, giving the user an ease of use similar to Object-Oriented Programming, but with the low-level control that C offers. This library has functionality to read/write from/to CSV files as well as data cleaning and statistical analysis functionality. My goal is to bridge the gap between Python and C, and explore what it takes to write the underlying implementation of libraries that one might otherwise take for granted if accustomed to programming in higher-level languages like Python.

---
>###### NOTE: In order to acheive Object-Oriented style functionality, the first parameter for every user-facing function must be the address of the dataLynx struct (or 'object') you have declared / constructed using the constructor (e.g. `myData.function(&myData)`). Although this is of course not a true object in the OOP sense, moving forward I will be referring to dataLynx structs as objects since this is the intended use of the struct. *All memory management is handled for you, as long as you run `myData.freeAll(&myData)` at the end of your program.*

<hr>



<div align="center">

<!-- TABLE OF CONTENTS -->
Table of Contents
=========
</div>
<ol>

##### <li>[The DataLynx 'Object'](#dataLynx)</li>
##### <li>[Statistics / Value Counts (Aggregate Data)](#stat_list)</li>
##### <li>[Function List (i.e. 'methods')](#functions)</li>
  <ul>
  
  ###### <li>[Functions for Reading/Writing from/to a CSV file](#csv)</li> 
  ###### <li>[Functions for Data Wrangling/Cleaning](#data_wrangling)</li>
  ###### <li>[Functions for Statistical / Aggregate Data](#stats)</li>
  ###### <li>[Functions for Printing](#print)</li>
  </ul>
</ol>
<hr>


<!-- DATALYNX OBJECT -->
<div align="center">
  
The DataLynx 'Object'
=========
</div>
<a id="dataLynx"></a>

###### The dataLynx 'object' is of course actually a struct with function pointers, pointers to data structures and parameters that the user can set to alter the behavior of the 'object'. For full flexibility, there are multiple data structures to choose from. This is a big part of what I mean when I say "I want to bridge the gap between Python and C". I wanted to offer the ease of use of higher-level languages and libraries like Python's built in CSV library, and Pandas, but still offer a C programmer the control that they are used to in being able to choose the underlying structures that make up that higher-level structures found in Python (lists, dictionaries etc). Below is a list and description of the data structures available.
<div align="center">
  
##### DATA STRUCTURES:
###### (Regardless of which data structure you choose, the header will be always be stored as a 2D array (i.e. an array of strings) separately from the rest of the data.)
</div>

##### Simple Data Structures:
<ul>
  
  >###### NOTE: *The simple data structures (e.g. 'raw' & 'rows') are intended only to be used as a simpler, less memory-intensive data structures for certain instances. These data strucures do NOT offer the full capability of data processing that other data structures offer (i.e. Grid V3, Grid & Dict Grid)*. They are only offered for convenience and full flexibility.
  >###### From this point the data can be parsed into various data structures, however, typically you will do this is in one fell swoop using csv.reader(), csv.dictReader or csv.reader_v3() (all of which     internally use csv.fileReader() to first read the file into memory before parsing into various data structures. More on these data structures later.) I have provided the option to break this up into multiple steps for ultimate flexibility (e.g. if doing minimal or no data processing, you might want to read the file using fileReader(), because the string takes significantly less memory than the other data structures).
  </ul>
<ul> 
  
  ##### <li>Raw Data</li>
  <ul>
  
  ###### <li>This is a string. It is *one* long string containg the entire dataset, hence 'raw data'.</li>
  <li>

  ```C
myData.csv.fileReader(&myData);
```
  </li>
  <ul>
  
  ###### <li>The example code above will read an opened CSV file into memory as raw data.</li>
  
  </ul>
  </ul>

##### <li>Rows</li>
<ul>

  ###### <li>This is a 2D array (i.e. an array of strings). Each string in the array correlates to 1 row. Each row is stored as one long string.</li>
  <li>

  ```C
myData.csv.fileRowReader(&myData);
```
  </li>
  <ul>
  
  ###### <li>The example code above will read an opened CSV file into memory as row data (i.e. an array of strings).</li>
  
  </ul>
  </ul>

</ul>


##### Full Functionality Data Structures:
<ul>
  
  ##### <li>Grid V3</li>
  <ul>
  
  ###### <li>This is a 3D array (i.e. an array of arrays, in which each secondary array is an array of strings).</li>
  ###### <li>In other words, this is an array, which stores arrays (one for each row), each of which stores strings (one per field).</li>

  <li>

  ```C
myData.csv.reader_v3(&myData);
```
  </li>
  <ul>
  
  ###### <li>The example code above will read an opened CSV file into memory as a 3D array.</li>
  
  </ul>
  </ul>
  <a id="grid_v3_diagram"></a>
  
  ![Grid V3](https://github.com/Dvidal7788/dataLynx/assets/91298183/45cb902f-5622-42e8-b14d-dc994a112c23)

  <ul align="center">
  
  ###### <li>The diagram above shows the basic layout of the Grid V3 data structure (i.e. 3D array).</li>
  </ul>

  ----
  ##### <li>Grid</li>
  <ul>

  ###### <li>This is an array of linked lists. Each linked list in the array correlates to 1 row. Each node in the linked list correlates to one field in the data.</li>
  <li>

  ```C
myData.csv.reader(&myData);
```
  </li>
  <ul>
  
  ###### <li>The example code above will read an opened CSV file into memory as grid data (i.e. an array of linked lists).</li>
  
  </ul>
  </ul>
  <a id="grid_diagram"></a>

  ![Grid](https://github.com/Dvidal7788/dataLynx/assets/91298183/b6b349f8-5ac1-4543-8c09-2597b5c6055e)

  <ul align="center">
  
  ###### <li>The diagram above shows the basic layout of the Grid data structure (i.e. array of linked lists).</li>
  </ul>

  ---
  ##### <li>Dict Grid</li>
  <ul>

  ###### <li>This is an array of dict-style linked lists. Each linked list in the array correlates to 1 row. Each node in the linked list correlates to one field in the data.</li>
  ###### <li>Each node in each linked list contains not only a string of the value associated with that field, but also a string of the *column name* that that particular field is in, hence '*dict*-style linked lists'.</li>
  <li>

  ```C
myData.csv.dictReader(&myData);
```
  </li>
  <ul>
  
  ###### <li>The example code above will read an opened CSV file into memory as dict-grid data (i.e. an array of dict-style linked lists).</li>
  
  </ul>
  </ul>
  <a id="dict_grid_diagram"></a>
  
  ![Dict Grid](https://github.com/Dvidal7788/dataLynx/assets/91298183/bae10ce7-886d-4757-92ab-eac5c9a954de)

  <ul align="center">
  
  ###### <li>The diagram above shows the basic layout of the Dict Grid data structure (i.e. array of dict-style linked lists).</li>
  </ul>

</ul>



<hr>

<!-- AGGREGATE DATA -->
<div align="center">
<a id="stat_list"></a>
  
Statistics / Value Counts (Aggregate Data)
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
  
###### <li>Value Counts for each unique value in a given column (i.e. how many times that specific value appears in a column).</li>
###### <li>Is Null (number of empty field in a given column)</li>
###### <li>Not Null (number of non-empty field in a given column)</li>
</ul>
<div align="center">

###### (See [Functions for Statistical / Aggregate Data](#stats) below for more details.)
</div>
<hr>
<div align="center">




<!-- FUNCTIONS -->
FUNCTIONS
=========
<a id="functions"></a>
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


#####  Example Code:

```C
dataLynx myData = dataLynxConstructor();
```
<ul>
 
###### <li>The above code will create a dataLynx object and intitialize the internal attributes the their default state.</li>
</ul>
<hr>




<!-- FUNCTIONS FOR READ/WRITE FROM/TO CSV -->
<a id="csv"></a>
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


#####  Example Code:

```C
myData.csv.openFile(&myData, "csv/staff.csv");
```
<hr>


<!-- CSV.FILEREADER() -->
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
###### <li> From this point the data can be parsed into various data structures. Typically, however, you will do this is in one fell swoop using csv.reader(), csv.dictReader or csv.reader_v3() (all of which internally use csv.fileReader() to first read the file into memory before parsing into various data structures. More on these data structures later.) I have provided the option to break this up into multiple steps for ultimate flexibility (e.g. if doing minimal or no data processing, you might want to read the file using fileReader(), because the string takes significantly less memory than the other data structures).</li>
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


<!-- CSV.FILEROWREADER() -->
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
###### <li>*The data will be stored in memory as a 3D array (i.e. an array of arrays, in which each secondary array is an array of strings). In other words, this is an array, which stores arrays (one for each row), each of which stores strings (one per field). [See diagram](#grid_v3_diagram).*</li>

</ul>

##### RETURN:
<ul>

###### <li>On success, returns a pointer to a 3D array.</li>
###### <li>Note: You do not need to assign this return value to anything. It is automatically assigned within the function to a pointer within your dataLynx object. The data structure is only returned for convenience.</li>
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
###### <li>*The data will be stored in memory as an array of linked lists. Each linked list represents a row. Each node in the linked list represents a value (i.e. field). [See diagram](#grid_diagram).*</li>
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
###### <li>*The data will be stored in memory as an array of dict-style linked lists. Each linked list represents a row. Each node in the linked list represents a value (i.e. field). In addition to storing a value, each node also stores the column name that corresponds with that particular value (e.g. {'Employee ID', '4511'}), in which 'Employee ID' is the column name and '4511' is the corresponding value for that row (i.e. linked list) that this particular node is in. [See diagram](#dict_grid_diagram).*</li>
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
###### <li>Integer row index of field you wish to access.</li>
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
###### <li>Integer row index of field you wish to access.</li>
###### <li>Integer column index of field you wish to access.</li>
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
###### <li>Integer row index of field you wish to update/write to.</li>
###### <li>String of column name of field you wish to update/write to.</li>
###### <li>String of new value you wish to update field to.</li>
</ul>

##### To Use:
<ul>

###### <li>This function updates/changes and exists field in an opened CSV file (see [openFile()](https://github.com/Dvidal7788/dataLynx/tree/master#csvopenfile)) who's filename and file pointer are stored in your dataLynx object./li>
###### <li>NOTE: This function does NOT update the corresponding field in the data structure in *memory* (see [updateField()](https://github.com/Dvidal7788/dataLynx/tree/master#updateFiled) if this is what you are looking to do). You can do both at the same time by giving yourself CSV writer permission and running updateField() in destructive mode.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
// Give CSV write permission
myData.csv_write_permission = true;

// Update field in CSV file
myData.csv.fieldWriter(&myData, 3, "Last Name", "Smith");
```
<ul>
 
###### <li>This will update the value at row 3/column "Last Name" to "Smith". (The previous value in this field will be erased.)</li>
</ul>
<hr>


<!-- CSV.FIELDWRITER2-->
<h4 align="center">csv.fieldWriter2()</h4>
<h6 align="center">bool fieldWriter2(dataLynx *self, uintmax_t row, uintmax_t column, char *new_field)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>Integer row index of field you wish to update/write to.</li>
###### <li>Integer column index of field you wish to update/write to.</li>
###### <li>String of new value you wish to update field to.</li>
</ul>

##### To Use:
<ul>

###### <li>This function updates/changes and exists field in an opened CSV file (see [openFile()](https://github.com/Dvidal7788/dataLynx/tree/master#csvopenfile)) who's filename and file pointer are stored in your dataLynx object./li>
###### <li>NOTE: This function does NOT update the corresponding field in the data structure in *memory* (see [updateField()](https://github.com/Dvidal7788/dataLynx/tree/master#updateFiled) if this is what you are looking to do). You can do both at the same time by giving yourself CSV writer permission and running updateField() in destructive mode.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
// Give CSV write permission
myData.csv_write_permission = true;

// Update field in CSV file
myData.csv.fieldWriter2(&myData, 3, 2, "Smith");
```
<ul>
 
###### <li>This will update the value at row 3/column 2 to "Smith". (The previous value in this field will be erased.)</li>
</ul>
<hr>



<!-- CSV.ROWWRITER-->
<a id="rowWriter"></a>
<h4 align="center">csv.rowWriter()</h4>
<h6 align="center">bool rowWriter(dataLynx *self, char *values[])</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>Array of strings (one string for each value contained in the new row)</li>
</ul>

##### To Use:
<ul>

###### <li>This function writes a new row to a CSV file. *You must first explicitly give yourself CSV write permission.* See example code below.</li>
###### <li>Supply the parameters and the function will write the new row to the opened CSV file (see [openFile()](https://github.com/Dvidal7788/dataLynx/tree/master#csvopenfile)) who's filename and file pointer are stored in your dataLynx object.</li>
###### <li>The values in the value array must be in the correct column order.</li>
###### <li>NOTE: This function does NOT insert a new row into the data structure in *memory* (see [insertRow()](https://github.com/Dvidal7788/dataLynx/tree/master#insertRow) if this is what you are looking to do).</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
// Give CSV write permission
myData.csv_write_permission = true;

// Create array of the values to be contained in the new row
char *values[] = {
                  "4511",
                  "John",
                  "Smith",
                  "Sales",
                  "333",
                  "70000"
};

// Write new row to CSV file
myData.csv.rowWriter(&myData, values);
```
<ul>
 
###### <li>This will write a new row to the opened CSV file. The row will be appended to the end of the file.</li>
</ul>
<hr>



<!-- CSV.ROW DICT WRITER -->
<a id="rowDictWriter"></a>
<h4 align="center">csv.rowDictWriter()</h4>
<h6 align="center">bool rowDictWriter(dataLynx *self, dict values[])</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
###### <li>Array of dicts (each dict containing a string of a value and a string of the column name corresponding to that value.)</li>
</ul>

##### To Use:
<ul>

###### <li>This function writes a new row to a CSV file. *You must first explicitly give yourself CSV write permission.* See example code below.</li>
###### <li>Supply the parameters and the function will write the new row to the opened CSV file (see [openFile()](https://github.com/Dvidal7788/dataLynx/tree/master#csvopenfile)) who's filename and file pointer are stored in your dataLynx object.</li>
###### <li>The values do NOT need to be in the correct column order! This is the purpose of using csv.rowDictWriter() over csv.rowWriter(). This function will write the row to the CSV file, using the column names in the dict array as a guide, so the values will always be in the correct order!</li>
###### <li>NOTE: This function does NOT insert a new row into the data structure in *memory* (see [insertRow()](https://github.com/Dvidal7788/dataLynx/tree/master#insertRow) if this is what you are looking to do).</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
// Give CSV write permission
myData.csv_write_permission = true;

// Create array of dicts (i.e. {column name, value}) with the values to be contained in the new row
dict values[] = {
                  {"Employee ID", "4511"},
                  {"First Name", "John" },
                  {"Last Name",  "Smith"  },
                  {"Department", "Sales"},
                  {"Ext",        "333"  },
                  {"Salary",     "70000"}
};

// Write new row to CSV file
myData.csv.rowDictWriter(&myData, values);
```
<ul>
 
###### <li>This will write a new row to the opened CSV file. The row will be appended to the end of the file.</li>
###### <li>These values will be put in the correct order regardless of their order in the dict array!</li>
</ul>
<hr>





<!-- FUNCTIONS FOR DATA WRANGLING / DATA CLEANINING -->
<div align="center">
  
Functions for Data Wrangling / Data Cleaning:
--------
</div>
<a id="data_wrangling"></a>

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
###### <li>DESTRUCTIVE MODE: This function can be run in destructive mode, which will simultaneously change the column name in the header in memory, as well as the header in the opened CSV file. You must give yourself CSV write permission, *as well as* set the dataLynx object to destructive mode: `myData.csv_write_permission = true;` `myData.destructive_mode;`. (These are both set to false by default.)</li>
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
###### <li>DESTRUCTIVE MODE: This function can be run in destructive mode, which will simultaneously change the column name in the header in memory, as well as the header in the opened CSV file. You must give yourself CSV write permission, *as well as* set the dataLynx object to destructive mode: `myData.csv_write_permission = true;` `myData.destructive_mode;`. (These are both set to false by default.)</li>
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



<!-- UPDATE FIELD -->
<h4 align="center">updateField()</h4>
<h6 align="center">bool updateField(dataLynx *self, uintmax_t row, char *column, char *new_value)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>Integer row index of field you wish to update.</li>
###### <li>String of column name of field you wish to update.</li>
###### <li>String of the new value you wish to update the desired field to.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters, and the function will update the field at the specified row/column location in the in-memory dataset with the provided new value (provided that the row/column name is valid).</li>
###### <li>If you wish to update the desired field to be blank/empty field/NULL, simply input an empty string (e.g. "") as the new_value parameter. NULL strings will be rejected.</li>
###### <li>NOTE: Whether updating a field in a numeric OR non-numeric column, the input parameter for the new value will be a string for simplicity's sake.</li>
###### <li>DESTRUCTIVE MODE: This function can be run in destructive mode, which will simultaneously update the corresponding field in the opened CSV file. You must give yourself CSV write permission, *as well as* set the dataLynx object to destructive mode: `myData.csv_write_permission = true;` `myData.destructive_mode;`. (These are both set to false by default.)</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, return true.</li>
###### <li>On faiure, return false (e.g. invalid dataLynx object address, out of range row index, invalid column name or invalid pointer (i.e. string) to the new value).</li>
</ul>


##### Example Code:

```C
myData.updateField(&myData, 3, "Revenue", "34000");
```
<ul>
 
###### <li>This will update (i.e. change) the value currently in row 3/column name "Revenue" to be 34,000 in the dataset in memory.</li>
</ul>

```C
// Give CSV write permission / set object to destructive mode
myData.csv_write_permission = true;
myData.csv_write_permission = false;

// Run updateField in destructive mode
myData.updateField(&myData, 4, "Department", "Sales");
```
<ul>
 
###### <li>This will update (i.e. change) the value currently in row 4/column name "Department" to be "Sales" in the dataset in memory, *as well as* the CSV file.</li>
</ul>
<hr>



<!-- UPDATE FIELD2 -->
<h4 align="center">updateField2()</h4>
<h6 align="center">bool updateField2(dataLynx *self, uintmax_t row, uintmax_t column, char *new_value)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>Integer row index of field you wish to update.</li>
###### <li>Integer column index of field you wish to update.</li>
###### <li>String of the new value you wish to update the desired field to.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters, and the function will update the field at the specified row/column location in the in-memory dataset with the provided new value (provided that the row/column name is valid).</li>
###### <li>If you wish to update the desired field to be blank/empty field/NULL, simply input an empty string (e.g. "") as the new_value parameter. NULL strings will be rejected.</li>
###### <li>NOTE: Whether updating a field in a numeric OR non-numeric column, the input parameter for the new value will be a string for simplicity's sake.</li>
###### <li>DESTRUCTIVE MODE: This function can be run in destructive mode, which will simultaneously update the corresponding field in the opened CSV file. You must give yourself CSV write permission, *as well as* set the dataLynx object to destructive mode: `myData.csv_write_permission = true;` `myData.destructive_mode;`. (These are both set to false by default.)</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, return true.</li>
###### <li>On faiure, return false (e.g. invalid dataLynx object address, out of range row index, invalid column name or invalid pointer (i.e. string) to the new value).</li>
</ul>


##### Example Code:

```C
myData.updateField2(&myData, 3, 4, "34000");
```
<ul>
 
###### <li>This will update (i.e. change) the value currently in row 3/column 4 to be 34,000 in the dataset in memory.</li>
</ul>

```C
// Give CSV write permission / set object to destructive mode
myData.csv_write_permission = true;
myData.csv_write_permission = false;

// Run updateField2 in destructive mode
myData.updateField2(&myData, 4, 3, "Sales");
```
<ul>
 
###### <li>This will update (i.e. change) the value currently in row 4/column 3 to be "Sales" in the dataset in memory, *as well as* the CSV file.</li>
</ul>
<hr>



<!-- REPLACE ALL -->
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

<hr>


<!-- STRIP FIELD -->
<h4 align="center">stripField()</h4>
<h6 align="center">bool stripField(dataLynx *self, uintmax_t row, char *column_name)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>Integer row index of field you wish to strip.</li>
###### <li>Column name of field you wish to strip.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and this function will strip any whitespace (i.e. spaces, tabs and new line characters, and carriage returns) from both the left and right side of the field.</li>
###### <li>NOTE: This will NOT strip white space between non-whitespace characters.</li> 

<ul>

  ###### <li>Example: " &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Strategic Initiatives & Programs &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" will become "Strategic Initiatives & Programs". You will not lose the internal whitespace.</li>
</ul>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.stripField(&myData, 4, "First Name");
```

<ul>
 
###### <li>The above code will strip any whitespace that appears on the left or right side of the field at row 4/column "First Name".</li>
</ul>
<hr>




<!-- STRIP ALL -->
<h4 align="center">stripAll()</h4>
<h6 align="center">bool stripAll(dataLynx *self)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
</ul>

##### To Use:
<ul>

###### <li>This function will strip any whitespace (i.e. spaces, tabs and new line characters, and carriage returns) from both the left and right side of ALL fields in the data set.</li>
###### <li>NOTE: This will NOT strip white space between non-whitespace characters.</li> 

<ul>

  ###### <li>Example: " &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Strategic Initiatives & Programs &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" will become "Strategic Initiatives & Programs". You will not lose the internal whitespace.</li>
</ul>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.stripAll(&myData);
```

<ul>
 
###### <li>The above code will strip any whitespace that appears on the left or right side of all fields in the data set.</li>
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

###### <li>On success, returns true.</li>
###### <li>On failure, returns false (i.e. out of range row index supplied as parameter, malloc/realloc failure, or invalid dataLynx object address).</li>
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



<!-- INSERT ROW() -->

<h4 align="center">insertRow()</h4>
<h6 align="center">bool insertRow(dataLynx *self, char *values[])</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>An array of strings, each string containing a value corresponding to the columns of the dataset. The values must be in the correct column order.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and the function will insert the row of data provided from the array of strings (i.e. values) into the in-memory dataset (i.e. the row will be appended to the end of the dataset).</li>
###### <li>If the row is successfully inserted, the row count (i.e. self.rowCount) will be incremented by 1.</li>
###### <li>DESTRUCTIVE MODE: This function can be run in destructive mode, which will simultaneously insert (append) the row in the opened CSV file. You must give yourself CSV write permission, *as well as* set the dataLynx object to destructive mode: `myData.csv_write_permission = true;` `myData.destructive_mode;`. (These are both set to false by default.) (If you wish to *only* insert the row into the CSV file, you may use [csv.rowWriter()](#rowWriter)).</li>
###### <li>OTHER USE: You can also use this function to create a data set from scratch!</li>
  <ul>
    
###### <li>Either set the column count then self.insertRow() (this will automatically create a generic header (e.g. "Column 1", "Column 2", etc...) OR use [createHeader()](#create_header) to create a header with the column names of your choosing, then use insertRow().</li>
###### <li>NOTE: This will *only* create a Grid V3 (i.e. 3D array). When creating a data structure from scratch like this using insertRow(), (as opposed to reading in the data from a CSV), you do not have the options (yet) of creating any data structure you wish. That functionality will come in the future!</li>
  </ul>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
// -----  A DATA SET IS ALREADY PRESENT in dataLynx object -----
char *values[] = {
                  "4511",
                  "John",
                  "Smith",
                  "Sales",
                  "333",
                  "70000"
};
myData.insertRow(&myData, values);
```
<ul>
 
###### <li>If a data structure already exists in the dataLynx object, this will insert a new row with the values contained in the array named values.</li>
</ul>

```C
// -----  CREATE DATA STRUCTURE FROM SCRATCH (i.e. NO DATA EXISTS in dataLynx object yet) (1st method) -----
// Set column count (i.e. number of values in the row) so that insertRow can create a generic header
myData.columnCount = 6;

// Insert 1st row of data
char *values[] = {
                  "4511",
                  "John",
                  "Smith",
                  "Sales",
                  "333",
                  "70000"
};
myData.insertRow(&myData, values);
```

<ul>
 
###### <li>If *no* data structure exists yet in the dataLynx object, this will create a generic header (e.g. "Column 1", "Column 2", etc..) and create a new data structure (Grid V3) and insert a row with the values contained in the array named values.</li>
</ul>


```C
// -----  CREATE DATA STRUCTURE FROM SCRATCH (i.e. NO DATA EXISTS in dataLynx object yet) (2nd method) -----
// Create header array
char *header[] = {
                  "Employee ID",
                  "First Name",
                  "Last Name",
                  "Department",
                  "Ext",
                  "Salary"
};
unsigned int column_count = 6;
myData.createHeader(&myData, header, column_count);

// Create values array / Insert 1st row of data
char *values[] = {
                  "4511",
                  "John",
                  "Smith",
                  "Sales",
                  "333",
                  "70000"
};
myData.insertRow(&myData, values);
```

<ul>
 
###### <li>If no data structure exists yet in the dataLynx object, this will create a header based on the array passed as a parameter to createHeader(), as well as create a new data structure (Grid V3), and insert a row with the values contained in the values array.</li>
</ul>

<hr>




<!-- INSERT ROW DICT()-->
<h4 align="center">insertRowDict()</h4>
<h6 align="center">bool insertRowDict(dataLynx *self, dict values[])</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>An array of dicts, each dict containing a string of the column name and a string of the value corresponding to that column name. These dicts do NOT need to be in correct column order!</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and the function will insert the row of data provided from the array of dicts into the data set (i.e. the row will be appended to the end of the data set).</li>
###### <li>NOTE: The dicts in the dict array do NOT need to be in correct column order. As long as the column names are correct, the function will automatically rearrange the valuesto the correct order!</li>
###### <li>If the row is successfully inserted, the row count (i.e. self.rowCount) will be incremented by 1.</li>
###### <li>DESTRUCTIVE MODE: This function can be run in destructive mode, which will simultaneously insert (append) the row in the opened CSV file. You must give yourself CSV write permission, *as well as* set the dataLynx object to destructive mode: `myData.csv_write_permission = true;` `myData.destructive_mode;`. (These are both set to false by default.) (If you wish to *only* insert the row into the CSV file, you may use [csv.rowDictWriter()](#rowDictWriter)).</li>
###### <li>OTHER USE: You can also use this function to create a data set from scratch!</li>
  <ul>
    
###### <li>If calling insertRowDict() to create a data structure from scratch, the function will automatically create a header from the column names provided in the dict array.</li>
##### <li>The function will then insert the row using the values provided in the dict array.</li>
###### <li>NOTE: This will *only* create a Grid V3 (i.e. 3D array). When creating a data structure from scratch like this using insertRow()/insertRowDict(), (as opposed to reading in the data from a CSV), you do not have the options (yet) of creating any data structure you wish. That functionality will come in the future!</li>
  </ul>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
// Create array of dicts (i.e. {column_name, value})
dict values[] = {
                  {"Employee ID", "4511" },
                  {"First Name",  "John" },
                  {"Last Name",   "Smith"},
                  {"Department",  "Sales"},
                  {"Ext",         "333"  },
                  {"Salary",      "70000"}
};

// Insert row
myData.insertRowDict(&myData, values);
```
<ul>
 
###### <li>If a data structure already exists in the dataLynx object, the above code will insert a new row with the values contained in the values array.</li>
###### <li>If *no* data structure exists yet in the dataLynx object, the above code will create a header from the first string (i.e. column name) in each dict within the dict array. It will then create a new data structure (Grid V3) and insert a row with the values contained in the array named values.</li>
</ul>

<hr>



<!-- CREATE HEADER()-->
<a id="create_header"></a>
<h4 align="center">createHeader()</h4>
<h6 align="center">bool createHeader(dataLynx *self, char *header[], uint32_t header_size)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>Array of strings containing all of the column names you want in the header. You may input NULL here if you wish to create a generic header (e.g. "Column 1", "Column 2", etc...)</li>
###### <li>An integer of the number of columns. Must be correct or you run the risk of overrunning the header buffer.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and the function will create a header from the column names provided in the header parameter.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, return true.</li>
###### <li>On failure, return false.</li>
</ul>


##### Example Code:

```C
// Set up parameters
char *header[] = {"Employee ID", "First Name", "Last Name", "Department", "Ext", "Salary"};
unsigned int column_count = 6;

// Create header
myData.createHeader(&myData, header, column_count);
```

<ul>
 
###### <li>This will create a header in the dataLynx object with the column names provided.</li>
</ul>
<hr>




<!-- GET FIELD() -->
<h4 align="center">getField()</h4>
<h6 align="center">char *getField(dataLynx *self, uintmax_t desired_row, char *desired_column);</h6>

##### PARAMETERS:
<ul>

###### <li>Pointer to (i.e. address of) original dataLynx object.</li>
###### <li>Integer row index of field you wish to access.</li>
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
###### <li>Integer row index of field you wish to access.</li>
###### <li>Integer of column index of field you wish to access.</li>

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
<h6 align="center">double getStat(dataLynx *self, char *column_name, char *stat)</h6>

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
myData.getStat(&myData, "Revenue", "std");
```

<ul>
 
###### <li>This will return the standard deviation for the "Revenue" column in the data set. NOTE: "standard deviation" is also accepted and will return the same value.</li>
</ul>
<hr>



<!-- FUNCTIONS FOR PRINTING-->
<a id="print"></a>
<div align="center">
  
Functions for Printing:
--------
</div

<!-- PRINT HEADER() -->
<h4 align="center">printHeader()</h4>
<h6 align="center">bool printHeader(dataLynx *self)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### To Use:
<ul>

###### <li>This function will print only the header of the data stored in the dataLynx object.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.printHeader(&myData);
```

<hr>





<!-- PRINT DATA() -->
<h4 align="center">printData()</h4>
<h6 align="center">bool printData(dataLynx *self)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### To Use:
<ul>

###### <li>If the number of rows in the data set does not exceed the number set by self.maxPrintRows, this function will print the entire data set in the style of whichever data structure is currently in use by that dataLynx object.</li>
###### <li>If the number of rows in the data set exceeds the number set by self.maxPrintRows (ex: `myData.maxPrintRows`), this function will print the 1st 10 and last 10 rows.</li>
###### <li>By default, self.maxPrintRows is set to 25, however you can set this to any number 0 or greater that you wish.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.printData(&myData);
```

<hr>



<!-- PRINT DATA TABLE -->
<h4 align="center">printDataTable()</h4>
<h6 align="center">bool printDataTable(dataLynx *self)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### To Use:
<ul>

###### <li>This function will print the entire dataset as a dynamically generated table (i.e. in the style of an SQLite3 table), regardless of which data structure you're dataLynx object is currently utilizing.</li>
###### <li>If the value of a field exceeds the value set by self.maxFieldPrintLength, the value will be truncated. By default this is set to 50, however, you may change this value to whatever you like. This becomes important, for example if password hashes are stored in your data, which can easily be 128 characters or more (e.g. SHA-512 produces a 128 character long string) and can make it difficult to see other data in your data set, even with a large monitor. SQLite3 does not have a built-in way of setting the maximum print length of fields, which is something I've aimed to improve with this feature.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.printDataTable(&myData);
```

<hr>

<!-- PRINT HEAD()-->
<h4 align="center">printHead()</h4>
<h6 align="center">bool printHead(dataLynx *self, uintmax_t number_of_rows)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>Integer of the number of rows you wish to print.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and the function will print the from the beginning (i.e. head) of the data set until the number of rows requested is met.</li>
###### <li>NOT to be confused with printHeader(). </li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.printHead(&myData, 5);
```

<ul>
 
###### <li>This will print the 1st 5 rows of the data.</li>
</ul>
<hr>




<!-- PRINT TAIL()-->
<h4 align="center">printTail()</h4>
<h6 align="center">bool printTail(dataLynx *self, uintmax_t number_of_rows)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object.</li>
###### <li>Integer of the number of rows you wish to print.</li>
</ul>

##### To Use:
<ul>

###### <li>Supply the parameters and the function will print the from the *end* (i.e. tail) of the data set until the number of rows requested is met.</li>
###### <li>NOT to be confused with printHeader(). </li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.printTail(&myData, 5);
```

<ul>
 
###### <li>This will print the *last* 5 rows of the data.</li>
</ul>
<hr>



<!-- PRINT STATS() -->
<h4 align="center">printStats()</h4>
<h6 align="center">bool printStats(dataLynx *self)</h6>

##### PARAMETERS:
<ul>
    
###### <li>Pointer to (i.e. address of) dataLynx object</li>
</ul>

##### To Use:
<ul>

###### <li>This function will print the aggregate data for every column in the data set.</li>
###### <li>For numeric columns, this function will print min, max, sum, mean, standard deviation, 25th percentile, median, 75th percentile, is null and not null.</li>
###### <li>For non-numeric column, this function will print the value count for every unique value in the data set, as well as is null/not null counts.</li>
</ul>

##### RETURN:
<ul>
    
###### <li>On success, returns true.</li>
###### <li>On failure, returns false.</li>
</ul>


##### Example Code:

```C
myData.printStats(&myData);
```

<hr>





###### WOW, you made it to the end of the README file! I'll let you in on a secret: you do not need to access the functions through the dataLynx object. For example, instead of `myData.dropRow(&myData, 7);` to drop the row at index location 7, you could simply write `dropRow(&myData, 7);`. This project was started with the intention of emulating features of Object-Oriented Programming, which is why the former syntax has been used in all examples. However, I have made the functions publicly accessible for ultimate flexibility, so feel free to use the shorter syntax instead... One more secret: you can use the internal functions too! They are *not* private! But be careful, because the internal functions have less error checking, as that is done mainly in the client-facing functions. Use at your own risk!


