package sumscope.lava.test.testroles;

import sumscope.lava.test.LavaMqCdhTest;
import sumscope.lava.test.LavaMqCdhTest2;
import sumscope.lava.test.LavaMqTest;

public final class LavaMqClientC {
    public static void main(String[] args) {
        /*
        int testMode = 1;
        switch (testMode) {
            case 1:
                (new LavaMqTest()).clientSubscribeTest(false);
                break;
            case 2:
                (new LavaMqTest()).clientSubscribeImageTest(true);
                break;
        }
        */
        LavaMqCdhTest2 t = new LavaMqCdhTest2();
        t.initClient();
        t.startClient();
    }
}
