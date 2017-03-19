#include "testbed.h"
#include "run_arkanoid.h"

#include <QDir>
#include <QDebug>

void testbed::run()
{
    run_arkanoid();
	//system("pause");
}

void main()
{
	testbed t;
	t.run();
}
