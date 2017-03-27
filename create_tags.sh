rm -f TAGS
find . -name "*.[ch]*"  -print | xargs etags -a
