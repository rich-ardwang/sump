package sumscope.lava.mq;

/**
 * 函数名称：compareTopic
 * 用途：topic匹配
 * 输入参数：
 *      srcTopic：被匹配的源topic
 *      destTopic：匹配的目标topic
 * 返回值：
 *      0：最匹配
 *      1：含有*的匹配
 *      2：含有**的匹配
 *      -1：含有(*或**)不匹配
 *      -2：完全(不含*或**)不匹配
 */
public interface TopicMatchBase {
    static int compareTopic(String srcTopic, String destTopic) {
        if (srcTopic.equals(destTopic)) {
            return 0;
        }
        if ("**".equals(srcTopic)) {
            return 2;
        }
        if ("**".equals(destTopic)) {
            return -1;
        }
        String[] strs1 = srcTopic.split("\\.");
        String[] strs2 = destTopic.split("\\.");
        int result = -2;
        for (int i = 0; i < strs1.length && i < strs2.length; i++) {
            String sub1 = strs1[i];
            String sub2 = strs2[i];
            if (sub1.equals(sub2)) {
                if (1 == result) {
                    continue;
                }
                result = 0;
            } else if ("*".equals(sub1)) {
                result = 1;
            } else if ("**".equals(sub1)) {
                result = 2;
                break;
            } else if ("*".equals(sub2) || "**".equals(sub2)) {
                result = -1;
                break;
            } else {
                result = -2;
                break;
            }
        }
        if ((0 == result) || (1 == result)) {
            if (strs1.length > strs2.length) {
                String last = strs1[strs2.length];
                if ("*".equals(last)) {
                    result = 1;
                } else if ("**".equals(last)) {
                    result = 2;
                } else {
                    result = -2;
                }
            } else if (strs1.length < strs2.length) {
                String last = strs2[strs1.length];
                if ("*".equals(last) || "**".equals(last)) {
                    result = -1;
                } else {
                    result = -2;
                }
            }
        }
        return result;
    }

    //this is default topic matching function.
    //you can overwrite it if you want to get extend.
    default int topicMatch(String srcTopic, String destTopic) {
        return TopicMatchBase.compareTopic(srcTopic, destTopic);
    }
}
