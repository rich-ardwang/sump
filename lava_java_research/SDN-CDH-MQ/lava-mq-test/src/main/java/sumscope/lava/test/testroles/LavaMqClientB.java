package sumscope.lava.test.testroles;

import sumscope.lava.test.LavaMqTest;

public final class LavaMqClientB {
    public static void main(String[] args) {
        int testMode = 2;
        switch (testMode) {
            case 1:
                (new LavaMqTest()).clientAsyncTest(true);
                break;
            case 2:
                (new LavaMqTest()).clientSyncTest(false);
                break;
        }
    }
}
