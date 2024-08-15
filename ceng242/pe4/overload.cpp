#include "nullary.h"
#include "unary.h"
#include "binary.h"

namespace sym
{
	__expr_t &operator-(const __expr_t &op)
	{
		__expr_t *result;
		if (op.is_nullary())
		{
			result = new NegOp(op.eval());
		}
		else
		{
			result = new NegOp(&op);
		}
		return *result;
	}
	__expr_t &exp(const __expr_t &op)
	{
		__expr_t *result;
		if (op.is_nullary())
		{
			result = new ExpOp(op.eval());
		}
		else
		{
			result = new ExpOp(&op);
		}
		return *result;
	}

	__expr_t &operator+(const __expr_t &lhs, const __expr_t &rhs)
	{
		__expr_t *result;
		if (lhs.is_nullary() && rhs.is_nullary())
		{
			result = new AddOp(lhs.eval(), rhs.eval());
		}
		else if (lhs.is_nullary() && (!rhs.is_nullary()))
		{
			result = new AddOp(lhs.eval(), &rhs);
		}
		else if ((!lhs.is_nullary()) && rhs.is_nullary())
		{
			result = new AddOp(&lhs, rhs.eval());
		}
		else
		{
			result = new AddOp(&lhs, &rhs);
		}
		return *result;
	}
	__expr_t &operator+(double lhs, const __expr_t &rhs)
	{
		Const *sabit = new Const(lhs);
		__expr_t *result;
		if (rhs.is_nullary())
		{
			result = new AddOp(sabit, rhs.eval());
		}
		else
		{
			result = new AddOp(sabit, &rhs);
		}
		return *result;
	}
	__expr_t &operator+(const __expr_t &lhs, double rhs)
	{
		Const *sabit = new Const(rhs);
		__expr_t *result;
		if (lhs.is_nullary())
		{
			result = new AddOp(lhs.eval(), sabit);
		}
		else
		{
			result = new AddOp(&lhs, sabit);
		}
		return *result;
	}

	__expr_t &operator*(const __expr_t &lhs, const __expr_t &rhs)
	{
		__expr_t *result;
		if (lhs.is_nullary() && rhs.is_nullary())
		{
			result = new MulOp(lhs.eval(), rhs.eval());
		}
		else if (lhs.is_nullary() && (!rhs.is_nullary()))
		{
			result = new MulOp(lhs.eval(), &rhs);
		}
		else if ((!lhs.is_nullary()) && rhs.is_nullary())
		{
			result = new MulOp(&lhs, rhs.eval());
		}
		else
		{
			result = new MulOp(&lhs, &rhs);
		}
		return *result;
	}
	__expr_t &operator*(double lhs, const __expr_t &rhs)
	{
		Const *sabit = new Const(lhs);
		__expr_t *result;
		if (rhs.is_nullary())
		{
			result = new MulOp(sabit, rhs.eval());
		}
		else
		{
			result = new MulOp(sabit, &rhs);
		}
		return *result;
	}
	__expr_t &operator*(const __expr_t &lhs, double rhs)
	{
		Const *sabit = new Const(rhs);
		__expr_t *result;
		if (lhs.is_nullary())
		{
			result = new MulOp(lhs.eval(), sabit);
		}
		else
		{
			result = new MulOp(&lhs, sabit);
		}
		return *result;
	}
}
