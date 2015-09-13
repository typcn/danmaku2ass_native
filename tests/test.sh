./danmaku2ass_native -in=../tests/testdata.xml -out=./test.out -w=1280 -h=720 -font="Heiti SC" -fontsize=25 -alpha=0.8 -dm=5 -ds=5
lcov --gcov-tool "gcov-4.8" --directory . --capture --output-file coverage.info
lcov --gcov-tool "gcov-4.8" --remove coverage.info 'tests/*' '/usr/*' 'rapid*' --output-file coverage.info
lcov --gcov-tool "gcov-4.8" --list coverage.info
coveralls-lcov -t $COVERALLS_TOKEN coverage.info