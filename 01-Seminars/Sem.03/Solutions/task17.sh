# 17.1: общия брой редове
awk 'END{print NR}' emp.data

# 17.2: третия ред
awk 'NR == 3 {print}' emp.data

# 17.3: последното поле от всеки ред
awk '{print $NF}' emp.data

# 17.4: последното поле на последния ред
awk 'END{print $NF}' emp.data

# 17.5: всеки ред, който има повече от 4 полета
awk 'NF > 4 {print}' emp.data

# 17.6: всеки ред, чието последно поле е по-голямо от 4
awk '$NF>4 {print $NF}' emp.data

# 17.7: общия брой полета във всички редове
awk '{print NF}' emp.data

# 17.8: броя редове, в които се среща низът Beth
awk '/Beth/ {print}' emp.data | awk 'END{print NR}'
# or
awk '/.*Beth.*/ {i+=1} END{print i}' emp.data

# 17.9: най-голямото трето поле и редът, който го съдържа
awk 'BEGIN {max = 0; line = " "} {if ($3 > max) {max = $3; line = $0}} END {printf line"\n"}' emp.data
#or
awk 'BEGIN{max=0; line=""} {if($3>max) {max=$3; line=$0}} END{print max ": " line}' emp.data

# 17.10: всеки ред, който има поне едно поле
awk 'NF > 0' emp.data
# or
awk '{if(NF >= 1) {print $0}}' emp.data

# 17.11: всеки ред, който има повече от 17 знака
awk 'length($0) > 17' emp.data
#or
awk 'length > 17' emp.data

# 17.12: броя на полетата във всеки ред и самият ред
awk '{print "Field:" NF "\nLine:" $0 "\n"}' emp.data
# or if we do not want an additional newline
awk '{print "Fields: " NF " | Line: " $0}' emp.data

# 17.13: всеки ред така, че първите две полета да са с разменени места
awk '{f1 = $1; f2 = $2} {print f2 "\t" f1}' emp.data
# or
awk '{{temp = $1; $1 = $2; $2 = temp} print}' emp.data
#or if we want other fields as well (3rd)
awk '{f1 = $1; f2 = $2} {print f2 "\t" f1 "\t" $3}' emp.data

# 17.14: всеки ред така, че на мястото на първото поле да има номер на реда
awk '{$1=NR} {print}' emp.data
#or
awk '{$1=NR":"} {print $0}' emp.data

#17.15: всеки ред без второто поле
awk '{$2=""} {print}' emp.data
#or
awk '{$2="\t"} {print}' emp.data

# 17.16: за всеки ред, сумата от второ и трето поле
awk 'BEGIN{sum=0} {sum=$2+$3} {print sum}' emp.data
#or
awk '{sum = $2 + $3; printf "%.2f\n", sum; sum = 0}' emp.data
#or
awk '{printf "%.2f\n", $2 + $3}' emp.data

# 17.17: сумата на второ и трето поле от всеки ред
awk 'BEGIN{sum = 0} {sum += $2 + $3} END{print sum}' emp.data