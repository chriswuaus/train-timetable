#!/bin/bash

echo "##########################"
echo "### Running e2e tests! ###"
echo "##########################\n"
count=0 # number of test cases run so far

files=("single_station1" "col_1" "col_2" "col_3" "big" "break" "breaks" "noarg_1" "noarg_2" "badtime_1" "badtime_2" "badtime_3" "badtime_4" "badtime_5" "badtime_6" "unsorted" "notrain" "sametime" "nextday" "multstations")

for file in ${files[@]};do
  cat tests/$file.in | xargs -a tests/$file.args ./timetable | diff - tests/$file.out || echo "Test $file: failed!" && echo "Test $file: passed!"
  count=$((count+1))
done

tests/empty.in | xargs -a tests/empty.args ./timetable &> tests/empty.txt 
diff tests/empty.out tests/empty.txt && echo "Test empty: passed!"
count=$((count+1))

echo "Finished running $count tests!"
