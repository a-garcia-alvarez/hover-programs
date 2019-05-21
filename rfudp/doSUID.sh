#!/bin/bash
SUDO=""

if [ $UID != 0 ]; then
	SUDO="sudo"
fi

${SUDO} chown root: "$@"
${SUDO} chmod u+s "$@"
