# Lecture One

## Command Line
#### echo
- display a line of text
	- echo "hello cs302"
- display a environment variable 
	- echo $HOME -->  /home/cs302
#### file 
- determine file type
#### ls
- -l detailed information of files
- -a all the files
#### mkdir
- make directiories
#### rm file/ rm -r directory
- -r  : remove directories and their contents recursively 
- -f  : ignore nonexistent files,  never prompt
#### cp sourcefile directfile
- -r : copy directions recursively
#### mv file directory
#### cat,more,less,head,tail
- cat :show all the content
- more/ less : divide and print one screen at a time
- head : from begin to end , 10 lines by default,  head -n number filename
- tail: similar to head 
#### grep keyword filename
- searhes files and prints the line in which keywords are found 
#### ps 
- displays information about a selection of the active processes
- aux : every process on the system ustin gBSD syntax
#### kill
- send a signal to a process, aggecting its behavior or killing it 
#### | (pipe operator)
- excute the sencond command result when excute the first line
#### > standard output stream 
## File permission
** drwxrwxrwx**
the first bit : file type 
rest: user --> group --> other
#### permission
- r: read
- w: wirte
- x :excute
#### changing permissions:
- +adds the designated permission(s) to a file or directory
- - removes the designated permission(s)from a file or directory
- = sets the designated permassions
