#include "simple_vector.h"

// Tests
#include "tests.h"

int main() {
    Test1();
    Test2();
    
    TestReserveConstructor();
    TestReserveMethod();
    
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiableMoveConstructor();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
//    
    return 0;
}
