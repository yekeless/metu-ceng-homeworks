#include "nullary.h"
#include "unary.h"
#include "binary.h"
#include <math.h>

namespace sym
{
	bool NegOp::is_neg() const { return true; }

	__expr_t *NegOp::eval(const var_map_t &vars) const
	{
		__expr_t *oper = operand->eval(vars);
		const Const *eva = dynamic_cast<Const *>(oper);
		if (eva != nullptr)
		{
			double val = (-1) * (dynamic_cast<Const *>(oper)->get_value());
			delete oper;
			return new Const(val);
		}
		else
		{
			return new NegOp(oper);
		}
	}

	__expr_t *NegOp::diff(const std::string &v) const
	{
		__expr_t *derivative = operand->diff(v);
		return NegOp(derivative).eval();
	}

	std::ostream &NegOp::operator<<(std::ostream &out) const
	{
		if (operand->is_nullary())
		{
			out << "-" << *operand;
		}
		else
		{
			out << "-" << "(" << *operand << ")";
		}
		return out;
	}

	bool NegOp::operator==(const __expr_t &other_) const
	{
		const NegOp *new_neg = dynamic_cast<const NegOp *>(&other_);
		if (new_neg != nullptr)
		{
			return *(new_neg->operand) == *operand;
		}
		return false;
	}
}

namespace sym
{
	bool ExpOp::is_exp() const { return true; }

	__expr_t *ExpOp::eval(const var_map_t &vars) const
	{
		__expr_t *expo = operand->eval(vars);
		const Const *ex = dynamic_cast<Const *>(expo);
		if (ex != nullptr)
		{
			double val = ex->get_value();
			delete expo;
			return new Const(std::exp(val));
		}
		else
		{
			return new ExpOp(expo);
		}
	}

	__expr_t *ExpOp::diff(const std::string &v) const
	{
		__expr_t *derivative = operand->diff(v);
		return MulOp(derivative, new ExpOp(operand->eval())).eval();
	}

	std::ostream &ExpOp::operator<<(std::ostream &out) const
	{
		if (operand->is_nullary())
		{
			out << "e^" << *operand;
		}
		else
		{
			out << "e^" << "(" << *operand << ")";
		}
		return out;
	}

	bool ExpOp::operator==(const __expr_t &other_) const
	{
		const ExpOp *new_exp = dynamic_cast<const ExpOp *>(&other_);
		if (new_exp != nullptr)
		{
			return *(new_exp->operand) == *operand;
		}
		return false;
	}
}
