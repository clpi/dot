#!/bin/bash
set -e; set -u; set -o pipefail

R='\033[0;31m'; LR='\033[1;31m'; G='\033[0;32m'; LG='\033[1;32m'
P='\033[0;35m'; LP='\033[1;35m'; C='\033[0;36m'; LC='\033[1;36m'
Y='\033[0;33m'; LY='\033[1;33m'; B='\033[0;34m'; LB='\033[1;34m'; N='\033[0m'

# global vars
VERBOSE=0
DEBUG=0
EXP=0


function usage() {
    echo -e "${LG}doti${N} ${Y}version${N} 0.1.0-development 2021."
    echo -e "${G}Help documentation.${N}"
    echo -e "${LG}Subcommands${N}"
    echo -e "${G} ∙ ${Y}server${N}"
    echo -e "${G} ∙ ${Y}client${N}"
    echo -e "${G} ∙ ${Y}login${N}"
    echo -e "${G} ∙ ${Y}channel${N}"
    echo -e "${G} ∙ ${Y}help${N}"
    echo -e "${G}${N}"
    echo -e "${LG}Options${N}"
    echo -e ""
    echo -e "${LG}Arguments${N}"
    echo -e ""
    return 0
}

SERVER_HOST=0.0.0.0
SERVER_PORT=8091
function server_cmd() {
    (( !$# )) && echo "No args provided! Need username / password" && usage && exit 1
    echo -e "${G}Got ${LB}channel${N} command with args ${LY}$@${N} ${Y}${N}"
    for arg in "$@"; do
        case $arg in
            -v|--verbose)      VERBOSE=1                 ;;
            -d|--debug)        DEBUG=1                   ;;
            -x|--experimental) EXP=1                     ;;
            * ) echo -e "${R}Invalid opt: ${Y}-$arg${N}" 1>&2; usage; exit 1 ;;
        esac
        shift
    done
}

INPUT_MSG=""
CHANNEL_NAME=""
DATA_DIR=""
function channel_cmd() {
    echo -e "${G}Got ${LB}channel${N} command: with args ${LY}$@${N} ${Y}${N}"
    for arg in "$@"; do
        case $arg in
            -v|--verbose)      VERBOSE=1                 ;;
            -d|--debug)        DEBUG=1                   ;;
            -x|--experimental) EXP=1                     ;;
            -n=*|--name=*)     CHANNEL_NAME="${arg#*=}"  ;;
            -n|--name)         CHANNEL_NAME=$2; shift    ;;
            -f=*|--folder=*)   DATA_DIR="${arg#*=}"      ;;
            -f|--folder)       DATA_DIR=$2; shift        ;;
            -m=*|--msg=*)      INPUT_MSG="${arg#*=}"      ;;
            -m|--msg)          INPUT_MSG=$2; shift        ;;
            -h|--help)   usage; exit 0                   ;;
            * ) echo -e "${R}Invalid opt: ${Y}-$arg${N}" 1>&2; usage; exit 1 ;;
        esac
        shift
    done
}


function client_cmd() {
    (( !$# )) && echo "No args provided! Need username / password" && usage && exit 1
    echo -e "${G}Got ${LB}client${N} command: with args ${LY}$@${N} ${Y}${N}"
    for arg in "$@"; do
        case $arg in
            -v|--verbose)      VERBOSE=1            ;;
            -d|--debug)        DEBUG=1              ;;
            -x|--experimental) EXP=1                ;;
            * ) echo -e "${R}Invalid opt: ${Y}-$arg${N}" 1>&2; usage; exit 1 ;;
        esac
        shift
    done

}

EMAIL=""
API_KEY=""
USERNAME=""
function login_cmd() {
    (( !$# )) && echo "No args provided! Need username / password" && usage && exit 1
    echo -e "${G}Got ${LB}login${N} command: with args ${LY}$@${N} ${Y}${N}"
    for arg in "$@"; do
        case $arg in
            -v|--verbose)      VERBOSE=1            ;;
            -d|--debug)        DEBUG=1              ;;
            -x|--experimental) EXP=1                ;;
            -k=*|--apikey=*)   API_KEY="${arg#*=}"  ;;
            -k|--apikey)       API_KEY=$2; shift    ;;
            -u=*|--username=*) USERNAME="${arg#*=}" ;;
            -u|--username)     USERNAME=$2; shift   ;;
            -e=*|--email=*)    EMAIL="${arg#*=}"    ;;
            -e|--email)        EMAIL=$2; shift      ;;
            -h|--help)         usage; exit 0        ;;
            * ) echo -e "${R}Invalid opt: ${Y}-$arg${N}" 1>&2; usage; exit 1 ;;
        esac
        shift
    done
}


CMD=""
function main() {
    if [ $# -lt 1 ]; then
        echo -e "${LY}No args given!${N}";
        usage;
        exit 0;
    fi
    CMD=
    case $1 in
        server)  CMD=server_cmd        ;;
        client)  CMD=client_cmd        ;;
        channel) CMD=channel_cmd       ;;
        login)   CMD=login_cmd         ;;
        help)    usage; exit 0           ;;
        *) echo -e "${R}Invalid opt: ${Y}-$arg${N}" 1>&2; usage; exit 1 ;;
    esac
    shift
    echo -e "Got subcommand ${LG}$CMD${N} with args ${LY}$@${N}"
    $CMD $@
}

main $@
