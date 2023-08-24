reqnum=10
while [ $# -gt 0 ] ; do
    case "$1" in
        -n)
            if [ $# -lt 2 ]; then
                echo "-n missing value"
                exit -1
            fi
            reqnum=$2
            shift 2
            ;;
        -h|--help|*)
            echo "-n request numbers"
            exit 0
            ;;
    esac
done

for((i=0; i<${reqnum}; i++)); do
    echo "request $i"
    ./client.out &
done
