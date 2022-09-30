package sumscope.lava.test.testroles;

import sumscope.lava.test.LavaMqTest;

public final class LavaMqClientD {
    public static void main(String[] args) {
        int testMode = 2;
        switch (testMode) {
            case 1:
                (new LavaMqTest()).clientSubscribeTest(true);
                break;
            case 2:
                (new LavaMqTest()).clientSubscribeImageTest(true);
                break;
        }
    }
}
