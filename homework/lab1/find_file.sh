#!/bin/bash
#judge if the directories exist
if [ ! -d "$HOME/lab1" ];then
        cd $HOME
        mkdir lab1
fi
cd $HOME/lab1
if [ ! -d "$HOME/lab1/11510086" ];then
        cd $HOME/lab1
        mkdir studentID
fi
if [ ! -d "$HOME/lab1/studentID/file.info" ];then
        cd $HOME/lab1/studentID
        touch file.info
fi 
directory_count=0
file_count=0
file=$HOME	
findFile()
{
	#cut out a piece of string
	str=$1
	str1=${str##*/}
	echo [ $str1 ] >> file.info
	for i in $(ls $1)
	do
		str=$1"/"$i
		str1=${str#*home}
		echo $str1 >> file.info
	done
	echo >> file.info
	for i in $(ls $1)
	do
		if [ -d $1"/"$i ]
		 then
			let directory_count++
			#judge if the directory is empty
			if [ `ls -A $1"/"$i | wc -m` != "0" ]; then
				findFile $1"/"$i
			fi
			#findFile $1"/"$i
		elif [ -f $1"/"$i ] 
		then
			let file_count++
		fi
	done

}
findFile $HOME
echo [ Directories Count ]:$directory_count >> file.info
echo>>file.info
echo [ Files Count ]:$file_count>>file.info





