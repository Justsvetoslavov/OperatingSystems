echo "abc" > f1 | touch f2 | cp f1 f3 | mkdir d1

cd d1

../{f1,f2,f3} . # move f1,f2,f3 to currect folder