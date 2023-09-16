​​# Library in C for Data Wrangling, Data Cleaning & Analytics

Overview:
--------

---
>###### This is a library of functions written in C, for use in C programs, inspired by Python's standard CSV library as well as Pandas & SQLite3, giving the user an ease of use similar to Object-Oriented Programming, but with the low-level control that C offers. This library has functionality to read/write from/to CSV files as well as data cleaning and statistical analysis functionality. My goal is to bridge the gap between Python and C, and explore what it takes to write the underlying implementation of libraries that one might otherwise take for granted if accustomed to programming in higher-level languages like Python.

---
>###### NOTE: In order to acheive Object-Oriented style functionality, the first parameter for every user-facing function must be the address of the dataLynx struct (or 'object') you have declared / constructed using the constructor (e.g. `myData.function(&myData)`). Although this is of course not a true object in the OOP sense, moving forward I will be referring to dataLynx structs as objects since this is the intended use of the struct. All memory management is handled for you, as long as you run `myData.freeAll(&myData)` at the end of your program.

<hr>
<h3 align="center">FUNCTIONS:</h3>
<div align="center">

###### The following is a complete list of *user-facing* functions.
###### (Assume the dataLynx object that was declared has been named myData.)
</div>
<hr>


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


##### Example Code:

```C
dataLynx myData = dataLynxConstructor();
```
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

###### <li>The function will attempt to open the file of the filename given. If a filename has already been set and a NULL is passed in place of a filename, the function will open the filename already in memory. If a filename has already been set and new filename is passed as a parameter, the function will open the new filename passed in.</li>
</ul>

##### RETURN:
<ul>

###### <li>Returns true if the file is successfully opened</li>
###### <li>Returns false if the file is not successfully opened</li>
</ul>


##### Example Code:

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
###### <li> From this point it can be parsed into various data structures. Typically, however, you will do this is in one fell swoop using csv.reader(), csv.dictReader or csv.reader_v3() (all of which internally use csv.fileReader() to first read into memory before parsing into various data structures), however I have provided the option to break this up into multiple steps for ultimate flexibility (e.g. if doing minimal or no data processing, you might want to read the file using fileReader(), because the string takes significantly less memory than the other data structures.</li>
###### <li> NOTE: *The data structure that this function creates is intended to only be used as a simpler, less memory-intensive data structure for certain instances. This data strucure does NOT offer the full capability of data processing that other data structures (like those created by csv.reader, csv.dictReader & csv.reader_v3) offer*</li>
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



<h4 align="center"></h4>
<h6 align="center"></h6>

##### PARAMETERS:
<ul>

###### <li></li>
</ul>

##### Use:
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
<hr>



<h4 align="center"></h4>
<h6 align="center"></h6>

##### PARAMETERS:
<ul>

###### <li></li>
</ul>

##### Use:
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
<hr>



<h4 align="center"></h4>
<h6 align="center"></h6>

##### PARAMETERS:
<ul>

###### <li></li>
</ul>

##### Use:
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
<hr>



<h4 align="center"></h4>
<h6 align="center"></h6>

##### PARAMETERS:
<ul>

###### <li></li>
</ul>

##### Use:
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
<hr>



<h4 align="center"></h4>
<h6 align="center"></h6>

##### PARAMETERS:
<ul>

###### <li></li>
</ul>

##### Use:
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
<hr>



