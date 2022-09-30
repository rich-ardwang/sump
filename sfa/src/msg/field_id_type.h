#pragma once
#include <vector>
#include <map>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class FieldIdAndType{
public:
	FieldIdAndType() = default;
	virtual ~FieldIdAndType() = default;

	FieldIdAndType(int fid, FieldType type);
	FieldIdAndType(int fid, FieldType type, const std::vector<FieldType>& children);
	FieldIdAndType(int fid, FieldType type, const std::vector<FieldType>& children,
		const std::map<int, FieldType>& kvChildren);

public:
	const std::vector<FieldType>& GetChildren(void) const;
	void SetChildren(const std::vector<FieldType>& children);

	const std::map<int, FieldType>& GetKVChildren(void) const;
	void SetKVChildren(const std::map<int, FieldType>& kvChildren);

	int GetFid(void) const;
	void SetFid(int fid);

	FieldType GetType(void) const;
	void SetType(FieldType type);

private:
	int fid_ = 0;
	FieldType type_;

	std::map<int, FieldType> kvChildren_;
	std::vector<FieldType> children_;
};

}
}