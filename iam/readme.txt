########################
####IAM��Ŀ����˵��#####
########################

һ������˵��
	login server��iam��Ŀ����Ҫ��ɲ��֣�����QB��QM��Excel�����������ͻ��˽���dds�ĵ�¼��֤��
Ϊ������չ��Ŀ��������iam��Ŀ��ʹ�õ��������߷���vendorsĿ¼�£���login_server����srcĿ¼�£�
�պ����iam��Ŀ��Ҫ��������Ӧ�ó���������ֱ�ӽ�source����srcĿ¼�£������µĹ���������sln
����ͳһ�����ɡ�

��������Ŀ¼˵��
1��IAM����rootĿ¼��/SFC/iam
2��/SFC/iam/bin������win32 debug���������������exe��dll�������ļ���
3��/SFC/iam/config�����������ļ�������
4��/SFC/iam/doc������iam���������ĵ�
5��/SFC/iam/include������iam���̹�ͨ��includeͷ�ļ�
6��/SFC/iam/lib������linux�����±�������������п��ļ���CMakeLists.txt��ʹ�ô�Ŀ¼
7��/SFC/iam/src/iam_login_server������login server���������ļ�
8��/SFC/iam/vendors������iam��������������3����
9��/SFC/iam/vendors/linux��3�����linux�汾
10��/SFC/iam/vendors/windows��3�����windows�汾

����login server������֪
1��login server����ȫʹ��c++11�����ģ��������õ�boost����κι��ܣ�ԭ�������ǲ������boostģ��ģ�
���������������������msgbus������������boost�������޷��ӱܱ���ʱ����boost��
2��boost�汾˵����
	2.1��windows���뻷��ʹ�õ�boost�汾Ϊ1.47.0
	2.2��linux cent6.5����ʹ�õ�boost�汾�����Ըߣ���Ϊmsgbus����Ϊcentos7�������汾������Ҳ�����boost�汾


