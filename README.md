```
$ make test2orig

./test.sh new.sh @responsefile.txt
real    0m0.013s
user    0m0.007s
sys     0m0.007s
67597 chars, 1044 args

./test.sh newbash.sh @responsefile.txt
real    0m1.209s
user    0m1.062s
sys     0m0.168s
67597 chars, 1044 args

./test.sh old.sh @responsefile.txt
real    0m2.110s
user    0m1.953s
sys     0m0.157s
67597 chars, 1044 args

./test.sh orig.sh @responsefile.txt
real    3m25.268s
user    3m16.181s
sys     0m8.944s
67597 chars, 1044 args
```
