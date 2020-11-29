FILE="./table.csv"
PROG="./app.exe"

printf "n\tpop array\tpush array\tpop list\tpush list\n" > $FILE

for i in 10 50 100 500 1000 5000 10000 50000 100000 500000 1000000
do
  echo i=$i
  printf "$i" >> $FILE

  result=$($PROG -sarray --time=$i | sed -z -Ee "s/push: ([0-9]+\.[0-9]+) s\npop:  ([0-9]+\.[0-9]+) s/\1\t\2/g" | sed -e "s/\./,/g")
  echo array $result
  printf "\t$result" >> $FILE

  result=$($PROG -slist --time=$i | sed -z -Ee "s/push: ([0-9]+\.[0-9]+) s\npop:  ([0-9]+\.[0-9]+) s/\1\t\2/g" | sed -e "s/\./,/g")
  echo list $result
  printf "\t$result\n" >> $FILE
done
