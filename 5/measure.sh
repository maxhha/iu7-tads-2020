FILE="./table.csv"
PROG="./app.exe"

printf "n\tpush array\tpop array\tpush list\tpop list\n" > $FILE

for i in 10 50 100 500 1000 5000 10000 50000 100000 500000 1000000
do
  echo i=$i
  printf "$i" >> $FILE

  result=$(
    (
      echo type
      echo 2
      echo t
      echo $i
      echo q
    ) |
    $PROG |
    sed -z -Ee "s/.+Добавление: ([0-9]+) тиков\nЧтение: ([0-9]+) тиков.+/\1\t\2/g"
  )

  echo array $result
  printf "\t$result" >> $FILE

  result=$(
    (
      echo type
      echo 1
      echo t
      echo $i
      echo q
    ) |
    $PROG |
    sed -z -Ee "s/.+Добавление: ([0-9]+) тиков\nЧтение: ([0-9]+) тиков.+/\1\t\2/g"
  )

  echo list $result
  printf "\t$result\n" >> $FILE
done
