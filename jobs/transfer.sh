#!/bin/bash

DEBUG=0
NARGS=$((${#}))
MINARGS=3

# Remote to Local Transfer  
# 	remote-local-remote : local->remote (on remote)
# 	local-local-remote  : local->remote (on local)
# 	remote-remote-local : remote->local (on remote)
# 	local-remote-local  : remote->local (on local)
TRANSFER=${1:-local-remote-local}


# Path
local_path=${2:-"/home/matt/tmp/data"}
remote_path=${3:-"~/jobs/dat"}

# Home
local_home=/home/matt
remote_home=/h/mduschen

# User
local_user=matt
remote_user=mduschen

# Ip
local_ip=142.120.19.208 #$(who | grep "$USER" | awk '{print $NF}' | tail -n1 | sed "s%[(,)]%%g")
remote_ip=vectorq

# Address
case ${TRANSFER} in
	
	remote-local-remote)	
		local_address=${local_user}@${local_ip}: 
		remote_address=
	;;

	local-local-remote)	
		local_address=
		remote_address=${remote_ip}:
	;;

	remote-remote-local)	
		local_address=${local_user}@${local_ip}:
		remote_address=
	;;

	local-remote-local)	
		local_address=
		remote_address=${remote_ip}:
	;;

esac			


# Options
shift $(( ${NARGS} > ${MINARGS} ? ${MINARGS} : ${NARGS} ))
options=(${@})

# Transfer commands
exe=rsync
flags=(-avzhsR --stats --progress)


# Process special path characters 
#	~ are parsed as home paths
#   /./ with -R/--relative allow for relative paths to be transferred from /./ onwards
#   Multiple files must be transferred to existing directory, or new directory that ends with / i.e) new-dir/ to be mkdir
#   Multiple folders can be transferred to existing directory, or new directory that has optional ends with / i.e) new-dir/ to be mkdir
local_path="$(echo ${local_path} | sed "s%~%${local_home}%g")"
remote_path="$(echo ${remote_path} | sed "s%~%${remote_home}%g")"


# Command
cmd=()
cmd+=(${exe})
cmd+=(${flags})
cmd+=(${options})

case ${TRANSFER} in
	
	remote-local-remote)	
		cmd+=("${local_address}${local_path}")
		cmd+=("${remote_address}${remote_path}")
	;;

	local-local-remote)	
		cmd+=("${local_address}${local_path}")
		cmd+=("${remote_address}${remote_path}")
	;;

	remote-remote-local)	
		cmd+=("${remote_address}${remote_path}")
		cmd+=("${local_address}${local_path}")
	;;

	local-remote-local)	
		cmd+=("${remote_address}${remote_path}")
		cmd+=("${local_address}${local_path}")
	;;

esac	

echo "${cmd[@]}"
if [[ ${DEBUG} -eq 0 ]]
then
	${cmd[@]}
fi