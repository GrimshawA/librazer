#include "testbed.h"
#include "run_arkanoid.h"
#include "run_pong.h"

#include <QDir>
#include <QDebug>

void testbed::run()
{
    //run_arkanoid();
    run_pong_game();
	//system("pause");
}

void main()
{
	testbed t;
	t.run();
}
