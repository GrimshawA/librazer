TEMPLATE = subdirs

SUBDIRS = librazer \
          testbed  \
          unit_tests

librazer.file = build/qmake/librazer.pro

testbed.file = testbed/testbed.pro
testbed.depends = librazer

unit_tests.file = test/unit_tests.pro
unit_tests.depends = librazer
