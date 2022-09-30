#pragma once
#include <map>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class Fields{
public:
	Fields() = default;
	Fields(const Fields& fields);
	Fields(Fields&& fields);
	Fields& operator=(const Fields& fields);
	Fields& operator=(Fields&& fields);
	virtual ~Fields();

	IFieldValue* Get(int fid) const;
	void Add(int fid, const IFieldValue *value);
	void Del(int fid);

	void Add(const std::map<int, IFieldValue*>& mapFields);
	std::vector<int> GetAllFieldId(void) const;
	int Size(void) const;

private:
	void Clear(void);

private:
	std::map<int, IFieldValue*> mapFields_;
};

}
}