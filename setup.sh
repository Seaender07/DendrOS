#!/bin/bash


DEF='\033[0m'
RED='\033[0;31m'
GRN='\033[0;32m'
YLW='\033[0;33m'

DEPS="make gcc nasm qemu-system-i386 vim sl"
ifmDEPS=0



##
printf "\n${YLW}WARNING: THIS TOOL HAS NOT BEEN TESTED ON ALL OF THE SUPPORTED DISTROS YET.${DEF}\n"
##


if [ $EUID -ne 0 ]; then
	printf "${RED}Please execute as root.${DEF}\n"
	exit
fi


printf "\n:: Fetching the name of your distribution...\n"

if [ -f /etc/os-release ]; then
	. /etc/os-release
	OS=$NAME
elif type lsb_release 2>/dev/null; then
	OS=$(lsb_release -si)
elif [ -f /etc/lsb-release ]; then
	. /etc/lsb-release
	OS=$DISTRIB_ID
elif [ -f /etc/debian_version ]; then
	OS="Debian"
elif [ -f /etc/SuSe-release ]; then
	OS="OpenSUSE"
elif [ -f /etc/redhat-release ]; then
	OS="RedHat"
else
	printf "\n ${RED}No valid distribution names found.${DEF} Aborting...\n"
	exit
fi



printf " Is your distribution $OS (Y/n)? "
read

if [ $REPLY != "Y" ]; then
	printf " ${RED}There was an error: no valid distro found.${DEF}\n"
	exit
fi

printf "\n:: Checking missing utilities...\n"



for i in $DEPS
do
	which $i > /dev/null 2>&1 || mDEPS+="$i " || ifmDEPS=1
done



mDEPS=${mDEPS/-system-x86/}



if [ $ifmDEPS -gt 0 ]; then
	printf "\n There are some missing packages: $mDEPS\n Do you want to install them? "
	read
	printf "\n"
	if [ $REPLY == "Y" ]; then
		case $OS in
		
			"Arch Linux")
				sudo pacman --noconfirm -S $mDEPS || printf "\n${RED} ERROR: COULD NOT INSTALL PACKAGES${GRN}"
				;;
		
			"Debian" | "*buntu" | "Ubuntu*" | "Linux Mint")
				sudo apt install $mDEPS || printf "\n${RED} ERROR: COULD NOT INSTALL PACKAGES${GRN}"
				;;
		
			"Fedora")
				sudo dnf install $mDEPS || printf "\n${RED} ERROR: COULD NOT INSTALL PACKAGES${GRN}"
				;;
	
			*)
				printf "${RED} Error: no known package manager found for your distribution.${DEF}\n"
				exit
				;;
		
		esac
	else
		printf "${GRN}:: Aborting...${DEF}\n\n"
		exit
	fi
else
	printf "\n${GRN} No dependencies to install.${DEF}\n"
fi



printf "\n${GRN} Setup has finished.${DEF}\n"
