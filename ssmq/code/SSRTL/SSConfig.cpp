#include <sstream>
#include "TinyXML/tinyxml.h"
#include "SSRTL/SSConfig.h"
#include "SSRTL/SSString.h"
#include "SSRTL/SSLog.h"


static const char * DEFAULT_CONFIGFILE = "./gft_config.xml";


namespace ss{

	typedef std::list<const TiXmlElement *> elem_list ;
	typedef std::list<std::string> string_list ;

	void split_key(const std::string& key , string_list& keylist)
	{
		std::string strlist[10] ;
		int scount = 0 ;

		scount = string_split(key , strlist , 10 , '.') ;
		keylist.clear() ;

		for(int idx = 0 ; idx < scount ; idx++)
		{
			keylist.push_back(strlist[idx]) ;		
		}
	}


	const TiXmlElement * find_first_of(const TiXmlElement * root , string_list& names)
	{
		const TiXmlElement* elem = root ;
		for(string_list::const_iterator iter = names.begin() ; iter != names.end() && elem != NULL ; iter++)
		{
			elem = elem->FirstChildElement(*iter) ;
		}

		return elem ;
	}

	bool get_elements(const TiXmlElement * root , const std::string& path , elem_list & elems)
	{
		if(root == NULL)
			return false;

		if(path.empty() == true)
		{
			//如果path为空，那么就是求取当前元素
			elems.push_back(root) ;
			return true ;
		}

		string_list keys ;
		split_key(path , keys) ;
		std::string name = keys.back() ;

		const TiXmlElement *elem = find_first_of(root , keys) ;

		while(elem)
		{
			if(name == elem->ValueStr())
			{
				elems.push_back(elem) ;
			}

			elem = elem->NextSiblingElement() ;		
		}

		if(elems.empty())
			return false ;
		else
			return true ;
	}

	config_child::config_child()
	{
		elem_ = NULL ;
	}

	config_child::config_child(const config_child& child)
	{
		elem_ = child.elem_ ;
	}

	config_child::~config_child()
	{
		elem_ = NULL ;
	}


	bool config_child::convert_bool(const std::string & value) 
	{
		if(value == "1" || value == "yes" || value == "YES" || value == "true" || value == "TRUE")
			return true ;
		else
			return false ;
	}

	bool config_child::get_value(const std::string& path, int& value) const
	{
		value = 0 ;
		std::string valstr ;

		if(get_value(path , valstr) == false)
			return false ;

		std::istringstream iss(valstr);
		iss >> value;

		return true;
	}

	bool config_child::get_value(const std::string& path, std::string& value) const
	{
		value.clear() ;

		string_list values ;
		if(get_values(path , values) == false)
			return false ;

		if(!values.empty())
		{
			value = values.front() ;
		}

		return true;
	}

	bool config_child::get_value(const std::string& path, double& value) const
	{
		value = 0 ;

		std::string valstr ;

		if(get_value(path , valstr) == false)
			return false ;

		std::istringstream iss(valstr) ;
		iss >> value;

		return true;
	}

	bool config_child::get_value(const std::string& path, int64& value) const
	{
		value = 0 ;

		std::string valstr ;

		if(get_value(path , valstr) == false)
			return false ;

		std::istringstream iss(valstr) ;
		iss >> value;

		return true;
	}

	bool config_child::get_value(const std::string& path, bool& value) const
	{
		value = false ;

		std::string valstr ;
		if(get_value(path , valstr) == false)
			return false ;

		value = convert_bool(valstr) ;
		return true;
	}

	bool config_child::get_values(const std::string& path, std::list<std::string> & values) const
	{
		if(elem_ == NULL)
		{
			return false;
		}

		elem_list elems ;
		elem_list::const_iterator citer ;

		if(get_elements(elem_ , path , elems) == false)
			return false ;

		for(citer = elems.begin() ; citer != elems.end() ; citer++)
		{
			const TiXmlElement *elem = (*citer) ;
			if(elem == NULL)
				continue ;

			if(elem->GetText())
			{
				values.push_back(elem->GetText()) ;		
			}
		}

		if(values.empty())
			return false ;
		else
			return true;
	}

	bool config_child::get_attribute(const std::string& path , const std::string& name , int& value) const
	{
		value = 0 ;
		std::string valstr ;

		if(get_attribute(path , name , valstr) == false)
			return false ;

		std::istringstream iss(valstr) ;
		iss >> value;
		return true ;
	}

	bool config_child::get_attribute(const std::string& path , const std::string& name , std::string& value) const
	{
		value = "" ;

		if(elem_ == NULL)
			return false ;

		elem_list elems ;
		elem_list::const_iterator citer ;
		if(get_elements(elem_ , path , elems) == false)
			return false ;

		if(elems.empty())
			return false ;

		if(elems.front() == NULL)	
			return false ;

		elems.front()->QueryStringAttribute(name.c_str() , &value) ;
		return true ;
	}

	bool config_child::get_attribute(const std::string& path , const std::string& name , double& value) const
	{
		value = 0 ;

		std::string valstr ;
		if(get_attribute(path , name , valstr) == false)
			return false ;

		std::istringstream iss(valstr) ;
		iss >> value;
		return true ;
	}

	bool config_child::get_attribute(const std::string& path , const std::string& name , bool& value) const
	{
		value = false ;

		std::string valstr ;
		if(get_attribute(path , name , valstr) == false)
			return false ;

		value = convert_bool(valstr) ;
		return true ;	
	}

