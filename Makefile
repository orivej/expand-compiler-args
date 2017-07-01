default: expand-compiler-args

test: test1 test2
	true

test1: expand-compiler-args ghc_7.rsp
	./test.sh new.sh @ghc_7.rsp
	./test.sh old.sh @ghc_7.rsp

test2: expand-compiler-args responsefile.txt
	./test.sh new.sh @responsefile.txt
	./test.sh old.sh @responsefile.txt

test2orig: expand-compiler-args responsefile.txt
	./test.sh new.sh @responsefile.txt
	./test.sh old.sh @responsefile.txt
	./test.sh orig.sh @responsefile.txt

ghc_7.rsp:
	wget "https://gist.githubusercontent.com/pbogdan/9d6986bf931b58a70d75e14eb40ee8a1/raw/79a59fd108ba276aa069ef730ec7d9ffea3f1b8f/ghc_7.rsp"

responsefile.txt:
	wget "https://github.com/NixOS/nixpkgs/files/1116415/responsefile.txt"
