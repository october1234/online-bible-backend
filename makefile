build:
	/usr/bin/clang++ \
	-fcolor-diagnostics \
	-fansi-escape-codes \
	-g \
	./app.cpp \
	-I/opt/homebrew/Cellar/boost/1.82.0_1/include/ \
	-std=c++17 \
	-lsqlite3 \
	-o app
