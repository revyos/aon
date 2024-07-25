find . -name "*.c" -o -name "*.h" -o -name "*.mk" -o -name "Makefile" | xargs chmod -x
find . -name "*.c" -o -name "*.h" -o -name "*.mk" -o -name "Makefile" | xargs dos2unix
