package sumscope.lava.test.testroles;

import sumscope.lava.test.LavaMqTest;

public final class LavaMqServerD {
    public static void main(String[] args) {
        (new LavaMqTest()).serverSubscribeImageTest(true);
        //(new LavaMqTest()).serverSubscribeTest(true);
    }
}
