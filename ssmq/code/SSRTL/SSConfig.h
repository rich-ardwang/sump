#ifndef __SS_CONFIG_H
#define __SS_CONFIG_H 1

#include <list>
#include <string>
#include <vector>
#include "SSRTL/SSType.h"

class TiXmlDocument ;
class TiXmlElement ;

namespace ss{

	class config_child {	
	public:
		config_child();
		config_child(const config_child& child);
		virtual ~config_child();

		bool get_value(const std::string& path, int& value) const;
		bool get_value(const std::string& path, std::string& value) const;
		bool get_value(const std::string& path, double& value) const;
		bool get_value(const std::string& path, bool& value) const;
		bool get_value(const std::string& path, int64& value) const;
		bool get_values(const std::string& path , std::list<std::string>& values) const;

		bool get_attribute(const std::string& path , const std::string& name , int& value) const;
		bool get_attribute(const std::string& path , const std::string& name , std::string& value) const;
		bool get_attribute(const std::string& path , const std::string& name , double& value) const;
		bool get_attribute(const std::string& path , const std::string& name , bool& value) const;
		bool get_attribute(const std::string& path , const std::string& name , int64& value) const;

		bool get_child_names(const std::string& path , std::list<std::string>& names) const ;

		bool get_value(int& value) const;
		bool get_value(std::string& value) const;
		bool get_value(double& value) const;
		bool get_value(bool& value) const;
		bool get_value(int64& value) const;
		bool get_values(std::list<std::string>& values) const;

		bool get_attribute(const std::string& name , int& value) const;
		bool get_attribute(const std::string& name , std::string& value) const;
		bool get_attribute(const std::string& name , double& value) const;
		bool get_attribute(const std::string& name , bool& value) const;
		bool get_attribute(const std::string& name , int64& value) const;

		bool get_child_names(std::list<std::string>& names) const ;

		bool get_childs(const std::string& path , std::vector<config_child>& childs) const;
		bool get_childs(std::vector<config_child>& childs) const;
		const std::string get_name() const ;

		static bool convert_bool(const std::string & value);
	protected:
		const TiXmlElement *elem_ ;
		friend class config ;
	};

	class config : public config_child{	
	public:
		config(const std::string & config_file);
		~config();

		typedef config_child child ;

		bool init(const std::string& config_file);
		bool parse(const char *buffer) ;
		bool final() ;
		bool inited() const;

		const std::string& get_config_file() const;
	private:
		std::string config_file_;
		TiXmlDocument * document_;
		bool inited_;
	};
}




#endif /** __SS_CONFIG_H */
