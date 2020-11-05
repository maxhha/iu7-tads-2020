
FILE="result.csv"
PROG="./app.exe"
echo "Begin"

printf "size\tpercent\ttm\tts\n" > $FILE

for size in 10 25 50 75 100 250 500 750 1000
do
    for percent in 10 20 30 40 50 60 70 80 90
    do
        printf "$size\t$percent" >> $FILE
        for type in tm ts
        do
            echo "size: $size, percent: $percent, type: $type"

            t=$(
                (
                    echo $type
                    echo $size
                    echo $percent
                    echo $size $size
                    echo $percent
                    echo q
                ) |
                $PROG |
                grep "мкс" |
                sed -zE -e "s/.*? ([0-9]+?) мкс/\1/g"
            )
            printf "\t$t" >> $FILE
            echo $t
        done
        printf "\n" >> $FILE
    done
done

echo "End"
