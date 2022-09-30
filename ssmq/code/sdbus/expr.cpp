
#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/expr.h"

#include <string>

namespace sdbus{

const ExprNode ExprNode::EMPTY ;

ExprNode::ExprNode()
{
    Parent = NULL ;
    Left = NULL ;
    Right = NULL ;

    Type = kNodeUnknown ;
}

ExprNode::~ExprNode()
{
    Free() ;
}

ExprNode::ExprNode(const ExprNode& node)
{
    Assign(node) ;
}

ExprNode& ExprNode::operator=(const ExprNode& node)
{
    Assign(node) ;
    return (*this) ;
}

void ExprNode::Free() 
{
    if(Left != NULL)
    {
        Left->Free() ;
        delete Left ;
        Left = NULL ;
    }

    if(Right != NULL)
    {
        Right->Free() ;
        delete Right ;
        Right = NULL ;
    }
}

bool ExprNode::operator==(const ExprNode& node) const
{
    if(Type == node.Type)
        return true ;
    else
        return false ;
}

bool ExprNode::operator!=(const ExprNode& node) const
{
    return !((*this) == node) ;
}

void ExprNode::Assign(const ExprNode& node)
{
    Type = node.Type ;
}

int  ExprNode::Scan(const char * str , int len) 
{
    return 0 ;
}

const sdbus::string ExprNode::ToString() const 
{
    char str[256] ;
    ::sprintf(str , "TYPE[%d]" , Type) ;

    return sdbus::string(str) ;
}



Expr::Expr()
{
    root_ = NULL ;
}

Expr::~Expr()
{
    Clear() ;
}

void Expr::Clear() 
{
    if(root_ != NULL)
    {
        delete root_ ;
        root_ = NULL ;
    }
}

int Expr::SkipSpace(const char * str , int slen)  
{
    int size = 0 ;
    char ch = 0 ;
    while((size < slen) && ((ch = str[size]) != 0))
    {
        if(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
            ++size ;
        else
            break ;
    }

    return size ;
}

bool Expr::CheckLogical(const char * str , int size , sdbus::string& upper) 
{
    if(str == NULL || size > 3)
        return false ;

    char ustr[32] ;
    int ulen = 0 ;

    while(ulen < size)
    {
        char ch = str[ulen] ;
        if(ch >= 'a' && ch <= 'z')
            ustr[ulen] = ch - 'a' + 'A' ;
        else
            ustr[ulen] = ch ;

        ++ulen ;
    }
    ustr[ulen] = '\0' ;

    if(strcmp(ustr , "AND") == 0 || strcmp(ustr , "OR") == 0)
    {
        upper = ustr ;
        return true ;
    }
    else
        return false ;
}

bool Expr::CheckDigit(const char * str , int slen) 
{
    if(str == NULL || slen <= 0)
        return false ;

    int cidx = 0 ;
    while(cidx < slen)
    {
        char ch = str[cidx] ;
        if(ch < '0' || ch > '9')
            break ;

        ++cidx ;
    }

    return (cidx == slen) ;
}

int Expr::ScanName(const char * str , int slen , const char *& name , int& nlen) 
{
    int offset = SkipSpace(str , slen) ;
    const char * nstr = str + offset ;
    int size = 0 , left = slen - offset ;
    char ch = 0 ;
    while((size < slen) && (ch = nstr[size]) != 0)
    {
        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' ||  (ch >= '0' && ch <= '9'))
            ++size ;
        else 
            break ;    
    }

    if(size > 0)
    {
        //如果是数字开头的，必须是全部数字
        ch = nstr[0] ;
        if(ch >= '0' && ch <= '9')
        {
            if(CheckDigit(nstr , size) == false)
                return 0 ;
        }

        name = nstr ;
        nlen = size ;
        return (offset + size) ;
    }

    return 0 ;
}

int Expr::ScanRelation(const char * str , int slen , const char *& oper , int& olen) 
{
    int offset = Expr::SkipSpace(str , slen) ;
    int left = slen - offset ;

    if(left <= 0)
        return 0 ;

    olen = 0 ;

    const char *ostr = str + offset ;
    char ch1 = ostr[0] ;

    if(ch1 == '=')
    {
        olen = 1 ;
    }
    else if(ch1 == '<' || ch1 == '>')
    {
        olen = 1 ;
        if(left >= 2)
        {
            char ch2 = ostr[1] ;
            if(ch2 == '=')
                olen = 2 ;
        }        
    }
    else if(ch1 == '!')
    {
        if(left >= 2 && ostr[1] == '=')
            olen = 2 ;
    }

    if(olen == 0)
        return 0 ;

    oper = ostr ;
    return (offset + olen) ;
}

int Expr::ScanValue(const char * str , int slen , const char *& value , int& vlen) 
{
    int offset = Expr::SkipSpace(str , slen) ;
    int left = slen - offset ;

    if(left <= 0)
        return 0 ;

    const char *vstr = str + offset ;
    int size = 0 , begin = 0 ;

    bool quoted = false ;

    while(size < left && quoted == false)
    {
        char ch = vstr[size] ;

        if(ch == '"')
        {
            //碰到引号，一直到另外一个引号出现
            if(size != 0)
                return 0 ;
    
            quoted = true ;
            ++vstr ;
            --left ;

            while(size < left)
            {
                char qch = vstr[size] ;
                if(qch == '"')
                    break ;
                else
                    ++size ;
            }

            if(size == left)
            {
                //没有找到双引号结束，则返回错误
                return 0 ;
            }

            offset += 2 ;
            break ;
        }

        if(ch == ' ')
            break ;
        else
            ++size ;
    }

    if(size <= 0)
        return 0 ;

    value = vstr ;
    vlen = size ;

    return (offset + size) ;
}

int Expr::ScanLogical(const char * str , int slen , sdbus::string& logical)
{
    int offset = Expr::SkipSpace(str , slen) ;

    const char *lstr = str + offset ;
    int left = slen - offset ;

    char data[8] ;
    int size = 0 ;
    while(size < left && size < 7)
    {
        char ch = lstr[size] ;
        if(ch >= 'a' && ch <= 'z')
        {
            data[size] = ch - 'a' + 'A' ;        
        }
        else if(ch >= 'A' && ch <= 'Z')
        {
            data[size] = ch ;
        }
        else
        {
            break ;
        }

        ++size ;
    }

    if(size <= 0 || size > 3)
        return 0 ;

    data[size] = '\0' ;

    if(strcmp(data , "OR") == 0 || strcmp(data , "AND") == 0)
    {
        logical = data ;
        return (offset + size) ;
    }

    return 0 ;
}

bool Expr::Parse(const char * expr)
{
    Clear() ;
    int offset = 0 ;
    int elen = 0 ;
    if(expr != NULL)
        elen = ::strlen(expr) ;

    while(offset < elen)
    {
        int space = SkipSpace(expr + offset , elen - offset) ;
        if(space != 0)
        {
            offset += space ;
            continue ;
        }

        const char& ch = expr[offset] ;
        if(ch == '(')
        {
            DeguacusNode * node = new DeguacusNode() ;
            int length = node->Scan(expr + offset , elen - offset) ;
            if(length <= 0)
            {
                delete node ;
                return false ;
            }

            if(AddExpr(node) == false)
                return false ;

            offset += length ;
        }
        else if(ch == '\0')
        {
            break ;
        }
        else
        {
            const char * nstr = NULL ;
            int nlen ;
            int length = Expr::ScanName(expr + offset , elen - offset , nstr , nlen) ;
            if(length <= 0)
                return false ;

            sdbus::string name ;
            if(CheckLogical(nstr , nlen , name) == true)
            {
                LogicalNode * node = new LogicalNode() ;
                node->Operator = name ;

                if(AddLogical(node) == false)
                    return false ;
            }
            else
            {
                ArithNode * node = new ArithNode() ;
                length = node->Scan(expr + offset , elen - offset) ;
                if(length <= 0)
                {
                    delete node ;
                    return false ;
                }

                if(AddExpr(node) == false)
                    return false ;
            }

            offset += length ;
        }//end if(ch == ...)
    
    }//end while(true) ;

    return true ;    
}

const ExprNode* Expr::First() const
{
    return Left(root_) ;
}

const ExprNode* Expr::Next(const ExprNode* node) const
{
    if(node == NULL)
        return NULL ;

    const ExprNode * parent = node->Parent ;
    if(parent == NULL)
    {
        //没有父亲节点，就是根节点，直接返回。
        return NULL ;
    }

    const ExprNode * left = parent->Left ;
    const ExprNode * right = parent->Right ;

    if(left == node)
    {
        //如果当前节点为父节点的左节点，那么访问父节点的右节点
        if(right == NULL)
            return parent ;

        return Left(right) ;
    }

    if(right == node) 
    {
        return parent ;
    }

    return NULL ;
}

const ExprNode * Expr::Left(const ExprNode * node) const
{
    if(node == NULL)
        return NULL ;

    const ExprNode * left = NULL , *cur = node;
    while((left = cur->Left) != NULL)
        cur = left ;

    return cur ;
}

bool Expr::AddExpr(ExprNode * node)
{
    if(root_ == NULL)
    {
        root_ = node ;
        return true ;
    }

    if(root_->Type != ExprNode::kNodeLogical || root_->Left == NULL || root_->Right != NULL)
    {
        delete node ;
        return false ;
    }

    node->Parent = root_ ;
    root_->Right = node ;
    return true ;
}

bool Expr::AddLogical(LogicalNode * node)
{
    if(root_ == NULL)
    {
        delete node ;
        return false ;
    }

    root_->Parent = node ;
    node->Left = root_ ;
    root_ = node ;
    return true ;
}

ArithNode::ArithNode()
{
    Type = ExprNode::kNodeArith ;
    Tag = 0 ;
}

ArithNode::~ArithNode()
{
    //
}

const sdbus::string ArithNode::ToString() const 
{
    char str[4096] ;
    ::sprintf(str , "Type[Arith] Name[%s] Operator[%s] Value[%s]" , Name.c_str() , Operator.c_str() , Value.c_str()) ;
    return sdbus::string(str) ;
}

int ArithNode::Scan(const char * str , int len)
{
    //1、扫描名字
    const char * name = NULL;
    int nlen = 0 , offset = 0;

    int nsize  = Expr::ScanName(str , len , name , nlen) ;
    if(nsize == 0)
        return 0 ;
    offset += nsize ;

    //2、扫描操作符
    const char * oper = NULL ;
    int olen = 0 ;

    int osize = Expr::ScanRelation(str + offset , len - offset , oper , olen) ;
    if(osize <= 0)
        return 0 ;

    offset += osize ;

    //3、扫描值
    const char * value = NULL ;
    int vlen = 0 ;

    int vsize = Expr::ScanValue(str + offset , len - offset , value , vlen) ;
    if(vsize <= 0)
        return 0 ;

    offset += vsize ;

    Name.assign(name , nlen) ;
    if(Expr::CheckDigit(Name.c_str() , Name.length()) == true)
        Tag = (uint32_t)::atoi(Name.c_str()) ;

    Operator.assign(oper , olen) ;
    Value.assign(value , vlen) ;

    return offset ;
}


LogicalNode::LogicalNode()
{
    Type = ExprNode::kNodeLogical ;
}

LogicalNode::~LogicalNode()
{
    //
}

const sdbus::string LogicalNode::ToString() const 
{
    char str[256] ;
    ::sprintf(str , "Type[Logical] Operator[%s]" , Operator.c_str()) ;
    return sdbus::string(str) ;
}



DeguacusNode::DeguacusNode()
{
    Type = ExprNode::kNodeDeguacus ;
}

DeguacusNode::~DeguacusNode()
{
    //
}

int DeguacusNode::ScanCont(const char * str , int len , const char *&dstr , int&dlen)
{
    int offset = Expr::SkipSpace(str , len) ;

    const char * cstr = str + offset ;
    int csize = len - offset ;

    if(cstr[0] != '(')
        return 0 ;

    ++offset ;
    ++cstr ;
    --csize  ;

    char ch = 0 ;
    int size = 0 ;
    while((size < csize) && (ch = cstr[size]) != ')')
        ++size ;

    if(size == csize)
        return 0 ;

    dstr = cstr ;
    dlen = size ;

    return (offset + size + 1) ;
}

int DeguacusNode::ParseCont(const char * str , int slen) 
{
    int offset = Expr::SkipSpace(str , slen) ;
    const char * cstr = str + offset ;
    int clen = slen - offset ;

    int size = 0 ;
    while(size < clen)
    {
        //第一个只会是关系表达式
        const char * astr = cstr + size ;
        int alen = clen - size ;

        ArithNode * an = new ArithNode() ;
        int asize = an->Scan(astr , alen) ;
        if(asize <= 0)
        {
            delete an ;
            return 0 ;
        }

        if(AddArith(an) == false)
            return 0 ;

        size += asize ;

        //第二个可能是逻辑符号或者没有
        int llen = clen - size ;
        if(llen <= 0)
            break ;

        const char *lstr = cstr + size ;

        sdbus::string logical ;
        int lsize = Expr::ScanLogical(lstr , llen , logical) ;
        if(lsize == 0)
            break ;

        LogicalNode * ln = new LogicalNode() ;
        ln->Operator = logical ;

        if(AddLogical(ln) == false)
            return 0 ;

        size += lsize ;
    }

    if(size <= 0)
        return 0 ;

    return offset + size ;
}

const sdbus::string DeguacusNode::ToString() const 
{
    char str[256] ;
    ::sprintf(str , "Type[Deguacus] Operator[%s]" , Operator.c_str()) ;
    return sdbus::string(str) ;
}

bool DeguacusNode::AddArith(ArithNode * arith)
{
    if(Right != NULL)
    {
        delete arith ;
        return false ;
    }

    if(Left == NULL)
    {
        arith->Parent = this ;
        Left = arith ;

        return true ;
    }

    if(Left->Type != ExprNode::kNodeLogical || Left->Left == NULL)
    {
        delete arith ;
        return false ;
    }

    Left->Right = arith ;
    arith->Parent = Left ;

    return true ;
}

bool DeguacusNode::AddLogical(LogicalNode * logical)
{
    if(Left == NULL || Right != NULL)
    {
        delete logical ;
        return false ;
    }

    Left->Parent = logical ;

    logical->Parent = this ;
    logical->Left = Left ;
    Left = logical ;

    return true ;
}


int DeguacusNode::Scan(const char * str , int len)
{
    const char * dstr = NULL ;
    int dlen = 0 ;

    int offset = ScanCont(str , len , dstr , dlen) ;
    if(offset <= 0)
        return 0 ;

    Operator = "()" ;

    int clen = ParseCont(dstr , dlen) ;
    if(clen <= 0)
        return 0 ;

    return offset ;
}


}

