package sumscope.lava.test.testroles;

import sumscope.lava.test.LavaMqCdhTest;
import sumscope.lava.test.LavaMqCdhTest2;
import sumscope.lava.test.LavaMqTest;

public final class LavaMqServerC {
    public static void main(String[] args) {
        //(new LavaMqTest()).serverSubscribeTest(false);
        LavaMqCdhTest2 t = new LavaMqCdhTest2();
        t.initServer();
        t.startServer();
    }
}
