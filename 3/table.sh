FILE="./r.txt"

echo -e "type\tsize\tpercent\ttime"
cat $FILE |
    sed -zEe "s/ -----\n.*? ([0-9]+?) мкс/, \1 мкс/g" -e "s/----- //g" |
    sed -Ee "s/type: (.+?), size: (.+?), percent: (.+?), (.+?) мкс/\1\t\2\t\3\t\4/g"
