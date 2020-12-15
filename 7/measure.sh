
PROG=./app.exe
FILE=result.csv

printf "ver n\tedges percent\tВремя чтения m\tВремя дистанций m\tВремя записи m\tВремя чтения l\tВремя дистанций l\tВремя записи l\n" > $FILE

for ver_n in 10 100 250
do
  for edges_perc in 25 50 75
  do
      printf "$ver_n\t$edges_perc\t" >> $FILE

      result=$($PROG "in_${ver_n}_${edges_perc}.txt" 1 0 m nodraw | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g")
      printf "$result" | grep "Время" | sed -z -e "s/\n\n/\n/g" | sed -Ee "s/.+ ([0-9]+) .+/\1/g" | sed -z -Ee "s/\n/\t/g" >> $FILE

      result=$($PROG "in_${ver_n}_${edges_perc}.txt" 1 0 l nodraw | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g")
      printf "$result" | grep "Время" | sed -z -e "s/\n\n/\n/g" | sed -Ee "s/.+ ([0-9]+) .+/\1/g" | sed -z -Ee "s/\n([0-9]+)/\t\1/g" >> $FILE
  done
done
