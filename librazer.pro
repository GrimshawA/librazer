TEMPLATE = subdirs

SUBDIRS = librazer \
		  testbed

librazer.file = build/qmake/librazer.pro

testbed.file = testbed/testbed.pro
testbed.depends = librazer
