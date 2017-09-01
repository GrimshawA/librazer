TEMPLATE = subdirs

SUBDIRS = librazer \
          testbed  \
          test

librazer.file = build/qmake/librazer.pro

testbed.file = testbed/testbed.pro
testbed.depends = librazer

test.file = test/test.pro
test.depends = librazer
