
PROG=./app.exe
FILE=result.csv

printf "file\telement\tВремя построения дерева\tВремя балансирования дерева\tВремя построения хеш таблицы\tВремя удаления из ДДП\tВремя удаления из сбалансированного дерева\tВремя удаления из хеш таблицы\tВремя удаления из файла\tСравнений ДДП\tСравнений АВЛ\tСравнений хеш-таблицы\tСравнений в файле\n" > $FILE

for i in in0.txt in1.txt in2.txt in3.txt
do
  git restore $i
  for j in $(cat $i)
  do
    printf "$i\t$j\t" >> $FILE
    result=$((echo 3; echo $j) | $PROG $i | sed -r "s/\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g")
    printf "$result" | grep "Время" | sed -r "s/.+реструктуризации хеш таблицы:.+//g" | sed -z -e "s/\n\n/\n/g" | sed -Ee "s/.+ ([0-9]+) .+/\1/g" | sed -z -Ee "s/\n/\t/g" >> $FILE
    printf "$result" | grep "сравнений" | sed -Ee "s/.+ ([0-9]+) .+/\1/g" | sed -z -Ee "s/\n([0-9]+)/\t\1/g" >> $FILE
    git restore $i
  done
done
