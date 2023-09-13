​​# Library in C for data analytics

This is a library of functions written in C, for use in C programs, inspired by Python's standard CSV library as well as Pandas & SQLite3, giving the user an ease of use similar to Object-Oriented Programming, but with the low-level control that C offers. This library has functionality to read/write from/to CSV files as well as data clean up and statistical analysis functionality. My goal is to bridge the gap between Python and C, and explore what it takes to write the underlying implementation of libraries that one might otherwise take for granted if accustomed to programming in higher-level languages like Python.


<h6 align="center">NOTE: In order to acheive Object-Oriented style functionality, the first parameter for every user-facing function must be the address of the csvWizard struct (or 'object') you have declared / constructed using the constructor (e.g. myData.function(&myData)). Although this is of course not a true object in the OOP sense, moving forward I will be referring to csvWizard structs as objects since this is the intended use of the struct.</h6>

<hr>
<h3 align="center">LIST OF FUNCTIONS:</h3>
<hr>


<h5 align="center">csvWizardConstructor()</h5>
<h6 align="center">csvWizard csvWizardConstructor(void)</h6>
<ul>
  <li>PARAMETERS: This function takes no parameters</li>
  <li>TO USE:</li>
  <ol>
	<li>Declare a csvWizard object and assign the return of this function to the object</li>
  </ol>
	<li>Example Code:</li>
		
```C
csvWizard myData = csvWizardConstructor();
```
  <li>RETURN: Returns a csvWizard struct (i.e. 'object').</li>
</ul>
![csvWizardConstructor](https://user-images.githubusercontent.com/91298183/267458924-a89e2e29-98ae-4ba9-a588-591f380f3787.png)

<img src="https://user-images.githubusercontent.com/91298183/267458924-a89e2e29-98ae-4ba9-a588-591f380f3787.png">

<h5 align="center">csv.openFile()</h5>
<h6 align="center">void build_dblink_list(char **s_ptr, node **head, node **last)</h6>
<ul>
  <li>PARAMETERS: Takes pointer to string that will is intended to be in node, pointer to head node pointer and pointer to last node pointer. All 3 need to be pointers to pointers, so I can changed what they are pointing to inside the function. NOTE: String input must be dyamically allocated string, as this buffer will be taken over by linked list node. Otherwise buffer will be popped off the stack in calling function. Can not be string literal, otherwise will be popped off the stack when this function returns.</li>
  <ol>
	<li>Mallocs new node.</li>
	<li>New node takes over string (i.e. string pointer in node now points to string).</li>
	<li>New node is appended to linked list or added as first node if head is NULL.</li>
  </ol>
  <li>RETURN: No return value.</li>
</ul>

<hr>
<h5 align="center">BUILD_DICT_LINK_LIST</h5>
<h6 align="center">void build_dict_link_list(char **s_ptr, dict_node **head, dict_node **last, char *current_column_name);</h6>
<ul>
  <li>PARAMETERS: Takes pointer to string that will appear in new node, pointer to head node pointer, pointer to last node pointer and string of column name. NOTE: String input must be dyamically allocated string, as this buffer will be taken over by linked list node. Otherwise buffer will be popped off the stack in calling function. Can not be string literal, otherwise will be popped off the stack when this function returns. Column_name string input CAN be string literal.</li>
  <ol>
	<li>Mallocs new node.</li>
	<li>New node takes over string (i.e. string pointer in node now points to string).</li>
	<li>String for column_name of node is malloced and copied from current_column_name given as input.</li>
	<li>New node is appended to linked list or added as first node if head is NULL.</li>
  </ol>
  <li>RETURN: No return value.</li>
</ul>


