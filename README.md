# ResourceTracker
A Windows command line utility for printing names of resources (pdf, docx, txt etc.) in a  directory.  
It works recursively finding all resources in the directory and subdirectories.  

# Building
Make an empty VisualStudio console application project and add the   
resourcetracker.cpp file and build the project.  

# For better workflow
After building this file into an exe, store the exe into a folder  
and add that folder path to Windows environment variables.  
Let `ResourceTracker` be the name of the exe.  

# Usage
Go to the directory you want then type  
`ResourceTracker -r > D:\resources.txt`
and hit enter.  
`resources.txt` will contain all the resource files and other information.  

If `-r` flag is omitted then the program will search only the current directory.  

You can track resources only for certain extensions too.  
e.g. `ResourceTracker -r pdf txt`

If you want to include all files in the  
generated list add a '*'  
e.g.`ResourceTracker -r * `

