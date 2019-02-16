#include <gtest/gtest.h>
#include <razer/vm/VirtualMachine.h>

class VirtualMachineTest : public ::testing::Test
{
public:
    RzEngine engine;
};

TEST_F(VirtualMachineTest, testInit)
{
    RzVirtualMachine vm(engine);

    auto& mainThread = vm.getMainThread();
    ASSERT_TRUE(mainThread.getStackSize() > 0);

    ASSERT_EQ((char*)mainThread.esp, mainThread.getStackEnd());
}
