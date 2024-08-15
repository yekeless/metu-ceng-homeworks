#include "core.h"
#include "binary.h"
#include "unary.h"
#include "nullary.h"

namespace sym
{
	bool __expr_t::is_unary() const
	{
		const __unary_op_t *bo = dynamic_cast<const __unary_op_t *>(this);
		if (bo)
			return true;
		else
			return false;
	}
	bool __expr_t::is_binary() const
	{
		const __binary_op_t *bo = dynamic_cast<const __binary_op_t *>(this);
		if (bo)
			return true;
		else
			return false;
	}
	bool __expr_t::is_nullary() const
	{
		const __nullary_op_t *bo = dynamic_cast<const __nullary_op_t *>(this);
		if (bo)
			return true;
		else
			return false;
	}
}
