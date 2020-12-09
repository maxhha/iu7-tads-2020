PROG=./app.exe
FILE=result2.csv

printf "file\tВремя реструктуризации\n" > $FILE

for i in in0.txt in1.txt in2.txt in3.txt
do
  git restore $i
  for j in 6 3 1
  do
    printf "$i\t$j\t" >> $FILE
    result=$((echo $j; echo 0) | $PROG $i | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g")
    printf "$result" | grep "реструктуризации хеш таблицы:" | sed -Ee "s/.+ ([0-9]+) .+/\1/g" >> $FILE
    git restore $i
  done
done