	bool config_child::get_attribute(const std::string& path , const std::string& name , int64& value) const
	{
		value = 0 ;

		std::string valstr ;
		if(get_attribute(path , name , valstr) == false)
			return false ;

		std::istringstream iss(valstr) ;
		iss >> value;
		return true ;	
	}

	bool config_child::get_child_names(const std::string& path , std::list<std::string>& names) const
	{
		if(elem_ == NULL)
			return false ;

		elem_list elems ;

		if(get_elements(elem_ , path , elems) == false)
			return false ;

		elem_list::const_iterator iter ;
		const TiXmlElement * elem , * child ;
		for(iter = elems.begin() ; iter != elems.end() ; iter++)
		{
			elem = *iter ;
			if(elem == NULL)
				continue ;

			child = elem->FirstChildElement() ;
			while(child)
			{
				names.push_back(child->ValueStr()) ;		
				child = child->NextSiblingElement() ;
			}
		}

		return true ;
	}

	bool config_child::get_value(int& value) const
	{
		return get_value("" , value) ;
	}

	bool config_child::get_value(std::string& value) const
	{
		return get_value("" , value) ;
	}

	bool config_child::get_value(double& value) const
	{
		return get_value("" , value) ;
	}

	bool config_child::get_value(bool& value) const
	{
		return get_value("" , value) ;
	}

	bool config_child::get_value(int64& value) const
	{
		return get_value("" , value) ;
	}

	bool config_child::get_values(std::list<std::string>& values) const
	{
		return get_values("" , values) ;
	}

	bool config_child::get_attribute(const std::string& name , int& value) const
	{
		return get_attribute("" , name ,value) ;
	}

	bool config_child::get_attribute(const std::string& name , std::string& value) const
	{
		return get_attribute("" , name , value) ;
	}

	bool config_child::get_attribute(const std::string& name , double& value) const
	{
		return get_attribute("" , name , value) ;
	}

	bool config_child::get_attribute(const std::string& name , bool& value) const
	{
		return get_attribute("" , name , value) ;
	}

	bool config_child::get_attribute(const std::string& name , int64& value) const
	{
		return get_attribute("" , name , value) ;
	}

	bool config_child::get_child_names(std::list<std::string>& names) const 
	{
		return get_child_names("" , names) ;
	}


	bool config_child::get_childs(const std::string& path , std::vector<config_child>& childs) const
	{
		childs.clear() ;
		if(elem_ == NULL)
			return false ;

		elem_list elems ;

		if(get_elements(elem_ , path , elems) == false)
			return false ;

		elem_list::const_iterator iter ;
		const TiXmlElement * elem , * childe ;
		config_child c ;

		for(iter = elems.begin() ; iter != elems.end() ; iter++)
		{
			elem = *iter ;
			if(elem == NULL)
				continue ;

			childe = elem->FirstChildElement() ;
			while(childe)
			{
				c.elem_ = childe ;
				childs.push_back(c) ;
				childe = childe->NextSiblingElement() ;
			}
		}

		return true ;
	}

	bool config_child::get_childs(std::vector<config_child>& childs) const
	{
		return get_childs("" , childs) ;
	}

	const std::string config_child::get_name() const
	{
		if(elem_ == NULL)
			return "" ;
		else
			return elem_->ValueStr() ;
	}

	config::config(const std::string & config_file)
	{
		document_ = NULL ;
		elem_ = NULL ;
		inited_ = false ;

		if(config_file.empty() == false)
			init(config_file) ;
	}

	config::~config()
	{
		final() ;
	}

	bool config::inited() const
	{
		return inited_;
	}

	const std::string& config::get_config_file () const
	{
		return config_file_ ;
	}	

	bool config::parse(const char *buffer)
	{
		final() ;

		document_ = new TiXmlDocument(config_file_.c_str()) ;
		if(document_ == NULL)
		{
			LOGERR("GOD , can not new TiXmlDocument, file name[%s]" , config_file_.c_str()) ;
			return false ;
		}

		//if(document_->Parse(buffer) == false)
		document_->Parse(buffer) ;
		if(document_->Error() == true)
		{
			LOGERR("can not Parse buffer , Errmsg[%s]" , document_->ErrorDesc()) ;
			LOGBUFERR(buffer , ::strlen(buffer) + 1) ;

			delete document_ ;
			document_ = NULL ;
			return false ;
		}
		else
		{
			elem_ = document_->RootElement() ;
			inited_ = true ;
			return true ;
		}	
	}

	bool config::init(const std::string& config_file)
	{
		final() ;

		if(!config_file.empty())
		{
    		config_file_ = config_file;
		}
		else
		{
    		config_file_ = DEFAULT_CONFIGFILE;
		}

		document_ = new TiXmlDocument(config_file_.c_str()) ;
		if(document_ == NULL)
		{
			LOGERR("GOD , can not new TiXmlDocument, file name[%s]" , config_file_.c_str()) ;
			return false ;
		}

		if(document_->LoadFile() == false)
		{
			LOGERR("can not LoadFile , Errmsg[%s]" , document_->ErrorDesc()) ;

			delete document_ ;
			document_ = NULL ;
			return false ;
		}
		else
		{
			elem_ = document_->RootElement() ;
			inited_ = true ;
			return true ;
		}
	}

	bool config::final()
	{
		if(inited_  == false )
			return true ;

		if(document_ != NULL)
		{
			delete document_ ;
			document_ = NULL ;
			inited_ = false ;
		}

		return true ;
	}

}