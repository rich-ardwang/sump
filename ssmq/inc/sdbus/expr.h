
#ifndef __SDBUS_EXPR_H
#define __SDBUS_EXPR_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"

namespace sdbus{

/*
    �Ƚ��Ѻõ��������ã�������SQL��WHERE �����������и�������ͬ��֮����OR����ͬ����֮����AND�����£�
    (Name = "ABC" OR Name = "BCD") AND Yield > 0.5
*/
class SDBUSAPI ExprNode{
public:
    ExprNode() ;
    virtual ~ExprNode() ;

    ExprNode(const ExprNode& node) ;
    ExprNode& operator=(const ExprNode& node) ;

    ExprNode * Parent ;
    ExprNode * Left ;
    ExprNode * Right ;

    int Type ;

    //��������
    static const int kNodeUnknown   =   0 ;
    static const int kNodeLogical   =   1 ;     //�߼������ OR ��AND
    static const int kNodeArith     =   2 ;     //��������� > < = ..
    static const int kNodeDeguacus  =   3 ;     //С����

    void Free() ;

    bool operator==(const ExprNode& node) const ;
    bool operator!=(const ExprNode& node) const ;

    virtual int Scan(const char * str , int len) ;
    virtual const sdbus::string ToString() const ;

    static const ExprNode EMPTY ;

private:
    void Assign(const ExprNode& node) ;
} ;

class SDBUSAPI ArithNode : public ExprNode{
public:
    ArithNode() ;
    virtual ~ArithNode() ;

    int      Tag ;
    sdbus::string Name ;
    sdbus::string Operator ;
    sdbus::string Value ;

    virtual int Scan(const char * str , int len) ;

    virtual const sdbus::string ToString() const ;
} ;

class SDBUSAPI LogicalNode : public ExprNode{
public:
    LogicalNode() ;
    virtual ~LogicalNode() ;

    sdbus::string Operator ;

    virtual const sdbus::string ToString() const ;
} ;

class SDBUSAPI DeguacusNode : public ExprNode{
public:
    DeguacusNode() ;
    virtual ~DeguacusNode() ;

    virtual int Scan(const char * str , int len) ;

    //ɨ��С����֮������ݷ�Χ
    int ScanCont(const char * str , int len , const char *&dstr , int&dlen) ;

    //����С����֮�������
    int ParseCont(const char * str , int slen) ;

    sdbus::string Operator ;

    virtual const sdbus::string ToString() const ;
private:
    bool AddArith(ArithNode * arith) ;
    bool AddLogical(LogicalNode * logical) ;
} ;


class SDBUSAPI Expr{
public:
    Expr() ;
    ~Expr() ;

    bool Parse(const char * expr) ;

    void Clear() ;

    /*
        �������
    */
    const ExprNode* First() const ;
    const ExprNode* Next(const ExprNode* node) const ;

     static int SkipSpace(const char * str , int slen) ;

     /*
        ��[A-Z , a-z , _]��ͷ����������[A-Z , a-z , _ , 0-9]�������OR , AND����ô������ɴ�д��

        2014-06-06
        ������Ҫ֧��FID��Ϊ���֣������Ҫ��ԭ��Name�Ļ����ϣ�����ȫ���ֵ�֧�֡�
     */
     static int ScanName(const char * str , int slen , const char *& name , int& nlen) ;


     static bool CheckLogical(const char * str , int slen , sdbus::string& upper) ;

     static bool CheckDigit(const char * str , int slen) ;

     /*
        ɨ���ϵ��������
     */
     static int ScanRelation(const char * str , int slen , const char *& oper , int& olen) ;

     /*
        ɨ��ֵ�������""Ϊ�磬��ôȡ�����ڲ������ݣ����û�У����Կո�Ϊ��
     */
     static int ScanValue(const char * str , int slen , const char *& value , int& vlen) ;

     /*
        ɨ���߼��������ֻ����"OR"������"AND"
     */
     static int ScanLogical(const char * str , int slen , sdbus::string& logical) ;
private:
    ExprNode * root_ ;

    const ExprNode * Left(const ExprNode * node) const ; 

    bool AddExpr(ExprNode * node) ;
    bool AddLogical(LogicalNode * node) ;
} ;

}

#endif  /**__SDBUS_EXPR_H  */

