default: expand-compiler-args

test: expand-compiler-args ghc_7.rsp
	./test.sh new.sh @ghc_7.rsp
	./test.sh old.sh @ghc_7.rsp

ghc_7.rsp:
	wget "https://gist.githubusercontent.com/pbogdan/9d6986bf931b58a70d75e14eb40ee8a1/raw/79a59fd108ba276aa069ef730ec7d9ffea3f1b8f/ghc_7.rsp"
